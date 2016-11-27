#pragma once
#include "Asset.h"
class TextureAsset : public Asset
{
public:
	TextureAsset();
	TextureAsset(GLuint assetID);
	~TextureAsset();
};

