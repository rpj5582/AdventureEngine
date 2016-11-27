#include "ModelAsset.h"

ModelAsset::ModelAsset() : Asset()
{
}

ModelAsset::ModelAsset(GLuint assetID, unsigned int vertexCount) : Asset(assetID)
{
	m_vertexCount = vertexCount;
}

ModelAsset::~ModelAsset()
{
}

const unsigned int ModelAsset::getVertexCount() const
{
	return m_vertexCount;
}
