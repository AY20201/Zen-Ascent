#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"engine_headers/GameObject.h"
#include"engine_headers/Transform.h"
#include"engine_headers/Mesh.h"
#include"engine_headers/MeshScene.h"
#include"engine_headers/Skybox.h"
#include"engine_headers/Material.h"
#include"engine_headers/ObjectHandler.h"
#include"engine_headers/LightHandler.h"
#include"engine_source/PointLight.cpp"
#include"engine_source/DirectionalLight.cpp"
#include"engine_headers/Camera.h"
#include"engine_headers/FrameBufferObject.h"
#include"engine_headers/ComputeShader.h"
#include"engine_headers/Plane.h"

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
	Vertex{glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec2(0.0f, 2.0f), glm::vec3(0.0f, -1.0f, 0.0f)}, // Bottom side
	Vertex{glm::vec3(0.5f, 0.0f,  -0.5f), glm::vec2(2.0f, 2.0f), glm::vec3(0.0f, -1.0f, 0.0f)}, // Bottom side
	Vertex{glm::vec3(0.5f, 0.0f,  0.5f), glm::vec2(2.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)}, // Bottom side

	Vertex{glm::vec3(-0.5f, 0.0f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(-0.8f, 0.5f, 0.0f)}, // Left side
	Vertex{glm::vec3(-0.5f, 0.0f,  -0.5f), glm::vec2(2.0f, 0.0f), glm::vec3(-0.8f, 0.5f, 0.0f)}, // Left side
	Vertex{glm::vec3(0.0f, 0.8f,  0.0f), glm::vec2(1.0f, 2.0f), glm::vec3(-0.8f, 0.5f,  0.0f)}, // Left side

	Vertex{glm::vec3(-0.5f, 0.0f,  -0.5f), glm::vec2(2.0f, 0.0f), glm::vec3(0.0f, 0.5f, -0.8f)}, // Non-facing side
	Vertex{glm::vec3(0.5f, 0.0f,  -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.5f, -0.8f)}, // Non-facing side
	Vertex{glm::vec3(0.0f, 0.8f,  0.0f), glm::vec2(1.0f, 2.0f), glm::vec3(0.0f, 0.5f, -0.8f)}, // Non-facing side

	Vertex{glm::vec3(0.5f, 0.0f,  -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.8f, 0.5f,  0.0f)}, // Right side
	Vertex{glm::vec3(0.5f, 0.0f,  0.5f), glm::vec2(2.0f, 0.0f), glm::vec3(0.8f, 0.5f,  0.0f)}, // Right side
	Vertex{glm::vec3(0.0f, 0.8f,  0.0f), glm::vec2(1.0f, 2.0f), glm::vec3(0.8f, 0.5f,  0.0f)}, // Right side

	Vertex{glm::vec3(0.5f, 0.0f,  0.5f), glm::vec2(2.0f, 0.0f), glm::vec3(0.0f, 0.5f,  0.8f)}, // Facing side
	Vertex{glm::vec3(-0.5f, 0.0f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.5f,  0.8f)}, // Facing side
	Vertex{glm::vec3(0.0f, 0.8f,  0.0f), glm::vec2(1.0f, 2.0f), glm::vec3(0.0f, 0.5f,  0.8f)} // Facing side
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

glm::vec3 modelPosition = glm::vec3(0.0f, 0.0f, 0.0f);

float nearClipPlane = 0.1f;
float farClipPlane = 100.0f;

int main()
{
	glfwInit();

	//specify opengl version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
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

	//compute shaders always first for easy binding in shaders
	ComputeShader jitterComputeShader("engine_resource/Shaders/jitter.comp");
	jitterComputeShader.AttachTexture(2048, 2048);

	//create secondary framebuffers
	FrameBufferObject shadowMapFrameBuffer(2048, 2048);
	
	//specify opengl viewport
	glViewport(0, 0, width, height);

	Texture* defaultAlbedo = new Texture("engine_resource/Textures/default_albedo.png", GL_TEXTURE_2D, GL_LINEAR, /*1,*/ GL_RGB, GL_UNSIGNED_BYTE);
	Texture* defaultNormalMap = new Texture("engine_resource/Textures/default_normal.png", GL_TEXTURE_2D, GL_LINEAR, /*1,*/ GL_RGB, GL_UNSIGNED_BYTE);
	//Texture* defaultSpecMap = new Texture("engine_resource/Textures/default_albedo.png", GL_TEXTURE_2D, GL_LINEAR, /*1,*/ GL_RGB, GL_UNSIGNED_BYTE);

	Texture::defaultAlbedo = defaultAlbedo;
	Texture::defaultNormalMap = defaultNormalMap;
	//Texture::defaultSpecMap = defaultSpecMap;
	
	Texture* brickTexture = new Texture("engine_resource/Textures/marble_herringbone_albedo.png", GL_TEXTURE_2D, GL_LINEAR, /*0,*/ GL_RGB, GL_UNSIGNED_BYTE);
	Texture* normalMap = new Texture("engine_resource/Textures/marble_herringbone_normal.png", GL_TEXTURE_2D, GL_LINEAR, /*1,*/ GL_RGB, GL_UNSIGNED_BYTE);
	//Texture* marbleSpecMap = new Texture("engine_resource/Textures/marble_herringbone_roughness.png", GL_TEXTURE_2D, GL_LINEAR, /*1,*/ GL_RGB, GL_UNSIGNED_BYTE);
	
	//ObjectHandler objectHandler;
	Shader shaderProgram("engine_resource/Shaders/default.vert", "engine_resource/Shaders/default.frag");
	Shader skyBoxShaderProgram("engine_resource/Shaders/skybox.vert", "engine_resource/Shaders/skybox.frag");
	Shader shadowShaderProgram("engine_resource/Shaders/depth.vert", "engine_resource/Shaders/depth.frag");

	//Shader waterShader("engine_resource/Shaders/water.vert", "engine_resource/Shaders/water.frag");

	std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	Camera camera(width, height, glm::vec3(0.0f, 0.5f, 2.0f));

	PointLight light(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
	PointLight light2(glm::vec3(-0.8f, 1.0f, -0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
	PointLight light3(glm::vec3(0.8f, 1.0f, -0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);

	DirectionalLight globalDirectionalLight(glm::vec3(0.6f, -0.5f, -0.5f), glm::vec3(1.0f, 0.98f, 0.94f), 1.25f);

	Material* material = new Material(shaderProgram, TexParam{ "albedo", brickTexture, 1.0 }, TexParam{"normalMap", normalMap, 1.0}/*TexParam{"specMap", marbleSpecMap, 1.0}*/);

	//Material* waterMaterial = new Material(waterShader, nullptr, nullptr);

	Skybox sceneSkyBox(std::vector<const char*>{
		"engine_resource/Textures/cloud_skybox/clouds1_east.bmp", //right
		"engine_resource/Textures/cloud_skybox/clouds1_west.bmp", //left
		"engine_resource/Textures/cloud_skybox/clouds1_up.bmp", //up
		"engine_resource/Textures/cloud_skybox/clouds1_down.bmp", //down
		"engine_resource/Textures/cloud_skybox/clouds1_north.bmp", //front
		"engine_resource/Textures/cloud_skybox/clouds1_south.bmp"}, //back
		skyBoxShaderProgram);

	Mesh pyramid(verts, ind, material, false);
	Plane plane(10.0f, 10, 1.0, /*waterMaterial*/ material);

	Transform defaultTransform(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	
	//MeshScene importedCube(Transform(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0f), glm::vec3(0.25f)), "engine_resource/3D Objects/cube/cube.obj", shaderProgram, nullptr);
	MeshScene importedMonkey(Transform(glm::vec3(-1.0f, 0.5f, 0.0f), glm::vec3(0.0f), glm::vec3(0.35f)), "engine_resource/3D Objects/textured_monkey/monkey.obj", shaderProgram, nullptr);
	MeshScene importedDonut(Transform(glm::vec3(-2.5f, 0.25f, 0.0f), glm::vec3(0.0f), glm::vec3(0.5f)), "engine_resource/3D Objects/colored_donut/colored_donut.obj", shaderProgram, nullptr);

	GameObject smallPyramid(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f), pyramid);
	GameObject planeObject(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f), plane.mesh);
	
	//lighting shader
	shaderProgram.Activate();
	LightHandler::Instance.SetLightUniforms(shaderProgram);

	//waterShader.Activate();
	//glUniform1f(glGetUniformLocation(waterShader.ID, "nearClipPlane"), nearClipPlane);
	//glUniform1f(glGetUniformLocation(waterShader.ID, "farClipPlane"), farClipPlane);

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

		camera.FlyController(window);
		camera.UpdateMatrix(45.0f, nearClipPlane, farClipPlane);
		camera.SetMatrix(shaderProgram, "camMatrix");
		//camera.SetMatrix(waterShader, "camMatrix");
		skyBoxShaderProgram.Activate();
		glUniformMatrix4fv(glGetUniformLocation(skyBoxShaderProgram.ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(camera.projection * glm::mat4(glm::mat3(camera.view))));

		shaderProgram.Activate();
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);

		glUniform1i(glGetUniformLocation(shaderProgram.ID, "skybox"), sceneSkyBox.texUnit);
		sceneSkyBox.Bind();

		glm::mat4 lightProj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f); //hard coded change later
		glm::mat4 lightView = glm::lookAt(-globalDirectionalLight.direction * glm::vec3(10.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
		shadowShaderProgram.Activate();
		glUniformMatrix4fv(glGetUniformLocation(shadowShaderProgram.ID, "lightMatrix"), 1, GL_FALSE, glm::value_ptr(lightProj * lightView));
		shaderProgram.Activate();
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "lightMatrix"), 1, GL_FALSE, glm::value_ptr(lightProj * lightView));
		
		jitterComputeShader.Dispatch();

		glCullFace(GL_FRONT);
		shadowMapFrameBuffer.BindFrameBuffer();
		glClear(GL_DEPTH_BUFFER_BIT);
		ObjectHandler::Instance.DrawMeshes(shadowShaderProgram);
		shadowMapFrameBuffer.UnbindFrameBuffer();
		glCullFace(GL_BACK);

		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram.Activate();
		shadowMapFrameBuffer.SetTexture(shadowMapFrameBuffer.depthTexture, shaderProgram, "shadowMap");
		//jitterComputeShader.SetTexture(shaderProgram, "jitterMap");

		glUniform1i(glGetUniformLocation(shaderProgram.ID, "jitterMap"), jitterComputeShader.textureUnit);
		jitterComputeShader.BindTexture();

		ObjectHandler::Instance.DrawMeshes();
		sceneSkyBox.Draw();

		//shadowMapFrameBuffer.UnbindTexture();

		//clear and draw screen
		glfwSwapBuffers(window);
		//pool all glfw events like window resizing
		glfwPollEvents();
	}

	//importedCube.Clear();
	importedMonkey.Clear();
	importedDonut.Clear();

	shaderProgram.Delete();
	skyBoxShaderProgram.Delete();
	shadowShaderProgram.Delete();
	//waterShader.Delete();
	shadowMapFrameBuffer.Delete();

	delete defaultAlbedo;
	delete defaultNormalMap;
	
	delete brickTexture;
	delete normalMap;

	delete material;
	//delete waterMaterial;

	glfwTerminate();
	return 0;
}