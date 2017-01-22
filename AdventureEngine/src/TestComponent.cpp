#include "TestComponent.h"

#include <iostream>

TestComponent::TestComponent()
{
}

TestComponent::~TestComponent()
{
}

void TestComponent::init()
{
}

void TestComponent::update(float deltaTime)
{
	float angularSpeed = 90;

	float maxSpeed = 50;
	float minSpeed = 10;
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
		object->rotation.y -= angularSpeed * deltaTime;
	}

	if (Input::getButton(GLFW_KEY_LEFT))
	{
		object->rotation.y += angularSpeed * deltaTime;
	}

	if (Input::getButton(GLFW_KEY_UP))
	{
		object->rotation.x += angularSpeed * deltaTime;
	}

	if (Input::getButton(GLFW_KEY_DOWN))
	{
		object->rotation.x -= angularSpeed * deltaTime;
	}

	//if (Input::getButton(GLFW_KEY_W))
	//{
	//	object->position += object->getUp() * speed * deltaTime;
	//}

	//if (Input::getButton(GLFW_KEY_S))
	//{
	//	object->position -= object->getUp() * speed * deltaTime;
	//}

	if (Input::getButton(GLFW_KEY_W))
	{
		object->position -= object->getForward() * speed * deltaTime;
	}

	if (Input::getButton(GLFW_KEY_S))
	{
		object->position += object->getForward() * speed * deltaTime;
	}

	if (Input::getButton(GLFW_KEY_D))
	{
		object->position += object->getRight() * speed * deltaTime;
	}

	if (Input::getButton(GLFW_KEY_A))
	{
		object->position -= object->getRight() * speed * deltaTime;
	}
}
