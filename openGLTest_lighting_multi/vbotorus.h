#ifndef VBOTORUS_H
#define VBOTORUS_H

#include "GL/glew.h"
#include <glm/mat4x4.hpp>


class VBOTorus 
{
private:
    GLuint VBO_position, VBO_normal, IBO;
    unsigned int vaoHandle;
    int faces, rings, sides;
	
	GLuint vbo_cube_vertices, vbo_cube_normals, ibo_cube_elements;
    void generateVerts(float * , float * , unsigned int *,
                       float , float);

public:
    VBOTorus(float, float, int, int);
	virtual ~VBOTorus();
	void draw() const;
};

#endif // VBOTORUS_H
