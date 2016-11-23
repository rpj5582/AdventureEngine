#pragma once

#include <vector>
#include <unordered_map>
#include <typeinfo>
#include <glm\glm.hpp>

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

		template<typename T, typename... Args>
		T* addComponent(Args... args);

		// The component must be registered before calling this function
		template<typename... Args>
		Component* addComponentByName(std::string componentName, Args... args);

		template <typename T>
		static bool registerComponent(std::string componentName);

		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;

	private:
		std::string m_name;
		std::vector<Component*> m_components;

		template <typename T>
		static std::unordered_map<std::string, T*(Object::*)()> m_componentTypes;
	};

	template <typename T>
	std::unordered_map<std::string, T*(Object::*)()> Object::m_componentTypes = std::unordered_map<std::string, T*(Object::*)()>();

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

	template<typename T, typename... Args>
	inline T* Object::addComponent(Args... args)
	{
		static_assert(std::is_base_of<Component, T>::value, "Type is not a Component type");

		T* component = new T(this, args...);
		m_components.push_back(component);

		return component;
	}

	template<typename... Args>
	inline Component* Object::addComponentByName(std::string componentName, Args... args)
	{
		if (m_componentTypes.find(componentName) != m_components.end())
		{
			return m_componentTypes.at(componentName)(args);
		}

		assert("The component " + componentName + " could not be found. Make sure to register the component before adding it.");
		return nullptr;
	}

	template<typename T>
	inline bool Object::registerComponent(std::string componentName)
	{
		if (m_componentTypes<T>.find(componentName) == m_componentTypes<T>.end())
		{
			m_componentTypes<T>[componentName] = &addComponent<T>;
			return true;
		}

		return false;
	}
}
