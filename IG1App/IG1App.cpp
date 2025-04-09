#include "IG1App.h"

#include <iostream>

using namespace std;

// static single instance (singleton pattern)
IG1App IG1App::s_ig1app; // default constructor (constructor with no parameters)

// Print OpenGL errors and warnings
void GLAPIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                   GLsizei length, const GLchar* message, const void* userParam)
{
	const char* prefix = (type == GL_DEBUG_TYPE_ERROR)
		? "\x1b[31m[ERROR]\x1b[0m "
		: "\x1b[33m[WARNING]\x1b[0m ";
	cout << prefix << message << endl;
}

void
IG1App::close()
{
	cout << "Closing GLFW...\n";
	glfwSetWindowShouldClose(mWindow, true); // stops main loop
}

void
IG1App::run() // enters the main event processing loop
{
	if (mWindow == 0) // if not intilialized
		init();

	// IG1App main loop
	while (!glfwWindowShouldClose(mWindow)) {

		// Redisplay the window if needed
		if (mNeedsRedisplay) {
			if (!display2cams) display();
			else display2c();
			mNeedsRedisplay = false;
		}

		if (mUpdateEnabled) {
			if (glfwGetTime() >= mNextUpdate) {
				mNextUpdate += FRAME_DURATION;
				mScenes[mCurrentScene]->update();
				mNeedsRedisplay = true;
			}
			glfwWaitEventsTimeout(mNextUpdate - glfwGetTime());
		}
		else glfwWaitEvents();

		// Stop and wait for new events

	}

	destroy();
}

void
IG1App::init()
{
	// create an OpenGL Context
	iniWinOpenGL();

	// create the scene after creating the context
	// allocate memory and resources
	mViewPort = new Viewport(mWinW, mWinH);
	mCamera = new Camera(mViewPort);
	mScenes.push_back(new Scene1);
	mScenes.push_back(new Scene2);
	mScenes.push_back(new Scene3);
	mScenes.push_back(new Scene4);
	mScenes.push_back(new Scene5);
	mScenes.push_back(new Scene6);

	mCamera->set2D();

	mScenes[0]->init();

	for (int i = 1; i < mScenes.size(); ++i) {
		mScenes[i]->init();
		mScenes[i]->unload();
	}
}

void
IG1App::iniWinOpenGL()
{ // Initialization
	cout << "Starting GLFW...\n";
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // enable on macOS

	// Create window with its associated OpenGL context, return window's pointer
	mWindow = glfwCreateWindow(mWinW, mWinH, "IG1App", nullptr, nullptr);

	if (mWindow == nullptr) {
		glfwTerminate();
		throw std::logic_error("cannot create GLFW window");
	}

	glfwMakeContextCurrent(mWindow);

	// Initialize OpenGL extension library
	if (GLenum err = glewInit(); err != GLEW_OK) {
		glfwTerminate();
		throw std::logic_error("Error while loading extensions: "s +
		                       reinterpret_cast<const char*>(glewGetErrorString(err)));
	}

	// Callback registration
	glfwSetWindowSizeCallback(mWindow, s_resize);
	glfwSetCharCallback(mWindow, s_key);
	glfwSetKeyCallback(mWindow, s_specialkey);
	glfwSetWindowRefreshCallback(mWindow, s_display);
	glfwSetMouseButtonCallback(mWindow, s_mouse);
	glfwSetCursorPosCallback(mWindow, s_motion);
	glfwSetScrollCallback(mWindow, s_mouseWheel);

	// Error message callback (all messages)
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0u, 0, GL_TRUE);
	glDebugMessageCallback(debugCallback, nullptr);

	cout << glGetString(GL_VERSION) << '\n';
	cout << glGetString(GL_VENDOR) << '\n';
}

void
IG1App::destroy()
{ // release memory and resources
	for (Scene* scene : mScenes)
		delete scene;
	mScenes.clear();

	delete mCamera;
	mCamera = nullptr;
	delete mViewPort;
	mViewPort = nullptr;

	glfwTerminate();
}

void
IG1App::display() const
{ // double buffering

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clears the back buffer

	mScenes[mCurrentScene]->render(*mCamera); // uploads the viewport and camera to the GPU

	glfwSwapBuffers(mWindow); // swaps the front and back buffer
}

void
IG1App::display2c() const
{ // double buffering

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clears the back buffer

	Camera auxCamera = *mCamera;
	Viewport auxVP = *mViewPort;

	mViewPort->setSize(mWinW / 2, mWinH);
	auxCamera.setSize(mViewPort->width(), mViewPort->height());
	mViewPort->setPos(0, 0);

	mScenes[mCurrentScene]->render(auxCamera); // uploads the viewport and camera to the GPU

	mViewPort->setPos(mWinW / 2, 0);
	auxCamera.setCenital();

	mScenes[mCurrentScene]->render(auxCamera);

	*mViewPort = auxVP;
	glfwSwapBuffers(mWindow); // swaps the front and back buffer
}

void
IG1App::resize(int newWidth, int newHeight)
{
	mWinW = newWidth;
	mWinH = newHeight;

	// Resize Viewport to the new window size
	mViewPort->setSize(newWidth, newHeight);

	// Resize Scene Visible Area such that the scale is not modified
	mCamera->setSize(mViewPort->width(), mViewPort->height());
}

void
IG1App::key(unsigned int key)
{
	bool need_redisplay = true;

	switch (key) {
		case '+':
			mCamera->setScale(+0.01); // zoom in  (increases the scale)
			break;
		case '-':
			mCamera->setScale(-0.01); // zoom out (decreases the scale)
			break;
		case 'l':
			mCamera->set3D();
			break;
		case 'o':
			mCamera->set2D();
			break;
		case 'u':
			mUpdateEnabled = !mUpdateEnabled;
			break;
		case 'a':
			mCamera->moveLR(-5);
			break;
		case 'd':
			mCamera->moveLR(5);
			break;
		case 'w':
			mCamera->moveUD(5);
			break;
		case 's':
			mCamera->moveUD(-5);
			break;
		case 'W':
			mCamera->moveFB(5);
			break;
		case 'S':
			mCamera->moveFB(-5);
			break;
		case 'p':
			mCamera->changeProj();
			break;
		case 'k':
			display2cams = !display2cams;
			mNeedsRedisplay = true;
			break;
		default:
			if (key >= '0' && key <= '9' && !changeScene(key - '0')) {
				cout << "[NOTE] There is no scene " << char(key) << ".\n";
				need_redisplay = false;
			}
			break;
	} // switch

	if (need_redisplay)
		mNeedsRedisplay = true;
}

void
IG1App::specialkey(int key, int scancode, int action, int mods)
{
	// Only interested in press events
	if (action == GLFW_RELEASE)
		return;

	bool need_redisplay = true;

	// Handle keyboard input
	// (key reference: https://www.glfw.org/docs/3.4/group__keys.html)
	switch (key) {
		case GLFW_KEY_ESCAPE:                     // Escape key
			glfwSetWindowShouldClose(mWindow, true); // stops main loop
			break;
		case GLFW_KEY_RIGHT:
			if (mods == GLFW_MOD_CONTROL)
				mCamera->rollReal(-5); // rotates -1 on the X axis
			else
				mCamera->yawReal(5); // rotates 1 on the X axis
			break;
		case GLFW_KEY_LEFT:
			if (mods == GLFW_MOD_CONTROL)
				mCamera->rollReal(5); // rotates 1 on the Y axis
			else
				mCamera->yawReal(-5); // rotate -1 on the Y axis
			break;
		case GLFW_KEY_UP:
			mCamera->pitchReal(5); // rotates 1 on the Z axis
			break;
		case GLFW_KEY_DOWN:
			mCamera->pitchReal(-5); // rotates -1 on the Z axis
			break;
		default:
			need_redisplay = false;
			break;
	} // switch

	if (need_redisplay)
		mNeedsRedisplay = true;
}

bool
IG1App::changeScene(size_t sceneNr)
{
	// Check whether the scene exists
	if (sceneNr >= mScenes.size())
		return false;

	// Change only if a different scene
	if (sceneNr != mCurrentScene) {
		mScenes[mCurrentScene]->unload();
		mCurrentScene = sceneNr;
		mScenes[mCurrentScene]->load();
	}

	return true;
}

void 
IG1App::mouse(int button, int action, int mods) {
	if (action == GLFW_PRESS) mMouseButt = button;
	else if (action == GLFW_RELEASE) mMouseButt = -50;
	double x, y, height;
	glfwGetCursorPos(mWindow, &x, &height);
	y = mViewPort->height() - height;
	mMouseCoord = { x, y };
}

void 
IG1App::motion(double x, double y) {
	glm::dvec2 mp = {x - mMouseCoord.x, y - mMouseCoord.y};
	mMouseCoord = { x, y };

	if (mMouseButt == GLFW_MOUSE_BUTTON_LEFT) mCamera->orbit(mp.x * 0.05, mp.y);
	else if (mMouseButt == GLFW_MOUSE_BUTTON_RIGHT) {
		mCamera->moveUD(mp.y);
		mCamera->moveLR(mp.x);
	}

	mNeedsRedisplay = true;
}

void 
IG1App::mouseWheel(double dx, double dy) {
	if (glfwGetKey(mWindow, GLFW_KEY_LEFT_CONTROL)) {
		mCamera->setScale(dy/10);
	}
	else {
		mCamera->moveFB(dy * 10);
	}

	mNeedsRedisplay = true;
}