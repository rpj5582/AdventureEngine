#pragma once

#include <GL\glew.h>

class Asset
{
public:
	Asset(GLuint assetID);
	virtual ~Asset();

	const GLuint getAssetID() const;

protected:
	GLuint m_assetID;
};

