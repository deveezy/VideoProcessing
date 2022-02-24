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

  glfwMakeContextCurrent(window);

  uint8_t *data = new uint8_t[100 * 100 * 3];
	for (size_t y = 0; y < 100; ++y) {
		for (size_t x = 0; x < 100; ++x) {
			data[y * 100 * 3 + x * 3    ] = 0xff;
			data[y * 100 * 3 + x * 3 + 1] = 0x00;
			data[y * 100 * 3 + x * 3 + 2] = 0x00;
		}
	}

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawPixels(100, 100, GL_RGB, GL_UNSIGNED_BYTE, data); 
		glfwSwapBuffers(window);
		glfwWaitEvents();
  }

  delete[] data;

  return 0;
}