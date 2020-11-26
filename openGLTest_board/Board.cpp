#include "Board.h"

GLfloat positions[] = {
-0.5f, -0.5f, 0.5f, 1.0f,  //0
-0.5f, 0.5f, 0.5f, 1.0f, //1
0.5f, 0.5f, 0.5f, 1.0f, //2
0.5f, -0.5f, 0.5f, 1.0f, //3
-0.5f, -0.5f, -0.5f, 1.0f, //4
-0.5f, 0.5f, -0.5f, 1.0f, //5
0.5f, 0.5f, -0.5f, 1.0f, //6
0.5f, -0.5f, -0.5f, 1.0f //7
};
GLfloat positions_color[] = {
	1.0, 0.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 0.0, 1.0,
	1.0, 1.0, 1.0,
};

Board::Board() {
	Board::setup();
}
void Board::setup() {

	// 1. vao ����
	glGenVertexArrays(1, &vaoHandle); // vao 1 �� ����
	glBindVertexArray(vaoHandle); // = �� vao�� ������  �ҰŴ�..activate..picking..

	// 2. vbo ����
	glGenBuffers(1, &vboVertex); // vbo 1�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vboVertex); // �� vbo�� �����Ѵ�.
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), &positions, GL_STATIC_DRAW);
	glVertexAttribPointer(
		0, // id ��ȣ
		4, // �� vertex �� ����Ÿ ��
		GL_FLOAT, // ����Ÿ Ÿ��
		GL_FALSE, // ����ȭ(normalize) �Ǿ��ֳ�? GL_FALSE = �ȵ��ִ�
		0, // �� vertex �� ũ��
		0 //  offset
	);
	glEnableVertexAttribArray(0); // id 2�� enable
	// 2. vbo ����
	glGenBuffers(1, &vboVertex); // vbo 1�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vboVertex); // �� vbo�� �����Ѵ�.
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions_color), &positions_color, GL_STATIC_DRAW);
	glVertexAttribPointer(
		1, // id ��ȣ
		3, // �� vertex �� ����Ÿ ��
		GL_FLOAT, // ����Ÿ Ÿ��
		GL_FALSE, // ����ȭ(normalize) �Ǿ��ֳ�? GL_FALSE = �ȵ��ִ�
		0, // �� vertex �� ũ��
		0 //  offset
	);
	glEnableVertexAttribArray(1); // id 2�� enable

	// 3. vao close
	glBindVertexArray(0); // ���ڸ� 0 ������ VAO close
}

void Board::draw() {
	glBindVertexArray(vaoHandle);
	int size;
	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size / sizeof(float), GL_FLOAT, 0);
}
