#include "load_frame.hpp"
#include <GLFW/glfw3.h>
#include <libavcodec/avcodec.h>
#include <libavcodec/packet.h>
#include <libavformat/avformat.h>
#include <libavutil/frame.h>
#include <stdint.h>
#include <stdio.h>

int main() {
  GLFWwindow *window;
  if (!glfwInit()) {
    fprintf(stderr, "Couldn't init GLFW\n");
    return 1;
  }

  window = glfwCreateWindow(800, 600, "test window", nullptr, nullptr);
  if (!window) {
    fprintf(stderr, "Couldn't open window\n");
    return 1;
  }

  int32_t frame_width;
  int32_t frame_height;
  uint8_t *frame_data;

  if (!load_frame("/home/devpc/dev/media/demo.mp4", &frame_width, &frame_height, &frame_data)) {
    fprintf(stderr, "Couldn't load video frame\n");
    return 1;
  }

  glfwMakeContextCurrent(window);

  GLuint tex_handle;
  glGenTextures(1, &tex_handle);
  glBindTexture(GL_TEXTURE_2D, tex_handle);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame_width, frame_height, 0, GL_RGB, GL_UNSIGNED_BYTE,
               frame_data);

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    int32_t window_width;
    int32_t window_height;
    glfwGetFramebufferSize(window, &window_width, &window_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, window_width, 0, window_height, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex_handle);
    glBegin(GL_QUADS);
    // clang-format off
    glTexCoord2d(0, 0); glVertex2i(200, 200);
    glTexCoord2d(1, 0); glVertex2i(200 + frame_width, 200);
    glTexCoord2d(1, 1); glVertex2i(200 + frame_width, 200 + frame_width);
    glTexCoord2d(0, 1); glVertex2i(200, 200 + frame_height);
    // clang-format on
    glEnd();
    glTexCoord2d(0, 0);
    glVertex2i(0, 0);
    glDisable(GL_TEXTURE_2D);

    glfwSwapBuffers(window);
    glfwWaitEvents();
  }
  delete[] frame_data;
  return 0;
}