#include "Vec.h"
#include <vector>

#include "ParticleSystem.h"

struct GLFWwindow;
struct GLFWmonitor;
struct ImGuiIO;
struct ImVec4;

class Engine {
public:
	int renderFrame();
	Engine(ParticleSystem* system);
	~Engine();
private:
	int initParticleShader();
	int initParticleBuffer();
	int renderParticles();
	GLFWwindow* window;
	GLFWmonitor* monitor;
	ImGuiIO* io;
	Vec<4> clearColor;
	int renderWidth, renderHeight, windowWidth, windowHeight, screenWidth, screenHeight;
	unsigned int vertexShader, fragmentShader, shaderProgram;
	unsigned int VBO, VAO;
	ParticleSystem* system;
	bool fullscreen;
};