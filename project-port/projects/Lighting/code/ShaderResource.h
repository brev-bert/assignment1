#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <tuple>

#include "MathLib.h"

namespace ResourceLib {

	/** Class representing a shader resource. */
	class ShaderResource {
	public:
		// Shader type.
		const std::vector<std::string> tokens = {
			"vertex",
			"fragment"
		};

		/** The Location type. */
		enum UniformType {
			/** Matrix4D type. */
			Mat4,

			/** Vector4D type. */
			Vec4,

			/** Vector3D type. */
			Vec3,

			/** Vector2D type. */
			Vec2,

			/** Float type. */
			Float,

			/** Int type. */
			Int,

			/** Sampler2D type. */
			Sam2,

			/** Light list type. */
			Lights,

			/** Not implemented type. */
			None
		};

		bool loaded = false;

		/** The type, uniform name, resource pointer, uniform handle, and . */
		std::vector<std::tuple<UniformType, std::string, std::shared_ptr<void>, int, std::string>> uniforms;

		// The data to be destroyed later on.
		std::string data;

		// The shader in question, used as glProgram key.
		std::string filename;

		// Index, Shader.
		std::vector<std::pair<size_t, std::string>> shaders;

		/** Loads shader string. */
		bool Load(const std::string& filename) {


			// Set name for shader key later.
			this->filename = filename;

			// Open shader file.
			std::ifstream vert(filename);
			// Make sure vertex file actually opened.
			if (vert) {
				// Read file into string buffer.
				data = std::string(
					std::istreambuf_iterator<char>(vert),
					std::istreambuf_iterator<char>()
				);
				// Close file when done.
				vert.close();
			}
			else {
				std::cout << "Could not open shader file '" << filename << "'\n";
				loaded = false;
				return false;
			}

			//////////////
			// Do stuff //
			//////////////

			// Shader array.
			shaders = std::vector<std::pair<size_t, std::string>>();

			// Push shaders to vector.
			size_t pos = 0;
			while ((pos = data.find("#type")) != std::string::npos) {

				// Erase beginning + #type
				data.erase(0, pos + 5);

				// Get position of end of the first line.
				pos = data.find("\n");

				// Look for vertex and fragment tokens.
				size_t index = std::string::npos;
				for (size_t i = 0; i < tokens.size(); i++) {
					if (data.substr(0, pos).find(tokens[i]) != std::string::npos) {
						index = i;
						break;
					}
				}

				// Erase rest of line.
				data.erase(0, pos + 1);

				// Find the beginning of next (or no) shader.
				pos = data.find("#type");

				// Push shader type and string to list.
				shaders.push_back(
					std::pair<size_t, std::string>(
						index,
						data.substr(0, pos)
						)
				);

				// Erase shader portion for next shader.
				data.erase(0, pos);
			}

			return loaded = true;
		}

		//std::tuple<UniformType, std::string, std::shared_ptr<void>, int, std::string> 

		/** Get Mat4 uniform handle so editing can be done. */
		std::shared_ptr<MathLib::Mat4> GetUniformMat4(std::string name) {
			auto iter = std::find_if(this->uniforms.begin(), this->uniforms.end(), [name](std::tuple<UniformType, std::string, std::shared_ptr<void>, int, std::string> a) { return std::get<1>(a) == name; });

			if (iter != this->uniforms.end())
				// Pointer fuckery.
				return *(std::shared_ptr<MathLib::Mat4>*)(&std::get<2>(*iter));
			else
				return nullptr;
		}

		/** Get Vec4 uniform handle so editing can be done. */
		std::shared_ptr<MathLib::Vec4> GetUniformVec4(std::string name) {
			auto iter = std::find_if(this->uniforms.begin(), this->uniforms.end(), [name](std::tuple<UniformType, std::string, std::shared_ptr<void>, int, std::string> a) { return std::get<1>(a) == name; });

			if (iter != this->uniforms.end())
				// Pointer fuckery.
				return *(std::shared_ptr<MathLib::Vec4>*)(&std::get<2>(*iter));
			else
				return nullptr;
		}

		/** Unloads shader string from cpu. */
		void Unload() {
			loaded = false;

			data.clear();
			data.shrink_to_fit();

			shaders.clear();
			shaders.shrink_to_fit();

			uniforms.clear();
			uniforms.shrink_to_fit();
		}

	private:
	};
}