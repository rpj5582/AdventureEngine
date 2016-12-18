#include "Object.h"

#include "Component.h"

namespace AdventureEngine
{
	Object::Object(std::string name) : Object(name, { 0, 0, 0 }) { }

	Object::Object(std::string name, glm::vec3 position) : Object(name, position, { 0, 0, 0 }) { }

	Object::Object(std::string name, glm::vec3 position, glm::vec3 rotation) : Object(name, position, rotation, { 1, 1, 1 }) { }

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

	void Object::update(float deltaTime)
	{
		for (unsigned int i = 0; i < m_components.size(); i++)
		{
			m_components[i]->update(deltaTime);
		}
	}

	std::string Object::getName() const
	{
		return m_name;
	}

	glm::mat4 Object::getTranslationMatrix() const
	{
		return glm::translate(position);
	}

	glm::mat4 Object::getRotationMatrix() const
	{
		return glm::yawPitchRoll(glm::radians(rotation.y), glm::radians(rotation.x), glm::radians(rotation.z));
	}

	glm::mat4 Object::getScaleMatrix() const
	{
		return glm::scale(scale);
	}

	glm::mat4 Object::getModelMatrix() const
	{
		return getTranslationMatrix() * getRotationMatrix() * getScaleMatrix();
	}

	glm::vec3 Object::getRight() const
	{
		return (glm::mat3)getRotationMatrix() * glm::vec3(1, 0, 0);
	}

	glm::vec3 Object::getUp() const
	{
		return (glm::mat3)getRotationMatrix() * glm::vec3(0, 1, 0);
	}

	glm::vec3 Object::getForward() const
	{
		return (glm::mat3)getRotationMatrix() * glm::vec3(0, 0, 1);
	}
}
