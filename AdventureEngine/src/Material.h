#pragma once

#include "Texture.h"

namespace AdventureEngine
{
	class Material
	{
	public:
		friend class AssetManager;

		~Material();

		const Texture* getTexture() const;

		float getReflectivity() const;
		float getSpecularDamping() const;

	private:
		Material(Texture* texture, float reflectivity, float specularDamping);

		Texture* m_texture;

		float m_reflectivity;
		float m_specularDamping;
	};
}

