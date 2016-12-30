#pragma once
#include "Component.h"

#include "Shader.h"

namespace AdventureEngine
{
	class WaterComponent : public Component
	{
	public:
		WaterComponent();
		WaterComponent(const Texture* dudvMap);
		WaterComponent(const Texture* dudvMap, const Texture* normalMap);
		WaterComponent(const Texture* dudvMap, const Texture* normalMap, float reflectivity);
		WaterComponent(const Texture* dudvMap, const Texture* normalMap, float reflectivity, float specularDamping);
		WaterComponent(const Texture* dudvMap, const Texture* normalMap, float reflectivity, float specularDamping, glm::vec4 color);
		WaterComponent(const Texture* dudvMap, const Texture* normalMap, float reflectivity, float specularDamping, glm::vec4 color, const Shader* shader);


		~WaterComponent();

		glm::vec4 color;
		const Texture* dudvMap;
		const Texture* normalMap;
		float reflectivity;
		float specularDamping;
		const Shader* shader;

		float waveMovement;

	private:
		virtual void init() override;
		virtual void update(float deltaTime) override;
	};
}