#include "TestScene.h"

TestScene::TestScene(float* aspectRatio) : Scene(aspectRatio)
{
}

TestScene::~TestScene()
{
	delete terrainShader;
}

bool TestScene::load()
{
	if (!Scene::load()) return false;

	terrainShader = new Shader("vertexShader.glsl", "terrainFragmentShader.glsl");
	if (!terrainShader->load())
	{
		std::cout << "Failed to load terrain shader program." << std::endl;
		return false;
	}
	
	Texture* heightMap = AssetManager::loadTexture("terrainHeightMapPool.png");
	Model* terrainModel = AssetManager::genTerrainModel(heightMap);

	Material* blendMap = AssetManager::createMaterial(AssetManager::loadTexture("blendMap.png"));
	Material* material0 = AssetManager::createMaterial(AssetManager::loadTexture("testGrass.png"));
	Material* material1 = AssetManager::createMaterial(AssetManager::loadTexture("testStone.png"));
	Material* material2 = AssetManager::createMaterial(AssetManager::loadTexture("testFlowers.png"));
	Material* material3 = AssetManager::createMaterial(AssetManager::loadTexture("testDirt.png"));
	std::vector<const Material*> terrainMaterials = { blendMap, material0, material1, material2, material3 };

	Object* terrain = new Object("terrain", glm::vec3(), glm::vec3(), glm::vec3(10, 5, 10));
	terrain->addComponent<RenderComponent>(terrainModel, terrainMaterials, terrainShader);
	objects.push_back(terrain);

	Model* quadModel = AssetManager::loadModel("quad");
	Model* teapotModel = AssetManager::loadModel("teapot.obj");
	Model* stallModel = AssetManager::loadModel("stall.obj");

	Texture* testObjTexture = AssetManager::loadTexture("testAtlas.png", 0, glm::uvec2(2, 2));
	Texture* testObj2Texture = AssetManager::loadTexture("testAtlas.png", 1, glm::uvec2(2, 2));

	Object* testObj = new Object("testObj", glm::vec3(0, 0.5f, 0));
	RenderComponent* test = testObj->addComponent<RenderComponent>(quadModel, AssetManager::createMaterial(testObjTexture));
	objects.push_back(testObj);

	Object* testObj2 = new Object("testObj2", glm::vec3(0, 0.5f, 3));
	testObj2->addComponent<RenderComponent>(quadModel, AssetManager::createMaterial(testObj2Texture));
	objects.push_back(testObj2);

	Object* teapot = new Object("teapot", glm::vec3(3, 0.5f, 0), glm::vec3(-90.0f, 0, 0), glm::vec3(0.001f, 0.001f, 0.001f));
	teapot->addComponent<RenderComponent>(teapotModel, AssetManager::createMaterial(AssetManager::loadTexture("lambert.png"), 5.0f, 10.0f));
	objects.push_back(teapot);

	Object* stall = new Object("stall", glm::vec3(-5, 0, 0), glm::vec3(0, 180.0f, 0), glm::vec3(0.5f, 0.5f, 0.5f));
	stall->addComponent<RenderComponent>(stallModel, AssetManager::createMaterial(AssetManager::loadTexture("stall.png")));
	objects.push_back(stall);

	Object* water = new Object("water", glm::vec3(0, -0.5f, 0), glm::vec3(-90, 0, 0), glm::vec3(5, 5, 1));
	water->addComponent<RenderComponent>(quadModel, AssetManager::createMaterial(AssetManager::loadTexture("water.png")));
	objects.push_back(water);

	Object* camera = new Object("mainCamera", glm::vec3(0, 5, 5), glm::vec3(-45.0f, 0, 0));
	mainCamera = camera->addComponent<CameraComponent>(aspectRatio);
	camera->addComponent<TestComponent>();
	objects.push_back(camera);

	//Object* light0 = new Object("Point Light", glm::vec3(-5, 3, 0));
	//light0->addComponent<LightComponent>(Light::POINT, 1.0f, 5.0f);
	//objects.push_back(light0);
	//lights.push_back(light0);

	Object* light1 = new Object("Directional Light", glm::vec3(), glm::vec3(135, 0, 0));
	light1->addComponent<LightComponent>(Light::DIRECTIONAL);
	objects.push_back(light1);
	lights.push_back(light1);

	//Object* light2 = new Object("Spotlight", glm::vec3(7, 1, 0), glm::vec3(90, 0, 0));
	//light2->addComponent<LightComponent>(Light::CONE, 10.0f, 5.0f, glm::vec3(1, 1, 1));
	//objects.push_back(light2);
	//lights.push_back(light2);

	float guiSize = 0.25f;
	Object* guiTest2 = new Object("GUI Panel", glm::vec3(-0.7f, -0.8f, 0), glm::vec3(), glm::vec3(guiSize, guiSize * *aspectRatio, 1));
	guiTest2->addComponent<GUIComponent>(heightMap);
	guiObjects.push_back(guiTest2);
	
	Object* guiTest = new Object("GUI Panel", glm::vec3(-0.775f, -0.85f, 0), glm::vec3(), glm::vec3(guiSize, guiSize * *aspectRatio, 1));
	guiTest->addComponent<GUIComponent>(AssetManager::loadTexture("scoreTestGUI.png"));
	guiObjects.push_back(guiTest);

	return true;
}
