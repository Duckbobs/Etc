#include "CheckeredFloor.h"


int GLsize = 0;
CheckeredFloor::CheckeredFloor(float size, int nSquares) {
	CheckeredFloor::setup(size, nSquares);
}
void CheckeredFloor::setup(float size, int nSquares) {

	floorColor1[0] = 200;
	floorColor1[1] = 200;
	floorColor1[2] = 200;

	floorColor2[0] = 0;
	floorColor2[1] = 0;
	floorColor2[2] = 0;


	std::vector <glm::vec3> vlists;
	std::vector <glm::vec3> clists;

	// parameters:
	float maxX = size / 2, maxY = size / 2;
	float minX = -size / 2, minY = -size / 2;

	int x, y, v[3], i;
	float xp, yp, xd, yd;
	v[2] = 0;
	xd = (maxX - minX) / ((float)nSquares);
	yd = (maxY - minY) / ((float)nSquares);


	for (x = 0, xp = minX; x < nSquares; x++, xp += xd) {
		for (y = 0, yp = minY, i = x; y < nSquares; y++, i++, yp += yd) {
			if (i % 2 == 1) {
				clists.push_back(glm::vec3(floorColor1[0], floorColor1[1], floorColor1[2]));
				clists.push_back(glm::vec3(floorColor1[0], floorColor1[1], floorColor1[2]));
				clists.push_back(glm::vec3(floorColor1[0], floorColor1[1], floorColor1[2]));
				clists.push_back(glm::vec3(floorColor1[0], floorColor1[1], floorColor1[2]));
				clists.push_back(glm::vec3(floorColor1[0], floorColor1[1], floorColor1[2]));
				clists.push_back(glm::vec3(floorColor1[0], floorColor1[1], floorColor1[2]));
			}
			else {
				clists.push_back(glm::vec3(floorColor2[0], floorColor2[1], floorColor2[2]));
				clists.push_back(glm::vec3(floorColor2[0], floorColor2[1], floorColor2[2]));
				clists.push_back(glm::vec3(floorColor2[0], floorColor2[1], floorColor2[2]));
				clists.push_back(glm::vec3(floorColor2[0], floorColor2[1], floorColor2[2]));
				clists.push_back(glm::vec3(floorColor2[0], floorColor2[1], floorColor2[2]));
				clists.push_back(glm::vec3(floorColor2[0], floorColor2[1], floorColor2[2]));

			}
			vlists.push_back(glm::vec3(xp, -0.001f, yp));
			vlists.push_back(glm::vec3(xp, -0.001f, yp + yd));
			vlists.push_back(glm::vec3(xp + xd, -0.001f, yp + yd));

			vlists.push_back(glm::vec3(xp, -0.001f, yp));
			vlists.push_back(glm::vec3(xp + xd, -0.001f, yp + yd));
			vlists.push_back(glm::vec3(xp + xd, -0.001f, yp));
		}
	}

	// 1. vao ����
	glGenVertexArrays(1, &vaoHandle); // vao 1 �� ����
	glBindVertexArray(vaoHandle); // = �� vao�� ������  �ҰŴ�..activate..picking..

	GLsize = vlists.size() * 3;
	// 2. vbo ����
	glGenBuffers(1, &vboVertex); // vbo 1�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vboVertex); // �� vbo�� �����Ѵ�.
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vlists.size() * 3, vlists.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(
		0, // id ��ȣ
		3, // �� vertex �� ����Ÿ ��
		GL_FLOAT, // ����Ÿ Ÿ��
		GL_FALSE, // ����ȭ(normalize) �Ǿ��ֳ�? GL_FALSE = �ȵ��ִ�
		0, // �� vertex �� ũ��
		0 //  offset
	);
	glEnableVertexAttribArray(0); // id 0�� enable
	// 2. vbo ����
	glGenBuffers(1, &vboColor); // vbo 1�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vboColor); // �� vbo�� �����Ѵ�.
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * clists.size() * 3, clists.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(
		1, // id ��ȣ
		3, // �� vertex �� ����Ÿ ��
		GL_FLOAT, // ����Ÿ Ÿ��
		GL_FALSE, // ����ȭ(normalize) �Ǿ��ֳ�? GL_FALSE = �ȵ��ִ�
		0, // �� vertex �� ũ��
		0 // offset
	);
	glEnableVertexAttribArray(1); // id 1�� enable

// 3. vao close
	glBindVertexArray(0); // ���ڸ� 0 ������ VAO close
}

void CheckeredFloor::draw() {
	glBindVertexArray(vaoHandle);
	glDrawArrays(GL_TRIANGLES, 0, GLsize);
}
