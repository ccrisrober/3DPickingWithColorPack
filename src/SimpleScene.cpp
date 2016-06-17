#include "SimpleScene.h"

SimpleScene::SimpleScene(int w, int h) : IScene(w, h) { }
SimpleScene::~SimpleScene() {
}

void SimpleScene::initScene() {
	compileAndLinkShaders();
	glClearColor(0.5f,0.5f,0.5f,1.0f);

	cube = new Mesh("../models/cube.obj");

	for(int i = 0; i < 23; i++) {
		float r = ((float)( std::rand() % 1000)) * 0.001;
		float g = ((float)( std::rand() % 1000)) * 0.001;
		float b = ((float)( std::rand() % 1000)) * 0.001;
		float a = ((float)( std::rand() % 1000)) * 0.001;
		if(a >= 0.85) {
			a = 0.4;
		}
		colors.push_back(glm::vec4(r, g, b, a));
	}

	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}


void SimpleScene::update( float t ) {
	angle += 0.1 * t;
}

#define NUM 15
void SimpleScene::draw(Camera* camera) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	view = camera->GetViewMatrix();
	projection = camera->GetProjectionMatrix();

	progPick.use();
	
	int n = 0;
	for (int i = -NUM; i < NUM; i++) {
		for (int j = -NUM; j < NUM; j++) {
			for (int k = -NUM; k < NUM; k++) {
				vec4 kd = colors[n++ % colors.size()];
				progPick.send_uniform("id", n);
				model = glm::translate(mat4(1.0f), vec3(i * 2, j * 2, k * 2));
				model = glm::rotate(model, glm::radians(-angle), glm::vec3(0, 1, 0));
				model = glm::scale(model, glm::vec3(size));
				progPick.send_uniform("selected", selected == n);
				updateMatrices();
				cube->render();
			}
		}
	}
	progPick.unuse();
	/**/
	if (pick) {
		GLubyte color[4];
		glReadPixels(x, WINDOW_HEIGHT - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);

		glm::vec4 cc;
		cc.r = color[0];
		cc.g = color[1];
		cc.b = color[2];
		cc.a = color[3];

		unsigned int v = (unsigned int)(cc.b + cc.g * 256.0 + cc.r * 256.0 * 256.0);
		if (v != 8355711 && v != 16711680) {
			std::cout << x << " - " << y << ". ID: " << v << std::endl;
			selected = v;
		}
		pick = false;
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/**/

	/**/
	prog.use();
	n = 0;
	for (int i = -NUM; i < NUM; i++) {
		for (int j = -NUM; j < NUM; j++) {
			for (int k = -NUM; k < NUM; k++) {
				vec4 kd = colors[n++ % colors.size()];
				prog.send_uniform("Kd", kd);
				model = glm::translate(mat4(1.0f), vec3(i * 2, j * 2, k * 2));
				model = glm::rotate(model, glm::radians(-angle), glm::vec3(0, 1, 0));
				model = glm::scale(model, glm::vec3(size));
				prog.send_uniform_b("selected", selected == n);
				updateMatrices();
				cube->render();
			}
		}
	}
	prog.unuse();
	/**/
}

void SimpleScene::updateMatrices() {
	glm::mat4 modelView = view * model;

	progPick.send_uniform("modelViewProj", projection * modelView);

	prog.send_uniform("normal", glm::mat3(glm::vec3(modelView[0]), glm::vec3(modelView[1]), glm::vec3(modelView[2])));
	prog.send_uniform("modelView", modelView);
	prog.send_uniform("modelViewProj", projection * modelView);
}

void SimpleScene::resize(int w, int h) {
	glViewport(0, 0, w, h);
	width = w;
	height = h;
}

void SimpleScene::compileAndLinkShaders() {
	prog.load("../shader/shader.vert", GL_VERTEX_SHADER);
	prog.load("../shader/shader.frag", GL_FRAGMENT_SHADER);
	prog.compile_and_link();
	prog.add_uniform("normal");
	prog.add_uniform("modelView");
	prog.add_uniform("modelViewProj");
	prog.add_uniform("Kd");
	prog.add_uniform("selected");

	progPick.load("../shader/pick.vert", GL_VERTEX_SHADER);
	progPick.load("../shader/pick.frag", GL_FRAGMENT_SHADER);
	progPick.compile_and_link();
	progPick.add_uniform("modelViewProj");
	progPick.add_uniform("id");
	progPick.add_uniform("selected");
}