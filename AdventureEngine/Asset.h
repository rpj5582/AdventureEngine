#pragma once

#include <GL\glew.h>

class Asset
{
public:
	Asset();
	Asset(GLuint assetID);
	virtual ~Asset();

	const GLuint getAssetID() const;
	bool isEmpty() const;

protected:
	GLuint m_assetID;
};

