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

            if (opcode == ARTNET_OPCODE_DMX) {
                uint8_t sequence = artnetPacket[12];
                uint16_t incomingUniverse = artnetPacket[14] | artnetPacket[15]
                                                                   << 8;
                uint16_t dmxDataLength = artnetPacket[17] | artnetPacket[16]
                                                                << 8;

                if (incomingUniverse != CONFIG_ARTNET_UNIVERSE) continue;

                int min_size = CONFIG_LEDS_COUNT * 3 <
                                       dmxDataLength - CONFIG_ARTNET_ADDRESS
                                   ? CONFIG_LEDS_COUNT * 3
                                   : dmxDataLength;
                memcpy(led_dmx_buffer,
                       artnetPacket + ARTNET_DMX_START + CONFIG_ARTNET_ADDRESS,
                       min_size);
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
          ARTNET_INIT_ASYNC_TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1,
          artnet_init_async_task_stack, &artnet_init_async_task_buffer);
}
