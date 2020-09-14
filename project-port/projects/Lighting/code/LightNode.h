#pragma once

#include "GraphicsNode.h"

namespace ResourceLib {

	class LightNode : public GraphicsNode {    
    public:
		static MathLib::Vec4 position;
		static MathLib::Vec4 color;
		static float power;
		static MathLib::Vec4 ambient;
		static MathLib::Vec4 specular;
		static int mode;

		LightNode(std::shared_ptr<MeshResource> mr, std::shared_ptr<TextureResource> tr, std::shared_ptr<ShaderResource> sr)
		: GraphicsNode(mr, tr, sr) {

		}
    };
}