#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <string>

#define W_WIDTH 1280
#define W_HEIGHT 720

static unsigned int compile_shader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();

    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char message[length];
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Shid happend! :DDDDDD\n" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << " shader compiling failure\n" << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

static unsigned int create_shader(const std::string& vertex_shader, const std::string& fragment_shader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertex_shader);
    unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

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
    Triangle(float vertices[9]) {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(*(vertices)), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0); 

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
    }

    ~Triangle() {
	std::cout << "Calling the destructor!" << std::endl;
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
	-0.5f,  -0.25f, 0.0f,
        -0.25f,  -0.25f, 0.0f,
        -0.375f, 0.25f, 0.0f
    };

    float vert1[] = {
	0.5f,  -0.25f, 0.0f,
        0.25f, -0.25f, 0.0f,
        0.375f, 0.25f, 0.0f
    };

    std::string vsh1 = "#version 330 core\n"
	"layout (location = 0) in vec3 mPos;\n"
	"void main() {\n"
	"    gl_Position = vec4(mPos.x, mPos.y, mPos.z, 1.0);\n"
	"}\0";

    std::string fsh1 = "#version 330 core\n"
	"out vec4 color;\n"
	"void main() {\n"
	"    color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\0";

    std::string fsh2 = "#version 330 core\n"
	"out vec4 color;\n"
	"void main() {\n"
	"    color = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
	"}\0";

    unsigned int prog = create_shader(vsh1, fsh1);
    unsigned int prog2 = create_shader(vsh1, fsh2);
    Triangle t1(vert0);
    Triangle t2(vert1);
    glUseProgram(prog);

    std::cout << glGetString(GL_VERSION) << std::endl;
    while(!glfwWindowShouldClose(window))
    {
	process_input(window);
	
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(prog);
	glBindVertexArray(t1.VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindVertexArray(t2.VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	
	glfwSwapBuffers(window);
	glfwPollEvents();
    }


    glfwTerminate();
    
    return 0;
}
