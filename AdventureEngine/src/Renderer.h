#pragma once

#include "Shader.h"

#include <vector>

#define MAX_LIGHTS 16

namespace AdventureEngine
{
	class Renderer
	{
	public:
		Renderer();
		virtual ~Renderer();

		virtual bool load() = 0;

	protected:
		Shader* m_defaultShader;
	};
}