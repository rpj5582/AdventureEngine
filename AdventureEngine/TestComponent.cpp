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
	float speed = 0.1f;
	float angularSpeed = 0.025f;

	if (Input::getButton(GLFW_KEY_RIGHT))
	{
		object->rotation.y -= angularSpeed;
	}

	if (Input::getButton(GLFW_KEY_LEFT))
	{
		object->rotation.y += angularSpeed;
	}

	if (Input::getButton(GLFW_KEY_UP))
	{
		object->rotation.x += angularSpeed;
	}

	if (Input::getButton(GLFW_KEY_DOWN))
	{
		object->rotation.x -= angularSpeed;
	}

	if (Input::getButton(GLFW_KEY_W))
	{
		object->position += object->getForward() * speed;
	}

	if (Input::getButton(GLFW_KEY_S))
	{
		object->position -= object->getForward() * speed;
	}

	if (Input::getButton(GLFW_KEY_D))
	{
		object->position += object->getRight() * speed;
	}

	if (Input::getButton(GLFW_KEY_A))
	{
		object->position -= object->getRight() * speed;
	}
}
