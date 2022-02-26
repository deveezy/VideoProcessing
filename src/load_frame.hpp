#pragma once
#include <stdint.h>

bool load_frame(const char *filename, int32_t *frame_width, int32_t *frame_height,
                uint8_t **frame_data);