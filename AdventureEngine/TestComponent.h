#pragma once
#include "Component.h"

#include "Input.h"

using namespace AdventureEngine;

class TestComponent : public Component
{
public:
	TestComponent(Object* object);
	~TestComponent();

	virtual void update() override;
};
