#pragma once
#include "Component.h"

namespace AdventureEngine
{
	class LightComponent : public Component
	{
	public:
		LightComponent();
		LightComponent(float radius);
		LightComponent(float radius, glm::vec3 color);
		LightComponent(float radius, glm::vec3 color, bool isDirectionalLight);
		~LightComponent();

		float radius;
		glm::vec3 color;
		bool isDirectionalLight;

	private:
		void init() override;
		void update() override;
	};
}


