#include <iostream> // ���� ��� ����
#include <glm/glm.hpp> // ���� ��� ����

#include <GL/glew.h> // ������ �ٲ�� �ȵ�
#include <GLFW/glfw3.h> // ������ �ٲ�� �ȵ�
#include "Loader.h"
#include "MyGlWindow.h"

static void key_callback(GLFWwindow*, int, int, int, int);

int main() {
	/* Initialize the library */
	if (!glfwInit()) // �ʱ�ȭ ������ �ؾ���.
	{
		std::cout << "glfw init error" << std::endl;
		return 0;
	}

	// openGL ���� ���� = MAJOR.MINOR = 4.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// CORE = �ֽű��, COMPATIBLE = �����
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	int width = 800;
	int height = 800;
	/* Create a windowed mode window and its OpenGL context */
	// glfw = â, �̺�Ʈ
	GLFWwindow* window;
	window = glfwCreateWindow(width, height, u8"���� openGL������", NULL, NULL); // NULL NULL = â���� ���ڴ�. �ٸ� �� ������ Ǯ��ũ�� ����
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // �Ŵ��� ����ü ����

	//glfwSetWindowTitle(window, u8"���� openGL������"); // �ѱ��� u8�� �ٿ������.
	glfwSetKeyCallback(window, key_callback);


	/* Make the window's context current */
	// glew = Helper
	// glewExperimental = GL_TRUE; // ������ ��. �ھ����������� ���� TRUE �� �ֵ��� ������µ� ����� �ڵ����� ��.
	GLenum err = glewInit(); // ���� �ʱ�ȭ
	if (err != GLEW_OK)
	{
		//Problem: glewInit failed, something is seriously wrong.
		std::cout << "glewInit failed, aborting." << std::endl;
		return 0;
	}
	glfwSwapInterval(1);  //enable vsync = ����� �ֻ����� ����ȭ

	printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

	while (!glfwWindowShouldClose(window))
	{
		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	else if (key == GLFW_KEY_R && action == GLFW_PRESS)
		printf("r");
}
