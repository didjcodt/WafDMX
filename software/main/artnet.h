#pragma once

#include <stdint.h>

void artnet_init();
void artnet_print_packet_header();
void artnet_print_packet_content();

extern uint8_t led_dmx_buffer[];
