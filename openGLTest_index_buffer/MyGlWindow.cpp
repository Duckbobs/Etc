#include "MyGlWindow.h"
#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp> // translate()
#include <glm/gtc/type_ptr.hpp> // value_ptr()

#include <glm/gtx/string_cast.hpp> // to_string()


const GLushort cube_elements[] = {
	0, 1, 2,             2, 3, 0,             1, 5, 6,
	6, 2, 1,             7, 6, 5,             5, 4, 7,
	4, 0, 3,             3, 7, 4,             4, 5, 1,
	1, 0, 4,             3, 2, 6,             6, 7, 3,
};
void ColorCube::setup() {
	// 1. vao ����
	glGenVertexArrays(1, &vaoHandle); // vao 1 �� ����
	glBindVertexArray(vaoHandle); // = �� vao�� ������  �ҰŴ�..activate..picking..
		// 2. vbo ����
		glGenBuffers(1, &vboVertex); // vbo 1�� ����
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboVertex); // �� vbo�� �����Ѵ�.
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0); // id 0�� enable
	// 3. vao close
	glBindVertexArray(0); // ���ڸ� 0 ������ VAO close
}
void ColorCube::draw() {
	glBindVertexArray(vaoHandle);
	int size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
}

void MyGlWindow::setupBuffer()
{
	shaderProgram = new ShaderProgram();
	shaderProgram->initFromFiles("simpler.vert", "simpler.frag"); // ���̴� ����
	m_cube = new ColorCube();
}
MyGlWindow::MyGlWindow(int w, int h)
{
	width = w;
	height = h;

	setupBuffer(); // �� �ѹ� ȣ��
}
// �� ������ ȣ�� ��
void MyGlWindow::draw() {
	// @------------------------------------------------------------------------------@
	// | Settings																	  |
	// @------------------------------------------------------------------------------@
#pragma region Settings
	glClearColor(0.2, 0.2, 0.2, 1); // ��׶��� ���� ����
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, width, height); //����Ʈ(viewport) ����
#pragma endregion

	shaderProgram->use(); // shader ȣ��

	if (!m_cube) {
		m_cube->draw();
	}

	shaderProgram->disable();
}
void MyGlWindow::resize(int w, int h) {
	width = w;
	height = h;
}
void MyGlWindow::setSize(int w, int h) {
	width = w;
	height = h;
}
glm::mat4 MyGlWindow::lookAt(glm::vec3 campos, glm::vec3 look, glm::vec3 up) {
	glm::vec3 zc = glm::normalize(campos - look);
	glm::vec3 xc = glm::normalize(glm::cross(up, zc));
	glm::vec3 yc = glm::normalize(glm::cross(zc, xc));

	glm::mat4 Mat4 = {
		{xc.x, yc.x, zc.x, 0},
		{xc.y, yc.y, zc.y, 0},
		{xc.z, yc.z, zc.z, 0},
		{0, 0, 0, 1}
	};
	glm::mat4 Mat4_1 = {
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{-campos.x, -campos.y, -campos.z, 1}
	};
	std::cout << glm::to_string(Mat4 * Mat4_1) << std::endl;
	return Mat4;
}
glm::mat4 MyGlWindow::perspective(float fovy, float aspect, float near, float far) {
	float A = -(far + near) / (far - near);
	float B = -(2 * far + near) / (far - near);
	glm::mat4 Mat4 = {
		{1 / (aspect * glm::tan(fovy / 2)), 0, 0, 1},
		{0, 1 / glm::tan(fovy / 2), 0, 1},
		{0, 0, A, -1},
		{0, 0, B, 0}
	};
	std::cout << glm::to_string(Mat4) << std::endl;
	return Mat4;
}

