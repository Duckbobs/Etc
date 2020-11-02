#version 450 core

// x, y, z, w = vec4
layout(location = 0) in vec4 vertexPosition; // id 0
// R, G, B = vec3
layout(location = 1) in vec3 vertexColor; // id 1
//cpp �� �ִ� id�� ���� ��ġ�ž��Ѵ�.

uniform mat4 model; // ������ app���� ���� ���� ��..
// �������� ���� read�� ����


//out vec3 color;
out vec3 fColor; // ���� frag���� �ؾ���. ���� ����. = �����׸�Ʈ ���̴��� ���� ����Ÿ
void main()
{
	//color = vec3(1, 0, 0);
	fColor = vertexColor;

	//�ﰢ���� �Űܺ���.. (0.2, 0.2) ��ŭ : translation ���
	//mat4 model = { {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0.2f, 0.2f, 0, 1} };
	
	
	// gl_Position(vec4) ���� (NDC ��ǥ��)
	// vertexPosition : local, model, object ��ǥ��
	// gl_Position = NDC, Clipped ��ǥ��,
	// ���⿡ ���� �����ϴ� ���� perspective division ����...
	gl_Position = model * vertexPosition;


}