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
		unsigned int getAtlasIndex() const;

		float getReflectivity() const;
		float getSpecularDamping() const;

	private:
		Material(Texture* texture, unsigned int atlasIndex, float reflectivity, float specularDamping);

		Texture* m_texture;
		unsigned int m_atlasIndex;

		float m_reflectivity;
		float m_specularDamping;
	};
}

