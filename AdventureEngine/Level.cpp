#include "Level.h"

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <json.hpp>

#include <iostream>
#include <fstream>

using nlohmann::json;

namespace AdventureEngine
{
	Level::Level(float* aspectRatio)
	{
		m_aspectRatio = aspectRatio;

		m_assetManager = new AssetManager();
	}

	Level::~Level()
	{
		delete m_assetManager;

		for (unsigned int i = 0; i < m_objects.size(); i++)
		{
			delete m_objects[i];
		}

		m_aspectRatio = nullptr;
	}

	bool Level::load(const char* levelPath)
	{
		// Opens the level json file
		std::ifstream ifs(levelPath);
		if (ifs.is_open())
		{
			// Reads in the json
			std::string jsonString = std::string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
			const char* jsonCString = jsonString.c_str();

			// Parses the string to a json object
			json j = json::parse(jsonCString);

			// Gets the list of objects in the level
			json objects = j["objects"];

			// Map of all assets loaded by this level
			std::unordered_map<std::string, Asset*> assetMap;

			// Register all components
			ComponentRegistry registry;

			for (unsigned int i = 0; i < objects.size(); i++)
			{
				json object = objects[i];

				// Load in the object name if it exists
				std::string name;
				if (object.count("name") == 1)
				{
					name = object["name"];
				}

				// Loads in the position if it exists
				glm::vec3 position;
				if (object.count("position") == 1)
				{
					position = glm::vec3(object["position"]["x"], object["position"]["y"], object["position"]["z"]);
				}

				// Loads in the rotation if it exists
				glm::vec3 rotation;
				if (object.count("rotation") == 1)
				{
					rotation = glm::vec3(object["rotation"]["x"], object["rotation"]["y"], object["rotation"]["z"]);
				}

				// Loads in the scale if it exists
				glm::vec3 scale;
				if (object.count("scale") == 1)
				{
					scale = glm::vec3(object["scale"]["x"], object["scale"]["y"], object["scale"]["z"]);
				}

				// Creates the Object
				Object* obj = new Object(name, position, rotation, scale);

				// Adds each component in the component list
				if (object.count("components") == 1)
				{
					json components = object["components"];
					for (unsigned int i = 0; i < components.size(); i++)
					{
						json component = components[i];
						std::string componentName = component["name"];
						json componentAssets = component["assets"];
						json componentArgs = component["args"];

						Component* c = obj->addRegisteredComponent(componentName);
						c->initFromJSON(m_assetManager, componentAssets, componentArgs);
					}
				}

				m_objects.push_back(obj);
			}

			// Creates a main camera
			m_mainCamera = new Object("mainCamera", { 0.0f, 0.0f, -100.0f });
			m_mainCamera->addComponent<CameraComponent>(5.0f, 100.0f);
			m_mainCamera->addComponent<TestComponent>();

			m_objects.push_back(m_mainCamera);

			ifs.close();
			return true;
		}

		return false;
	}

	void Level::update()
	{
		for (unsigned int i = 0; i < m_objects.size(); i++)
		{
			m_objects[i]->update();
		}
	}

	void Level::draw()
	{
		for (unsigned int i = 0; i < m_objects.size(); i++)
		{
			glm::mat4 translationMatrix = glm::translate(m_objects[i]->position);
			glm::mat4 rotationMatrix = glm::yawPitchRoll(m_objects[i]->rotation.y, m_objects[i]->rotation.x, m_objects[i]->rotation.z);
			glm::mat4 scaleMatrix = glm::scale(m_objects[i]->scale);

			// Model matrix
			glm::mat4 modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;

			// model-view-projection matrix
			glm::mat4 mvp = m_mainCamera->getComponent<CameraComponent>()->calculateCameraMatrix(*m_aspectRatio) * modelMatrix;

			// Sends the mvp matrix to the graphics card
			glUniformMatrix4fv(3, 1, GL_FALSE, &mvp[0][0]);

			// Draws the object
			RenderComponent* renderComponent = m_objects[i]->getComponent<RenderComponent>();
			if (renderComponent && renderComponent->model)
			{
				if (renderComponent->texture)
				{
					glBindTexture(GL_TEXTURE_2D, renderComponent->texture->getAssetID());
				}
				else
				{
					glBindTexture(GL_TEXTURE_2D, 0);
				}

				glBindVertexArray(renderComponent->model->getAssetID());
				glDrawArrays(GL_TRIANGLES, 0, renderComponent->model->getVertexCount());
			}
		}
	}
}
