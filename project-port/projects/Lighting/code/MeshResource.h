#pragma once

#include "MathLib.h"

//#include "ResourceBase.h"

// Planning on using this some day, but it needs extending.
//#include "half.h"
//using namespace half_float;

// memcpy() for union copy instruction.
#include <cstring>

#include <iostream>
#include <typeinfo>
#include <string>
#include <fstream>
#include <vector>
#include <map>


//#include "matlib.h"

namespace ResourceLib {

	/** Class representing a single attribute. */
	class Attribute {
	public:
		/** The amount of elements to read before going to next vertex. */
		size_t length;

		/** The offset from stride to first element. */
		size_t offset;

		/** The stride of the data block. */
		size_t stride;

		/** The attribute field name. */
		std::string name;

		/** Copy constructor. */
		Attribute(const Attribute& other) {
			this->length = other.length;
			this->offset = other.offset;
			this->stride = other.stride;
			this->name = other.name;
		}

		/**
		 * Create an attribute.
		 *
		 * @data is the pointer to a block of data
		 * @type is the way element type in the data block.
		 * @size is the total element size of the data block.
		 * @length is the element length of this attribute.
		 * @stride is the distance in elements to the next vertex.
		 * @name is the name of this attribute.
		 */
		Attribute(size_t length = 0, size_t offset = 0, size_t stride = 0, std::string name = "") {
			this->length = length;
			this->offset = offset;
			this->stride = stride;
			this->name = name;
		}

		~Attribute() {
			std::cout << this->name << "\n";
		}
	};

	// -------------------------------------------------- //

	// TODO: make this part of a "VAO" in the future.
	/** Class representing a uniform. */
	class Uniform {
	public:

		Uniform() {}

		/** The type representing this  */
		enum class Type {
			Matrix,
			Float,
			Sampler2D,
			None
		};

		/** The type of this uniform. */
		Type type = Type::Matrix;

		/** The name of this uniform. */
		std::string name;
	};

	// -------------------------------------------------- //

	/** Class representing a mesh resource. */
	class MeshResource {
	public:
		/** The loaded state of this resource. */
		bool loaded = false;

		/** The raw mesh data of this mesh resource. */
		std::vector<float> data = std::vector<float>();
		/** the handle index for external use. */
		//size_t dataIndex = 0;

		/** The mesh draw mode of the mesh resource. */
		enum class Mode {
			/** Mode representing indexed drawing. */
			Indexed,
			/** Mode representing instanced draw. */
			//Instanced,
			/** Mode representing raw vertex  */
			Raw
		};

		/** The draw mode of the mesh resource. */
		Mode mode = Mode::Indexed;

		/** The filename that was loaded. */
		std::string filename;

		/** The mesh handle. */
		std::string meshHandle;

		/** The index list of the mesh resource. */
		std::vector<unsigned int> indices;
		/** The about of index elements, needed after Unload(). */
		size_t indicesCount = 0;
		/** The handle index for external use. */
		//size_t indicesIndex = 0;

		/** The vertex attributes of the mesh resource. */
		std::vector<Attribute> attributes;

		/** Default mesh resource. */
		MeshResource() {}

		/**  */
		MeshResource(const std::string& filename) {
			//this->filename = filename;
			this->Load(filename);
		}

		/** Generates a cube with at size with pos, col, uv and indices. */
		static MeshResource GenerateQuad(float size = 1) {
			// Create mesh resource.
			MeshResource mr;

			// Set up interleaved vertex data.
			mr.data = {
				-size, size, 0.0f,
				1.0f, 0.0f, 0.0f, 1.0f,
				0.0f, 1.0f,

				-size, -size, 0.0f,
				0.0f, 1.0f, 0.0f, 1.0f,
				0.0f, 0.0f,

				size, size, 0.0f,
				0.0f, 0.0f, 1.0f, 1.0f,
				1.0f, 1.0f,

				size, -size, 0.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				1.0f, 0.0f,
			};
			// Positions.
			mr.attributes.push_back(Attribute(3, 0, 9, "pos"));
			// Colors
			mr.attributes.push_back(Attribute(4, 3, 9, "color"));
			// UV coordinates.
			mr.attributes.push_back(Attribute(2, 7, 9, "uv"));

			// Set up indices.
			mr.indices = {
				// Triangle 0
				0, 1, 2,
				// Triangle 1
				2, 1, 3,
			};

			mr.indicesCount = mr.indices.size();

			mr.loaded = true;

			return mr;
		}

		/** Generates a cube with at size with pos, col, uv and indices. */
		static MeshResource GenerateCube(float size = 1) {
			// Create mesh resource.
			MeshResource mr;

			mr.data = {
				-size, size, size,	// 0 (8,10)
				1, 1, 1, 1,
				0, 0.5f,

				-size, -size, size,	// 1 (9,12)
				1, 1, 1, 1,
				0, 0.25f,

				size, size, size,	// 2
				1, 1, 1, 1,
				0.25f, 0.5f,

				size, -size, size,	// 3
				1, 1, 1, 1,
				0.25f, 0.25f,

				size, size, -size,	// 4
				1, 1, 1, 1,
				0.5f, 0.5f,

				size, -size, -size,	// 5
				1, 1, 1, 1,
				0.5f, 0.25f,

				-size, size, -size,	// 6 (11)
				1, 1, 1, 1,
				0.75f, 0.5f,

				-size, -size, -size,// 7 (13)
				1, 1, 1, 1,
				0.75f, 0.25f,

				-size, size, size,	// 8 (0,10)
				1, 1, 1, 1,
				1, 0.5f,

				-size, -size, size,	// 9 (1,12)
				1, 1, 1, 1,
				1, 0.25f,

				-size, size, size,	// 10 (0,8)
				1, 1, 1, 1,
				0.25f, 0.75f,

				-size, size, -size,	// 11 (6)
				1, 1, 1, 1,
				0.5f, 0.75f,

				-size, -size, size,	// 12 (1,9)
				1, 1, 1, 1,
				0.25f, 0,

				-size, -size, -size,// 13 (7)
				1, 1, 1, 1,
				0.5f, 0,
			};
			// Positions.
			mr.attributes.push_back(Attribute(3, 0, 9, "pos"));
			// Colors
			mr.attributes.push_back(Attribute(4, 3, 9, "color"));
			// UV coordinates.
			mr.attributes.push_back(Attribute(2, 7, 9, "uv"));

			// Set up indices.
			mr.indices = {
				// Triangle 0
				0, 1, 2,
				// Triangle 1
				2, 1, 3,
				// Rest of them.
				2, 3, 4,
				4, 3, 5,
				4, 5, 6,
				6, 5, 7,
				6, 7, 8,
				8, 7, 9,
				10, 2, 11,
				11, 2, 4,
				3, 12, 5,
				5, 12, 13,
			};

			mr.indicesCount = mr.indices.size();

			mr.loaded = true;

			mr.filename = "GeneratedCube";

			return mr;
		}


		/** Load data to mesh resource using filename. */
		bool Load() {
			return Load(this->filename);
		}

		/** Loads data to mesh resource from filename. */
		bool Load(std::string filename) {

			// Set name for shader key later.
			this->filename = filename;

			std::string data;

			std::vector<float> pos;
			std::vector<float> uv;
			std::vector<float> norm;

			std::ifstream mesh(filename);
			// Make sure vertex file actually opened.
			if (mesh) {
				// Read file into string buffer.
				//data = std::string(
				//	std::istreambuf_iterator<char>(mesh),
				//	std::istreambuf_iterator<char>()
				//);

				int maxIndex = 0;
				// clear data and indices
				this->data.clear();
				this->indices.clear();

				// Map that helps reduce duplicate vertices.
				std::map<std::string, int> vertIndexList;

				// For each line.
				for (std::string line; std::getline(mesh, line); ) {
					// Token list.
					std::vector<std::string> tokens;

					// String rep of line.
					char* str = const_cast<char*>(line.c_str());

					// Tokenize line.
					char* tok = strtok(str, " ");
					while (tok != NULL) {
						tokens.push_back(tok);
						tok = strtok(NULL, " ");
					}

					// do stuff depending on type

					if (tokens.size() == 0)
						continue;

					if (tokens.size() > 0 && tokens[0] == "v") {
						// Push 3 position floats to array.
						for (int i = 1; i < 4; i++)
							pos.push_back(std::stof(tokens[i]));
					}
					else if (tokens[0] == "vt") {
						// Push 2 UV floats to array.
						for (int i = 1; i < 3; i++)
							uv.push_back(std::stof(tokens[i]));
					}
					else if (tokens[0] == "vn") {
						// Push 3 normal floats to array.
						for (int i = 1; i < 4; i++)
							norm.push_back(std::stof(tokens[i]));
					}
					else if (tokens[0] == "f") {
						// Assemble first triangle.
						// pos  = i*3
						// uv   = i*2
						// norm = i*3

						int indices[4];

						// Parse first three vertices.
						for (int i = 1; i < tokens.size(); i++) {
							
							// Current vertex.
							char* cver = const_cast<char*>(tokens[i].c_str());

							// Set up iterator.
							std::map<std::string, int>::iterator existing;
							// If vertex already exists.
							if ((existing = vertIndexList.find(cver)) != vertIndexList.end()) {
								indices[i-1] = existing->second;	// Set index to existing.
								continue; 							// Goto next vertex.
							}
							// Use string as key for new vertex.
							vertIndexList[cver] = maxIndex;


							// Sub vertex indices.
							char* svi;

							// parse vertex index
							svi = strtok(cver, "/");
							int ix = (std::stoi(svi) - 1) * 3;

							this->data.push_back(pos[ix]);
							this->data.push_back(pos[ix+1]);
							this->data.push_back(pos[ix+2]);

							// parse uv index
							svi = strtok(NULL, "/");
							ix = (std::stoi(svi) - 1) * 2;
							this->data.push_back(uv[ix]);
							this->data.push_back(uv[ix + 1]);

							// parse normal index
							svi = strtok(NULL, "/");
							ix = (std::stoi(svi) - 1) * 3;
							this->data.push_back(norm[ix]);
							this->data.push_back(norm[ix + 1]);
							this->data.push_back(norm[ix + 2]);


							// Add new vertex if it doesn't exist.
							indices[i-1] = maxIndex++;
						}

						if (tokens.size() == 4) {
							// Push vertices to ibo.
							this->indices.push_back(indices[0]);
							this->indices.push_back(indices[1]);
							this->indices.push_back(indices[2]);
						}
						else if (tokens.size() == 5) {
							// Push three first vertices to ibo.
							this->indices.push_back(indices[0]);
							this->indices.push_back(indices[1]);
							this->indices.push_back(indices[3]);
							// Push three last vertices to ibo
							this->indices.push_back(indices[1]);
							this->indices.push_back(indices[2]);
							this->indices.push_back(indices[3]);
						}
						else {
							printf("boi shit's fucked yo\n");
						}
					}
				}

				// Set index count for whatever reason
				this->indicesCount = this->indices.size();

				vertIndexList.clear(); // Keep memory leaks fewer.

				this->attributes.clear();
				// Positions.
				this->attributes.push_back(Attribute(3, 0, 8, "pos"));
				// UV coordinates.
				this->attributes.push_back(Attribute(2, 3, 8, "uv"));
				// Normals.
				this->attributes.push_back(Attribute(3, 5, 8, "normal"));

				// Close file when done.
				mesh.close();
			}
			else {
				std::cout << "Could not open obj file '" << filename << "'\n";
				return this->loaded = false;
			}

			return this->loaded = true;
		}

		/** Unloads the local data from this mesh resource. */
		void Unload() {
			// Clear and shrink data.
			this->data.clear();
			this->data.shrink_to_fit();
			// Clear and shrink to ensure no data leak.
			this->indices.clear();
			this->indices.shrink_to_fit();
			this->loaded = false;
		}
	};
}

/*

* * * * * * * *
* * * * * * * *
* * * * * * * *
* * * * * * * *
* * * * * * * *
* * * * * * * *
* * * * * * * *
* * * * * * * *

*/