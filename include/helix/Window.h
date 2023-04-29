class Window {
    int width_;
    int height_;

public:
    int& width();
    int& height();

    Window(int width, int height);
};