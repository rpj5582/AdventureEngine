#include "TestComponent.h"

TestComponent::TestComponent(Object* object) : Component(object)
{
	Object::registerComponent<TestComponent>("TestComponent");
}

TestComponent::~TestComponent()
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
