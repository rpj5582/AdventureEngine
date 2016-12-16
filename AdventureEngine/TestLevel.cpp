#include "TestLevel.h"

TestLevel::TestLevel(float* aspectRatio) : Level(aspectRatio)
{
}

TestLevel::~TestLevel()
{
	delete terrainShader;
}

bool TestLevel::load()
{
	if (!Level::load()) return false;

	terrainShader = new Shader("Shaders/vertexShader.glsl", "Shaders/terrainFragmentShader.glsl");
	if (!terrainShader->load())
	{
		std::cout << "Failed to load terrain shader program." << std::endl;
		return false;
	}
	
	Texture* heightMap = AssetManager::loadTexture("Textures/testTerrainHeightmap.png");

	Object* terrain = new Object("terrain", glm::vec3(), glm::vec3(), glm::vec3(5, 2.5f, 5));
	terrain->addComponent<TerrainModelComponent>(heightMap);

	Material* blendMap = AssetManager::createMaterial(AssetManager::loadTexture("Textures/blendMap.png"));
	Material* material0 = AssetManager::createMaterial(AssetManager::loadTexture("Textures/testGrass.png"));
	Material* material1 = AssetManager::createMaterial(AssetManager::loadTexture("Textures/testStone.png"));
	Material* material2 = AssetManager::createMaterial(AssetManager::loadTexture("Textures/testFlowers.png"));
	Material* material3 = AssetManager::createMaterial(AssetManager::loadTexture("Textures/testDirt.png"));
	std::vector<const Material*> terrainMaterials = { blendMap, material0, material1, material2, material3 };

	terrain->addComponent<RenderComponent>(terrainMaterials);
	terrain->addComponent<ShaderComponent>(terrainShader);
	objects.push_back(terrain);

	Texture* testAtlas = AssetManager::loadTexture("Textures/testAtlas.png", glm::uvec2(2, 2));

	Object* testObj = new Object("testObj", glm::vec3(0, 0.5f, 0));
	testObj->addComponent<ModelComponent>(AssetManager::loadModel("quad"));
	testObj->addComponent<RenderComponent>(AssetManager::createMaterial(testAtlas, (unsigned int)0));
	objects.push_back(testObj);

	Object* testObj2 = new Object("testObj2", glm::vec3(0, 0.5f, 3));
	testObj2->addComponent<ModelComponent>(AssetManager::loadModel("quad"));
	testObj2->addComponent<RenderComponent>(AssetManager::createMaterial(testAtlas, (unsigned int)1));
	objects.push_back(testObj2);

	Object* teapot = new Object("teapot", glm::vec3(3, 0.5f, 0), glm::vec3(-1.5707963267948966192313216916398f, 0, 0), glm::vec3(0.001f, 0.001f, 0.001f));
	teapot->addComponent<ModelComponent>(AssetManager::loadModel("Models/teapot.obj"));
	teapot->addComponent<RenderComponent>(AssetManager::createMaterial(AssetManager::loadTexture("Textures/lambert.png"), 1.0f, 10.0f));
	objects.push_back(teapot);

	Object* stall = new Object("stall", glm::vec3(-5, 0, 0), glm::vec3(0, 3.1415926535897932384626433832795f, 0), glm::vec3(0.5f, 0.5f, 0.5f));
	stall->addComponent<ModelComponent>(AssetManager::loadModel("Models/stall.obj"));
	stall->addComponent<RenderComponent>(AssetManager::createMaterial(AssetManager::loadTexture("Textures/stall.png")));
	objects.push_back(stall);

	Object* camera = new Object("mainCamera", glm::vec3(0, 5, 5), glm::vec3(-0.78539816339744830961566084581988f, 0, 0));
	camera->addComponent<CameraComponent>(1.0471975511965977461542144610932f, 5, 100, true);
	camera->addComponent<TestComponent>();
	objects.push_back(camera);

	mainCamera = camera;

	Object* light0 = new Object("light0", glm::vec3(-5, 3, 0));
	light0->addComponent<LightComponent>(5.0f, glm::vec3(1, 1, 1));
	objects.push_back(light0);
	lights.push_back(light0);

	Object* light1 = new Object("light1", glm::vec3(), glm::vec3(0.78539816339744830961566084581988f, 3.1415926535897932384626433832795, 0));
	light1->addComponent<LightComponent>(5.0f, glm::vec3(1, 0.75f, 0.5f), true);
	objects.push_back(light1);
	lights.push_back(light1);

	return true;
}
