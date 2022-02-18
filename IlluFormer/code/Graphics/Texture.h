#pragma once

#include "Renderer.h"
#include <vector>

enum TextureName
{
	TEXTURE_PLAYER = 0,
	TEXTURE_WALL = 1,
	TEXTURE_COIN = 2,
	TEXTURE_ENEMY = 3,
	TEXTURE_BG = 4
};

class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
public:

	Texture(const std::string& path);
	~Texture();
	//Texture(const Texture& oldTexture); //Copy Init

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};

class TextureList
{
private:
public:
	static std::vector<Texture*> textures; // Init all textures and make unload class

	static bool GenerateTextures();
	static void ClearTextures();
	static Texture* EvalTextureName(std::string name);
};

/*
TEXTURE_PLAYER = 0
TEXTURE_BRICK = 1
TEXTURE_COIN = 2
TEXTURE_ENEMY = 3


a list of textures to save memory on drawing them
 */