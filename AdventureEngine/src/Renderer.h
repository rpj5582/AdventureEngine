#pragma once

#include "Shader.h"

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