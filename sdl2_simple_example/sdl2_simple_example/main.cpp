#include <GL/glew.h>
#include <chrono>
#include <thread>
#include <exception>
#include <glm/glm.hpp>
#include <SDL2/SDL_events.h>
#include "MyWindow.h"
#include "json/json.h"
#include "fstream"
using namespace std;

using hrclock = chrono::high_resolution_clock;
using u8vec4 = glm::u8vec4;
using ivec2 = glm::ivec2;
using vec3 = glm::dvec3;

static const ivec2 WINDOW_SIZE(512, 512);
static const unsigned int FPS = 60;
static const auto FRAME_DT = 1.0s / FPS;

static void init_openGL() {
	glewInit();
	if (!GLEW_VERSION_3_0) throw exception("OpenGL 3.0 API is not available.");
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.5, 0.5, 0.5, 1.0);
}

static void draw_triangle(const u8vec4& color, const vec3& center, double size) {
	glColor4ub(color.r, color.g, color.b, color.a);
	glBegin(GL_TRIANGLES);
	glVertex3d(center.x, center.y + size, center.z);
	glVertex3d(center.x - size, center.y - size, center.z);
	glVertex3d(center.x + size, center.y - size, center.z);
	glEnd();
}

static void display_func(Json::Value triangle) {
	draw_triangle(u8vec4(triangle["color"][0].asInt(), triangle["color"][1].asInt(), triangle["color"][2].asInt(), triangle["color"][3].asInt()),
		vec3(triangle["center"][0].asFloat(), triangle["center"][1].asFloat(), triangle["center"][2].asFloat()), triangle["size"].asFloat());
}

static bool processEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			return false;
		}
	}
	return true;
}

int main(int argc, char** argv) {
	MyWindow window("SDL2 Simple Example", WINDOW_SIZE.x, WINDOW_SIZE.y);

	init_openGL();

	Json::Value config;
	ifstream doc("config.json", ifstream::binary);
	Json::CharReaderBuilder readerBuilder;
	string errs;
	Json::parseFromStream(readerBuilder, doc, &config, &errs);

	// Probant el color
	// printf("%d", config["trian"]["color"][1].asInt());

	while (processEvents()) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		const auto t0 = hrclock::now();
		display_func(config["trian"]);
		display_func(config["trian2"]);
		display_func(config["trian3"]);
		window.swapBuffers();
		const auto t1 = hrclock::now();
		const auto dt = t1 - t0;
		if(dt<FRAME_DT) this_thread::sleep_for(FRAME_DT - dt);
	}

	return 0;
}