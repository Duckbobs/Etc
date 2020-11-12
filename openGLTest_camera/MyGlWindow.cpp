#include "MyGlWindow.h"
#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp> // translate()
#include <glm/gtc/type_ptr.hpp> // value_ptr()

#include <glm/gtx/string_cast.hpp> // to_string()


GLfloat cube_vertices[] = {
	// front
	-1.0, -1.0, 1.0,
	1.0, -1.0, 1.0,
	1.0, 1.0, 1.0,
	-1.0, 1.0, 1.0,
	// back
	-1.0, -1.0, -1.0,
	1.0, -1.0, -1.0,
	1.0, 1.0, -1.0,
	-1.0, 1.0, -1.0,
};
GLfloat cube_colors[] = {
	// front colors
	1.0, 0.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 0.0, 1.0,
	1.0, 1.0, 1.0,
	// back colors
	1.0, 0.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 0.0, 1.0,
	1.0, 1.0, 1.0,
};
const GLushort cube_elements[] = {
	0, 1, 2,             2, 3, 0,             1, 5, 6,
	6, 2, 1,             7, 6, 5,             5, 4, 7,
	4, 0, 3,             3, 7, 4,             4, 5, 1,
	1, 0, 4,             3, 2, 6,             6, 7, 3,
};
ColorCube::ColorCube() {
	ColorCube::setup();
}
void ColorCube::setup() {

	// 1. vao ����
	glGenVertexArrays(1, &vaoHandle); // vao 1 �� ����
	glBindVertexArray(vaoHandle); // = �� vao�� ������  �ҰŴ�..activate..picking..

		// 2. vbo ����
		glGenBuffers(1, &vboVertex); // vbo 1�� ����
		glBindBuffer(GL_ARRAY_BUFFER, vboVertex); // �� vbo�� �����Ѵ�.
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
		glVertexAttribPointer( // position
			0, // id ��ȣ : 0 (position)
			3, // �� vertex �� ����Ÿ ��
			GL_FLOAT, // ����Ÿ Ÿ��
			GL_FALSE, // ����ȭ(normalize) �Ǿ��ֳ�? GL_FALSE = �ȵ��ִ�
			0, // �� vertex �� ũ��
			0 //  offset
		);
		glEnableVertexAttribArray(0); // id 0�� enable
		// 2. vbo ����
		glGenBuffers(1, &vbo_cube_colors); // vbo 1�� ����
		glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colors); // �� vbo�� �����Ѵ�.
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colors), cube_colors, GL_STATIC_DRAW);
		glVertexAttribPointer( // position
			1, // id ��ȣ : 0 (position)
			3, // �� vertex �� ����Ÿ ��
			GL_FLOAT, // ����Ÿ Ÿ��
			GL_FALSE, // ����ȭ(normalize) �Ǿ��ֳ�? GL_FALSE = �ȵ��ִ�
			0, // �� vertex �� ũ��
			0 //  offset
		);
		glEnableVertexAttribArray(1); // id 1�� enable
		// 2. ibo ����
		glGenBuffers(1, &ibo_cube_elements); // ibo 1�� ����
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements); // �� vbo�� �����Ѵ�.
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);
		//glEnableVertexAttribArray(1); // id 1�� enable

	// 3. vao close
	glBindVertexArray(0); // ���ڸ� 0 ������ VAO close
}
void MyGlWindow::setupBuffer()
{
	shaderProgram = new ShaderProgram();
	shaderProgram->initFromFiles("simpler.vert", "simpler.frag"); // ���̴� ����
	m_cube = new ColorCube();
}
void MyGlWindow::initialize() {
	MyGlWindow::setupBuffer();

	glm::vec3 eye = m_viewer->getViewPoint();
	glm::vec3 look = m_viewer->getViewCenter();
	glm::vec3 up = m_viewer->getUpVector();
	view = lookAt(eye, look, up);

	const glm::mat4 mat1 = glm::mat4(1.0f);
	const float rad1 = glm::radians(40.0f);
	const glm::vec3 vec = glm::vec3(1, 1, 1);
	glm::mat4 rot = glm::rotate(mat1, rad1, vec);
	glm::mat4 trans = glm::translate(mat1, vec);
	glm::mat4 scale = glm::scale(mat1, vec);
	glm::mat4 model_matrix = trans * scale * rot;  //������ �� (�ݵ�� �̷���  �ؾ��ϴ� ���� �ƴ�)
	model = model_matrix;

	projection = perspective(45.0f, 1.0f * width / height, 0.1f, 500.0f);

	//shaderProgram->addUniform("model");
	//shaderProgram->addUniform("view");
	//shaderProgram->addUniform("proj");
	mvp = projection * view * model;
	shaderProgram->addUniform("mvp");
}
MyGlWindow::MyGlWindow(int w, int h)
{
	width = w;
	height = h;
	m_cube = 0;


	static float DEFAULT_VIEW_POINT[3] = { 5, 5, 5 };
	static float DEFAULT_VIEW_CENTER[3] = { 0, 0, 0 };
	static float DEFAULT_UP_VECTOR[3] = { 0, 1, 0 };
	glm::vec3 viewPoint(DEFAULT_VIEW_POINT[0], DEFAULT_VIEW_POINT[1], DEFAULT_VIEW_POINT[2]);
	glm::vec3 viewCenter(DEFAULT_VIEW_CENTER[0], DEFAULT_VIEW_CENTER[1], DEFAULT_VIEW_CENTER[2]);
	glm::vec3 upVector(DEFAULT_UP_VECTOR[0], DEFAULT_UP_VECTOR[1], DEFAULT_UP_VECTOR[2]);

	float aspect = (w / (float)h);
	m_viewer = new Viewer(viewPoint, viewCenter, upVector, 45.0f, aspect);

	initialize();
}


void ColorCube::draw() {
	glBindVertexArray(vaoHandle);
	int size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
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
	glm::mat4 mview = view * model;

	shaderProgram->use(); // shader ȣ��

	glUniformMatrix4fv(shaderProgram->uniform("mvp"), 1, GL_FALSE, glm::value_ptr(mvp));

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

