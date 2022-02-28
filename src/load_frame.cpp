#include <libavcodec/codec.h>
#include <libavcodec/codec_par.h>
#include <libavutil/avutil.h>
#include <stdio.h>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}
#include "load_frame.hpp"

bool load_frame(const char *filename, int32_t *frame_width, int32_t *frame_height,
                uint8_t **frame_data) {
  AVFormatContext *format_ctx = avformat_alloc_context();
  if (!format_ctx) {
    fprintf(stderr, "Couldn't create AVFormatContext\n");
    return false;
  }

  if (avformat_open_input(&format_ctx, filename, nullptr, nullptr) != 0) {
    fprintf(stderr, "Couldn't open video file\n");
    return false;
  }

  int32_t video_stream_index = -1;

  for (size_t i = 0; i < format_ctx->nb_streams; ++i) {
    auto stream = format_ctx->streams[i];
    AVCodecParameters *codec_params = format_ctx->streams[i]->codecpar;
    AVCodec *codec = avcodec_find_decoder(codec_params->codec_id);
    if (!codec) continue;
    if (codec->type == AVMEDIA_TYPE_VIDEO) {
      video_stream_index = i;
      break;
    }

    if (codec_params->codec_type == AVMEDIA_TYPE_AUDIO) {}
  }

  if (video_stream_index == -1) {
    fprintf(stderr, "Couldn't find valid video stream insided file\n");
    return false;
  }
  return true;
}