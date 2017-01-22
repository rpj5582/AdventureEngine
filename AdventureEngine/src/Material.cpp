#include "Material.h"

namespace AdventureEngine
{
	Material::Material(const Texture* texture, float reflectivity, float specularDamping)
	{
		m_textureStruct.m_textureType = TextureType::TEXTURE;
		m_textureStruct.m_textureUnion.m_texture = texture;
		m_atlasIndex = 0;

		m_reflectivity = reflectivity;
		m_specularDamping = specularDamping;
	}

	Material::Material(const TextureAtlas* textureAtlas, unsigned int atlasIndex, float reflectivity, float specularDamping)
	{
		m_textureStruct.m_textureType = TextureType::TEXTURE_ALTAS;
		m_textureStruct.m_textureUnion.m_textureAtlas = textureAtlas;
		m_atlasIndex = atlasIndex;

		m_reflectivity = reflectivity;
		m_specularDamping = specularDamping;
	}

	Material::Material(const Cubemap* cubemap, float reflectivity, float specularDamping)
	{
		m_textureStruct.m_textureType = TextureType::CUBEMAP;
		m_textureStruct.m_textureUnion.m_cubeMap = cubemap;
		m_atlasIndex = 0;

		m_reflectivity = reflectivity;
		m_specularDamping = specularDamping;
	}

	Material::~Material()
	{
	}

	const TextureType Material::getTextureType() const
	{
		return m_textureStruct.m_textureType;
	}

	const Texture* Material::getTexture() const
	{
		if (m_textureStruct.m_textureType == TextureType::TEXTURE)
		{
			return m_textureStruct.m_textureUnion.m_texture;
		}

		return nullptr;
	}

	const TextureAtlas* Material::getTextureAtlas() const
	{
		if (m_textureStruct.m_textureType == TextureType::TEXTURE_ALTAS)
		{
			return m_textureStruct.m_textureUnion.m_textureAtlas;
		}

		return nullptr;
	}

	const Cubemap* Material::getCubemap() const
	{
		if (m_textureStruct.m_textureType == TextureType::CUBEMAP)
		{
			return m_textureStruct.m_textureUnion.m_cubeMap;
		}

		return nullptr;
	}

	const unsigned int Material::getAtlasIndex() const
	{
		return m_atlasIndex;
	}

	float Material::getReflectivity() const
	{
		return m_reflectivity;
	}

	float Material::getSpecularDamping() const
	{
		return m_specularDamping;
	}
}
