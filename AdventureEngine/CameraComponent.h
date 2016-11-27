#pragma once
#include "Component.h"

#include <glm\gtc\matrix_transform.hpp>

namespace AdventureEngine
{
	class CameraComponent : public Component
	{
	public:
		CameraComponent(Object* object);
		CameraComponent(Object* object, float perspectiveFOV);
		CameraComponent(Object* object, float orthoSize, float maxZ);
		CameraComponent(Object* object, float perspectiveFOV, float orthoSize, float maxZ, bool usePerspective);
		~CameraComponent();

		void initFromJSON(AssetManager* assetManager, json assets, json args) override;
		void update() override;

		glm::mat4 calculateCameraMatrix(float aspectRatio) const;

	private:
		float m_fov;
		float m_orthoSize;
		float m_maxZ;
		bool m_usePerspective;
	};
}
