#pragma once

#include <glad/glad.h>

#include <file_utils.hh>

#include <string>
#include <iostream>

class Shader {
public:
    Shader(const std::string& vpath, const std::string fpath) {
	prog_id = glCreateProgram();
	std::string vshcode = read_file(vpath);
	std::string fshcode = read_file(fpath);
	uint32_t vs = compile_shader(GL_VERTEX_SHADER, vshcode);
	uint32_t fs = compile_shader(GL_FRAGMENT_SHADER, fshcode);
	glAttachShader(prog_id, vs);
	glAttachShader(prog_id, fs);
	glLinkProgram(prog_id);
	glValidateProgram(prog_id);
	
	int result;
	glGetProgramiv(prog_id, GL_LINK_STATUS, &result);
	if (result == GL_FALSE)
        {
	    int length;
	    glGetProgramiv(prog_id, GL_INFO_LOG_LENGTH, &length);
	    char message[length];
	    glGetProgramInfoLog(prog_id, length, &length, message);
	    std::cout << "Shid happend! :DDDDD\n" << message << std::endl;
        }
        glDeleteShader(vs);
        glDeleteShader(fs);
    }
    void set_bool(const std::string& name, const bool& value) const { glUniform1i(glGetUniformLocation(prog_id, name.c_str()), value); }

    void set_int(const std::string& name, const int& value) const { glUniform1i(glGetUniformLocation(prog_id, name.c_str()), value); }
    
    void set_float(const std::string& name, const float& value) const { glUniform1f(glGetUniformLocation(prog_id, name.c_str()), value); }

    void use() { glUseProgram(prog_id); }


private:
    uint32_t prog_id;

    static unsigned int compile_shader(unsigned int type, const std::string& source) {
	unsigned int shader_id = glCreateShader(type);
        const char* src = source.c_str();
	
	glShaderSource(shader_id, 1, &src, nullptr);
	glCompileShader(shader_id);
	
	int result;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
	    int length;
	    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
	    char message[length];
	    glGetShaderInfoLog(shader_id, length, &length, message);
            std::cout << "Shid happend! :DDDDDD\n" << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << " shader compiling failure\n" << message << std::endl;
	    glDeleteShader(shader_id);
	    return 0;
	}
	return shader_id;
    }
};
