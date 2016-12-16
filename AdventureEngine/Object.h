#pragma once

#include <vector>
#include <unordered_map>
#include <typeinfo>

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>

namespace AdventureEngine
{
	class Model;
	class Material;
	class Component;

	class Object
	{
	public:
		Object(std::string name);
		Object(std::string name, glm::vec3 position);
		Object(std::string name, glm::vec3 position, glm::vec3 rotation);
		Object(std::string name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
		virtual ~Object();

		void update();

		template <typename T>
		T* getComponent() const;

		template<typename T, typename... Args>
		T* addComponent(Args... args);

		std::string getName() const;

		glm::mat4 getTranslationMatrix() const;
		glm::mat4 getRotationMatrix() const;
		glm::mat4 getScaleMatrix() const;
		glm::mat4 getModelMatrix() const;

		glm::vec3 getRight() const;
		glm::vec3 getUp() const;
		glm::vec3 getForward() const;

		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;

	private:
		std::string m_name;
		std::vector<Component*> m_components;
	};

	template<typename T>
	inline T* Object::getComponent() const
	{
		static_assert(std::is_base_of<Component, T>::value, "Type is not a Component type");

		for (unsigned int i = 0; i < m_components.size(); i++)
		{
			const std::type_info& componentType = typeid(*m_components[i]);
			const std::type_info& paramType = typeid(T);

			T* test = dynamic_cast<T*>(m_components[i]);
			if (test)
			{
				return test;
			}

			//if (componentType == paramType)
			//{
			//	return static_cast<T*>(m_components[i]);
			//}
		}

		return nullptr;
	}

	template<typename T, typename... Args>
	inline T* Object::addComponent(Args... args)
	{
		Component* component = new T(args...);
		component->object = this;
		component->init();

		m_components.push_back(component);

		return static_cast<T*>(component);
	}
}
