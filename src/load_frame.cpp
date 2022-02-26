#include "load_frame.hpp"

bool load_frame(const char *filename, int32_t *frame_width,
                int32_t *frame_height, uint8_t **frame_data) {
  *frame_width = 100;
  *frame_height = 100;
  *frame_data = new uint8_t[100 * 100 * 3];

  auto ptr = *frame_data;
  for (size_t y = 0; y < 100; ++y) {
    for (size_t x = 0; x < 100; ++x) {
      *ptr++ = 0xff;
      *ptr++ = 0x00;
      *ptr++ = 0x00;
    }
  }
  return true;
}