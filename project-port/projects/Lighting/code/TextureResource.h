#pragma once
#include "stb_image.h"

#include <string>

namespace ResourceLib {
	/** Resource representing texture. */
	class TextureResource {
	public:
		/** The loaded state of the image. */
		bool loaded = false;

		/** The horizontal size of the image. */
		int x = 0;
		/** The vertical size of the image. */
		int y = 0;
		/** The number of channels the image has. */
		int n = 0;

		/** The actual image data. */
		unsigned char* buffer = nullptr;

		/** The external index in a handle array. */
		size_t bufferIndex = 0;

		/** The texture name. */
		std::string filename;

		/** Default constructor. */
		TextureResource() {}

		/** Initiate texture resource. */
		TextureResource(const std::string& filename) {
			this->Load(filename);
			this->filename = filename;
		}

		/** Load texture resource. */
		unsigned char* Load() {
			if (this->filename.empty()) {
				printf("No filename for texture to load.\n");
				return nullptr;
			}

			return Load(this->filename);
		}

		/** Load texture resource. */
		unsigned char* Load(const std::string& filename) {

			// Set vertical flip for acceptable UV coordinates.
			stbi_set_flip_vertically_on_load(true);
			// Load data to array.
			buffer = stbi_load(filename.c_str(), &x, &y, &n, 0);
			// Check if data was loaded.
			if (buffer == nullptr) {
				//throw(std::string("Failed to load texture"));
				loaded = false;
				return nullptr;
			}

			this->filename = filename;
			loaded = true;

			// Return buffer for outside use.
			return buffer;
		}

		/** Frees a buffer from memory. */
		void Unload() {
			loaded = false;
			stbi_image_free(buffer);
		}
	};
}