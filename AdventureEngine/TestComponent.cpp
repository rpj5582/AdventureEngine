#include "TestComponent.h"

TestComponent::TestComponent()
{
}

TestComponent::~TestComponent()
{
}

void TestComponent::init()
{
}

void TestComponent::update()
{
	float angularSpeed = 0.025f;

	float maxSpeed = 0.5f;
	float minSpeed = 0.1f;
	float speed = minSpeed;

	if (Input::getButton(GLFW_KEY_LEFT_SHIFT))
	{
		speed = maxSpeed;
	}

	if (Input::getButtonUp(GLFW_KEY_LEFT_SHIFT))
	{
		speed = minSpeed;
	}

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

	//if (Input::getButton(GLFW_KEY_W))
	//{
	//	object->position += object->getUp() * speed;
	//}

	//if (Input::getButton(GLFW_KEY_S))
	//{
	//	object->position -= object->getUp() * speed;
	//}

	if (Input::getButton(GLFW_KEY_W))
	{
		object->position -= object->getForward() * speed;
	}

	if (Input::getButton(GLFW_KEY_S))
	{
		object->position += object->getForward() * speed;
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
