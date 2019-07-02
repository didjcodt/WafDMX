// stdlib
#include <stdlib.h>

// FreeRTOS includes
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

// ESP specific includes
#include "esp_log.h"
#include "esp_system.h"
#include "esp_event.h"
#include "nvs_flash.h"

// Other
#include "tcpip_adapter.h"
#include "eth.h"
#include "artnet.h"
#include "led.h"
#include "mqtt.h"
#include "ota.h"
#include "queues.h"
#include "wifi.h"

// GPIO Definition
#define PIN_NUM_MOSI 14
#define PIN_NUM_CLK 13

static const char *TAG = "MAIN_APP";

void init_logging() {
    ESP_LOGI(TAG, "[APP] System initialization\n");
    ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("TRANSPORT_TCP", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_SSL", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT", ESP_LOG_VERBOSE);
    esp_log_level_set("OUTBOX", ESP_LOG_VERBOSE);
    esp_log_level_set("WIFI", ESP_LOG_VERBOSE);
    esp_log_level_set("MQTT_CLIENT", ESP_LOG_VERBOSE);
}

void app_main() {
    init_logging();

    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES ||
        err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // OTA app partition table has a smaller NVS partition size than the
        // non-OTA partition table. This size mismatch may cause NVS
        // initialization to fail. If this happens, we erase NVS partition and
        // initialize NVS again.
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Initialize static queues
    queues_init();

    // Initialize spi device
    apa102_spi_device_t device = {.clock_speed_hz = CONFIG_SPI_SPEED_HZ,
                                  .mosi = PIN_NUM_MOSI,
                                  .clk = PIN_NUM_CLK,
                                  .cs = 4};
    led_init(&device);

    // Initialize networking
    tcpip_adapter_init();
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_ERROR_CHECK(tcpip_adapter_set_default_eth_handlers());
    // Wifi init initalizes net_event_group needed for eth, call first!
    wifi_init();
    eth_init();

    mqtt_init();

    artnet_init();

    // Depends on MQTT (and so, WiFi)
    ESP_LOGI("MQTT", "Waiting for mqtt");
    xEventGroupWaitBits(mqtt_event_group, MQTT_CONNECTED_BIT, false, true,
                        portMAX_DELAY);
    ota_init();
}
