#ifndef MYGL_WINDOW_HPP
#define MYGL_WINDOW_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

namespace MyGL
{
  enum class Key { 
    Escape = GLFW_KEY_ESCAPE,
    W = 'W', A = 'A', S = 'S', D = 'D',
    Space = GLFW_KEY_SPACE
  };

  class Window
  {
  public:
    class InitializationError : public std::runtime_error
    {
    public:
      explicit InitializationError(const std::string &msg) : 
        std::runtime_error(msg) { }
    };

    Window() : _handle(nullptr) { }
    Window(const char *title, int width, int height, bool fullscreen = false);
    Window(Window &&other) : _handle(std::move(_handle)) { }
    ~Window();
    bool ShouldClose() const;
    bool IsKeyPressed(Key key) const;
    std::size_t Width() const;
    std::size_t Height() const;
    Window &operator=(Window &&other);
    void SwapBuffers();
    void PollEvents();
  private:
    Window(const Window &);
    Window &operator=(const Window &);

    GLFWwindow *_handle;
  };
}

#endif