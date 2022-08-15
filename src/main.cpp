#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>

#include "shader.hh"

#define W_WIDTH 1280
#define W_HEIGHT 720


void process_input(GLFWwindow* win) {
    if(glfwGetKey(win, GLFW_KEY_Q) == GLFW_PRESS)
	glfwSetWindowShouldClose(win, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


struct Triangle {
    unsigned int VAO, VBO;
    Triangle(float vertices[18]) {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), vertices, GL_STATIC_DRAW); 

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0); 

	glBindVertexArray(0);
    }

    ~Triangle() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
    }
};


int main () {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "ДА СУКАААААА", NULL, NULL);
    if (window == NULL)
    {
	std::cout << "Failed to create GLFW window" << std::endl;
	glfwTerminate();
	return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
	return -1;
    }
    glViewport(0, 0, W_WIDTH, W_HEIGHT);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    float vert0[] = {
	-0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f,
	0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 0.0f,
	0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f,
    };

    Triangle t1(vert0);
    Shader sh("shaders/test.vert", "shaders/test.frag");

    std::cout << glGetString(GL_VERSION) << std::endl;
    while(!glfwWindowShouldClose(window))
    {
	process_input(window);
	
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	sh.use();
	// float timevalue = glfwGetTime();
	// float value = std::sin(timevalue);
	// int vertcolorlocation = glGetUniformLocation(prog, "ourColor");
	// glUniform4f(vertcolorlocation, (value / 2.0f) + 0.5f, value, 1-value, 1.0f);
	
	glBindVertexArray(t1.VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glfwSwapBuffers(window);
	glfwPollEvents();
    }


    glfwTerminate();
    
    return 0;
}
