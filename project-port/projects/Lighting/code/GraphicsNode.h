#pragma once

#include <functional>
#include <map>
#include <memory>

#include "MeshResource.h"
#include "TextureResource.h"
#include "ShaderResource.h"

#include "MathLib.h"

namespace ResourceLib {

	class Transform {
	public:
		MathLib::Vec4 location;
		MathLib::Vec4 rotation;
		MathLib::Vec4 scale = MathLib::Vec4(1.0f, 1.0f, 1.0f);

		MathLib::Mat4 GetTransform() {
			return
				MathLib::Mat4::Translate(location[0], location[1], location[2]) *
				MathLib::Mat4::Scale(scale[0], scale[1], scale[2]) *
				MathLib::Mat4::RotateEulerX(rotation[0]) *
				MathLib::Mat4::RotateEulerY(rotation[1]) *
				MathLib::Mat4::RotateEulerZ(rotation[2]);
		}
	};

	class GraphicsNode {
	protected:
		std::shared_ptr<MeshResource> mr;
		std::shared_ptr<TextureResource> tr;
		std::shared_ptr<ShaderResource> sr;

	public:
		// Map of active graphics nodes.
		static std::map<GraphicsNode*, bool> activeGraphicsNodes;

		Transform transform;
		
		GraphicsNode(std::shared_ptr<MeshResource> mr, std::shared_ptr<TextureResource> tr, std::shared_ptr<ShaderResource> sr) {
			this->mr = mr;
			this->tr = tr;
			this->sr = sr;

			// Activate object when adding to active nodes.
			activeGraphicsNodes[this] = true;
		}

		// "Delete" object, ie. disable object until it dies.
		void Delete() {
			activeGraphicsNodes[this] = false;
		}

		// Remove object from draw list when shared pointer reference dies.
		~GraphicsNode() {
			activeGraphicsNodes.erase(this);
		}

		// Getters

		std::shared_ptr<MeshResource> GetMeshResource() {
			return mr;
		}
		std::shared_ptr<TextureResource> GetTextureResource() {
			return tr;
		}
		std::shared_ptr<ShaderResource> GetShaderResource() {
			return sr;
		}

		// Setters

		GraphicsNode SetMeshResource(std::shared_ptr<MeshResource> mr) {
			this->mr = mr;
			return *this;
		}
		GraphicsNode SetTextureResource(std::shared_ptr<TextureResource> tr) {
			this->tr = tr;
			return *this;
		}
		GraphicsNode SetShaderResource(std::shared_ptr<ShaderResource> sr) {
			this->sr = sr;
			return *this;
		}


		// The update function of this object.
		std::function<void(void)> Update;
	};
}
