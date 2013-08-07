#include "Window.hpp"
#include <cstdlib>

namespace MyGL
{
  namespace
  {
    const int OpenGLMajorVersion = 3;
    const int OpenGLMinorVersion = 2;
    const int OpenGLProfile = GLFW_OPENGL_CORE_PROFILE;

    struct TerminationSentinel
    {
      ~TerminationSentinel() { glfwTerminate(); }
    } termination_sentinel_;

    struct GlewSentinel
    {
      GlewSentinel() : initialized_(false) { }
      bool Init()
      {
        if(initialized_) return true; 
        glewExperimental = GL_TRUE;
        initialized_ = glewInit() == GLEW_OK;
        return initialized_;
      }
      bool initialized_;
    } glew_sentinel_;
  }

  Window::Window(const char *title, int width, int height, bool fullscreen) :
    _handle(nullptr)
  {
    if(glfwInit() == GL_FALSE)
      throw InitializationError("Failed to initialize GLFW");
      
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, OpenGLProfile);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OpenGLMajorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OpenGLMinorVersion);
    if(!(_handle = glfwCreateWindow(width, height, title, 
      fullscreen ? glfwGetPrimaryMonitor() : 0, 0)))
      throw InitializationError("Failed to create window");
    glfwMakeContextCurrent(_handle);
    glfwSetWindowSize(_handle, width, height);

    if(!glew_sentinel_.Init())
      throw InitializationError("Failed to initialize GLEW");
  }

  Window::~Window()
  {
    glfwDestroyWindow(_handle);
  }

  bool Window::ShouldClose() const
  {
    return glfwWindowShouldClose(_handle) != 0 ? true : false;
  }

  bool Window::IsKeyPressed(Key key) const
  {
    return glfwGetKey(_handle, static_cast<int>(key)) == GLFW_PRESS;
  }

  std::size_t Window::Width() const
  {
    int width, height;
    glfwGetWindowSize(_handle, &width, &height);
    return static_cast<std::size_t>(width);
  }

  std::size_t Window::Height() const
  {
    int width, height;
    glfwGetWindowSize(_handle, &width, &height);
    return static_cast<std::size_t>(height);
  }

  Window &Window::operator=(Window &&other)
  {
    if(this != &other)
    {
      glfwDestroyWindow(_handle);
      _handle = other._handle;
      other._handle = nullptr;
    }
    return *this;
  }

  void Window::SwapBuffers()
  {
    glfwSwapBuffers(_handle);
  }

  void Window::PollEvents()
  {
    glfwPollEvents();
  }
}