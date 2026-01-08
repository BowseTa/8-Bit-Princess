// COMP710 GP Framework 2025
// 
// This include:
#include "texture.h"


#include <iostream>//


// Local includes:
#include "logmanager.h"
#include "imgui/imgui.h"

// Library include:
#include <SDL_image.h>
#include <cassert>
#include "glew.h"
#include <SDL_ttf.h>

Texture::Texture()
	: m_uiTextureId(0)
	, m_iHeight(0)
	, m_iWidth(0)
{

}

Texture::~Texture()
{
	glDeleteTextures(1, &m_uiTextureId);
}

bool Texture::Initialise(const char* pcFilename)
{
	SDL_Surface* pSurface = IMG_Load(pcFilename);

	if (pSurface)
	{
		m_iWidth = pSurface->w;
		m_iHeight = pSurface->h;


		std::cout << "Loaded texture " << pcFilename << " with size: "
			<< m_iWidth << "x" << m_iHeight << std::endl; // <-- ADD THIS HERE


		int bytesPerPixel = pSurface->format->BytesPerPixel;
		unsigned int format = 0;

		if (bytesPerPixel == 3)
		{
			format = GL_RGB;
		}
		else if (bytesPerPixel == 4)
		{
			format = GL_RGBA;
		}

		glGenTextures(1, &m_uiTextureId);
		glBindTexture(GL_TEXTURE_2D, m_uiTextureId);

		glTexImage2D(GL_TEXTURE_2D, 0, format, m_iWidth, m_iHeight, 0, format, GL_UNSIGNED_BYTE, pSurface->pixels);

		SDL_FreeSurface(pSurface);
		pSurface = 0;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else
	{
		printf("Failed to load texture: %s\n", pcFilename); // 👈 Add this line

		LogManager::GetInstance().Log("Texture failed to load!");
		assert(0);
		return false;
	}
	return true;
}

void Texture::SetActive()
{
	glBindTexture(GL_TEXTURE_2D, m_uiTextureId);
}

int Texture::GetWidth() const
{
	assert(m_iWidth);
	return (m_iWidth);
}

int Texture::GetHeight() const
{
		assert(m_iHeight);
		return (m_iHeight);
}

void Texture::LoadTextTexture(const char* text, const char* fontname, int pointsize)
{
	TTF_Font* pFont = 0;
	TTF_Init();

	// If pointer is not referencing a font yet, then open the font file
	if (pFont == 0)
	{
		pFont = TTF_OpenFont(fontname, pointsize);
	}

	SDL_Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 100;

	SDL_Surface* pSurface = TTF_RenderText_Blended(pFont, text, color);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, pSurface->pitch / pSurface->format->BytesPerPixel);
	LoadSurfaceIntoTexture(pSurface);

	TTF_CloseFont(pFont);
	pFont = 0;
}

void Texture::LoadSurfaceIntoTexture(SDL_Surface* pSurface)
{
	if (pSurface)
	{
		m_iWidth = pSurface->w;
		m_iHeight = pSurface->h;
		int bytesPerPixel = pSurface->format->BytesPerPixel;
		unsigned int format = 0;

		if (bytesPerPixel == 3)
		{
			format = GL_RGB;
		}
		else if (bytesPerPixel == 4)
		{
			format = GL_RGBA;
		}

		glGenTextures(1, &m_uiTextureId);
		glBindTexture(GL_TEXTURE_2D, m_uiTextureId);

		glTexImage2D(GL_TEXTURE_2D, 0, format, m_iWidth, m_iHeight, 0, format, GL_UNSIGNED_BYTE, pSurface->pixels);

		// Add mipmapping and linear filtering
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		SDL_FreeSurface(pSurface);
		pSurface = 0;
	}
}

void
Texture::DebugDraw()
{
	ImGui::PushID(this);
	ImGui::Text("%d by %d", m_iWidth, m_iHeight);
	ImVec2 size;
	size.x = static_cast<float>(m_iWidth);
	size.y = static_cast<float>(m_iHeight);
	ImGui::Image((void*)(intptr_t)m_uiTextureId, size);
	ImGui::PopID();
}