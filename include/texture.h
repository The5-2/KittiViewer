#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <iostream>
#include "../contrib/stbimage/stb_image.h"

class Texture {
public:
	enum class WrapMode {
		Clamp,
		Repeat
	};

	enum class FilterMode {
		Nearest,
		Bilinear,
		Trilinear
	};

	enum class Format {
		R,
		RG,
		RGB,
		RGBA
	};

	enum class Channels {
		Bits_8,
		Bits_16,
		Bits_32
	};


	int w, h, channels;
	GLuint index, formatSize;
	GLenum internal_format, format;
	WrapMode m_wrapMode;
	FilterMode m_filterMode;
public:

	Texture() {
	}

	~Texture() {
		glDeleteTextures(1, &index);
	}

	Texture(int w, int h, GLenum internal_format, GLenum format, int type){

		this->w = w;
		this->h = h;
		this->format = format;
		this->internal_format = internal_format;

		switch (format) {
			case GL_RED: this->formatSize = 1; break;
			case GL_RG: this->formatSize = 2; break;
			case GL_RGB: this->formatSize = 3; break;
			case GL_RGBA: this->formatSize = 4; break;
			default: this->formatSize = -1; break;
		}

		glGenTextures(1, &index);
		glBindTexture(GL_TEXTURE_2D, index);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, internal_format, w, h, 0, format, type, 0);
	}

	Texture(int w, int h, GLenum internal_format, GLenum format, int type, unsigned char *data) {

		this->w = w;
		this->h = h;
		this->format = format;
		this->internal_format = internal_format;

		switch (format) {
		case GL_RGB: this->formatSize = 3; break;
		case GL_RGBA: this->formatSize = 4; break;
		default: this->formatSize = -1; break;
		}

		glGenTextures(1, &index);
		glBindTexture(GL_TEXTURE_2D, index);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, internal_format, w, h, 0, format, type, data);
	}

	Texture(std::string path, WrapMode wrapMode, FilterMode filterMode) {
		int width, height, nrComponents;

		stbi_set_flip_vertically_on_load(1);
		unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
		stbi_set_flip_vertically_on_load(0);

		std::cout << "Texture w: " << width << std::endl;
		std::cout << "Texture h: " << height << std::endl;
		std::cout << "Texture nrComponents: " << nrComponents << std::endl;

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glPixelStorei(GL_PACK_ALIGNMENT, 1);

		this->w = width;
		this->h = height;
		this->channels = nrComponents;

		glGenTextures(1, &index);
		glBindTexture(GL_TEXTURE_2D, index);

		m_wrapMode = wrapMode;
		m_filterMode = filterMode;
		setWrapMode(m_wrapMode);
		setFilterMode(m_filterMode);

		glTexImage2D(GL_TEXTURE_2D, 
		0, 
		setInternalFormat(getFormatFromNumChannels(nrComponents), Channels::Bits_8),
		width, 
		height, 
		0, 
		setFormat(getFormatFromNumChannels(nrComponents)),
		GL_UNSIGNED_BYTE,
		data);
		free(data);
	}

	void reloadFromImage(std::string path) {
		int width, height, nrComponents;

		stbi_set_flip_vertically_on_load(1);
		unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
		stbi_set_flip_vertically_on_load(0);


		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glPixelStorei(GL_PACK_ALIGNMENT, 1);

		bool createNew = false;

		if(this->w != width){
			std::cout << "Warning: Widht image (" << width << ") to buffer (" << this->w << ") changed." << std::endl;
			createNew = true;
		}
		
		if(this->h != height){
			std::cout << "Warning: Height image (" << height << ") to buffer (" << this->h << ") changed." << std::endl;
			createNew = true;
		}

		if(this->channels != nrComponents){
			std::cout << "Warning: Components image (" << nrComponents << ") to buffer (" << this->channels << ") changed." << std::endl;
			createNew = true;
		}

		if(!createNew){
			glBindTexture(GL_TEXTURE_2D, index);
			glTexSubImage2D(GL_TEXTURE_2D, 
			0,
			0, 
			0,
			width, 
			height, 
			setFormat(getFormatFromNumChannels(nrComponents)),
			GL_UNSIGNED_BYTE,
			data);
		}else{
			this->w = width;
			this->h = height;
			this->channels = nrComponents;
			glBindTexture(GL_TEXTURE_2D, index);

			glDeleteTextures(1, &index);
			index = 0;
			glGenTextures(1, &index);
			glBindTexture(GL_TEXTURE_2D, index);
			setWrapMode(m_wrapMode);
			setFilterMode(m_filterMode);

			glTexImage2D(GL_TEXTURE_2D, 
			0, 
			setInternalFormat(getFormatFromNumChannels(nrComponents), Channels::Bits_8),
			width, 
			height, 
			0, 
			setFormat(getFormatFromNumChannels(nrComponents)),
			GL_UNSIGNED_BYTE,
			data);
		}

		free(data);
	}

	void Bind() {
		glBindTexture(GL_TEXTURE_2D, index);
	}

	int Unbind() {
		glBindTexture(GL_TEXTURE_2D, 0);
		return index;
	}

	GLuint Index() const { 
		return index; 
	}
	
	unsigned char* transferToCPU() {
		glBindTexture(GL_TEXTURE_2D, index);
		
		GLint numBytes = 0;
		numBytes = this->w * this->h * formatSize;

		if (numBytes > 0) {
			unsigned char *pixels = (unsigned char*)malloc(numBytes); // allocate image data into RAM

			//glGetTexImage(GL_TEXTURE_2D, 0, internalFormat, GL_UNSIGNED_BYTE, pixels);
			glGetTexImage(GL_TEXTURE_2D, 0, format, GL_UNSIGNED_BYTE, pixels);

			glBindTexture(GL_TEXTURE_2D, 0);
			return pixels;
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		return nullptr;
	}

	void setWrapMode(WrapMode wrapMode) {
		switch (wrapMode) {
			case WrapMode::Repeat:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				break;
			case WrapMode::Clamp:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
				break;
		}
	}

	void setFilterMode(FilterMode filterMode) {
		switch (filterMode) {
			case FilterMode::Nearest: 		
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				break;
			case FilterMode::Bilinear: 				
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				break;
			case FilterMode::Trilinear: 
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				break;
			default:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				break;
		}

	}

	GLuint setFormat(Format format) {
		switch (format) {
		case Format::R:
			std::cout << "GL_RED" << std::endl;
			return GL_RED;
			break;
		case Format::RG:
			std::cout << "GL_RG" << std::endl;
			return GL_RG;
			break;
		case Format::RGB:
			std::cout << "GL_RGB" << std::endl;
			return GL_RGB;
			break;
		case Format::RGBA:
			std::cout << "GL_RGBA" << std::endl;
			return GL_RGBA;
			break;
		default:
			std::cout << "GL_RGBA (Default)" << std::endl;
			return GL_RGBA;
			break;
		}
	}
	
	Format getFormatFromNumChannels(int channels) {
		switch (channels) {
		case 1:
			std::cout << "Format::R" << std::endl;
			return Format::R;
			break;
		case 2:
			std::cout << "Format::RG" << std::endl;
			return Format::RG;
			break;
		case 3:
			std::cout << "Format::RGB" << std::endl;
			return Format::RGB;
			break;
		case 4:
			std::cout << "Format::RGBA" << std::endl;
			return Format::RGBA;
			break;
		default:
			std::cout << "Format::RGBA (Default)" << std::endl;
			return Format::RGBA;
			break;
		}
	}

	GLuint setInternalFormat(Format format, Channels channel) {
		switch (format) {
		case Format::R:
			switch (channel) {
				case  Channels::Bits_8: std::cout << "GL_R8" << std::endl; return GL_R8; break;
				case  Channels::Bits_16: std::cout << "GL_R16" << std::endl; return GL_R16; break;
				case  Channels::Bits_32: std::cout << "GL_R32F" << std::endl; return GL_R32F; break;
				default: std::cout << "GL_R32F" << std::endl; return GL_R32F; break;
			}
		case Format::RG:
			switch (channel) {
				case  Channels::Bits_8: std::cout << "GL_RG8" << std::endl; return GL_RG8; break;
				case  Channels::Bits_16: std::cout << "GL_RG16" << std::endl; return GL_RG16; break;
				case  Channels::Bits_32: std::cout << "GL_RG32F" << std::endl; return GL_RG32F; break;
				default: std::cout << "GL_RG32F" << std::endl; return GL_RG32F; break;
			}
		case Format::RGB:
			switch (channel) {
				case  Channels::Bits_8: std::cout << "GL_RGB8" << std::endl; return GL_RGB8; break;
				case  Channels::Bits_16: std::cout << "GL_RGB16" << std::endl; return GL_RGB16; break;
				case  Channels::Bits_32: std::cout << "GL_RGB32F" << std::endl; return GL_RGB32F; break;
				default: std::cout << "GL_RGB32F" << std::endl; return GL_RGB32F; break;
			}
		case Format::RGBA:
			switch (channel) {
				case  Channels::Bits_8: std::cout << "GL_RGBA8" << std::endl; return GL_RGBA8; break;
				case  Channels::Bits_16: std::cout << "GL_RGBA16" << std::endl; return GL_RGBA16; break;
				case  Channels::Bits_32: std::cout << "GL_RGBA32F" << std::endl; return GL_RGBA32F; break;
				default: std::cout << "GL_RGBA32F" << std::endl; return GL_RGBA32F; break;
			}
		default:
			std::cout << "GL_RGBA" << std::endl; return GL_RGBA; break;
		}
	}
};

#endif
