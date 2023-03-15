#include<iostream>
#include<fstream>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"engine_headers/GameObject.h"
#include"engine_headers/Transform.h"
#include"engine_headers/Mesh.h"
#include"engine_headers/CollisionMesh.h"
#include"engine_headers/MeshScene.h"
#include"engine_headers/Skybox.h"
#include"engine_headers/Material.h"
#include"engine_headers/ObjectHandler.h"
#include"engine_headers/LightHandler.h"
#include"engine_headers/CollisionSolver.h"
#include"engine_source/PointLight.cpp"
#include"engine_source/DirectionalLight.cpp"
#include"engine_headers/Camera.h"
#include"engine_headers/FrameBufferObject.h"
#include"engine_headers/ComputeShader.h"
#include"engine_headers/Plane.h"
#include"engine_headers/BloomRenderer.h"
#include"engine_headers/SSAO.h"
#include"engine_headers/ShadowChunker.h"
#include"engine_headers/AudioPlayer.h"
#include"engine_headers/GameSaver.h"
#include"engine_headers/Transitioner.h"
#include"engine_headers/TextRenderer.h"

#include"game_headers/PlayerController.h"

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

//const unsigned int width = 1600;
//const unsigned int height = 1600;

glm::vec3 modelPosition = glm::vec3(0.0f, 0.0f, 0.0f);

float nearClipPlane = 0.1f;
float farClipPlane = 100.0f;

int main()
{
	glfwInit();

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	//specify opengl version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//specify opengl profile, core has most up to date functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	const int width = mode->width;
	const int height = mode->height;

	//create window
	GLFWwindow* window = glfwCreateWindow(width, height, "Project1", monitor, NULL);
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
	ComputeShader jitterComputeShader("../../engine_resource/Shaders/jitter.comp");
	jitterComputeShader.AttachTexture(2048, 2048);

	//create secondary framebuffers
	FrameBufferObject shadowMapFrameBuffer(2048, 2048, 1, 1);
	FrameBufferObject gBufferFrameBuffer(width, height, 3, 1);
	FrameBufferObject fogFrameBuffer(width, height, 1, 1);
	FrameBufferObject ssaoFrameBuffer(width / 3, height / 3, 1, 1);
	FrameBufferObject blurFrameBuffer(width, height, 1, 1);
	FrameBufferObject lightingFrameBuffer(width, height, 1, 1);
	FrameBufferObject basePostFrameBuffer(width, height);
	FrameBufferObject finalPassFrameBuffer(width, height, 1, 1);
	FrameBufferObject transitionFrameBuffer(width, height, 1, 1);
	basePostFrameBuffer.SetUpGBuffer();
	basePostFrameBuffer.InitializeRenderQuad();

	BloomRenderer bloomRenderer(width, height, 7);
	SSAO ssaoRenderer(8);

	TextRenderer textRenderer("../../engine_resource/Fonts/CutiveMono-Regular.ttf", 48, (float)width, (float)height);

	//specify opengl viewport
	glViewport(0, 0, width, height);

	Texture* defaultAlbedo = new Texture("../../engine_resource/Textures/default_albedo.png", GL_TEXTURE_2D, GL_LINEAR, /*1,*/ GL_RGB, GL_UNSIGNED_BYTE);
	Texture* defaultNormalMap = new Texture("../../engine_resource/Textures/default_normal.png", GL_TEXTURE_2D, GL_LINEAR, /*1,*/ GL_RGB, GL_UNSIGNED_BYTE);
	//Texture* defaultSpecMap = new Texture("../../engine_resource/Textures/default_albedo.png", GL_TEXTURE_2D, GL_LINEAR, /*1,*/ GL_RGB, GL_UNSIGNED_BYTE);
	
	Texture::defaultAlbedo = defaultAlbedo;
	Texture::defaultNormalMap = defaultNormalMap;
	//Texture::defaultSpecMap = defaultSpecMap;

	Texture* brickTexture = new Texture("../../engine_resource/Textures/marble_herringbone_albedo.png", GL_TEXTURE_2D, GL_LINEAR, /*0,*/ GL_RGB, GL_UNSIGNED_BYTE);
	Texture* normalMap = new Texture("../../engine_resource/Textures/marble_herringbone_normal.png", GL_TEXTURE_2D, GL_LINEAR, /*1,*/ GL_RGB, GL_UNSIGNED_BYTE);
	//Texture* marbleSpecMap = new Texture("../../engine_resource/Textures/marble_herringbone_roughness.png", GL_TEXTURE_2D, GL_LINEAR, /*1,*/ GL_RGB, GL_UNSIGNED_BYTE);
	
	//ObjectHandler objectHandler;
	Shader shaderProgram("../../engine_resource/Shaders/default.vert", "../../engine_resource/Shaders/defaultdeferred.frag");
	Shader glassShaderProgram("../../engine_resource/Shaders/glass.vert", "../../engine_resource/Shaders/glass.frag");
	Shader skyBoxShaderProgram("../../engine_resource/Shaders/skybox.vert", "../../engine_resource/Shaders/skybox.frag");
	Shader shadowShaderProgram("../../engine_resource/Shaders/depth.vert", "../../engine_resource/Shaders/depth.frag");
	Shader fogShaderProgram("../../engine_resource/Shaders/postprocess.vert", "../../engine_resource/Shaders/fog.frag");
	Shader ssaoShaderProgram("../../engine_resource/Shaders/postprocess.vert", "../../engine_resource/Shaders/ssao.frag");
	Shader blurShaderProgram("../../engine_resource/Shaders/postprocess.vert", "../../engine_resource/Shaders/blur.frag");
	Shader transitionShaderProgram("../../engine_resource/Shaders/postprocess.vert", "../../engine_resource/Shaders/transition.frag");
	Shader textShaderProgram("../../engine_resource/Shaders/text.vert", "../../engine_resource/Shaders/text.frag");
	Shader basePostShaderProgram("../../engine_resource/Shaders/postprocess.vert", "../../engine_resource/Shaders/basepostprocesser.frag");
	Shader lightingShaderProgram("../../engine_resource/Shaders/postprocess.vert", "../../engine_resource/Shaders/lighting.frag");
	Shader tonemapperShaderProgram("../../engine_resource/Shaders/postprocess.vert", "../../engine_resource/Shaders/tonemapper.frag");
	//Shader gbufferShaderProgram("../../engine_resource/Shaders/default.vert", "../../engine_resource/Shaders/defaultdeferred.frag");

	Shader bloomDownsampleShaderProgram("../../engine_resource/Shaders/postprocess.vert", "../../engine_resource/Shaders/downsampler.frag");
	Shader bloomUpsampleShaderProgram("../../engine_resource/Shaders/postprocess.vert", "../../engine_resource/Shaders/upsampler.frag");

	//Shader waterShader("../../engine_resource/Shaders/water.vert", "../../engine_resource/Shaders/water.frag");

	Transitioner::Instance.InitializeTransitions(std::vector<const char*>{ 
		"../../engine_resource/Textures/transitions/transition_up_down.png",
		"../../engine_resource/Textures/transitions/transition_swipe_up_right.png",
		"../../engine_resource/Textures/transitions/transition_swipe_down_right.png",
		"../../engine_resource/Textures/transitions/transition_swipe_right.png",
		"../../engine_resource/Textures/transitions/transition_swipe_down_left.png",
		"../../engine_resource/Textures/transitions/transition_swipe_up_left.png",
		"../../engine_resource/Textures/transitions/transition_swipe_left.png"
	}, transitionShaderProgram, 3.0f);
	Transitioner::Instance.SetTransition(0, false);

	std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	Camera camera(width, height, glm::vec3(0.0f, 0.5f, 2.0f));

	PointLight light(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
	PointLight light2(glm::vec3(-0.8f, 1.0f, -0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
	PointLight light3(glm::vec3(0.8f, 1.0f, -0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);

	PointLight light4(glm::vec3(5.8f, 1.8f, 2.4f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
	PointLight light5(glm::vec3(9.4f, 3.3f, 5.8f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
	PointLight light6(glm::vec3(7.3f, 4.5f, 9.4f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);

	PointLight light7(glm::vec3(6.0f, 2.5f, 5.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.5f, /*attenuation*/3.5f, 1.0f, 0.5f, 0.3f);
	PointLight light8(glm::vec3(5.0f, 7.5f, 5.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.5f, /*attenuation*/3.5f, 1.0f, 0.5f, 0.3f);
	PointLight light9(glm::vec3(5.0f, 12.5f, 6.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.5f, /*attenuation*/3.5f, 1.0f, 0.5f, 0.3f);
	PointLight light10(glm::vec3(4.0f, 17.5f, 5.5f), glm::vec3(1.0f, 1.0f, 1.0f), 0.5f, /*attenuation*/3.5f, 1.0f, 0.5f, 0.3f);

	DirectionalLight globalDirectionalLight(glm::vec3(0.6f, -0.5f, -0.5f), glm::vec3(1.0f, 0.98f, 0.94f), 2.5f);

	Material* material = new Material(shaderProgram, TexParam{ "albedo", brickTexture, 1.0f }, TexParam{"normalMap", normalMap, 1.0f }/*TexParam{"specMap", marbleSpecMap, 1.0}*/, false);
	Material* glass = new Material(glassShaderProgram, TexParam{ "", nullptr, 1.0f }, TexParam{ "", nullptr, 1.0f }, true);

	//Material* waterMaterial = new Material(waterShader, nullptr, nullptr);

	Skybox sceneSkyBox(std::vector<const char*>{
		"../../engine_resource/Textures/cloud_skybox/clouds1_east.bmp", //right
		"../../engine_resource/Textures/cloud_skybox/clouds1_west.bmp", //left
		"../../engine_resource/Textures/cloud_skybox/clouds1_up.bmp", //up
		"../../engine_resource/Textures/cloud_skybox/clouds1_down.bmp", //down
		"../../engine_resource/Textures/cloud_skybox/clouds1_north.bmp", //front
		"../../engine_resource/Textures/cloud_skybox/clouds1_south.bmp"}, //back
		skyBoxShaderProgram);

	/*
	Skybox sceneSkyBox(std::vector<const char*>{
		"../../engine_resource/Textures/sunset_skybox/sunsetflat_rt.jpg", //right
		"../../engine_resource/Textures/sunset_skybox/sunsetflat_lf.jpg", //left
		"../../engine_resource/Textures/sunset_skybox/sunsetflat_up.jpg", //up
		"../../engine_resource/Textures/sunset_skybox/sunsetflat_dn.jpg", //down
		"../../engine_resource/Textures/sunset_skybox/sunsetflat_ft.jpg", //front
		"../../engine_resource/Textures/sunset_skybox/sunsetflat_bk.jpg"}, //back
		skyBoxShaderProgram);
	*/

	Mesh pyramid(verts, ind, material, false, true, true);
	Plane plane(10.0f, 10, 1.0f, material);

	Transform defaultTransform(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	
	//MeshScene importedCube(Transform(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0f), glm::vec3(0.25f)), "../../engine_resource/3D Objects/cube/cube.obj", shaderProgram, nullptr);
	//MeshScene importedMonkey(Transform(glm::vec3(-1.0f, 0.5f, 0.0f), glm::vec3(0.0f), glm::vec3(0.35f)), nullptr, std::vector<const char*>{ "../../engine_resource/3D Objects/textured_monkey/monkey.obj", "../../engine_resource/3D Objects/textured_monkey/monkey_lod_1.obj" }, shaderProgram, nullptr, false, true);
	//MeshScene importedDonut(Transform(glm::vec3(-2.5f, 0.25f, 0.0f), glm::vec3(0.0f), glm::vec3(0.5f)), nullptr, std::vector<const char*>{ "../../engine_resource/3D Objects/colored_donut/colored_donut.obj", "../../engine_resource/3D Objects/colored_donut/colored_donut_lod_1.obj" }, shaderProgram, nullptr, false, true);
	
	MeshScene walls(Transform::Zero, nullptr, std::vector<const char*>{ "../../engine_resource/3D Objects/tower/walls/walls.obj" }, glassShaderProgram, glass, false, true);
	MeshScene floor1(Transform::Zero, nullptr, std::vector<const char*>{ "../../engine_resource/3D Objects/tower/floor1/floor1.obj" }, shaderProgram, nullptr, false, true);
	MeshScene floor2(Transform::Zero, nullptr, std::vector<const char*>{ "../../engine_resource/3D Objects/tower/floor2/floor2.obj" }, shaderProgram, nullptr, false, true);
	MeshScene floor3(Transform::Zero, nullptr, std::vector<const char*>{ "../../engine_resource/3D Objects/tower/floor3/floor3.obj" }, shaderProgram, nullptr, false, true);
	MeshScene floor4(Transform::Zero, nullptr, std::vector<const char*>{ "../../engine_resource/3D Objects/tower/floor4/floor4.obj" }, shaderProgram, nullptr, false, true);
	MeshScene floor5(Transform::Zero, nullptr, std::vector<const char*>{ "../../engine_resource/3D Objects/tower/floor5/floor5.obj" }, shaderProgram, nullptr, false, true);
	MeshScene floor6(Transform::Zero, nullptr, std::vector<const char*>{ "../../engine_resource/3D Objects/tower/floor6/floor6.obj" }, shaderProgram, nullptr, false, true);
	MeshScene floor7(Transform::Zero, nullptr, std::vector<const char*>{ "../../engine_resource/3D Objects/tower/floor7/floor7.obj" }, shaderProgram, nullptr, false, true);
	MeshScene floor8(Transform::Zero, nullptr, std::vector<const char*>{ "../../engine_resource/3D Objects/tower/floor8/floor8.obj" }, shaderProgram, nullptr, false, true);
	MeshScene floor9(Transform::Zero, nullptr, std::vector<const char*>{ "../../engine_resource/3D Objects/tower/floor9/floor9.obj" }, shaderProgram, nullptr, false, true);
	MeshScene floor10(Transform::Zero, nullptr, std::vector<const char*>{ "../../engine_resource/3D Objects/tower/floor10/floor10.obj" }, shaderProgram, nullptr, false, true);
	MeshScene floor11(Transform::Zero, nullptr, std::vector<const char*>{ "../../engine_resource/3D Objects/tower/floor11/floor11.obj" }, shaderProgram, nullptr, false, true);
	MeshScene floor12(Transform::Zero, nullptr, std::vector<const char*>{ "../../engine_resource/3D Objects/tower/floor12/floor12.obj" }, shaderProgram, nullptr, false, true);
	MeshScene floor13(Transform::Zero, nullptr, std::vector<const char*>{ "../../engine_resource/3D Objects/tower/floor13/floor13.obj" }, shaderProgram, nullptr, false, true);
	MeshScene floor14(Transform::Zero, nullptr, std::vector<const char*>{ "../../engine_resource/3D Objects/tower/floor14/floor14.obj" }, shaderProgram, nullptr, false, true);

	MeshScene challenge(Transform::Zero, nullptr, std::vector<const char*>{ "../../engine_resource/3D Objects/tower/challenge/challenge.obj" }, glassShaderProgram, glass, false, true);

	MeshScene tutorialSign(Transform::Zero, nullptr, std::vector<const char*>{ "../../engine_resource/3D Objects/tutorial/sign/tutorial_sign.obj" }, shaderProgram, nullptr, false, true);
	MeshScene tutorialBoard(Transform::Zero, nullptr, std::vector<const char*>{ "../../engine_resource/3D Objects/tutorial/board/tutorial_board.obj" }, shaderProgram, nullptr, false, true, true);
	
	Collectable* collectableBh1 = new Collectable(false, 70.0f);
	Collectable* collectableBh2 = new Collectable(false, 70.0f);
	Collectable* collectableBh3 = new Collectable(false, 70.0f);
	Collectable* collectableBh4 = new Collectable(false, 70.0f);
	Collectable* collectableBh5 = new Collectable(false, 70.0f);
	Collectable* collectableBh6 = new Collectable(false, 70.0f);
	Collectable* collectableBh7 = new Collectable(false, 70.0f);
	Collectable* collectableBh8 = new Collectable(false, 70.0f);

	glm::vec3 loadPlayerPos = glm::vec3(1.0f, 2.0f, 1.0f);
	int playerCollectablesPickedUp = 0;

	std::ofstream("../../engine_resource/Save Games/save1.txt", std::ios::binary | std::ios::app); //create file

	std::ifstream inputSave;
	inputSave.open("../../engine_resource/Save Games/save1.txt", std::ios::binary);

	inputSave.seekg(0, inputSave.end);
	std::streamoff length = inputSave.tellg();

	inputSave.seekg(0, std::ios::beg);
	
	if (length == 24)
	{
		GameSaver::Load(inputSave, &loadPlayerPos);
		GameSaver::Load(inputSave, &collectableBh1->isCollected);
		GameSaver::Load(inputSave, &collectableBh2->isCollected);
		GameSaver::Load(inputSave, &collectableBh3->isCollected);
		GameSaver::Load(inputSave, &collectableBh4->isCollected);
		GameSaver::Load(inputSave, &collectableBh5->isCollected);
		GameSaver::Load(inputSave, &collectableBh6->isCollected);
		GameSaver::Load(inputSave, &collectableBh7->isCollected);
		GameSaver::Load(inputSave, &collectableBh8->isCollected);
		GameSaver::Load(inputSave, &playerCollectablesPickedUp);
	}

	inputSave.close();

	MeshScene collectable1(Transform(glm::vec3(4.1f, 10.8f, 5.4f), glm::vec3(0.0f), glm::vec3(1.0f)), collectableBh1, std::vector<const char*>{ "../../engine_resource/3D Objects/tower/collectables/collectable.obj", "../../engine_resource/3D Objects/tower/collectables/collectable_lod.obj" }, shaderProgram, nullptr, false, false);
	MeshScene collectable2(Transform(glm::vec3(9.3f, 26.9f, 8.9f), glm::vec3(0.0f), glm::vec3(1.0f)), collectableBh2, std::vector<const char*>{ "../../engine_resource/3D Objects/tower/collectables/collectable.obj", "../../engine_resource/3D Objects/tower/collectables/collectable_lod.obj" }, shaderProgram, nullptr, false, false);
	MeshScene collectable3(Transform(glm::vec3(0.7f, 32.5f, 11.5f), glm::vec3(0.0f), glm::vec3(1.0f)), collectableBh3, std::vector<const char*>{ "../../engine_resource/3D Objects/tower/collectables/collectable.obj", "../../engine_resource/3D Objects/tower/collectables/collectable_lod.obj" }, shaderProgram, nullptr, false, false);
	MeshScene collectable4(Transform(glm::vec3(-1.9f, 42.2f, 2.3f), glm::vec3(0.0f), glm::vec3(1.0f)), collectableBh4, std::vector<const char*>{ "../../engine_resource/3D Objects/tower/collectables/collectable.obj", "../../engine_resource/3D Objects/tower/collectables/collectable_lod.obj" }, shaderProgram, nullptr, false, false);
	MeshScene collectable5(Transform(glm::vec3(0.4f, 45.6f, 1.2f), glm::vec3(0.0f), glm::vec3(1.0f)), collectableBh5, std::vector<const char*>{ "../../engine_resource/3D Objects/tower/collectables/collectable.obj", "../../engine_resource/3D Objects/tower/collectables/collectable_lod.obj" }, shaderProgram, nullptr, false, false);
	MeshScene collectable6(Transform(glm::vec3(13.8f, 54.4f, 5.1f), glm::vec3(0.0f), glm::vec3(1.0f)), collectableBh6, std::vector<const char*>{ "../../engine_resource/3D Objects/tower/collectables/collectable.obj", "../../engine_resource/3D Objects/tower/collectables/collectable_lod.obj" }, shaderProgram, nullptr, false, false);
	MeshScene collectable7(Transform(glm::vec3(-0.5f, 59.6f, 18.2f), glm::vec3(0.0f), glm::vec3(1.0f)), collectableBh7, std::vector<const char*>{ "../../engine_resource/3D Objects/tower/collectables/collectable.obj", "../../engine_resource/3D Objects/tower/collectables/collectable_lod.obj" }, shaderProgram, nullptr, false, false);
	MeshScene collectable8(Transform(glm::vec3(8.2f, 63.8f, -1.8f), glm::vec3(0.0f), glm::vec3(1.0f)), collectableBh8, std::vector<const char*>{ "../../engine_resource/3D Objects/tower/collectables/collectable.obj", "../../engine_resource/3D Objects/tower/collectables/collectable_lod.obj" }, shaderProgram, nullptr, false, false);

	//GameObject smallPyramid(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f), pyramid, nullptr);
	GameObject planeObject(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f), plane.mesh, nullptr);
	PlayerController* playerController = new PlayerController(2.0f, 0.6f, &camera, glm::vec3(0.35f, 0.7f, 0.35f));
	GameObject player(glm::vec3(loadPlayerPos.x, loadPlayerPos.y + 0.25f, loadPlayerPos.z)/*glm::vec3(-0.05, 71.65f, 0.15f)*/, glm::vec3(0.0f), glm::vec3(1.0f), Mesh(), playerController);
	
	playerController->collectablesPickedUp = playerCollectablesPickedUp;

	//CollisionMesh pyramidCollider(verts, ind, smallPyramid.transform.matrix, &smallPyramid, true);
	CollisionMesh planeCollider(plane.mesh.vertices, plane.mesh.indices, planeObject.transform.matrix, &planeObject, true);

	ShadowChunker shadowChunker(0.5f);
	ssaoRenderer.InitializeKernels();

	//set ssao kernel
	ssaoRenderer.SetSSAOKernel(ssaoShaderProgram);

	//lighting shader
	LightHandler::Instance.SetLightUniforms(lightingShaderProgram);

	//Audio player
	AudioPlayer::Instance.InitializeSoundEngine();

	AudioPlayer::Instance.SetSoundtracks(std::vector<Soundtrack>{
		Soundtrack{ "../../engine_resource/Sound/soundtracks/wind-troubles-the-water-138702.mp3", 0.05f },
		Soundtrack{ "../../engine_resource/Sound/soundtracks/slow-motion-121841.mp3", 0.05f },
		Soundtrack{ "../../engine_resource/Sound/soundtracks/relaxing-music-vol1-124477.mp3", 0.175f },
		Soundtrack{ "../../engine_resource/Sound/soundtracks/relaxing-music-vol12-131317.mp3", 0.1f }
	});
	AudioPlayer::Instance.PlaySoundtrack(2, false);

	irrklang::ISound* windSound = AudioPlayer::Instance.soundEngine->play2D("../../engine_resource/Sound/soundeffects/wind-blowing-sfx-12809.mp3", true, false, true);
	windSound->setVolume(0.0f);

	//load text data
	textRenderer.LoadCharacters();
	textRenderer.LoadVertexData();

	double currentTime = glfwGetTime();
	double previousTime = glfwGetTime();
	double renderedCurrentTime = currentTime;
	float deltaTime = 0.0f;

	ObjectHandler::Instance.Awake();

	//only close window if window is closed
	while (!glfwWindowShouldClose(window))
	{
		//set color
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		//overwrite front buffer with back buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*
		//create new imgui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		*/

		//calculate deltaTime
		currentTime = glfwGetTime();
		deltaTime = (float)currentTime - (float)previousTime;
		previousTime = currentTime;

		//camera.FlyController(window);
		camera.UpdateMatrix(45.0f, nearClipPlane, farClipPlane);
		camera.SetMatrices(shaderProgram, "view", "projection");
		camera.SetMatrices(glassShaderProgram, "view", "projection");
		//camera.SetMatrix(waterShader, "camMatrix");
		skyBoxShaderProgram.Activate();
		glUniformMatrix4fv(glGetUniformLocation(skyBoxShaderProgram.ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(camera.projection * glm::mat4(glm::mat3(camera.view))));

		lightingShaderProgram.Activate();

		glm::vec3 camPos = camera.Position;
		glUniform3f(glGetUniformLocation(lightingShaderProgram.ID, "camPos"), camPos.x, camPos.y, camPos.z);
		glUniform1i(glGetUniformLocation(lightingShaderProgram.ID, "skybox"), sceneSkyBox.texUnit);
		sceneSkyBox.Bind();

		shaderProgram.Activate();

		shadowChunker.Update(player.transform.position);
		glm::mat4 lightProj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f); //hard coded change later
		glm::mat4 lightView = glm::lookAt(-globalDirectionalLight.direction * glm::vec3(30.0) + shadowChunker.currentChunkPos, shadowChunker.currentChunkPos, glm::vec3(0.0, 1.0, 0.0));
		shadowShaderProgram.Activate();
		glUniformMatrix4fv(glGetUniformLocation(shadowShaderProgram.ID, "lightMatrix"), 1, GL_FALSE, glm::value_ptr(lightProj * lightView));
		shaderProgram.Activate();
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "lightMatrix"), 1, GL_FALSE, glm::value_ptr(lightProj * lightView));
		
		jitterComputeShader.Dispatch();

		ObjectHandler::Instance.Update(deltaTime, window);

		//shadow mapping
		//glCullFace(GL_FRONT);
		shadowMapFrameBuffer.BindFrameBuffer();
		glClear(GL_DEPTH_BUFFER_BIT);
		ObjectHandler::Instance.DrawMeshes(shadowShaderProgram, ObjectHandler::SKIP_TRANSPARENCY);
		shadowMapFrameBuffer.UnbindFrameBuffer();
		//glCullFace(GL_BACK);
		//
		glViewport(0, 0, width, height);
		//renders gBuffer
		basePostFrameBuffer.BindFrameBuffer();
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		shadowMapFrameBuffer.SetTexture(shadowMapFrameBuffer.depthTextures[0], lightingShaderProgram, "shadowMap");
		jitterComputeShader.SetTexture(lightingShaderProgram, "jitterMap");

		ObjectHandler::Instance.DrawMeshes(ObjectHandler::SKIP_TRANSPARENCY);
		//sceneSkyBox.Draw();
		basePostFrameBuffer.UnbindFrameBuffer();
		//
		//renders ssao
		
		ssaoFrameBuffer.BindFrameBuffer();
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		
		ssaoShaderProgram.Activate();

		basePostFrameBuffer.SetTexture(basePostFrameBuffer.colorTextures[2], ssaoShaderProgram, "gNormal");
		basePostFrameBuffer.SetTexture(basePostFrameBuffer.colorTextures[4], ssaoShaderProgram, "gViewSpacePosition");

		ssaoRenderer.SetTexture(ssaoRenderer.noiseTextureID, ssaoRenderer.noiseTextureUnit, ssaoShaderProgram, "noiseTexture");
		camera.SetViewMatrix(ssaoShaderProgram, "view");
		camera.SetProjectionMatrix(ssaoShaderProgram, "projection");
		glUniform2f(glGetUniformLocation(ssaoShaderProgram.ID, "noiseScale"), (float)width / (2.0f * 4.0f), (float)height / (2.0f * 4.0f));

		basePostFrameBuffer.RenderQuad(ssaoShaderProgram);

		ssaoFrameBuffer.UnbindFrameBuffer();

		//
		//
		//blurs ssao
		blurFrameBuffer.BindFrameBuffer();
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		blurShaderProgram.Activate();
		basePostFrameBuffer.SetTexture(ssaoFrameBuffer.colorTextures[0], blurShaderProgram, "renderedScene");

		basePostFrameBuffer.RenderQuad(blurShaderProgram);

		blurFrameBuffer.UnbindFrameBuffer();
		//
		

		//lights previously rendered gBuffer
		lightingFrameBuffer.BindFrameBuffer();
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		basePostFrameBuffer.SetTexture(basePostFrameBuffer.colorTextures[0], lightingShaderProgram, "gPosition");
		basePostFrameBuffer.SetTexture(basePostFrameBuffer.colorTextures[1], lightingShaderProgram, "gLightPosition");
		basePostFrameBuffer.SetTexture(basePostFrameBuffer.colorTextures[2], lightingShaderProgram, "gNormal");
		basePostFrameBuffer.SetTexture(basePostFrameBuffer.colorTextures[3], lightingShaderProgram, "gAlbedo");
		basePostFrameBuffer.SetTexture(blurFrameBuffer.colorTextures[0], lightingShaderProgram, "ssao");

		basePostFrameBuffer.RenderQuad(lightingShaderProgram);

		sceneSkyBox.Draw();

		lightingFrameBuffer.UnbindFrameBuffer();
		
		//make it render based on depth
		//glBindFramebuffer(GL_READ_FRAMEBUFFER, basePostFrameBuffer.bufferID);
		//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, finalPassFrameBuffer.bufferID);
		//glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		// 
		//transparent object rendering, need to render based on depth
		finalPassFrameBuffer.BindFrameBuffer();
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		glassShaderProgram.Activate();
		glUniform3f(glGetUniformLocation(glassShaderProgram.ID, "camPos"), camPos.x, camPos.y, camPos.z);
		glUniform1i(glGetUniformLocation(glassShaderProgram.ID, "skybox"), sceneSkyBox.texUnit);
		sceneSkyBox.Bind();
		basePostFrameBuffer.SetTexture(lightingFrameBuffer.colorTextures[0], glassShaderProgram, "colorSampler");

		basePostShaderProgram.Activate();
		basePostFrameBuffer.SetTexture(lightingFrameBuffer.colorTextures[0], basePostShaderProgram, "renderedScene");
		basePostFrameBuffer.SetTexture(lightingFrameBuffer.depthTextures[0], basePostShaderProgram, "renderedSceneDepth");
		glDepthFunc(GL_LEQUAL);
		basePostFrameBuffer.RenderQuad(basePostShaderProgram);
		glDepthFunc(GL_LESS);

		glassShaderProgram.Activate();
		ObjectHandler::Instance.DrawMeshes(ObjectHandler::DRAW_ONLY_TRANSPARENCY);

		finalPassFrameBuffer.UnbindFrameBuffer();
		
		
		//
		//fog pass 1, only renders within walls
		fogFrameBuffer.BindFrameBuffer();
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		basePostFrameBuffer.SetTexture(finalPassFrameBuffer.colorTextures[0], fogShaderProgram, "renderedScene");
		basePostFrameBuffer.SetTexture(finalPassFrameBuffer.depthTextures[0], fogShaderProgram, "renderedSceneDepth");
		basePostFrameBuffer.RenderQuad(fogShaderProgram);

		fogFrameBuffer.UnbindFrameBuffer();
		//

		bloomRenderer.RenderBloomTexture(bloomUpsampleShaderProgram, bloomDownsampleShaderProgram, fogFrameBuffer.colorTextures[0].textureID, fogFrameBuffer.colorTextures[0].textureUnit, 0.005f);

		transitionFrameBuffer.BindFrameBuffer();
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		tonemapperShaderProgram.Activate();

		glUniform1i(glGetUniformLocation(tonemapperShaderProgram.ID, "renderedScene"), fogFrameBuffer.colorTextures[0].textureUnit);
		glActiveTexture(GL_TEXTURE0 + fogFrameBuffer.colorTextures[0].textureUnit);
		glBindTexture(GL_TEXTURE_2D, fogFrameBuffer.colorTextures[0].textureID);

		glUniform1i(glGetUniformLocation(tonemapperShaderProgram.ID, "bloomBlur"), bloomRenderer.mipTexUnit);
		glActiveTexture(GL_TEXTURE0 + bloomRenderer.mipTexUnit);
		glBindTexture(GL_TEXTURE_2D, bloomRenderer.BloomTexture());

		basePostFrameBuffer.RenderQuad(tonemapperShaderProgram);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthFunc(GL_ALWAYS);

		if (!playerController->escaped)
		{
			renderedCurrentTime += deltaTime;
		}

		int hours = int(renderedCurrentTime) / 3600;
		int minutes = int(renderedCurrentTime) / 60 - hours * 60;
		double seconds = renderedCurrentTime - minutes * 60.0;

		textRenderer.RenderText(textShaderProgram, std::to_string(hours) + ":" 
			+ std::to_string(minutes) + ":" 
			+ std::to_string(seconds), 0.0f, (float)height - 48.0f * ((float)height / 2160.0f), 1.0f, true, false, glm::mat4(1.0f), textRenderer.projection, Transform::Zero, glm::vec3(0.7f, 0.7f, 0.7f));

		textRenderer.RenderText(textShaderProgram, "That's the end of the tower. Congratulations. You made it!", 0.0f, 256.0f, 3.0f, false, true, camera.view, camera.projection, 
			Transform(glm::vec3(3.35f, 71.65f, -7.85f), glm::vec3(0.0f), glm::vec3(1.0f / (float)height, 1.0f / (float)height, 1.0f)), glm::vec3(0.7f, 0.7f, 0.7f));
		
		if (playerController->collectablesPickedUp < 7)
		{
			textRenderer.RenderText(textShaderProgram, "There are still " + std::to_string(8 - playerController->collectablesPickedUp) + " floating monkey heads to pick up.", 0.0f, 0.0f, 3.0f, false, true, camera.view, camera.projection,
				Transform(glm::vec3(3.35f, 71.65f, -7.85f), glm::vec3(0.0f), glm::vec3(1.0f / (float)height, 1.0f / (float)height, 1.0f)), glm::vec3(0.7f, 0.7f, 0.7f));
		}
		else if(playerController->collectablesPickedUp == 7)
		{
			textRenderer.RenderText(textShaderProgram, "There is still one floating monkey head to pick up.", 0.0f, 0.0f, 3.0f, false, true, camera.view, camera.projection,
				Transform(glm::vec3(3.35f, 71.65f, -7.85f), glm::vec3(0.0f), glm::vec3(1.0f / (float)height, 1.0f / (float)height, 1.0f)), glm::vec3(0.7f, 0.7f, 0.7f));
		}
		else
		{
			textRenderer.RenderText(textShaderProgram, "You also picked up all of the floating monkey heads. That is quite an accomplishment.", 0.0f, 0.0f, 3.0f, false, true, camera.view, camera.projection,
				Transform(glm::vec3(3.35f, 71.65f, -7.85f), glm::vec3(0.0f), glm::vec3(1.0f / (float)height, 1.0f / (float)height, 1.0f)), glm::vec3(0.7f, 0.7f, 0.7f));
		}

		glDepthFunc(GL_LESS);
		glDisable(GL_BLEND);
		

		transitionFrameBuffer.UnbindFrameBuffer();

		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		Transitioner::Instance.Update(deltaTime);
		basePostFrameBuffer.SetTexture(transitionFrameBuffer.colorTextures[0], transitionShaderProgram, "renderedScene");
		basePostFrameBuffer.RenderQuad(transitionShaderProgram);

		AudioPlayer::Instance.UpdateSoundtracks();
		windSound->setVolume(fmin(fabs(playerController->velocity.y) * 0.06f - 0.24f, 0.06f));
		//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		
		/*
		ImGui::Begin("This is a new window");
		ImGui::Text("Hello World");
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		*/

		//std::cout << 1.0f / deltaTime << std::endl;

		//clear and draw screen
		glfwSwapBuffers(window);
		//pool all glfw events like window resizing
		glfwPollEvents();
	}

	//save game
	std::ofstream outputSave;
	outputSave.open("../../engine_resource/Save Games/save1.txt", std::ios::binary);

	GameSaver::Save(outputSave, player.transform.position);
	GameSaver::Save(outputSave, collectableBh1->isCollected);
	GameSaver::Save(outputSave, collectableBh2->isCollected);
	GameSaver::Save(outputSave, collectableBh3->isCollected);
	GameSaver::Save(outputSave, collectableBh4->isCollected);
	GameSaver::Save(outputSave, collectableBh5->isCollected);
	GameSaver::Save(outputSave, collectableBh6->isCollected);
	GameSaver::Save(outputSave, collectableBh7->isCollected);
	GameSaver::Save(outputSave, collectableBh8->isCollected);
	GameSaver::Save(outputSave, playerController->collectablesPickedUp);

	outputSave.close();

	//testSave << player.transform.position.x << " " << player.transform.position.y << " " << player.transform.position.z << std::endl;
	//testSave << camera.Orientation.x << " " << camera.Orientation.y << " " << camera.Orientation.z;

	/*
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	*/

	windSound->drop();
	AudioPlayer::Instance.currentSoundtrack->drop();
	AudioPlayer::Instance.DropLoopedSounds();
	AudioPlayer::Instance.soundEngine->drop();
	//importedCube.Clear();
	walls.Clear();
	floor1.Clear();
	floor2.Clear();
	floor3.Clear();
	floor4.Clear();
	floor5.Clear();
	floor6.Clear();
	floor7.Clear();
	floor8.Clear();
	floor9.Clear();
	floor10.Clear();
	floor11.Clear();
	floor12.Clear();
	floor13.Clear();
	floor14.Clear();

	challenge.Clear();

	tutorialSign.Clear();
	tutorialBoard.Clear();

	delete collectableBh1;
	delete collectableBh2;
	delete collectableBh3;
	delete collectableBh4;
	delete collectableBh5;
	delete collectableBh6;
	delete collectableBh7;
	delete collectableBh8;

	collectable1.Clear();
	collectable2.Clear();
	collectable3.Clear();
	collectable4.Clear();
	collectable5.Clear();
	collectable6.Clear();
	collectable7.Clear();
	collectable8.Clear();

	//importedMonkey.Clear();
	//importedDonut.Clear();

	shaderProgram.Delete();
	skyBoxShaderProgram.Delete();
	shadowShaderProgram.Delete();
	tonemapperShaderProgram.Delete();
	//waterShader.Delete();
	shadowMapFrameBuffer.Delete();
	basePostFrameBuffer.Delete();

	bloomRenderer.Delete();

	delete defaultAlbedo;
	delete defaultNormalMap;
	
	Transitioner::Instance.DeleteTransitions();

	delete brickTexture;
	delete normalMap;

	delete material;

	//ObjectHandler::Instance.Delete();
	//delete waterMaterial;

	glfwTerminate();
	return 0;
}