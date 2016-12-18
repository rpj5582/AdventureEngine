#pragma once
#include "Component.h"

#include "Input.h"

using namespace AdventureEngine;

class TestComponent : public Component
{
public:
	TestComponent();
	~TestComponent();

private:
	void init() override;
	void update(float deltaTime) override;
};
