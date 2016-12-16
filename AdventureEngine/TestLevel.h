#pragma once
#include "Level.h"

#include "TerrainModelComponent.h"
#include "TestComponent.h"

using namespace AdventureEngine;

class TestLevel : public Level
{
public:
	TestLevel(float* aspectRatio);
	~TestLevel();

	virtual bool load() override;

private:
	Shader* terrainShader;
};

