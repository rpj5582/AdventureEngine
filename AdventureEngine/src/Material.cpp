#include "Material.h"

namespace AdventureEngine
{
	Material::Material(Texture* texture, unsigned int atlasIndex, float reflectivity, float specularDamping)
	{
		m_texture = texture;
		m_atlasIndex = atlasIndex;

		m_reflectivity = reflectivity;
		m_specularDamping = specularDamping;
	}

	Material::~Material()
	{
	}

	const Texture* Material::getTexture() const
	{
		return m_texture;
	}

	unsigned int Material::getAtlasIndex() const
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
