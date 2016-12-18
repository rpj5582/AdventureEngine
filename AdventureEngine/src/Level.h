#pragma once

#include <iostream>

#include "AssetManager.h"

#include "Object.h"

#include "CameraComponent.h"
#include "RenderComponent.h"
#include "ShaderComponent.h"
#include "TerrainModelComponent.h"
#include "LightComponent.h"

#define MAX_LIGHTS 16

namespace AdventureEngine
{
	class Level
	{
	public:
		Level(float* aspectRatio);
		virtual ~Level();

		virtual bool load();
		void update(float deltaTime);
		void render() const;

	protected:
		GLuint getDefaultShaderID() const;
		std::vector<Object*> lights;
		std::vector<Object*> objects;
		Object* mainCamera;

	private:
		void uploadMatrices(const Object* const object) const;
		void renderModel(const Model* model) const;

		Shader* m_defaultShader;

		float* m_aspectRatio;
	};
}
