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

		Object* getObject() const;

	protected:
		Object* object;

	private:
		virtual void init() = 0;
		virtual void update(float deltaTime) = 0;
	};
}
