#include "artnet.h"

#include <string.h>

// FreeRTOS includes
#include "esp_log.h"
#include "freertos/FreeRTOS.h"

// LWIP includes
#include <lwip/netdb.h>
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"

// Other
#include "wifi.h"

uint8_t led_dmx_buffer[CONFIG_LEDS_COUNT * 3] = {0};

#define ARTNET_BUFFER_SIZE 530
static const char *TAG = "ARTNET";
static const uint8_t ARTNET_DMX_START = 18;
static const uint16_t ARTNET_PORT = 6454;

#define ARTNET_OPCODE_POLL 0x2000
#define ARTNET_OPCODE_POLL_REPLY 0x2100
#define ARTNET_OPCODE_ADDRESS 0x6000
#define ARTNET_OPCODE_INPUT 0x7000
#define ARTNET_OPCODE_IPPROG 0xf800
#define ARTNET_OPCODE_IPPROG_REPLY 0xf900
#define ARTNET_OPCODE_COMMAND 0x2400
#define ARTNET_OPCODE_DMX 0x5000
#define ARTNET_OPCODE_NZS 0x5100
#define ARTNET_OPCODE_SYNC 0x5200
#define ARTNET_OPCODE_TOD_REQUEST 0x8000
#define ARTNET_OPCODE_TOD_DATA 0x8100
#define ARTNET_OPCODE_TOD_CONTROL 0x8200
#define ARTNET_OPCODE_RDM 0x8300
#define ARTNET_OPCODE_RDM_SUB 0x8400
#define ARTNET_OPCODE_TIME_CODE 0x9700
#define ARTNET_OPCODE_TIME_SYNC 0x9800
#define ARTNET_OPCODE_TRIGGER 0x9900
#define ARTNET_OPCODE_DIAG_DATA 0x2300
#define ARTNET_OPCODE_FIRMWARE_MASTER 0xF200
#define ARTNET_OPCODE_FIRMWARE_REPLY 0xF300
#define ARTNET_OPCODE_DIRECTORY 0x9A00
#define ARTNET_OPCODE_DIRECTORY_REPLY 0x9B00
#define ARTNET_OPCODE_FILE_TN_MASTER 0xF400
#define ARTNET_OPCODE_FILE_FN_MASTER 0xF500
#define ARTNET_OPCODE_FILE_FN_REPLY 0xF600

static int udp_socket;
static uint8_t artnetPacket[ARTNET_BUFFER_SIZE];
static char txArtnetPacket[ARTNET_BUFFER_SIZE];

static void artnet_make_opcode_poll_reply() {
   // Packet size is 239 bytes
   // Fill with 0 to avoid completion
   memset(txArtnetPacket, 0, sizeof(char) * 239);
   int byteIdx = 0;
   // ID[8]
   strcpy(txArtnetPacket, "Art-Net");
   byteIdx += 8;
   // OpCode
   txArtnetPacket[byteIdx++] = ARTNET_OPCODE_POLL_REPLY & 0xFF;
   txArtnetPacket[byteIdx++] = (ARTNET_OPCODE_POLL_REPLY & 0xFF00) >> 8;
   // IP Address
   txArtnetPacket[byteIdx++] = 0;
   txArtnetPacket[byteIdx++] = 0;
   txArtnetPacket[byteIdx++] = 0;
   txArtnetPacket[byteIdx++] = 0;
   // Port, always 0x1936 (6454)
   txArtnetPacket[byteIdx++] = 0x1936 & 0xFF;
   txArtnetPacket[byteIdx++] = (0x1936 & 0xFF00) >> 8;
   // Firmware version (0, 0)
   byteIdx += 2;
   // NetSwitch, SubSwitch (0, 0)
   byteIdx += 2;
   // OEM Code, let's say DMXHub (0, 0)
   byteIdx += 2;
   // Ubea version (0)
   byteIdx++;
   // Status 1
   txArtnetPacket[byteIdx++] = 0b11100000;
   // ESTA manufacturer code (Kaizen Wolf's initials)
   txArtnetPacket[byteIdx++] = 'K';
   txArtnetPacket[byteIdx++] = 'W';
   // Short name (fixed-size 17 + \0)
   strcpy(&txArtnetPacket[byteIdx += 18], "WafDMX controller");
   // Long name (fixed-size 63 + \0)
   strcpy(&txArtnetPacket[byteIdx += 64], "WafDMX controller: Source code on github.com/didjcodt/WafDMX");
   // Node Report, formatted as #xxxx[yyyy...]zzzz... (fixed size, 63 + \0)
   static uint64_t poll_reply_index = 0; // Should overflow way above the time we use that thing
   sprintf(&txArtnetPacket[byteIdx += 64], "#0001[%lld]OK", poll_reply_index++);
   // number of io ports, useless for nodes (0, 0)
   byteIdx += 2;
   // Port types (Input for first)
   txArtnetPacket[byteIdx++] = 1 << 7;
   byteIdx += 3;
   // GoodInput
   txArtnetPacket[byteIdx++] = 1 << 7;
   byteIdx += 3;
   // GoodOutput
   byteIdx += 4;
   // SwIn
   txArtnetPacket[byteIdx++] = CONFIG_ARTNET_UNIVERSE;
   byteIdx += 3;
   // SwOut, SwVideo, SwMacro, SwRemote, Spare = 0
   // Style = Node
   byteIdx += 11;
   // MAC (High to Low)
   txArtnetPacket[byteIdx++] = 0;
   txArtnetPacket[byteIdx++] = 0;
   txArtnetPacket[byteIdx++] = 0;
   txArtnetPacket[byteIdx++] = 0;
   txArtnetPacket[byteIdx++] = 0;
   txArtnetPacket[byteIdx++] = 0;
   // Bind IP (0, idx 1: we're root device)
   byteIdx += 4;
   txArtnetPacket[byteIdx++] = 1;
   // Status 2
   txArtnetPacket[byteIdx++] = 0b00000111;
   byteIdx += 26;
}

static void artnet_parse_opcode_poll() {
    uint16_t protocol_version = artnetPacket[10] | artnetPacket[11] << 8;
    uint8_t talk_to_me = artnetPacket[12];
    uint8_t priority = artnetPacket[13];
    ESP_LOGI(TAG,
             "Received OpPoll packet: "
             "protocol version 0x%04x, "
             "talk to me %d, priority %d",
             protocol_version, talk_to_me, priority);
    // TODO Reply
    artnet_make_opcode_poll_reply();
}

static void artnet_parse_opcode_dmx() {
    uint8_t sequence = artnetPacket[12];
    uint16_t incomingUniverse = artnetPacket[14] | artnetPacket[15] << 8;
    uint16_t dmxDataLength = artnetPacket[17] | artnetPacket[16] << 8;

    if (incomingUniverse != CONFIG_ARTNET_UNIVERSE) return;

    int min_size = CONFIG_LEDS_COUNT * 3 < dmxDataLength - CONFIG_ARTNET_ADDRESS
                       ? CONFIG_LEDS_COUNT * 3
                       : dmxDataLength;
    memcpy(led_dmx_buffer,
           artnetPacket + ARTNET_DMX_START + CONFIG_ARTNET_ADDRESS, min_size);
}

#define ARTNET_READ_TASK_STACK_SIZE 4096
StaticTask_t artnet_read_task_buffer;
StackType_t artnet_read_task_stack[ARTNET_READ_TASK_STACK_SIZE];
static void artnet_read(void *pvParameter) {
    while (1) {
        ESP_LOGD(TAG, "Waiting for UDP data on %d", udp_socket);
        struct sockaddr_in source_addr;
        socklen_t socklen = sizeof(source_addr);
        ssize_t packetSize =
            recvfrom(udp_socket, artnetPacket, ARTNET_BUFFER_SIZE - 1, 0,
                     (struct sockaddr *)&source_addr, &socklen);
        ESP_LOGD(TAG, "Received stuff!");

        if (packetSize <= ARTNET_BUFFER_SIZE && packetSize > 0) {
            // Check that packetID is "Art-Net" else ignore
            if (memcmp(artnetPacket, "Art-Net", sizeof(char) * 7) != 0) {
                ESP_LOGI(TAG, "Non ArtNet packet received, ignoring...");
                continue;
            }

            uint16_t opcode = artnetPacket[8] | artnetPacket[9] << 8;

            switch (opcode) {
                case ARTNET_OPCODE_POLL:
                    artnet_parse_opcode_poll();
                    break;
                case ARTNET_OPCODE_POLL_REPLY:
                case ARTNET_OPCODE_DIAG_DATA:
                case ARTNET_OPCODE_COMMAND:
                case ARTNET_OPCODE_ADDRESS:
                case ARTNET_OPCODE_INPUT:
                case ARTNET_OPCODE_IPPROG:
                case ARTNET_OPCODE_IPPROG_REPLY:
                case ARTNET_OPCODE_NZS:
                case ARTNET_OPCODE_SYNC:
                case ARTNET_OPCODE_TOD_REQUEST:
                case ARTNET_OPCODE_TOD_DATA:
                case ARTNET_OPCODE_TOD_CONTROL:
                case ARTNET_OPCODE_RDM:
                case ARTNET_OPCODE_RDM_SUB:
                case ARTNET_OPCODE_TIME_CODE:
                case ARTNET_OPCODE_TIME_SYNC:
                case ARTNET_OPCODE_TRIGGER:
                case ARTNET_OPCODE_FIRMWARE_MASTER:
                case ARTNET_OPCODE_FIRMWARE_REPLY:
                case ARTNET_OPCODE_DIRECTORY:
                case ARTNET_OPCODE_DIRECTORY_REPLY:
                case ARTNET_OPCODE_FILE_TN_MASTER:
                case ARTNET_OPCODE_FILE_FN_MASTER:
                case ARTNET_OPCODE_FILE_FN_REPLY:
                    break;
                case ARTNET_OPCODE_DMX: {
                    artnet_parse_opcode_dmx();
                    break;
                }
                default:
                    ESP_LOGI(TAG, "Unknown opcode: 0x%04x", opcode);
            }
        } else {
            ESP_LOGE(TAG, "Wrong size: %d", packetSize);
        }
    }
}

#define ARTNET_INIT_ASYNC_TASK_STACK_SIZE 4096
StaticTask_t artnet_init_async_task_buffer;
StackType_t artnet_init_async_task_stack[ARTNET_INIT_ASYNC_TASK_STACK_SIZE];
static void artnet_init_async(void *pvParameter) {
    ESP_LOGI(TAG, "Waiting for network connectivity");
    xEventGroupWaitBits(net_event_group, WIFI_CONNECTED_BIT | ETH_CONNECTED_BIT,
                        false, false, portMAX_DELAY);

    udp_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (udp_socket < 0) {
        ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
        return;
    }
    ESP_LOGI(TAG, "Socket created");

    struct sockaddr_in dest_addr;
    dest_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(ARTNET_PORT);

    int err =
        bind(udp_socket, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (err < 0) {
        ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
        return;
    }
    ESP_LOGI(TAG, "Socket bound, port %d, fd %d", ARTNET_PORT, udp_socket);

    xTaskCreateStatic(&artnet_read, "artnet_read", ARTNET_READ_TASK_STACK_SIZE,
                      NULL, tskIDLE_PRIORITY + 1, artnet_read_task_stack,
                      &artnet_read_task_buffer);

    // XXX Ugly hack
    while (1) {
        vTaskDelay(portMAX_DELAY);
    }
}

void artnet_init() {
    xTaskCreateStatic(&artnet_init_async, "artnet_init",
                      ARTNET_INIT_ASYNC_TASK_STACK_SIZE, NULL,
                      tskIDLE_PRIORITY + 1, artnet_init_async_task_stack,
                      &artnet_init_async_task_buffer);
}
