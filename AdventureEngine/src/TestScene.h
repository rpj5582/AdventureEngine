#pragma once
#include "Scene.h"

#include "TestComponent.h"

using namespace AdventureEngine;

class TestScene : public Scene
{
public:
	TestScene(float* aspectRatio);
	~TestScene();

	virtual bool load() override;

private:
	Shader* terrainShader;
};

