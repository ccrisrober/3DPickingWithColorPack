#ifndef _SIMPLE_SCENE_H
#define _SIMPLE_SCENE_H

#include "IScene.h"
#include "SimpleGLShader.h"
#include "objects/Cube.h"
#include "objects/MonkeyHead.h"
#include "objects/Mesh.h"

#include "glm/glm.hpp"
#include <vector>

class SimpleScene : public IScene {
private:
	SimpleGLShader prog;
	SimpleGLShader progPick;

	RenderObject *cube;

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

    void updateMatrices();
    void compileAndLinkShaders();

	std::vector<glm::vec4> colors;

	float angle = 0.0f;

public:
	float size = 1.0f;
	bool pick = false;
	GLint x;
	GLint y;
	SimpleScene(int w, int h);
	~SimpleScene();

    void initScene();
    void update( float t );
	void draw(Camera* camera);
    void resize(int, int);

	int selected = 0;

};

#endif // _SIMPLE_SCENE_H
