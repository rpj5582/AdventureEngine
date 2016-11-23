#pragma once

#include "Object.h"

namespace AdventureEngine
{
	class Component
	{
	public:
		Component(Object* obj);
		virtual ~Component();

		virtual void update() = 0;

	protected:
		Object* object;
	};
}
