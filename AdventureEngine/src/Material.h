#pragma once

#include "Texture.h"
#include "TextureAtlas.h"
#include "Cubemap.h"

namespace AdventureEngine
{
	enum TextureType
	{
		TEXTURE,
		TEXTURE_ALTAS,
		CUBEMAP
	};

	class Material
	{
	public:
		friend class AssetManager;

		const TextureType getTextureType() const;
		const Texture* getTexture() const;
		const TextureAtlas* getTextureAtlas() const;
		const Cubemap* getCubemap() const;

		const unsigned int getAtlasIndex() const;

		float getReflectivity() const;
		float getSpecularDamping() const;

	private:
		Material(const Texture* texture, float reflectivity, float specularDamping);
		Material(const TextureAtlas* textureAtlas, unsigned int atlasIndex, float reflectivity, float specularDamping);
		Material(const Cubemap* cubemap, float reflectivity, float specularDamping);
		~Material();

		union TextureUnion
		{
			const Texture* m_texture;
			const TextureAtlas* m_textureAtlas;
			const Cubemap* m_cubeMap;
		};

		struct TextureStruct
		{
			TextureType m_textureType;
			TextureUnion m_textureUnion;
		} m_textureStruct;
		
		unsigned int m_atlasIndex;

		float m_reflectivity;
		float m_specularDamping;
	};
}

