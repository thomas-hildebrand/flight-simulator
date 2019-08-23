#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include "Cube.h"
#include "shader.h"
#include "Terrain.h"
#include "Water.h"
#include "Skybox.h"

#include "Transform.hpp"
#include "City.h"
#include "Geometry.hpp"
#include "Camera.h"
#include "Building.h"


class Window
{
public:
	static int width;
	static int height;
	static bool left_click;
	static double xmouse;
	static double ymouse;
	static glm::vec3 cam_pos;
	static glm::vec3 cam_look_at;
	static glm::vec3 cam_up;
	static glm::mat4 P; // P for projection
	static glm::mat4 V; // V for view
	static void initialize_objects();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static void display_callback(GLFWwindow*);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static glm::vec3 Window::trackBallMapping(double x, double y);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);




	static bool leftClick;
	static double curxPos;
	static double curyPos;

};

#endif
