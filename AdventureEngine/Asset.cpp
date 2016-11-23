#include "Asset.h"

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
