#include "Scene.h"

namespace AdventureEngine
{
	Scene::Scene(float* aspectRatio)
	{
		m_sceneRenderer = nullptr;
		m_guiRenderer = nullptr;

		this->aspectRatio = aspectRatio;
	}

	Scene::~Scene()
	{
		for (unsigned int i = 0; i < objects.size(); i++)
		{
			delete objects[i];
		}

		delete m_sceneRenderer;
		delete m_guiRenderer;

		AssetManager::clean();
	}

	bool Scene::load()
	{
		AssetManager::init();
		
		m_sceneRenderer = new SceneRenderer();
		if (!m_sceneRenderer->load())
		{
			return false;
		}

		m_guiRenderer = new GUIRenderer();
		if (!m_guiRenderer->load())
		{
			return false;
		}

		return true;
	}

	void Scene::update(float deltaTime)
	{
		for (unsigned int i = 0; i < objects.size(); i++)
		{
			/// UPDATE OBJECTS
			objects[i]->update(deltaTime);

			/// HANDLE COLLISIONS
			// NOT YET IMPLEMENTED
		}
	}

	void Scene::render() const
	{
		// Renders the scene
		m_sceneRenderer->render(objects, lights, mainCamera);

		// Renders the GUI
		m_guiRenderer->render(guiObjects);
	}
}
