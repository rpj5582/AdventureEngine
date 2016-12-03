#include "Object.h"

#include "Component.h"

namespace AdventureEngine
{
	std::unordered_map<std::string, Component*(Object::*)()> Object::m_componentTypes;

	Object::Object(std::string name) : Object(name, { 0, 0, 0 }) { }

	Object::Object(std::string name, glm::vec3 position) : Object(name, position, { 0, 0, 0 }, { 1, 1, 1 }) { }

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

	Component* Object::addRegisteredComponent(std::string componentName)
	{
		assert(m_componentTypes.find(componentName) != m_componentTypes.end());
		
		return (this->*(m_componentTypes.at(componentName)))();;
	}

	glm::mat3 Object::getRotationMatrix() const
	{
		return glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);
	}

	glm::vec3 Object::getRight() const
	{
		return getRotationMatrix() * glm::vec3(1, 0, 0);
	}

	glm::vec3 Object::getUp() const
	{
		return getRotationMatrix() * glm::vec3(0, 1, 0);
	}

	glm::vec3 Object::getForward() const
	{
		return getRotationMatrix() * glm::vec3(0, 0, -1);
	}
}
