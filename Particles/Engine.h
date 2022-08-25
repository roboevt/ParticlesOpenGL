#include "Vec.h"
#include <vector>

#include "ParticleSystem.h"

struct GLFWwindow;
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
	ImGuiIO* io;
	Vec<4> clearColor;
	int screenWidth;
	int screenHeight;
	unsigned int vertexShader, fragmentShader, shaderProgram;
	unsigned int VBO, VAO;
	ParticleSystem* system;
};