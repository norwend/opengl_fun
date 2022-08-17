#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.hh>
#include <camera.hh>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define W_WIDTH 1280
#define W_HEIGHT 720


void process_input(GLFWwindow* win);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

glm::vec3 camera_pos(0.0, 1.0, 3.0);
glm::vec3 camera_front(0.0, 0.0, 1.0);

Camera camera(W_WIDTH, W_HEIGHT, camera_pos, camera_front);

void process_input(GLFWwindow* win) {
    if (glfwGetKey(win, GLFW_KEY_Q) == GLFW_PRESS)
	glfwSetWindowShouldClose(win, true);

    if (glfwGetKey(win, GLFW_KEY_B) == GLFW_PRESS) {
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
	
    if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

struct Triangle {
    unsigned int VAO, VBO;
    Triangle(std::vector<float> verts) {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW); 

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0); 

	glBindVertexArray(0);
    }

    ~Triangle() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
    }
};


uint32_t create_texture(const std::string& file_name) {
    int width, height, nr_channels;
    u_char* tex_data = stbi_load(file_name.c_str(), &width, &height, &nr_channels, 0);
    uint32_t texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (tex_data) {
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
	glGenerateMipmap(GL_TEXTURE_2D);
    }
    else { std::cout << "Failed to load texture!" << std::endl; }

    stbi_image_free(tex_data);
    return texture;
}

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
    
    std::vector<float> vertices = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    
    std::vector<glm::vec3> cube_positions = {
	glm::vec3( 0.0f,  0.0f,   0.0f),
	glm::vec3( 2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f,  -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3( 2.4f, -0.4f,  -3.5f),
	glm::vec3(-1.7f,  3.0f,  -7.5f),
	glm::vec3( 1.3f, -2.0f,  -2.5f),
	glm::vec3( 1.5f,  2.0f,  -2.5f),
	glm::vec3( 1.5f,  0.2f,  -1.5f),
	glm::vec3(-1.3f,  1.0f,  -1.5f)
    };

    std::vector<float> floor_verts = {
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
	1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	
    };
    
    
    // Triangle t1(vert0);
    Triangle t1(vertices);
    Triangle t2(floor_verts);
    Shader sh("shaders/test.vert", "shaders/test.frag");
    
    stbi_set_flip_vertically_on_load(true); 
    
    uint32_t texture0 = create_texture("textures/kagome.png");
    uint32_t texture1 = create_texture("textures/smile.png");
    uint32_t texture2 = create_texture("textures/kagome2.png");
    uint32_t texture3 = create_texture("textures/cat.png");
    
    sh.use();
    sh.set_int("texture0", 0);
    sh.set_int("texture1", 1);

    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    
    glm::mat4 model(1.0f);
    model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    
    glm::mat4 view(1.0f);
    view = camera.get_view();
    
    glm::mat4 projection(1.0f);
    projection = glm::perspective(glm::radians(camera.get_zoom()), 16.0f/9.0f, 0.01f, 100.0f);
    
    glEnable(GL_DEPTH_TEST);
    
    std::cout << glGetString(GL_VERSION) << std::endl;
    float offset = -0.2f;

    float last_frame = glfwGetTime();
    float delta_time = 0;

    glm::mat4 floor_model(1.0f);
    floor_model = glm::scale(floor_model, glm::vec3(50.0f, 1.0f, 50.0f));
    floor_model = glm::rotate(floor_model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    
    while(!glfwWindowShouldClose(window)) {

	process_input(window);
	
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	
	
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
	    offset -= 0.005f;
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
	    offset += 0.005f;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
	    camera.move(FORWARD, delta_time);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
	    camera.move(BACKWARD, delta_time);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
	    camera.move(LEFT, delta_time);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
	    camera.move(RIGHT, delta_time);
	} 
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
	    camera.move(DOWN, delta_time);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
	    camera.move(UP, delta_time);
	} 
	
	view = camera.get_view();
	projection = glm::perspective(glm::radians(camera.get_zoom()), 16.0f/9.0f, 0.01f, 100.0f);
	glUniform1f(glGetUniformLocation(sh.get_id(), "offset"), offset);
	sh.use();
	
	
	sh.set_mat4("view", view);
	sh.set_mat4("projection", projection);

	sh.set_mat4("model", floor_model);
	glBindVertexArray(t2.VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	
	sh.set_mat4("model", model);

	glBindVertexArray(t1.VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	for(unsigned int i = 0; i < 10; i++) {
	    glm::mat4 model1 = glm::mat4(1.0f);
	    model1 = glm::translate(model1, cube_positions[i]);
	    float angle = 20.0f * i;
	    if (i % 3 == 0) angle *= glfwGetTime();
	    model1 = glm::rotate(model1, glm::radians(angle),
				 glm::vec3(1.0f, 0.3f, 0.5f));
	    sh.set_mat4("model", model1);
	    glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glfwSwapBuffers(window);
	glfwPollEvents();
	float current_frame = glfwGetTime();
	delta_time = current_frame - last_frame;
	last_frame = current_frame;
    }
    
    
    glfwTerminate();
    
    return 0;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    camera.mouse_callback(xpos, ypos);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.scroll_callback(xoffset, yoffset);
}
