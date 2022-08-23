#include "Vec.h"
#include <vector>
#include "Particle.h"

struct GLFWwindow;
struct ImGuiIO;
struct ImVec4;

class Engine {
public:
	int renderFrame();
	void setParticles(std::vector<Particle>& particles);
	Engine(std::vector<Particle>& particles);
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
	std::vector<Particle>* particles;
};