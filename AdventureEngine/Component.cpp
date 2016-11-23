#include "Component.h"

namespace AdventureEngine
{
	Component::Component(Object* obj)
	{
		object = obj;
	}

	Component::~Component()
	{
	}
}
