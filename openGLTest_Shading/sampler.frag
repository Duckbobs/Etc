#version 450 core

//in vec3 color;
//out vec4 fragColor;

in vec3 fColor;
out vec4 fragColor;
// ���� �߿��� ���� �ּ� �ϳ��� vec4�� ���� out�� ����
void main()
{
	//fragColor = vec4(1, 0, 1, 1);
	//fragColor = vec4(color, 1);
	fragColor = vec4(fColor, 1.0f); // 1.0f ���İ�
}