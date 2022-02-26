#include <GLFW/glfw3.h>
#include <gl/gl.h>
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

  uint8_t *data = new uint8_t[100 * 100 * 3];
  for (size_t y = 0; y < 100; ++y) {
    for (size_t x = 0; x < 100; ++x) {
      data[y * 100 * 3 + x * 3] = 0xff;
      data[y * 100 * 3 + x * 3 + 1] = 0x00;
      data[y * 100 * 3 + x * 3 + 2] = 0x00;
    }
  }

  for (size_t y = 25; y < 75; ++y) {
    for (size_t x = 25; x < 75; ++x) {
      data[y * 100 * 3 + x * 3] = 0x00;
      data[y * 100 * 3 + x * 3 + 1] = 0xff;
      data[y * 100 * 3 + x * 3 + 2] = 0x00;
    }
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
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 100, 100, 0, GL_RGB, GL_UNSIGNED_BYTE,
               data);

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
    glTexCoord2d(0, 0);
    glVertex2i(0, 0);
    glTexCoord2d(1, 0);
    glVertex2i(100, 0);
    glTexCoord2d(1, 1);
    glVertex2i(100, 100);
    glTexCoord2d(0, 1);
    glVertex2i(0, 100);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glfwSwapBuffers(window);
    glfwWaitEvents();
  }

  delete[] data;

  return 0;
}