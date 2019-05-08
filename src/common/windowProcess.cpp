
#include "windowProcess.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#ifdef PLATFORM_WINDOWS
#endif // PLATFORM_WINDOWS

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

AppWindow::AppWindow(int width, int height) :
	m_width(width),
	m_height(height),
	m_deltaTime(0.0f),
	m_lastFrame(0.0f)
{
	init();

	m_camera = new Camera(glm::vec3(0.0f, 0.0f, 20.0f));
}

AppWindow::~AppWindow()
{
	delete m_camera;

	cleanImgui();

	glfwDestroyWindow(m_window);
	glfwTerminate();
}

AppWindow* AppWindow::Instance()
{
	static AppWindow instance(800, 600);
	return &instance;
}

void AppWindow::beginFrame()
{
	// per-frame time logic
	// --------------------
	float currentFrame = glfwGetTime();
	m_deltaTime = currentFrame - m_lastFrame;
	m_lastFrame = currentFrame;

	// input
	// -----
	processInput();

	beginRenderImgui();
}

void AppWindow::endFrame()
{
	endRenderImgui();
	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	// -------------------------------------------------------------------------------
	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

int AppWindow::init()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(m_width, m_height, "OpenGLPractice", NULL, NULL);
	glfwMakeContextCurrent(m_window);
	if (m_window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
	glfwSetCursorPosCallback(m_window, ::mouse_callback);
	glfwSetScrollCallback(m_window, ::scroll_callback);

	// tell GLFW to capture our mouse
	//glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 关闭垂直同步
	glfwSwapInterval(0);

	initImgui();

	return 1;
}

void AppWindow::initImgui()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
}

void AppWindow::beginRenderImgui()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void AppWindow::endRenderImgui()
{
	{
		ImGui::Begin("FPS");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	//ImGui::EndFrame();
}

void AppWindow::cleanImgui()
{
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void AppWindow::processInput()
{
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(m_window, true);

	Camera* camera = AppWindow::Instance()->getCamera();
	float cameraSpeed = 2.5 * m_deltaTime;
	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
		camera->ProcessKeyboard(FORWARD, m_deltaTime);
	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
		camera->ProcessKeyboard(BACKWARD, m_deltaTime);
	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
		camera->ProcessKeyboard(LEFT, m_deltaTime);
	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
		camera->ProcessKeyboard(RIGHT, m_deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void AppWindow::mouse_callback(double xpos, double ypos)
{
	if (m_firstMouse)
	{
		m_lastX = xpos;
		m_lastY = ypos;
		m_firstMouse = false;
	}

	float xoffset = xpos - m_lastX;
	float yoffset = m_lastY - ypos; // reversed since y-coordinates go from bottom to top

	m_lastX = xpos;
	m_lastY = ypos;

	m_camera->ProcessMouseMovement(xoffset, yoffset);
}

void AppWindow::scroll_callback(double xoffset, double yoffset)
{
	m_camera->ProcessMouseScroll(yoffset);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	AppWindow::Instance()->mouse_callback(xpos, ypos);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	AppWindow::Instance()->scroll_callback(xoffset, yoffset);
}