#pragma once
#include "Asset.h"
class ModelAsset : public Asset
{
public:
	ModelAsset();
	ModelAsset(GLuint assetID, unsigned int vertexCount);
	~ModelAsset();

	const unsigned int getVertexCount() const;

private:
	unsigned int m_vertexCount;
};

