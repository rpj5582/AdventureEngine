#pragma once
#include "Scene.h"

#include "TestComponent.h"

using namespace AdventureEngine;

class TestScene : public Scene
{
public:
	TestScene(const int* windowWidth, const int* windowHeight);
	~TestScene();

	virtual bool load() override;

private:
	Shader* terrainShader;
};

