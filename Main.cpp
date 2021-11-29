#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"engine_headers/GameObject.h"
#include"engine_headers/Transform.h"
#include"engine_headers/Mesh.h"
#include"engine_headers/Material.h"
#include"engine_headers/ObjectHandler.h"
#include"engine_source/PointLight.cpp"
#include"engine_headers/Camera.h"

/*GLfloat vertices[] =
{
	-0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f, // Lower left corner
	-0.5f,	0.5f, 0.0f,	0.0f, 1.0f, 0.0f, // Upper left corner
	0.5f,  0.5f, 0.0f,	0.0f, 0.0f, 1.0f, // Upper right corner
	0.5f, -0.5f, 0.0f,	1.0f, 1.0f, 1.0f  // Lower right corner
};*/


Vertex vertices[] =
{
	Vertex{glm::vec3(-0.5f, 0.0f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)}, // Bottom side
	Vertex{glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec2(0.0f, 5.0f), glm::vec3(0.0f, -1.0f, 0.0f)}, // Bottom side
	Vertex{glm::vec3(0.5f, 0.0f,  -0.5f), glm::vec2(5.0f, 5.0f), glm::vec3(0.0f, -1.0f, 0.0f)}, // Bottom side
	Vertex{glm::vec3(0.5f, 0.0f,  0.5f), glm::vec2(5.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)}, // Bottom side

	Vertex{glm::vec3(-0.5f, 0.0f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(-0.8f, 0.5f, 0.0f)}, // Left side
	Vertex{glm::vec3(-0.5f, 0.0f,  -0.5f), glm::vec2(5.0f, 0.0f), glm::vec3(-0.8f, 0.5f, 0.0f)}, // Left side
	Vertex{glm::vec3(0.0f, 0.8f,  0.0f), glm::vec2(2.5f, 5.0f), glm::vec3(-0.8f, 0.5f,  0.0f)}, // Left side

	Vertex{glm::vec3(-0.5f, 0.0f,  -0.5f), glm::vec2(5.0f, 0.0f), glm::vec3(0.0f, 0.5f, -0.8f)}, // Non-facing side
	Vertex{glm::vec3(0.5f, 0.0f,  -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.5f, -0.8f)}, // Non-facing side
	Vertex{glm::vec3(0.0f, 0.8f,  0.0f), glm::vec2(2.5f, 5.0f), glm::vec3(0.0f, 0.5f, -0.8f)}, // Non-facing side

	Vertex{glm::vec3(0.5f, 0.0f,  -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.8f, 0.5f,  0.0f)}, // Right side
	Vertex{glm::vec3(0.5f, 0.0f,  0.5f), glm::vec2(5.0f, 0.0f), glm::vec3(0.8f, 0.5f,  0.0f)}, // Right side
	Vertex{glm::vec3(0.0f, 0.8f,  0.0f), glm::vec2(2.5f, 5.0f), glm::vec3(0.8f, 0.5f,  0.0f)}, // Right side

	Vertex{glm::vec3(0.5f, 0.0f,  0.5f), glm::vec2(5.0f, 0.0f), glm::vec3(0.0f, 0.5f,  0.8f)}, // Facing side
	Vertex{glm::vec3(-0.5f, 0.0f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.5f,  0.8f)}, // Facing side
	Vertex{glm::vec3(0.0f, 0.8f,  0.0f), glm::vec2(2.5f, 5.0f), glm::vec3(0.0f, 0.5f,  0.8f)} // Facing side
};

GLuint indices[] =
{
	0, 1, 2, // Bottom side
	0, 2, 3, // Bottom side
	4, 6, 5, // Left side
	7, 9, 8, // Non-facing side
	10, 12, 11, // Right side
	13, 15, 14 // Facing side
};

const unsigned int width = 1600;
const unsigned int height = 1600;

glm::vec3 lightPosition = glm::vec3(0.8f, 1.0f, 0.5f);
glm::vec3 lightDirection = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 lightColor = glm::vec3(0.8f, 0.6f, 0.9f);

glm::vec3 modelPosition = glm::vec3(0.0f, 0.0f, 0.0f);

int main()
{
	glfwInit();

	//specify opengl version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//specify opengl profile, core has most up to date functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create window
	GLFWwindow* window = glfwCreateWindow(width, height, "Project1", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Window failed to create" << std::endl;
		glfwTerminate();
		return -1;
	}

	//tell glfw to use window in current context
	glfwMakeContextCurrent(window);

	//load opengl through glad
	gladLoadGL();

	//specify opengl viewport
	glViewport(0, 0, width, height);

	Texture brickTexture = Texture("brick.png", GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE);

	//ObjectHandler objectHandler;
	Shader shaderProgram("default.vert", "default.frag");

	std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	//std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	Camera camera(width, height, glm::vec3(0.0f, 0.5f, 2.0f));
	//Player player(glm::vec3(0.0f, 0.25f, 2.0f), glm::vec3(0.0f, 0.25f, 0.0f), 0.6f);

	//PointLight light(lightPosition, lightColor);
	Material material(shaderProgram, brickTexture);
	Mesh pyramid(verts, ind, material);
	//Skybox skybox(std::vector<const char*> {"skybox_sky_right.jpg","skybox_sky_left.jpg","skybox_sky_top.jpg","skybox_sky_bottom.jpg","skybox_sky_front.jpg","skybox_sky_back.jpg"});
	//skybox.SetMeshData();
	GameObject smallPyramid = GameObject::Instantiate(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f), pyramid);
	//GameObject bigPyramid = GameObject::Instantiate(glm::vec3(2.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.5f), pyramid);

	//Chunk chunk(glm::vec2(0.0f, 0.0f), material);
	//World::Instance.GenerateWorld(material);

	//lighting shader
	shaderProgram.Activate();
	//light.SetUniforms(shaderProgram);

	double currentTime = glfwGetTime();
	double previousTime = glfwGetTime();
	float deltaTime = 0.0f;

	//only close window if window is closed
	while (!glfwWindowShouldClose(window))
	{
		//set color
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		//overwrite front buffer with back buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//calculate deltaTime
		currentTime = glfwGetTime();
		deltaTime = (float)currentTime - (float)previousTime;
		previousTime = currentTime;

		//player.Update(camera, deltaTime, window);
		camera.FlyController(window);
		camera.UpdateMatrix(45.0f, 0.1f, 100.0f);
		camera.SetMatrix(shaderProgram, "camMatrix");

		//skybox.SetUniform(skyboxShaderProgram);

		//shaderProgram.Activate();
		//glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPosition"), camera.Position.x, camera.Position.y, camera.Position.z);

		//skybox.Draw(skyboxShaderProgram, camera);
		//LightingHandler::Instance.SetLights(shaderProgram);
		ObjectHandler::Instance.DrawMeshes(shaderProgram);

		//clear and draw screen
		glfwSwapBuffers(window);
		//pool all glfw events like window resizing
		glfwPollEvents();
	}

	shaderProgram.Delete();

	glfwTerminate();
	return 0;
}