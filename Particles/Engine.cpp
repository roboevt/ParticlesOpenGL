#include "Engine.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <iostream>

static void glfw_error_callback(int error, const char* description){
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

Engine::Engine(ParticleSystem* system) : system(system), windowWidth(1280), windowHeight(720), fullscreen(false), frames(0) {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) std::cout << "Error with glfwInit" << std::endl;

    const char* glsl_version = "#version 430";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    window = glfwCreateWindow(windowWidth, windowHeight, "Particles", NULL, NULL);
    if (window == NULL) std::cout << "Error creating window" << std::endl;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0); // No Vsync
    // glfwSetWindowUserPointer(window, reinterpret_cast<void *>(this)); // Not currently used but helpful for callbacks

    if (!glewInit())  std::cout << "Error with glewInit" << std::endl;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    clearColor = Vec<4>({ 0.1f, 0.2f, 0.1f, 1.0f });
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_POINT_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    initParticleShader();
    initParticleBuffer();
}

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
// "layout (location = 1) in float size;"
"out vec2 pos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"   gl_PointSize = 10;\n"
"   pos = aPos.xy;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"in vec2 pos;\n"
"void main()\n"
"{\n"
"   vec2 fragPos = gl_PointCoord - vec2(0.5f, 0.5f);"
"   float distance = sqrt(dot(fragPos, fragPos));\n"
"   color = distance < 0.4f ? vec4(1) : vec4(0);"
"}\n\0";

int Engine::initParticleShader() {
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgram);
    return 0;
}

int Engine::initParticleBuffer() {
    glGenVertexArrays(1, &VAO_P);
    glGenBuffers(1, &VBO_P);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO_P);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_P);
    glBufferData(GL_ARRAY_BUFFER, system->getParticles().size() * sizeof(Particle), 
        system->getParticles().data(), GL_DYNAMIC_DRAW);

    // Position attribute
    glEnableVertexAttribArray(0);  // Neccesary?
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)0);

    // Set size (now hardcoded to 10)
    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(2 * sizeof(Vec2)));

    return 0;
}

int Engine::renderFrame() {
    const auto start = std::chrono::high_resolution_clock::now();
    if (glfwWindowShouldClose(window)) return 1;

    glfwPollEvents();

    int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if (state == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        // system->addParticle(Particle(Vec2(xpos / windowWidth * 2 - 1, 1 - ypos / windowHeight * 2), Vec2(0, 0)));
        system->setAttractor(xpos / windowWidth * 2 - 1, 1 - ypos / windowHeight * 2);
    } else {
        system->removeAttractor();
    }

    state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
    if (state == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        system->setRepulsor(xpos / windowWidth * 2 - 1, 1 - ypos / windowHeight * 2);
    } else {
        system->removeRepulsor();
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    {
        ImGui::Begin("FPS");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        if (ImGui::Button("Toggle Fullscreen")) {
            fullscreen = !fullscreen;
            const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            if (fullscreen) {
                const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
                screenWidth = mode->width;
                screenHeight = mode->height;
            }

            glfwSetWindowMonitor(window, fullscreen ? glfwGetPrimaryMonitor() : NULL, 0, 30, 
                fullscreen ? screenWidth : windowWidth, 
                fullscreen ? screenHeight : windowHeight, GLFW_DONT_CARE);
        }
        float G = system->getG();
        ImGui::SliderFloat("G", &G, 0.0f, 10.0f);
        system->setG(G);
        int subSteps = system->getSubSteps();
        ImGui::SliderInt("Substeps", &subSteps, 1, 64);
        system->setSubSteps(subSteps);
        ImGui::End();
    }

    //Render setup
    ImGui::Render();
    glfwGetFramebufferSize(window, &renderWidth, &renderHeight);
    glViewport(0, 0, renderWidth, renderHeight);
    glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
    glClear(GL_COLOR_BUFFER_BIT);
    
    //Render dynamic objects here:
    renderParticles();
    // renderTrail();


    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);

    frames++;

    const auto end = std::chrono::high_resolution_clock::now();
	const std::chrono::duration<float> elapsed = end - start;
	// std::cout << "Render Elapsed time: " << elapsed.count() * 1000 << "ms" << std::endl;

    return 0;
}

int Engine::renderParticles() {
    // update shader uniform
    glBindVertexArray(VAO_P);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_P);
    glBufferData(GL_ARRAY_BUFFER, system->getParticles().size() * sizeof(Particle), 
        system->getParticles().data(), GL_DYNAMIC_DRAW);

    // render the triangle
    glDrawArrays(GL_POINTS, 0, system->getParticles().size());
    return 0;
}

Engine::~Engine() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}