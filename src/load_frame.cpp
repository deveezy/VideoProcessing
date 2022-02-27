#include <stdio.h>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}
#include "load_frame.hpp"

bool load_frame(const char *filename, int32_t *frame_width, int32_t *frame_height,
                uint8_t **frame_data) {
  AVFormatContext *av_ctx = avformat_alloc_context();
  if (!av_ctx) {
    fprintf(stderr, "Couldn't create AVFormatContext\n");
    return false;
  }

  if (avformat_open_input(&av_ctx, filename, nullptr, nullptr) != 0) {
    fprintf(stderr, "Couldn't open video file\n");
    return false;
  }
  return true;
}