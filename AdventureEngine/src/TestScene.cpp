#include "TestScene.h"

#include "AudioManager.h"

TestScene::TestScene(const int* windowWidth, const int* windowHeight) : Scene(windowWidth, windowHeight)
{
	terrainShader = nullptr;
}

TestScene::~TestScene()
{
}

bool TestScene::load()
{
	if (!Scene::load()) return false;

	terrainShader = AssetManager::loadShaderProgram("vertexShader.glsl", "terrainFragmentShader.glsl");
	if (!terrainShader)
	{
		std::cout << "Failed to load terrain shader program." << std::endl;
		return false;
	}

	Object* cube = new Object("Cube", glm::vec3(0, 1, 0));
	addRenderable(cube->addComponent<RenderComponent>(AssetManager::getModelPrimitive(ModelPrimitive::CUBE), AssetManager::createMaterial(AssetManager::loadTexture("lambert.png"))));
	cube->addComponent<AABBColliderComponent>(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f));
	addObject(cube);

	AudioBuffer* sound = AudioManager::loadWAVFile("bounce.wav");

	Texture* heightMap = AssetManager::loadTexture("terrainHeightMapPool.png");
	Model* terrainModel = AssetManager::genTerrainModel(heightMap);

	Material* blendMap = AssetManager::createMaterial(AssetManager::loadTexture("blendMap.png", true));
	Material* material0 = AssetManager::createMaterial(AssetManager::loadTexture("testGrass.png", true));
	Material* material1 = AssetManager::createMaterial(AssetManager::loadTexture("testStone.png", true));
	Material* material2 = AssetManager::createMaterial(AssetManager::loadTexture("testFlowers.png", true));
	Material* material3 = AssetManager::createMaterial(AssetManager::loadTexture("testDirt.png", true));
	std::vector<const Material*> terrainMaterials = { blendMap, material0, material1, material2, material3 };

	Object* terrain = new Object("terrain", glm::vec3(), glm::vec3(), glm::vec3(10, 5, 10));
	addRenderable(terrain->addComponent<RenderComponent>(terrainModel, terrainMaterials, terrainShader));
	addObject(terrain);

	Model* quadModel = AssetManager::getModelPrimitive(ModelPrimitive::QUAD);
	Model* teapotModel = AssetManager::loadModel("teapot.obj");
	Model* stallModel = AssetManager::loadModel("stall.obj");

	TextureAtlas* textureAtlas = AssetManager::loadTextureAtlas("testAtlas.png", glm::uvec2(2, 2));

	//Texture* testObjTexture = AssetManager::getTextureFromAtlas(textureAtlas, 0, glm::uvec2(2, 2));
	//Texture* testObj2Texture = AssetManager::getTextureFromAtlas(textureAtlas, 1, glm::uvec2(2, 2));

	Object* testObj = new Object("testObj", glm::vec3(0, 0.5f, 0), glm::vec3(90, 0, 0));
	addRenderable(testObj->addComponent<RenderComponent>(quadModel, AssetManager::createMaterial(textureAtlas, 0)));
	addObject(testObj);

	Object* testObj2 = new Object("testObj2", glm::vec3(0, 0.5f, 3), glm::vec3(90, 0, 0));
	addRenderable(testObj2->addComponent<RenderComponent>(quadModel, AssetManager::createMaterial(textureAtlas, 1)));
	addObject(testObj2);

	Object* teapot = new Object("teapot", glm::vec3(3, 0.5f, 0), glm::vec3(-90.0f, 0, 0), glm::vec3(0.001f, 0.001f, 0.001f));
	addRenderable(teapot->addComponent<RenderComponent>(teapotModel, AssetManager::createMaterial(AssetManager::loadTexture("lambert.png", true), 5.0f, 10.0f)));
	teapot->addComponent<AABBColliderComponent>(glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1));
	addObject(teapot);

	Object* stall = new Object("stall", glm::vec3(-5, 0, 0), glm::vec3(0, 180.0f, 0), glm::vec3(0.5f, 0.5f, 0.5f));
	addRenderable(stall->addComponent<RenderComponent>(stallModel, AssetManager::createMaterial(AssetManager::loadTexture("stall.png", true))));
	addObject(stall);

	Object* water = new Object("water", glm::vec3(0, -0.5f, 0), glm::vec3(0, 0, 0), glm::vec3(10, 1, 10));
	//AudioSourceComponent* waterSound = water->addComponent<AudioSourceComponent>(sound);
	setWater(water->addComponent<WaterComponent>(AssetManager::loadTexture("water_dudv.jpg"), AssetManager::loadTexture("water_normalmap.jpg"), 1.0f, 40.0f));
	addObject(water);

	// Adds player controls to camera
	Object* cameraObject = mainCamera->getObject();

	cameraObject->position = glm::vec3(0, 5, 5);
	cameraObject->rotation = glm::vec3(-45, 0, 0);
	cameraObject->addComponent<TestComponent>();

	/*Object* light0 = new Object("Point Light", glm::vec3(0, 0, 11));
	addLight(light0->addComponent<LightComponent>(Light::POINT, 100.0f, 5.0f));
	addObject(light0);*/

	Object* light1 = new Object("Directional Light", glm::vec3(), glm::vec3(135, 0, 0));
	addLight(light1->addComponent<LightComponent>(Light::DIRECTIONAL));
	addObject(light1);

	//Object* light2 = new Object("Spotlight", glm::vec3(0, 1, 0), glm::vec3(90, 0, 0));
	//addLight(light2->addComponent<LightComponent>(Light::CONE, 10.0f, 5.0f, glm::vec3(1, 1, 1)));
	//addObject(light2);

	float guiSize = 0.25f;

	Object* reflectionGUI = new Object("Reflection Preview", glm::vec3(-0.7f, -0.8f, 0), glm::vec3(), glm::vec3(guiSize, guiSize * (*windowWidth / *windowHeight), 1));
	addGUI(reflectionGUI->addComponent<GUIComponent>(AssetManager::createMaterial(waterReflectionTexture)));
	addObject(reflectionGUI);

	Object* refractionGUI = new Object("Refraction Preview", glm::vec3(0.7f, -0.8f, 0), glm::vec3(), glm::vec3(guiSize, guiSize * (*windowWidth / *windowHeight), 1));
	addGUI(refractionGUI->addComponent<GUIComponent>(AssetManager::createMaterial(waterRefractionTexture)));
	addObject(refractionGUI);

	//waterSound->shouldLoop(AL_TRUE);
	//waterSound->play();

	return true;
}
