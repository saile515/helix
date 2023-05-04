#include "Window.h"

#define GLFW_INCLUDE_NONE
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

static float vertices[] = {
    // first triangle
    1.0f, 1.0f, 0.0f, // top right
    1.0f, -1.0f, 0.0f, // bottom right
    -1.0f, 1.0f, 0.0f, // top left
    // second triangle
    1.0f, -1.0f, 0.0f, // bottom right
    -1.0f, -1.0f, 0.0f, // bottom left
    -1.0f, 1.0f, 0.0f // top left
};

static const char* vertex_shader_code = "#version 330 core\nlayout(location = 0) in vec3 aPos;void main(){gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);}";
static const char* fragment_shader_code = "#version 330 core\nout vec4 FragColor;void main(){FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);}";

int& Window::width()
{
    return width_;
}

int& Window::height()
{
    return height_;
}

std::string& Window::name()
{
    return name_;
}

Window::Window(std::string name, int width, int height)
{
    name_ = name;
    width_ = width;
    height_ = height;
}

int Window::initialize()
{
    if (!glfwInit())
        return -1;

    window_ = glfwCreateWindow(width_, height_, name_.c_str(), NULL, NULL);
    if (!window_) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window_);

    glfwSetWindowUserPointer(window_, this);
    glfwSetWindowSizeCallback(window_, [](GLFWwindow* window, int width, int height) {
        Window* this_ = (Window*)glfwGetWindowUserPointer(window);
        this_->width_ = width;
        this_->height_ = height;
        glViewport(0, 0, width, height);
    });

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        return -1;
    }

    glViewport(0, 0, width_, height_);

    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    vertex_shader_ = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader_, 1, &vertex_shader_code, NULL);
    glCompileShader(vertex_shader_);

    fragment_shader_ = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(fragment_shader_, 1, &fragment_shader_code, NULL);
    glCompileShader(fragment_shader_);

    shader_program_ = glCreateProgram();
    glAttachShader(shader_program_, vertex_shader_);
    glAttachShader(shader_program_, fragment_shader_);
    glLinkProgram(shader_program_);

    return 0;
}

void Window::update()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shader_program_);

    glBindVertexArray(vertex_buffer_);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (3 * sizeof(float)));

    glfwSwapBuffers(window_);
    glfwPollEvents();
}

void Window::terminate()
{
    glfwTerminate();
}

bool Window::should_update()
{
    return !glfwWindowShouldClose(window_);
}