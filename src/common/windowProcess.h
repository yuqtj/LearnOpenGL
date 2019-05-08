
#pragma once

#include <iostream>

#include <common/camera.h>
#include <glfw3.h>
#include <glm/glm.hpp>


class AppWindow
{
public:
	~AppWindow();

	static AppWindow* Instance();

	void beginFrame();
	void endFrame();
	
	inline Camera* getCamera() const { return m_camera; }
	inline void setCamera(Camera* val) { m_camera = val; }

	inline GLFWwindow* getWindow() const { return m_window; }

	inline float getAspect() { return m_width * 1.0f / m_height; }

	inline glm::mat4 getViewMatrix() { return getCamera()->GetViewMatrix(); }

	void mouse_callback(double xpos, double ypos);
	void scroll_callback(double xoffset, double yoffset);
private:
	AppWindow(int width, int height);

	int init();
	void processInput();

	void initImgui();
	void beginRenderImgui();
	void endRenderImgui();
	void cleanImgui();

private:
	GLFWwindow* m_window;
	// camera
	Camera* m_camera;

	int m_width;
	int m_height;
	// timing
	float m_deltaTime;
	float m_lastFrame;

	float m_lastX = 800.0f / 2.0;
	float m_lastY = 600.0 / 2.0;
	bool m_firstMouse = true;

	const char* glsl_version = "#version 130";
};