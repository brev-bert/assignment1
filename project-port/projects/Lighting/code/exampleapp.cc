//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"

#include "MeshResource.h"
#include "TextureResource.h"
#include "GraphicsNode.h"

#include "LightNode.h"

#include "GraphicsGlue.h"
using namespace ResourceLib;


#include "stb_image.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <cstring>
#include <iostream>
#include <unordered_map>

#include "InputHandler.h"

const GLchar* vs =
"#version 430\n"
"layout(location=0) in vec3 pos;\n"
"layout(location=1) in vec4 color;\n"
"layout(location=2) in vec2 uv;\n"
"\n"
// uniform location 0 global for program.
"layout(location=0) uniform mat4 killMe;\n"
"\n"
"layout(location=0) out vec4 Color;\n"
"layout(location=1) out vec2 UV;\n"
"void main()\n"
"{\n"
"	gl_Position = killMe * vec4(pos, 1);\n"
"	Color = color;\n"
"	UV = uv;\n"
"}\n";

const GLchar* ps =
"#version 430\n"
"layout(location=0) in vec4 color;\n"
"layout(location=1) in vec2 uv;\n"
"\n"
// uniform location 1 global for program
"layout(location=1) uniform sampler2D wakeMeUpInside;\n"
"\n"
"out vec4 CantWakeUp;\n"
"void main()\n"
"{\n"
"	CantWakeUp = /*color */ texture(wakeMeUpInside, uv, 0);\n"
"}\n";


using namespace Display;
namespace Example
{

//------------------------------------------------------------------------------
/**
*/
ExampleApp::ExampleApp()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
ExampleApp::~ExampleApp()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
bool
ExampleApp::Open()
{
	App::Open();
	this->window = new Display::Window;

	/*window->SetKeyPressFunction([this](int32, int32, int32, int32){
		this->window->Close();
	});*/

	window->SetKeyPressFunction([this](int32 key, int32, int32 action, int32 mod){
		if (key == GLFW_KEY_W) { Input::Keys.W = action; }
		if (key == GLFW_KEY_A) { Input::Keys.A = action; }
		if (key == GLFW_KEY_S) { Input::Keys.S = action; }
		if (key == GLFW_KEY_D) { Input::Keys.D = action; }
		if (key == GLFW_KEY_ESCAPE) { this->window->Close(); }
	});

	window->SetMousePressFunction([this](int32 key, int32 action, int32 mod){
		switch (key) {
		case GLFW_MOUSE_BUTTON_LEFT: { Input::Keys.MouseLeft = action; }
		case GLFW_MOUSE_BUTTON_RIGHT: { Input::Keys.MouseRight = action; }

		default: break;
		}
	});

	window->SetMouseMoveFunction([this](float64 xpos, float64 ypos){
		Input::Mouse.oldx = Input::Mouse.xpos;
		Input::Mouse.oldy = Input::Mouse.ypos;
		Input::Mouse.xpos = xpos;
		Input::Mouse.ypos = ypos;
	});

	if (this->window->Open())
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		// set clear color to gray
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);



		// setup vertex shader
		this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
		GLint length = static_cast<GLint>(std::strlen(vs));
		glShaderSource(this->vertexShader, 1, &vs, &length);
		glCompileShader(this->vertexShader);

		// get error log
		GLint shaderLogSize;
		glGetShaderiv(this->vertexShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetShaderInfoLog(this->vertexShader, shaderLogSize, NULL, buf);
			printf("[VERTEX SHADER COMPILE ERROR]: %s", buf);
			delete[] buf;
		}

		// setup pixel shader
		this->pixelShader = glCreateShader(GL_FRAGMENT_SHADER);
		length = static_cast<GLint>(std::strlen(ps));
		glShaderSource(this->pixelShader, 1, &ps, &length);
		glCompileShader(this->pixelShader);

		// get error log
		shaderLogSize;
		glGetShaderiv(this->pixelShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetShaderInfoLog(this->pixelShader, shaderLogSize, NULL, buf);
			printf("[FRAGMENT SHADER COMPILE ERROR]: %s", buf);
			delete[] buf;
		}

		// create a program object
		this->program = glCreateProgram();
		glAttachShader(this->program, this->vertexShader);
		glAttachShader(this->program, this->pixelShader);
		glLinkProgram(this->program);
		glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetProgramInfoLog(this->program, shaderLogSize, NULL, buf);
			printf("[PROGRAM LINK ERROR]: %s", buf);
			delete[] buf;
		}
		else {
			// DELET
			this->uniform = glGetUniformLocation(this->program, "Uniform");
		}

		return true;
	}
	return false;
}



//------------------------------------------------------------------------------
/**
*/
void
ExampleApp::Run()
{
	// Float representing elapsed time.
	float t = 0;
	float* tp = &t; // time pointer for lambda capture.

	///////////////////////////
	// SET UP SHADER PROGRAM //
	///////////////////////////
	std::shared_ptr<ShaderResource> sr(new ShaderResource());
	sr->Load("./resources/blinnphong.glsl");
	GG::ResourceHandler::UploadShaderResource(sr);

	/*std::shared_ptr<ShaderResource> ls(new ShaderResource());
	sr->Load("./resources/shadeless.glsl");
	GG::ResourceHandler::UploadShaderResource(sr);*/


	//////////////////////////
	// SET UP MESH RESOURCE //
	//////////////////////////
	// Doesn't look hacky at all, does it?
	std::shared_ptr<MeshResource> qu(new MeshResource(MeshResource::GenerateCube()));
	std::shared_ptr<MeshResource> mr(new MeshResource("./resources/hare.obj"));
	GG::ResourceHandler::UploadMeshResource(mr);

	/////////////////////////////
	// SET UP TEXTURE RESOURCE //
	/////////////////////////////
	//std::shared_ptr<ResourceLib::TextureResource> tr(new TextureResource("./resources/textureMaster.png"));
	std::shared_ptr<ResourceLib::TextureResource> tr(new TextureResource("./resources/hare.png"));
	GG::ResourceHandler::UploadTextureResource(tr);

	///////////////////////////
	// SET UP GRAPHICS NODES //
	///////////////////////////
	std::shared_ptr<ResourceLib::GraphicsNode> gn(new GraphicsNode(mr, tr, sr));

	gn->Update = [gn](){
		if (Input::Keys.MouseLeft) {
			if ((Input::Mouse.dx *Input::Mouse.dx + Input::Mouse.dy*Input::Mouse.dy) >= 2.0) {
				gn->transform.rotation[2] += Input::Mouse.dx / 300.f;
				gn->transform.rotation[0] += Input::Mouse.dy / 300.f;
			}
		}

		if (Input::Keys.W) { gn->transform.location[2] -= 0.1f; }
		if (Input::Keys.A) { gn->transform.location[0] -= 0.1f; }
		if (Input::Keys.S) { gn->transform.location[2] += 0.1f; }
		if (Input::Keys.D) { gn->transform.location[0] += 0.1f; }
	};

	std::shared_ptr<ResourceLib::LightNode> ln(new LightNode(mr, tr, sr));
	//ln->transform.scale = MathLib::Vec4(0.1,0.1,0.1);
	ln->Update = [ln, tp]() {
		// Update light position.
		LightNode::position = MathLib::Vec4(sin(*tp)*4, 1, cos(*tp)*4);
		// Also update mesh.
		ln->transform.location = LightNode::position;
	};

	////////////////
	// MISC STUFF //
	////////////////

	// Create uniform matrix to be used.
	MathLib::Mat4 model = MathLib::Mat4::Identity;



	/////////////////
	// UPDATE LOOP //
	/////////////////

	// Loop while window is running .
	// TODO: Add option for different render paths in the future.
	while (this->window != nullptr && this->window->IsOpen())
	{
		Input::Mouse.dx = Input::Mouse.xpos - Input::Mouse.oldx;
		Input::Mouse.dy = Input::Mouse.ypos - Input::Mouse.oldy;


		// do stuff

		t = (float)glfwGetTime();
		//t = 0;

		// No idea what this does lol.
		this->window->Update();

		// Retrieve screen dimensions (do this before )
		int w, h;
		this->window->GetSize(w, h);


		////////////////////
		// OBJECT UPDATES //
		////////////////////

		// Sets the scale for the object
		gn->transform.scale = MathLib::Vec4(1.5f, 1.5f, 1.5f);


		////////////////////
		// CAMERA UPDATES //
		////////////////////

		// View lookat matrix.
		MathLib::Mat4 view = MathLib::Mat4::LookAt(
			MathLib::Vec4(0, 0, 4),
			//MathLib::Vec4(sin(t * 0.5f) * 4, 3, cos(t * 0.5f) * 4),
			MathLib::Vec4(0, 0, 0),
			MathLib::Vec4(0, 1, 0)
		);

		// Perspective projection matrix.
		MathLib::Mat4 projection = MathLib::Mat4::Perspective(
			0.1f,
			100.0f,
			//(90.0f+80.0f*sin(t)) / 180.0f * (float)M_PI,
			90.0f / 180.0f * (float)M_PI,
			(float)w / (float)h
		);
		
		GG::ResourceHandler::SetCameraMatrices(view, projection);

		////////////////////
		// RENDER SECTION //
		////////////////////

		// Clear window buffer before drawing.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Set viewport.
		glViewport(0, 0, w, h);


		///////////////////////////
		// UPDATE GRAPHICS NODES //
		///////////////////////////
		for (auto iter = ResourceLib::GraphicsNode::activeGraphicsNodes.begin();
			iter != ResourceLib::GraphicsNode::activeGraphicsNodes.end();
			iter++) {

			if (iter->second)
				iter->first->Update();
		}

		/////////////////////////
		// DRAW GRAPHICS NODES //
		/////////////////////////
		for (auto iter = ResourceLib::GraphicsNode::activeGraphicsNodes.begin();
			iter != ResourceLib::GraphicsNode::activeGraphicsNodes.end();
			iter++) {

			if (iter->second)
				GG::ResourceHandler::DrawGraphicsNode(iter->first);
		}

		
		////////////////////////////////////////////////////////
		// REMOVE STALE GRAPHICS NODES FROM UPDATE AND RENDER //
		////////////////////////////////////////////////////////
		for (auto iter = ResourceLib::GraphicsNode::activeGraphicsNodes.begin();
			iter != ResourceLib::GraphicsNode::activeGraphicsNodes.end();
			iter++) {

			if (!iter->second)
				ResourceLib::GraphicsNode::activeGraphicsNodes.erase(iter->first);
		}

		// Legacy render function
		//GG::ResourceHandler::DrawMeshTextureMatrix(&*mr, &*tr, &*sr, &(VP*model));


		// Swap rendered buffer to screen.
		this->window->SwapBuffers();
	}
	// Clean up the project before closure.
	GG::ResourceHandler::GPUClean();
}

} // namespace Example