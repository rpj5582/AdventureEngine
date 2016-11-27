#pragma once
#include "Component.h"

#include "Input.h"

using namespace AdventureEngine;

class TestComponent : public Component
{
public:
	TestComponent(Object* object);
	~TestComponent();

	void initFromJSON(AssetManager* assetManager, json assets, json args) override;
	void update() override;
};
