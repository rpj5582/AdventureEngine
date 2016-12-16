#pragma once


#include "Object.h"
#include "AssetManager.h"

namespace AdventureEngine
{
	class Component
	{
	public:
		friend class Object;
		Component();
		virtual ~Component();

	protected:
		Object* object;

	private:
		virtual void init() = 0;
		virtual void update() = 0;
	};
}
