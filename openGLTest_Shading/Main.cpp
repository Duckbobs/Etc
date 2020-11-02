#include <iostream> // ���� ��� ����
#include <glm/glm.hpp> // ���� ��� ����

#include <GL/glew.h> // ������ �ٲ�� �ȵ�
#include <GLFW/glfw3.h> // ������ �ٲ�� �ȵ�
#include "MyGlWindow.h"

GLFWwindow* window;
MyGlWindow* win;

static void key_callback(GLFWwindow*, int, int, int, int);
static void window_resize(GLFWwindow* window, int width, int height);

int main() {
	// @------------------------------------------------------------------------------@
	// | Settings																	  |
	// @------------------------------------------------------------------------------@
#pragma region Settings
	// @------------------------------------------------------------------------------@
	// | Initialize the library														  |
	// | openGL ���� ���� = MAJOR.MINOR = 4.3										  |
	// | CORE = �ֽű��, COMPATIBLE = �����										  |
	// @------------------------------------------------------------------------------@
	if (!glfwInit()) {
		// �ʱ�ȭ ������ �ؾ���.
		std::cout << "glfw init error" << std::endl;
		return 0;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



	// @------------------------------------------------------------------------------@
	// | Create a windowed mode window and its OpenGL context						  |
	// | glfw = â, �̺�Ʈ															  |
	// @------------------------------------------------------------------------------@
	int width = 800;
	int height = 800;
	GLFWwindow* window = glfwCreateWindow(width, height, u8"���� openGL������", NULL, NULL); // NULL NULL = â���� ���ڴ�. �ٸ� �� ������ Ǯ��ũ�� ���� // �ѱ��� u8�� �ٿ������.
	//glfwSetWindowTitle(window, u8"���� openGL������"); // �ѱ��� u8�� �ٿ������.
	if (!window) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // �Ŵ��� ����ü ����



	// @------------------------------------------------------------------------------@
	// | Callbacks																	  |
	// @------------------------------------------------------------------------------@
	glfwSetKeyCallback(window, key_callback);
	glfwSetWindowSizeCallback(window, window_resize);



	// @------------------------------------------------------------------------------@
	// | Make the window's context current											  |
	// | glew = Helper																  |
	// @------------------------------------------------------------------------------@
	GLenum err = glewInit(); // ���� �ʱ�ȭ
	if (err != GLEW_OK) {
		//Problem: glewInit failed, something is seriously wrong.
		std::cout << "glewInit failed, aborting." << std::endl;
		return 0;
	}
	glfwSwapInterval(1);
	// enable vsync = ����� �ֻ����� ����ȭ; glewExperimental = GL_TRUE; // ������ ��. �ھ����������� ���� TRUE �� �ֵ��� ������µ� ����� �ڵ����� ��.
#pragma endregion



	// @------------------------------------------------------------------------------@
	// | Rendering																	  |
	// @------------------------------------------------------------------------------@
	printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
	win = new MyGlWindow(width, height);

	glm::vec3 camPos = glm::vec3(5, 5, 5);
	glm::vec3 Look = glm::vec3(0, 0, 0);
	glm::vec3 Up = glm::vec3(0, 1, 0);

	win->lookAt(camPos, Look, Up);

	while (!glfwWindowShouldClose(window))
	{
		win->draw(); // �Ź� ȣ��

		glfwSwapBuffers(window); // Swap front and back buffers

		glfwPollEvents(); // Poll forand process events
	}

	// @------------------------------------------------------------------------------@
	// | Terminate																	  |
	// @------------------------------------------------------------------------------@
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}



// @------------------------------------------------------------------------------@
// | Callbacks																	  |
// @------------------------------------------------------------------------------@
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	else if (key == GLFW_KEY_R && action == GLFW_PRESS)
		printf("r");
}
static void window_resize(GLFWwindow* window, int width, int height) {
	win->resize(width, height);
}
