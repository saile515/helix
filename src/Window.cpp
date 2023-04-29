#include "Window.h"

int& Window::width()
{
    return width_;
}

int& Window::height()
{
    return height_;
}

Window::Window(int width, int height)
{
    width_ = width;
    height_ = height;
}