#include <fstream>
#include <iostream>
#include <glad/glad.h>
#include "shader.h"

#define SHADER 0
#define SHADER_PROGRAM 1

Shader::Shader(std::string vert_path, std::string frag_path)
{
    std::string vert_shader_src = read_from_file(vert_path);
    unsigned int vert_shader;
    vert_shader = glCreateShader(GL_VERTEX_SHADER);
    const char * vert_shader_csrc = vert_shader_src.c_str();
    glShaderSource(vert_shader, 1, &vert_shader_csrc, nullptr);
    glCompileShader(vert_shader);
    check_error(SHADER, vert_shader);

    std::string frag_shader_src = read_from_file(frag_path);
    unsigned int frag_shader;
    frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    const char * frag_shader_csrc = frag_shader_src.c_str();
    glShaderSource(frag_shader, 1, &frag_shader_csrc, nullptr);
    glCompileShader(frag_shader);
    check_error(SHADER, frag_shader);

    id = glCreateProgram();
    glAttachShader(id, vert_shader);
    glAttachShader(id, frag_shader);
    glLinkProgram(id);
    check_error(SHADER_PROGRAM);

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);
}

void Shader::use() { glUseProgram(id); }

unsigned int Shader::get_id() { return id; }

std::string Shader::read_from_file(std::string path)
{
    std::ifstream is(path);
    std::string data = "\0";
        
    if (is) getline(is, data, '\0');
    else std::cout << "Error: can't find " << path << "\n";
        
    is.close();
    return data;
}

void Shader::check_error(int type, unsigned int shader_id)
{
    int success;
    char logs[512];
    std::string title;

    if (type == SHADER_PROGRAM)
    {
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        title = "Shader program link error:\n";
    }
    else
    {
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
        title = "Shader compilation error:\n";
    }

    if (!success)
    {
        glGetShaderInfoLog(shader_id, 512, nullptr, logs);
        std::cout << title << logs << "\n";
    }
}

