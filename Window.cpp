#include "window.h"


// Declare Objects
Terrain * terrain;
Water * water;
Skybox * skybox;
City* city;
std::vector<Transform*> objects;
GLint terrainShaderProgram;
GLint waterShaderProgram;
GLint skyShaderProgram;
GLint shaderProgram;

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "../shader.vert"
#define FRAGMENT_SHADER_PATH "../shader.frag"

// Default camera parameters
Camera* camera;
glm::vec3 Window::cam_pos(20.0f, 30.0f, 60.0f);		// e  | Position of camera
glm::vec3 Window::cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 Window::cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is
glm::vec2 camEuler = glm::vec2(0, glm::pi<float>());

// Default mouse settings
glm::vec3 beginpoint;
glm::vec3 endpoint;
glm::vec2 mousePos;
double Window::xmouse = 0;
double Window::ymouse = 0;
bool Window::left_click = false;
bool Window::leftClick;

// Set up key maps
bool keys[1024];

// Timed Events
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// Default Plane speeds
Geometry* plane;
float planeSpeed = 8.0f;
float planePitchSpeed = 1.5f;
float planeYawSpeed = 1.5;
float planeRollSpeed = 1.5f;

// Window Setup
const char* window_title = "Flight Simulator";
int Window::width;
int Window::height;
glm::mat4 Window::P;
glm::mat4 Window::V;

void Window::initialize_objects()
{
	camera = new Camera();
	camera->SetFov(45.0f);
	camera->SetAspect((float)Window::width / Window::height);
	camera->SetPosition(glm::vec3(0.03f, 3.0f, -2.0f));
	camera->SetRotation(glm::quat(glm::vec3(camEuler, 0.0f)));

	plane = new Geometry("plane.obj", .05f/*1.0f*/, true);
	//plane = new Geometry("limb_s.obj", 1.0f, false);
	plane->addChild((Transform*)camera);
	plane->SetPosition(glm::vec3(0.0f, 30.0f, -200.0f));
	objects.push_back((Transform*)plane);

	// Create city / Load buildings into objects vector
	city = new City(&objects);

	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	terrain = new Terrain();
	water = new Water();
	skybox = new Skybox();
	// Load the shader program. Make sure you have the correct filepath up top
	terrainShaderProgram = LoadShaders("../terrainShader.vert", "../terrainShader.frag");
	waterShaderProgram = LoadShaders("../waterShader.vert", "../waterShader.frag");
	skyShaderProgram = LoadShaders("../skyboxshader.vert", "../skyboxshader.frag");
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	glDeleteProgram(skyShaderProgram);
	glDeleteProgram(terrainShaderProgram);
	glDeleteProgram(waterShaderProgram);

}


// Creates Window object 
GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);

	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

// Resize window if needed
void Window::resize_callback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	glfwGetFramebufferSize(window, &width, &height); // In case your Mac has a retina display
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);
	if (camera) camera->SetAspect((float)width / height);
	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

// Update objects, enviroment, etc as needed
void Window::idle_callback()
{
	GLfloat currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	glm::vec3 p = plane->GetPosition();
	glm::quat r = plane->GetWorldRotation();
	glm::vec3 fwd = glm::vec3(r * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	plane->SetPosition(p + fwd * planeSpeed * deltaTime);

	glm::quat lr = plane->GetRotation();

	if (keys[GLFW_KEY_W])
		lr *= glm::quat(glm::vec3(planePitchSpeed * deltaTime, 0.0f, 0.0f));
	else if (keys[GLFW_KEY_S])
		lr *= glm::quat(glm::vec3(-planePitchSpeed * deltaTime, 0.0f, 0.0f));

	if (keys[GLFW_KEY_Q])
		lr *= glm::quat(glm::vec3(0.0f, planeYawSpeed * deltaTime, 0.0f));
	else if (keys[GLFW_KEY_E])
		lr *= glm::quat(glm::vec3(0.0f, -planeYawSpeed * deltaTime, 0.0f));

	if (keys[GLFW_KEY_A])
		lr *= glm::quat(glm::vec3(0.0f, 0.0f, -planeRollSpeed * deltaTime));
	else if (keys[GLFW_KEY_D])
		lr *= glm::quat(glm::vec3(0.0f, 0.0f, planeRollSpeed * deltaTime));

	plane->SetRotation(lr);
}

// Render to screen
void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(skyShaderProgram);
	skybox->draw(camera, skyShaderProgram);

	glUseProgram(waterShaderProgram);
	water->draw(camera, waterShaderProgram);

	// Use the shader of programID
	glUseProgram(terrainShaderProgram);
	terrain->draw(camera, terrainShaderProgram);
	// Gets events, including input such as keyboard and mouse or window resizing
	glUseProgram(shaderProgram);

	for (int i = 0; i < objects.size(); i++) {
		objects[i]->draw(camera, shaderProgram);
	}
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);

}


void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		switch (key) {
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case GLFW_KEY_I:
			camera->SetFov(camera->GetFov() - 5);
			break;
		case GLFW_KEY_O:
			camera->SetFov(camera->GetFov() + 5);
			break;
		}
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (GLFW_PRESS == action)
			Window::leftClick = true;
		else if (GLFW_RELEASE == action)
			Window::leftClick = false;
	}

}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	
	glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, yoffset*5.0f));
	V = translation * V;


}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {

	if (Window::leftClick) {
		float dx = (xpos - mousePos.x) * .01f;
		float dy = (ypos - mousePos.y) * .01f;

		camEuler += glm::vec2(-dy, -dx);
		camera->SetRotation(glm::quat(glm::vec3(camEuler, 0.0f)));

	}
	mousePos.x = xpos;
	mousePos.y = ypos;

}

glm::vec3 Window::trackBallMapping(double x, double y)    // The CPoint class is a specific Windows class. Either use separate x and y values for the mouse location, or use a Vector3 in which you ignore the z coordinate.
{
	glm::vec3 v;    // Vector v is the synthesized 3D position of the mouse location on the trackball
	float d;     // this is the depth of the mouse location: the delta between the plane through the center of the trackball and the z position of the mouse
	v.x = (2.0*x - width) / width;   // this calculates the mouse X position in trackball coordinates, which range from -1 to +1
	v.y = (height - 2.0*y) / height;   // this does the equivalent to the above for the mouse Y position
	v.z = 0.0;   // initially the mouse z position is set to zero, but this will change below
	d = glm::length(v);    // this is the distance from the trackball's origin to the mouse location, without considering depth (=in the plane of the trackball's origin)
	d = (d < 1.0) ? d : 1.0;   // this limits d to values of 1.0 or less to avoid square roots of negative values in the following line
	v.z = sqrtf(1.001 - d * d);  // this calculates the Z coordinate of the mouse position on the trackball, based on Pythagoras: v.z*v.z + d*d = 1*1
	glm::normalize(v); // Still need to normalize, since we only capped d, not v.
	return v;  // return the mouse location on the surface of the trackball
}