#include "Component.h"

namespace AdventureEngine
{
	Component::Component()
	{
		object = nullptr;
	}

	Component::~Component()
	{
	}

	Object* Component::getObject() const
	{
		return object;
	}
}
