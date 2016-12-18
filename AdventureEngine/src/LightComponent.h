#pragma once
#include "Component.h"

namespace AdventureEngine
{
	enum Light
	{
		POINT,
		CONE,
		DIRECTIONAL
	};

	class LightComponent : public Component
	{
	public:
		LightComponent();
		LightComponent(Light lightType);
		LightComponent(Light lightType, float intensity);
		LightComponent(Light lightType, float intensity, float radius);
		LightComponent(Light lightType, float intensity, float radius, glm::vec3 color);
		LightComponent(Light lightType, float intensity, float radius, float coneAngle);
		LightComponent(Light lightType, float intensity, float radius, glm::vec3 color, float coneAngle);
		~LightComponent();

		Light lightType;
		float intensity;
		float radius;
		glm::vec3 color;
		float coneAngle;

	private:
		virtual void init() override;
		virtual void update(float deltaTime) override;
	};
}
