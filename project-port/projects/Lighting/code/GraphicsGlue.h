#pragma once

#include "ShaderResource.h"
#include "MeshResource.h"
#include "TextureResource.h"
#include "GraphicsNode.h"
#include "LightNode.h"

//#include "config.h"
#include "exampleapp.h"

#include <map>

namespace GG {

	class ResourceHandler {
	public:
		/** OpenGL specific handle for unloading resources when needed. */
		struct GLHandle {
			/** The resource type on the GPU. */
			enum class Type {
				VBO,
				//VAO,
				IBO,
				TEX,
				SHADER
			};
			/** The type of this GL handle. */
			Type type;
			/** The GPU buffer/sampler on the GPU. */
			GLuint handle;
		};

		/** The vector of opengl handles. */
		static std::map<std::string, std::pair<GLenum, GLint>> handles;

		/** Uploads a shader to the GPU and Unloads it from the CPU. */
		static void UploadShaderResource(std::shared_ptr<ResourceLib::ShaderResource> const& sr, bool unloadOnload = true);

		/** Uploads a mesh to the GPU and Unloads it from the CPU. */
		static void UploadMeshResource(std::shared_ptr<ResourceLib::MeshResource> const& mr);

		/** Uploads a texture to the GPU and unloads it from the CPU */
		static void UploadTextureResource(std::shared_ptr<ResourceLib::TextureResource> const& tr);

		/** Sets the camera matrix to be used when drawing objects. */
		static void SetCameraMatrices(const MathLib::Mat4& view, const MathLib::Mat4& projection);

		/** Draws a textured mesh multiplied with a matrix. */
		static void DrawMeshTextureMatrix(ResourceLib::MeshResource* mr, ResourceLib::TextureResource* tr, ResourceLib::ShaderResource* sr, MathLib::Mat4* mat);

		/** Draws a graphical object using a shader. */
		static void DrawGraphicsNode(ResourceLib::GraphicsNode* gn);

		/** Returns the camera projection matrix. */
		static MathLib::Mat4 GetCameraProjection();

		/** Returns the camera view matrix. */
		static MathLib::Mat4 GetCameraView();

		/** Removes all GL handles from the internal list of handles, which should cause garbage collector to do its thing. */
		static void GPUClean();
	private:

		static MathLib::Mat4 cameraView;
		static MathLib::Mat4 cameraProjection;
	};

}