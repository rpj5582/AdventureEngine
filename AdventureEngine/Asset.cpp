#include "Asset.h"

Asset::Asset()
{
	m_assetID = 0;
}

Asset::Asset(GLuint assetID)
{
	m_assetID = assetID;
}

Asset::~Asset()
{
}

const GLuint Asset::getAssetID() const
{
	return m_assetID;
}

bool Asset::isEmpty() const
{
	return m_assetID == 0;
}
