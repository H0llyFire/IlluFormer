#include "Texture.h"
#include "../Items/Consts.h"

#include "stb_image/stb_image.h"
#include "fstream"

Texture::Texture(const std::string& path)
	: m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0)
{
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(const unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

bool TextureList::GenerateTextures()
{
	std::ifstream texturesFile("res/textures/textures.dat");
	std::string line;
	while (std::getline(texturesFile, line))
	{
		textures.push_back(new Texture("res/textures/" + line));
	}
	return false;
}

void TextureList::ClearTextures()
{
	for (auto texture : textures)
	{
		delete texture;
	}
	textures.clear();
}

Texture* TextureList::EvalTextureName(std::string name)
{
	if (name == "Player") { return textures[TEXTURE_PLAYER]; }
	if (name == "Wall") { return textures[TEXTURE_WALL]; }
	if (name == "Coin") { return textures[TEXTURE_COIN]; }
	if (name == "Enemy") { return textures[TEXTURE_ENEMY]; }
	if (name == "Background") { return textures[TEXTURE_BG]; }
	if (name == "Ground") { return textures[TEXTURE_GROUND]; }
	if (name == "Flag") { return textures[TEXTURE_FLAG]; }
	if (name == "SpikeUp") { return textures[TEXTURE_SPIKE_UP]; }
	if (name == "SpikeRight") { return textures[TEXTURE_SPIKE_RIGHT]; }
	if (name == "SpikeDown") { return textures[TEXTURE_SPIKE_DOWN]; }
	if (name == "SpikeLeft") { return textures[TEXTURE_SPIKE_LEFT]; }
	return textures[TEXTURE_UNKNOWN];
}

std::vector<Texture*> TextureList::textures{};
