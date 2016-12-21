#include "Material.h"

namespace AdventureEngine
{
	Material::Material(Texture* texture, float reflectivity, float specularDamping)
	{
		m_texture = texture;

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

	float Material::getReflectivity() const
	{
		return m_reflectivity;
	}

	float Material::getSpecularDamping() const
	{
		return m_specularDamping;
	}
}
