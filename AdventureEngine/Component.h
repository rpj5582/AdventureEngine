#pragma once

#include <json.hpp>

#include "Object.h"
#include "AssetManager.h"

using nlohmann::json;

namespace AdventureEngine
{
	class Component
	{
	public:
		Component(Object* obj);
		virtual ~Component();

		virtual void initFromJSON(AssetManager* assetManager, json assets, json args) = 0;
		virtual void update() = 0;

	protected:
		Object* object;
	};
}
