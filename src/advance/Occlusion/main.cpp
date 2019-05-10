
#include "common/windowProcess.h"

#include <common/filesystem.h>
#include <common/shader.h>
#include <common/model.h>

int main()
{
	AppWindow* window = AppWindow::Instance();

	glEnable(GL_DEPTH_TEST);

	Shader fbxShader("1.model_loading.vs", "1.model_loading.fs");
	Model model(FileSystem::getPath("data/model/nanosuit/nanosuit.obj"));

	while (!glfwWindowShouldClose(window->getWindow()))
	{
		window->beginFrame();

		// render
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		// configure transformation matrices
		glm::mat4 projection = window->getProjectionMatrix();
		glm::mat4 view = window->getViewMatrix();
		fbxShader.use();
		fbxShader.setMat4("projection", projection);
		fbxShader.setMat4("view", view);

		glm::mat4 modelMT = glm::mat4(1.0f);
		modelMT = glm::translate(modelMT, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
		modelMT = glm::scale(modelMT, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		fbxShader.setMat4("model", modelMT);

		model.Draw(fbxShader);

		window->endFrame();
	}

	glfwTerminate();
	return 0;
}