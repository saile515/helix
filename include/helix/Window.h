#pragma once

#include <string>

typedef unsigned int GLuint;
class GLFWwindow;

class Window {
    int width_;
    int height_;
    std::string name_;
    GLFWwindow* window_;
    GLuint vertex_buffer_;
    GLuint vertex_shader_;
    GLuint fragment_shader_;
    GLuint shader_program_;

public:
    std::string& name();
    int& width();
    int& height();

    Window(std::string name, int width, int height);

    int initialize();
    void update();
    void terminate();

    bool should_update();
};