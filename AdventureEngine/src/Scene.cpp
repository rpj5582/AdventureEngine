#include "Scene.h"

namespace AdventureEngine
{
	Scene::Scene(const int* windowWidth, const int* windowHeight)
	{
		m_sceneRenderer = nullptr;
		m_waterRenderer = nullptr;
		m_guiRenderer = nullptr;

		m_water = nullptr;
		m_skybox = nullptr;
		mainCamera = nullptr;

		waterReflectionTexture = nullptr;
		waterRefractionTexture = nullptr;

		this->windowWidth = windowWidth;
		this->windowHeight = windowHeight;
	}

	Scene::~Scene()
	{
		for (unsigned int i = 0; i < m_objects.size(); i++)
		{
			delete m_objects[i];
		}

		delete m_sceneRenderer;
		delete m_waterRenderer;
		delete m_skyboxRenderer;
		delete m_guiRenderer;

		AudioManager::clean();
		AssetManager::clean();
	}

	void Scene::addObject(Object* object)
	{
		m_objects.push_back(object);
	}

	void Scene::addRenderable(RenderComponent* render)
	{
		m_renderables.push_back(render);
	}

	void Scene::setWater(WaterComponent* water)
	{
		m_water = water;
	}

	void Scene::addLight(LightComponent* light)
	{
		m_lights.push_back(light);
	}

	void Scene::addGUI(GUIComponent* gui)
	{
		m_guiObjects.push_back(gui);
	}

	bool Scene::load()
	{
		AssetManager::init();
		AudioManager::init();

		m_sceneRenderer = new SceneRenderer();
		if (!m_sceneRenderer->load())
		{
			return false;
		}

		m_waterRenderer = new WaterRenderer(windowWidth, windowHeight);
		if (!m_waterRenderer->load())
		{
			return false;
		}

		waterReflectionTexture = m_waterRenderer->getReflectionTexture();
		waterRefractionTexture = m_waterRenderer->getRefractionTexture();

		m_skyboxRenderer = new SkyboxRenderer();
		if (!m_skyboxRenderer->load())
		{
			return false;
		}

		m_guiRenderer = new GUIRenderer();
		if (!m_guiRenderer->load())
		{
			return false;
		}

		// Default camera
		Object* mainCameraObject = new Object("Main Camera", glm::vec3(0, 0, 5));
		mainCamera = mainCameraObject->addComponent<CameraComponent>(windowWidth, windowHeight, 0.1f, 100.0f);
		mainCameraObject->addComponent<AudioListenerComponent>();
		addObject(mainCameraObject);

		// Default skybox
		Cubemap* skyboxCubeMap = AssetManager::loadCubeMap(std::vector<std::string> { "skybox/right.png", "skybox/left.png", "skybox/bottom.png", "skybox/top.png", "skybox/back.png", "skybox/front.png" });

		Object* skyboxObject = new Object("Skybox");
		m_skybox = skyboxObject->addComponent<SkyboxComponent>(skyboxCubeMap);
		addObject(skyboxObject);

		// Default fog
		Object* fogObject = new Object("Fog");
		m_fog = fogObject->addComponent<FogComponent>(glm::vec3(0.8f, 0.8f, 1.0f), 0.005f, 1.0f, -0.25f, 0.25f);
		addObject(fogObject);

		return true;
	}

	void Scene::update(float deltaTime)
	{
		// Updates all objects
		for (unsigned int i = 0; i < m_objects.size(); i++)
		{
			m_objects[i]->update(deltaTime);
		}

		// Handle collision detection
		for (unsigned int i = 0; i < m_objects.size(); i++)
		{
			for (unsigned int j = 1; j < m_objects.size(); j++)
			{
				Object* object1 = m_objects[i];
				Object* object2 = m_objects[j];

				if (object1 == object2) continue;

				AABBColliderComponent* collider1 = object1->getComponent<AABBColliderComponent>();
				AABBColliderComponent* collider2 = object2->getComponent<AABBColliderComponent>();

				if (collider1 && collider2)
				{
					if (collider1->collidesWithAABB(*collider2))
					{
						std::cout << object1->getName() << " and " << object2->getName() << " are colliding!" << std::endl;
					}
				}
			}
		}
	}

	void Scene::render() const
	{
		// Sets the clear color to the fog color
		if (m_fog)
		{
			glm::vec3 fogColor = m_fog->getColor();
			glClearColor(fogColor.r, fogColor.g, fogColor.b, 1);
		}
		else
		{
			glClearColor(1, 1, 1, 1);
		}

		if (m_water) renderWaterFBOs();

		// Renders the scene to the screen
		m_sceneRenderer->render(m_renderables, m_lights, mainCamera, m_fog);

		// Renders the skybox to the screen
		if(m_skybox) m_skyboxRenderer->render(m_skybox, mainCamera, m_fog);

		// Renders the water
		if(m_water) m_waterRenderer->render(m_water, m_lights, mainCamera, m_fog);

		// Renders the GUI
		m_guiRenderer->render(m_guiObjects);
	}

	void Scene::renderWaterFBOs() const
	{
		const Object* waterObject = m_water->getObject();
		Object* mainCameraObject = mainCamera->getObject();

		float heightAboveWater = mainCameraObject->position.y - waterObject->position.y;

		// Adjusts the camera for the reflection
		mainCameraObject->position.y -= heightAboveWater * 2;
		mainCameraObject->rotation.x *= -1;
		mainCameraObject->rotation.z *= -1;

		// Binds rendering to the reflection FBO
		m_waterRenderer->bindReflectionFBO();

		// Renders the scene to the reflection FBO
		m_sceneRenderer->render(m_renderables, m_lights, mainCamera, m_fog, glm::vec4(0, 1, 0, -waterObject->position.y));

		// Renders the skybox to the reflection FBO
		m_skyboxRenderer->render(m_skybox, mainCamera, m_fog);

		// Puts the camera back
		mainCameraObject->position.y += heightAboveWater * 2;
		mainCameraObject->rotation.x *= -1;
		mainCameraObject->rotation.z *= -1;

		// Binds rendering to the refraction FBO
		m_waterRenderer->bindRefractionFBO();

		// Renders the scene to the refraction FBO
		m_sceneRenderer->render(m_renderables, m_lights, mainCamera, m_fog, glm::vec4(0, -1, 0, waterObject->position.y));

		// Renders the skybox to the refraction FBO
		m_skyboxRenderer->render(m_skybox, mainCamera, m_fog);

		// Unbinds the FBO
		m_waterRenderer->unbindFBOs();
	}
}
