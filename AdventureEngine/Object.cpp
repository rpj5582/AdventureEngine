#include "Object.h"

#include "Component.h"

namespace AdventureEngine
{
	Object::Object(std::string name) : Object(name, { 0, 0, 0 }) { }

	Object::Object(std::string name, glm::vec3 position) : Object(name, { 0, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1 }) { }

	Object::Object(std::string name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
	{
		m_name = name;
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;
	}

	Object::~Object()
	{
		for (unsigned int i = 0; i < m_components.size(); i++)
		{
			delete m_components[i];
		}
	}

	void Object::update()
	{
		for (unsigned int i = 0; i < m_components.size(); i++)
		{
			m_components[i]->update();
		}
	}
}
