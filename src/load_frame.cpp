
#include <libavutil/error.h>
#include <stdio.h>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavcodec/codec.h>
#include <libavcodec/codec_par.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}
#include "load_frame.hpp"

bool load_frame(const char *filename, int32_t *width_out, int32_t *height_out,
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

  AVCodecParameters *codec_params;
  const AVCodec *codec;
  for (size_t i = 0; i < format_ctx->nb_streams; ++i) {
    codec_params = format_ctx->streams[i]->codecpar;
    codec = avcodec_find_decoder(codec_params->codec_id);
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

  // Set up a codec context for the decoder
  AVCodecContext *codec_ctx = avcodec_alloc_context3(codec);
  if (!codec_ctx) {
    fprintf(stderr, "Couldn't create AVCodecContext\n");
    return false;
  }

  if (avcodec_parameters_to_context(codec_ctx, codec_params) < 0) {
    fprintf(stderr, "Couldn't initialize AVCodecCtx\n");
    return false;
  }

  if (avcodec_open2(codec_ctx, codec, nullptr) < 0) {
    fprintf(stderr, "Couldn't open codec\n");
    return false;
  }

  AVFrame *frame = av_frame_alloc();
  if (!frame) {
    fprintf(stderr, "Couldn't allocate frame\n");
    return false;
  }

  AVPacket *packet = av_packet_alloc();
  if (!packet) {
    fprintf(stderr, "Couldn't allocate packet\n");
    return false;
  }

  av_read_frame(format_ctx, packet);
  while (av_read_frame(format_ctx, packet) >= 0) {
    if (packet->stream_index != video_stream_index) continue;
    auto response = avcodec_send_packet(codec_ctx, packet);
    if (response < 0) {
      fprintf(stderr, "Failed to decode packet:\n");
      return false;
    }
    response = avcodec_receive_frame(codec_ctx, frame);
    if (response == AVERROR(EAGAIN) || response == AVERROR_EOF)
      continue;
    else if (response < 0) {
      fprintf(stderr, "Failed to decode packet\n");
      return false;
    }
    av_packet_unref(packet);
    break;
  }

  uint8_t *data = new uint8_t[frame->width * frame->height * 3];
  for (int x = 0; x < frame->width; ++x) {
    for (int y = 0; y < frame->height; ++y) {
      data[y * frame->width * 3 + x * 3] = 0xff;
      data[y * frame->width * 3 + x * 3 + 1] = 0x0;
      data[y * frame->width * 3 + x * 3 + 2] = 0x0;
    }
  }

  *frame_data = data;
  *width_out = frame->width;
  *height_out = frame->height;

  avformat_close_input(&format_ctx);
  avformat_free_context(format_ctx);
  av_frame_free(&frame);
  av_packet_free(&packet);
  avcodec_free_context(&codec_ctx);
  return true;
}