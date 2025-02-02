#include "mqtt.h"

#include <ctype.h>
#include <math.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "mqtt_client.h"
#include "queues.h"
#include "wifi.h"

static const char *TAG = "MQTT";
#define MQTT_PAYLOAD_MAX_SIZE_BYTES 256
#define TOPIC_OTA CONFIG_MQTT_PREFIX "/ota"
#define TOPIC_LOGS CONFIG_MQTT_PREFIX "/logs"
#define TOPIC_ANIM CONFIG_MQTT_PREFIX "/animate"
#define TOPIC_BRIG CONFIG_MQTT_PREFIX "/brightness"
#define TOPIC_COLO CONFIG_MQTT_PREFIX "/color"
#define TOPIC_LED_BRIG CONFIG_MQTT_PREFIX "/led/+/brightness"
#define TOPIC_LED_COLO CONFIG_MQTT_PREFIX "/led/+/color"

// MQTT Client
static esp_mqtt_client_handle_t client;

// MQTT event group
EventGroupHandle_t mqtt_event_group;

static void mqtt_subscribe() {
    int msg_id;
    msg_id = esp_mqtt_client_subscribe(client, TOPIC_OTA, 0);
    ESP_LOGI(TAG, "Subscribed to (%s), msg_id=%d", TOPIC_OTA, msg_id);

    msg_id = esp_mqtt_client_subscribe(client, TOPIC_ANIM, 0);
    ESP_LOGI(TAG, "Subscribed to (%s), msg_id=%d", TOPIC_ANIM, msg_id);

    msg_id = esp_mqtt_client_subscribe(client, TOPIC_BRIG, 0);
    ESP_LOGI(TAG, "Subscribed to (%s), msg_id=%d", TOPIC_BRIG, msg_id);

    msg_id = esp_mqtt_client_subscribe(client, TOPIC_COLO, 0);
    ESP_LOGI(TAG, "Subscribed to (%s), msg_id=%d", TOPIC_COLO, msg_id);

    msg_id = esp_mqtt_client_subscribe(client, TOPIC_LED_BRIG, 0);
    ESP_LOGI(TAG, "Subscribed to (%s), msg_id=%d", TOPIC_LED_BRIG, msg_id);

    msg_id = esp_mqtt_client_subscribe(client, TOPIC_LED_COLO, 0);
    ESP_LOGI(TAG, "Subscribed to (%s), msg_id=%d", TOPIC_LED_COLO, msg_id);
}

static void mqtt_parse_payload(esp_mqtt_event_handle_t event) {
    printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
    // Sanity check
    if (event->data_len >= MQTT_PAYLOAD_MAX_SIZE_BYTES - 1) {
        ESP_LOGI(TAG, "Payload is larger than buffer!");
        return;
    }

    if (memcmp(event->topic, TOPIC_OTA, event->topic_len) == 0) {
        ESP_LOGI(TAG, "OTA update!");
        char *payload = pvPortMalloc(sizeof(char) * QUEUE_SIZE_OTA);
        if (payload == NULL) {
            ESP_LOGE(TAG, "Could not allocate memory to get MQTT payload");
            return;
        }
        memcpy(payload, event->data, sizeof(char) * event->data_len);
        payload[event->data_len] = '\0';
        if (xQueueSend(dispatcher_queues[QUEUE_OTA], (void *)payload,
                       500 / portTICK_PERIOD_MS) != pdTRUE) {
            ESP_LOGI(TAG, "Queue is not available, ignoring message");
        }
        vPortFree(payload);
    } else if (memcmp(event->topic, TOPIC_ANIM, event->topic_len) == 0) {
        ESP_LOGI(TAG, "Animation call");

        // Sanity check and conversion
        uint8_t payload = 0;
        for (int i = 0; i < event->data_len; i++) {
            if (!isdigit((unsigned char)event->data[i])) {
                ESP_LOGI(TAG, "Wrong payload, expected an integer");
                return;
            }
            // uint overflow will just reset it if needed
            payload += (event->data[i] - '0') * pow10(i);
        }

        if (xQueueSend(dispatcher_queues[QUEUE_ANIM], (void *)&payload,
                       500 / portTICK_PERIOD_MS) != pdTRUE) {
            ESP_LOGI(TAG, "Queue is not available, ignoring message");
        }
    } else if (memcmp(event->topic, TOPIC_BRIG, event->topic_len) == 0) {
        ESP_LOGI(TAG, "Global brightness change");
        // Sanity check and conversion
        uint8_t payload = 0;
        for (int i = 0; i < event->data_len; i++) {
            if (!isdigit((unsigned char)event->data[i])) {
                ESP_LOGI(TAG, "Wrong payload, expected an integer");
                return;
            }
            // uint overflow will just reset it if needed
            payload += (event->data[i] - '0') * pow10(i);
        }
        if (payload > 100) payload = 100;

        if (xQueueSend(dispatcher_queues[QUEUE_BRIG], (void *)&payload,
                       500 / portTICK_PERIOD_MS) != pdTRUE) {
            ESP_LOGI(TAG, "Queue is not available, ignoring message");
        }
    } else if (memcmp(event->topic, TOPIC_COLO, event->topic_len) == 0) {
        ESP_LOGI(TAG, "GLobal color change");
        // Sanity check, payload should be a string like RRGGBB
        if (event->topic_len != 6) return;
        for (int i = 0; i < event->data_len; i++) {
            if (!isxdigit((unsigned char)event->data[i])) {
                ESP_LOGI(TAG, "Wrong payload, expected an integer");
                return;
            }
        }

        if (xQueueSend(dispatcher_queues[QUEUE_COLO], (void *)&event->data,
                       500 / portTICK_PERIOD_MS) != pdTRUE) {
            ESP_LOGI(TAG, "Queue is not available, ignoring message");
        }
    } else if (memcmp(event->topic, TOPIC_LED_BRIG, event->topic_len) == 0) {
        ESP_LOGI(TAG, "Led-specific brightness change");
        // Sanity check and conversion
        uint8_t payload = 0;
        for (int i = 0; i < event->data_len; i++) {
            if (!isdigit((unsigned char)event->data[i])) {
                ESP_LOGI(TAG, "Wrong payload, expected an integer");
                return;
            }
            // uint overflow will just reset it if needed
            payload += (event->data[i] - '0') * pow10(i);
        }
        if (payload > 100) payload = 100;

        if (xQueueSend(dispatcher_queues[QUEUE_LED_BRIG], (void *)&payload,
                       500 / portTICK_PERIOD_MS) != pdTRUE) {
            ESP_LOGI(TAG, "Queue is not available, ignoring message");
        }
    } else if (memcmp(event->topic, TOPIC_LED_COLO, event->topic_len) == 0) {
        ESP_LOGI(TAG, "Led-specific color change");
        // Sanity check, payload should be a string like RRGGBB
        if (event->topic_len != 6) return;
        for (int i = 0; i < event->data_len; i++) {
            if (!isxdigit((unsigned char)event->data[i])) {
                ESP_LOGI(TAG, "Wrong payload, expected an integer");
                return;
            }
        }

        if (xQueueSend(dispatcher_queues[QUEUE_LED_COLO], (void *)&event->data,
                       500 / portTICK_PERIOD_MS) != pdTRUE) {
            ESP_LOGI(TAG, "Queue is not available, ignoring message");
        }
    } else {
        ESP_LOGE(TAG, "Error, unhandled message from topic \"%s\"",
                 event->topic);
    }
}

static int mqtt_vprintf(const char *fmt, va_list ap) {
    // Get the formatted string
    char buf[256];
    int bufsz = vsprintf(buf, fmt, ap);
    esp_mqtt_client_publish(client, TOPIC_LOGS, buf, bufsz, 1, 0);
    return bufsz;
}

// MQTT event handler
static esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event) {
    switch (event->event_id) {
        case MQTT_EVENT_BEFORE_CONNECT:
            ESP_LOGI(TAG, "Event before connecting");
            break;
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "Connected");

            mqtt_subscribe();

            xEventGroupSetBits(mqtt_event_group, MQTT_CONNECTED_BIT);

            ESP_LOGI(
                TAG,
                "Connected to MQTT broker, redirecting logs to topic %s. Bye!",
                TOPIC_LOGS);
            esp_log_set_vprintf(mqtt_vprintf);
            ESP_LOGI(TAG,
                     "Connected to MQTT broker, logs redirected to topic %s",
                     TOPIC_LOGS);

            break;

        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "Disconnected");
            xEventGroupClearBits(mqtt_event_group, MQTT_CONNECTED_BIT);
            break;

        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGD(TAG, "Subscribed, msg_id=%d", event->msg_id);
            break;

        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGD(TAG, "Unsubscribed, msg_id=%d", event->msg_id);
            break;

        case MQTT_EVENT_PUBLISHED:
            ESP_LOGD(TAG, "Published, msg_id=%d", event->msg_id);
            break;

        case MQTT_EVENT_DATA:
            ESP_LOGD(TAG, "Data incoming");
            mqtt_parse_payload(event);
            break;

        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "Event error");
            break;
    }
    return ESP_OK;
}

#define MQTT_INIT_STACK_SIZE 2048
StaticTask_t mqtt_init_buffer;
StackType_t mqtt_init_stack[MQTT_INIT_STACK_SIZE];
static void mqtt_init_async(void *pvParameter) {
    ESP_LOGI(TAG, "Waiting for network connectivity");
    xEventGroupWaitBits(net_event_group, WIFI_CONNECTED_BIT, false, false,
                        portMAX_DELAY);
    esp_mqtt_client_start(client);

    // XXX Ugly hack
    while (1) {
        vTaskDelay(portMAX_DELAY);
    }
}

void mqtt_init() {
    mqtt_event_group = xEventGroupCreate();

    const esp_mqtt_client_config_t mqtt_cfg = {
        .uri = CONFIG_MQTT_URL,
        .client_id = CONFIG_MQTT_CLIENT_ID,
        .username = CONFIG_MQTT_CLIENT_ID,
        .password = CONFIG_MQTT_CLIENT_ID,
        .event_handle = mqtt_event_handler};

    client = esp_mqtt_client_init(&mqtt_cfg);

    xTaskCreateStatic(&mqtt_init_async, "mqtt_init", MQTT_INIT_STACK_SIZE, NULL,
                      tskIDLE_PRIORITY + 1, mqtt_init_stack, &mqtt_init_buffer);
}
