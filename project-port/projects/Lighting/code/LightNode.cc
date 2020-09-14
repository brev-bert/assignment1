#include "LightNode.h"

// Initialize map.
//std::map<ResourceLib::LightNode*, bool> ResourceLib::LightNode::activeLights = std::map<ResourceLib::LightNode*, bool>();
MathLib::Vec4 ResourceLib::LightNode::position = MathLib::Vec4(-1,1,3);
MathLib::Vec4 ResourceLib::LightNode::color = MathLib::Vec4(1,1,1);
float ResourceLib::LightNode::power = 2;
MathLib::Vec4 ResourceLib::LightNode::ambient = MathLib::Vec4(0.1,0.1,0.1);
MathLib::Vec4 ResourceLib::LightNode::specular = MathLib::Vec4(1,1,1);
int ResourceLib::LightNode::mode = 1;