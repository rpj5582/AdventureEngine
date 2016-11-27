#include "TestComponent.h"

TestComponent::TestComponent(Object* object) : Component(object)
{
}

TestComponent::~TestComponent()
{
}

void TestComponent::initFromJSON(AssetManager* assetManager, json assets, json args)
{
}

void TestComponent::update()
{
	if (Input::getButton(GLFW_KEY_RIGHT))
	{
		object->position.x += 0.1f;
	}

	if (Input::getButton(GLFW_KEY_LEFT))
	{
		object->position.x -= 0.1f;
	}

	if (Input::getButton(GLFW_KEY_UP))
	{
		object->position.y += 0.1f;
	}

	if (Input::getButton(GLFW_KEY_DOWN))
	{
		object->position.y -= 0.1f;
	}
}
