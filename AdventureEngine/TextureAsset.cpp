#include "TextureAsset.h"

TextureAsset::TextureAsset() : Asset()
{
}

TextureAsset::TextureAsset(GLuint assetID) : Asset(assetID)
{
}

TextureAsset::~TextureAsset()
{
	glDeleteTextures(1, &m_assetID);
}
