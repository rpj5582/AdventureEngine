#pragma once

#include <vector>
#include <unordered_map>
#include <typeindex>
#include <glm\glm.hpp>
#include <glm\gtx\euler_angles.hpp>

namespace AdventureEngine
{
	class Component;

	class Object
	{
	public:
		Object(std::string name);
		Object(std::string name, glm::vec3 position);
		Object(std::string name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
		virtual ~Object();

		void update();

		template <typename T>
		T* getComponent() const;

		template<typename T>
		Component* addComponentEmpty();

		template<typename T, typename... Args>
		T* addComponent(Args... args);

		Component* addRegisteredComponent(std::string componentName);

		template <typename T>
		static bool registerComponent(std::string componentName);

		glm::mat3 getRotationMatrix() const;
		glm::vec3 getRight() const;
		glm::vec3 getUp() const;
		glm::vec3 getForward() const;

		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;

	private:
		std::string m_name;
		std::vector<Component*> m_components;

		static std::unordered_map<std::string, Component*(Object::*)()> m_componentTypes;
	};

	template<typename T>
	inline T* Object::getComponent() const
	{
		static_assert(std::is_base_of<Component, T>::value, "Type is not a Component type");

		for (unsigned int i = 0; i < m_components.size(); i++)
		{
			if (typeid(*m_components[i]) == typeid(T))
			{
				return static_cast<T*>(m_components[i]);
			}
		}

		return nullptr;
	}

	template<typename T>
	inline Component* Object::addComponentEmpty()
	{
		static_assert(std::is_base_of<Component, T>::value, "Type is not a Component type");
		
		T* component = new T(this);
		m_components.push_back(component);

		return component;
	}

	template<typename T, typename... Args>
	inline T* Object::addComponent(Args... args)
	{
		static_assert(std::is_base_of<Component, T>::value, "Type is not a Component type");

		T* component = new T(this, args...);
		m_components.push_back(component);

		return component;
	}

	template<typename T>
	inline bool Object::registerComponent(std::string componentName)
	{
		if (m_componentTypes.find(componentName) == m_componentTypes.end())
		{
			m_componentTypes[componentName] = &addComponentEmpty<T>;
		}

		return false;
	}
}
