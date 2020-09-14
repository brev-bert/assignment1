#include "GraphicsGlue.h"
//using namespace ResourceLib;
#include <unordered_map>


//GG::ResourceHandler::handles = std::unordered_map<std::string, std::pair<GLenum, GLint>>();

//GG::ResourceHandler::shaders

void GG::ResourceHandler::UploadShaderResource(std::shared_ptr<ResourceLib::ShaderResource> const& sr, bool unloadOnLoad) {
	// Check if shader is loaded elsewhere.
	if (!sr->loaded) {
		std::cout << "Shader not loaded on cpu.\n";

		return;
	}

	// Generate shader handles for shaders.
	std::vector<GLuint> shaderHandles = std::vector<GLuint>(sr->shaders.size());

	// Sanity flag.
	bool error = false;

	// Compile shaders.
	for (size_t i = 0; i < sr->shaders.size(); i++) {
		GLint length = sr->shaders[i].second.size();

		GLchar const* shad = sr->shaders[i].second.c_str();

		GLenum shaderType;

		switch (sr->shaders[i].first) {
		case 0:
			shaderType = GL_VERTEX_SHADER;
			break;

		case 1:
			shaderType = GL_FRAGMENT_SHADER;
			break;

		default:
			std::cout << sr->tokens[i] << " shader compilation not supported yet.\n";
			break;
		}

		shaderHandles[i] = glCreateShader(shaderType);
		glShaderSource(
			shaderHandles[i],
			1,
			&shad,
			&length
		);
		glCompileShader(shaderHandles[i]);

		// get error log
		GLint shaderLogSize;
		glGetShaderiv(shaderHandles[i], GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetShaderInfoLog(shaderHandles[i], shaderLogSize, NULL, buf);
			printf("[%s shader compile error]: %s", sr->tokens[sr->shaders[i].first].c_str(), buf);
			delete[] buf;

			error = true;
		}
	}

	// Link and upload finished shader.
	if (!error) {
		// create a program object
		GLuint program = glCreateProgram();

		for (size_t i = 0; i < shaderHandles.size(); i++) {
			glAttachShader(program, shaderHandles[i]);
		}
		glLinkProgram(program);

		GLint shaderLogSize;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetProgramInfoLog(program, shaderLogSize, NULL, buf);
			printf("[program link error]: %s", buf);
			delete[] buf;

			error = true;
		}

		///////////////////
		// Find uniforms //
		///////////////////
		GLint count;

		GLint size; // size of the variable
		GLenum type; // type of the variable (float, vec3 or mat4, etc)

		const GLsizei bufSize = 32; // maximum uniform name length
		GLchar uniformName[bufSize]; // variable name in GLSL
		GLsizei length; // name length
		glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);
		printf("Active Uniforms: %d\n", count);

		for (GLuint i = 0; i < count; i++)
		{
			glGetActiveUniform(program, i, bufSize, &length, &size, &type, uniformName);
			GLint address = glGetUniformLocation(program, uniformName);
			printf("Uniform Name: %s Type: %d Handle: %d\n", uniformName, type, address);

			// Put in uniform map.
			std::string handleName = sr->filename+uniformName;
			handleName += std::to_string(program); // append location as well.
			handles[handleName] = std::pair<GLenum, GLint>(type, address);

			ResourceLib::ShaderResource::UniformType t;
			std::shared_ptr<void> u;

			switch (type) {
			case GL_INT:
				t = ResourceLib::ShaderResource::UniformType::Int;
				u = std::make_shared<int>();
				break;

			case GL_FLOAT:
				t = ResourceLib::ShaderResource::UniformType::Float;
				u = std::make_shared<float>();
				break;

			case GL_FLOAT_VEC2:
				t = ResourceLib::ShaderResource::UniformType::Vec2;
				u = std::make_shared<MathLib::Vec4>();
				break;

			case GL_FLOAT_VEC3:
				t = ResourceLib::ShaderResource::UniformType::Vec3;
				u = std::make_shared<MathLib::Vec4>();
				break;

			case GL_FLOAT_VEC4:
				t = ResourceLib::ShaderResource::UniformType::Vec4;
				u = std::make_shared<MathLib::Vec4>();
				break;

			case GL_FLOAT_MAT4:
				t = ResourceLib::ShaderResource::UniformType::Mat4;
				u = std::make_shared<MathLib::Mat4>();
				break;

			case GL_SAMPLER_2D:
				t = ResourceLib::ShaderResource::UniformType::Sam2;
				u = std::make_shared<ResourceLib::TextureResource>();
				break;

			default:
				t = ResourceLib::ShaderResource::UniformType::None;
				u = nullptr;
				printf("BAD UNIFORM TYPE");
				break;
			}

			// Push type and (parsed) uniform name to uniform list.
			sr->uniforms.push_back(
				std::make_tuple(t, handleName, u, address, "")
			);
		}
		// Add shader handle to list tbh.
		handles[sr->filename].first = GL_PROGRAM;
		handles[sr->filename].second = program;
	}

	// Shader array.
	shaderHandles.clear();
	shaderHandles.shrink_to_fit();
}

void GG::ResourceHandler::UploadMeshResource(std::shared_ptr<ResourceLib::MeshResource> const& mr) {
	if (mr->loaded) {
		GLuint glhVBO;
		glGenBuffers(1, &glhVBO);
		glBindBuffer(GL_ARRAY_BUFFER, glhVBO);
		glBufferData(
			GL_ARRAY_BUFFER,
			mr->data.size() * sizeof(float),
			&mr->data[0],
			GL_STATIC_DRAW
		);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		handles[mr->filename + "_VBO"] = std::pair<GLenum, GLint>(GL_ARRAY_BUFFER, glhVBO);

		GLuint glhIBO;
		glGenBuffers(1, &glhIBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glhIBO);
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			mr->indices.size() * sizeof(unsigned int),
			&mr->indices[0],
			GL_STATIC_DRAW
		);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		handles[mr->filename + "_IBO"] = std::pair<GLenum, GLint>(GL_ELEMENT_ARRAY_BUFFER, glhIBO);
		//mr->indicesIndex = handles.size() - 1;
	}
	else {
		throw("Mesh resource '"+mr->filename+"' aint loded yo\n");
	}
}

void GG::ResourceHandler::SetCameraMatrices(const MathLib::Mat4& view, const MathLib::Mat4& projection) {
	cameraView = view;
	cameraProjection = projection;
}

MathLib::Mat4 GG::ResourceHandler::cameraView = MathLib::Mat4::Identity;
MathLib::Mat4 GG::ResourceHandler::cameraProjection = MathLib::Mat4::Identity;

void GG::ResourceHandler::UploadTextureResource(std::shared_ptr<ResourceLib::TextureResource> const& tr) {
	if (tr->loaded) {
		// Create texture handle.
		GLuint glhTEX;
		// Generate texture handle.
		glGenTextures(1, &glhTEX);
		// Bind texture to do stuff.
		glBindTexture(GL_TEXTURE_2D, glhTEX);
		// Set scaling filtering for shrunk textures.
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		// Set scaling filtering for expanded textures.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Set texture clamping.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		// Load data to gpu.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, tr->x, tr->y, 0, GL_RGBA, GL_UNSIGNED_BYTE, tr->buffer);

		glGenerateMipmap(GL_TEXTURE_2D);

		// Reset binding of texture
		glBindTexture(GL_TEXTURE_2D, 0);

		handles[tr->filename + "_TEX"] = std::pair<GLenum, GLint>(GL_TEXTURE_2D, glhTEX);
		tr->bufferIndex = handles.size() - 1;
		
		tr->Unload();
	}
}


void GG::ResourceHandler::DrawMeshTextureMatrix(ResourceLib::MeshResource* mr, ResourceLib::TextureResource* tr, ResourceLib::ShaderResource* sr, MathLib::Mat4* mat) {


	/////////////////
	// BIND SHADER //
	/////////////////
	glUseProgram(handles[sr->filename].second);

	////////////////////////
	// BIND VERTEX BUFFER //
	////////////////////////

	// Bind the vertex buffer for this mesh.
	glBindBuffer(handles[mr->filename + "_VBO"].first, handles[mr->filename + "_VBO"].second);
	// Loop through each attribute and enable them.
	for (size_t i = 0; i < mr->attributes.size(); i++) {

		// Enable attribute i.
		glEnableVertexAttribArray(i);

		// "bind" attribute.
		glVertexAttribPointer(
			(GLuint)i,
			(GLint)mr->attributes[i].length,
			GL_FLOAT,
			GL_TRUE,
			(GLsizei)(mr->attributes[i].stride * sizeof(float)),
			(GLvoid*)(mr->attributes[i].offset * sizeof(float))
		);
	}


	///////////////////
	// BIND UNIFORMS //
	///////////////////

	/*unsigned int tex = 0;
	// Loop through uniforms.
	for (size_t i = 0; i < mr.uniforms.size(); i++) {
		// Switch the type of the uniform union.
		switch (mr.uniforms[i].type) {
		case Uniform::Type::Matrix:*/
		// Bind uniform to shader.
	glUniformMatrix4fv(
		0, // i when looping.
		1,
		GL_TRUE,
		//(GLfloat*)(&(*mat)[0][0])
		(GLfloat*)mat
		);
		/*break;
	case Uniform::Type::Float:
		// Bind uniform to shader.
		glUniform1f(i, mr.uniforms[i].f);
		break;
	case Uniform::Type::Sampler2D:
		glActiveTexture(GL_TEXTURE0 + tex);
		glBindTexture(GL_TEXTURE_2D, textureHandle);
		glUniform1i(1, tex);
		tex++;
		break;
	default:
		break;
	}
	}*/


	///////////////////
	// BIND TEXTURES //
	///////////////////

	// Activate and bind texture.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(handles[tr->filename + "_TEX"].first, handles[tr->filename + "_TEX"].second);
	// Give texture to uniform location 1 in shader.
	glUniform1i(1, 0);



	/////////////////////////////
	// DRAW USING INDEX BUFFER //
	/////////////////////////////

	// Additionally bind the index buffer.
	glBindBuffer(handles[mr->filename + "_IBO"].first, handles[mr->filename + "_IBO"].second);
	glDrawElements(GL_TRIANGLES, mr->indicesCount, GL_UNSIGNED_INT, (void*)0);


	/////////////////////////////
	// UNBIND BUFFERS/TEXTURES //
	/////////////////////////////

	// Unbind index buffer.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	// Unbind texture.
	glBindTexture(GL_TEXTURE_2D, 0);
	// Unbind vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/**  */
void GG::ResourceHandler::DrawGraphicsNode(ResourceLib::GraphicsNode* gn) {


	/////////////////
	// BIND SHADER //
	/////////////////
	if (handles[gn->GetShaderResource()->filename].first != GL_PROGRAM) {
		printf("Shader not valid.\n");
		return;
	}
	GLint program = handles[gn->GetShaderResource()->filename].second;
	glUseProgram(program);


	///////////////////////////////////////
	// CALCULATE AND SET SHADER UNIFORMS //
	///////////////////////////////////////
	auto uniSearch = [gn](std::string uniform) {
		uniform = gn->GetShaderResource()->filename + uniform + std::to_string(handles[gn->GetShaderResource()->filename].second);
		return std::find_if(
			gn->GetShaderResource()->uniforms.begin(),
			gn->GetShaderResource()->uniforms.end(),
			[uniform](
				const std::tuple<ResourceLib::ShaderResource::UniformType,
				std::string, std::shared_ptr<void>,
				int,
				std::string>& a) { /*printf((*(std::string*)&std::get<1>(a)).c_str()); printf("\n");*/ return std::get<1>(a) == uniform; });
	};

	// Find pointer to uniform with the name projection.
	auto iter = uniSearch("projection");
	// Set the uniform.
	if (iter != gn->GetShaderResource()->uniforms.end()) {
		**(std::shared_ptr<MathLib::Mat4>*)&std::get<2>(*iter) =
			GG::ResourceHandler::GetCameraProjection();
	}

	// Find pointer to uniform with the name projection.
	iter = uniSearch("modelView");
	// Set the uniform.		
	if (iter != gn->GetShaderResource()->uniforms.end()) {
		**(std::shared_ptr<MathLib::Mat4>*)&std::get<2>(*iter) =
			GG::ResourceHandler::GetCameraView() * gn->transform.GetTransform();
	}

	iter = uniSearch("normalMat");
	if (iter != gn->GetShaderResource()->uniforms.end()) {

		// Try and invert model matrix.
		MathLib::Mat4 inverted = MathLib::Mat4::Identity;

		if (!MathLib::Mat4::Inverse(gn->transform.GetTransform(), &inverted))
			printf("dicks");

		// Set uniform to the transposed inverted matrix.
		**(std::shared_ptr<MathLib::Mat4>*)&std::get<2>(*iter) =
			MathLib::Mat4::Transpose(inverted);
	}

	iter = uniSearch("light.Pos");
	if (iter != gn->GetShaderResource()->uniforms.end()) {
		**(std::shared_ptr<MathLib::Vec4>*)&std::get<2>(*iter) =
			ResourceLib::LightNode::position;
	}
	iter = uniSearch("light.Color");
	if (iter != gn->GetShaderResource()->uniforms.end()) {
		**(std::shared_ptr<MathLib::Vec4>*)&std::get<2>(*iter) =
			ResourceLib::LightNode::color;
	}
	iter = uniSearch("light.Power");
	if (iter != gn->GetShaderResource()->uniforms.end()) {
		**(std::shared_ptr<float>*)&std::get<2>(*iter) =
			ResourceLib::LightNode::power;
	}
	iter = uniSearch("light.Ambi");
	if (iter != gn->GetShaderResource()->uniforms.end()) {
		**(std::shared_ptr<MathLib::Vec4>*)&std::get<2>(*iter) =
			ResourceLib::LightNode::ambient;
	}
	iter = uniSearch("light.Spec");
	if (iter != gn->GetShaderResource()->uniforms.end()) {
		**(std::shared_ptr<MathLib::Vec4>*)&std::get<2>(*iter) =
			ResourceLib::LightNode::specular;
	}
	iter = uniSearch("mode");
	if (iter != gn->GetShaderResource()->uniforms.end()) {
		**(std::shared_ptr<int>*)&std::get<2>(*iter) =
			ResourceLib::LightNode::mode;
	}


	////////////////////////
	// BIND VERTEX BUFFER //
	////////////////////////
	// Bind the vertex buffer for this mesh.
	glBindBuffer(handles[gn->GetMeshResource()->filename + "_VBO"].first, handles[gn->GetMeshResource()->filename + "_VBO"].second);
	// Loop through each attribute and enable them.
	for (size_t i = 0; i < gn->GetMeshResource()->attributes.size(); i++) {

		GLint location = glGetAttribLocation(program, gn->GetMeshResource()->attributes[i].name.c_str());

		//printf("")

		// Enable attribute i.
		glEnableVertexAttribArray(location);

		// "bind" attribute.
		glVertexAttribPointer(
			(GLuint)location,
			(GLint)gn->GetMeshResource()->attributes[i].length,
			GL_FLOAT,
			GL_TRUE,
			(GLsizei)(gn->GetMeshResource()->attributes[i].stride * sizeof(float)),
			(GLvoid*)(gn->GetMeshResource()->attributes[i].offset * sizeof(float))
		);
	}

	//////////////////
	// SET UNIFORMS //
	//////////////////
	unsigned int tex = 0;
	// Loop through uniforms.
	for (size_t i = 0; i < gn->GetShaderResource()->uniforms.size(); i++) {

		// Pointer to shared_ptr<void>, dereference to resource with **
		void* sp = (void*)&std::get<2>(gn->GetShaderResource()->uniforms[i]);

		// Uniform type.
		ResourceLib::ShaderResource::UniformType ut = *(ResourceLib::ShaderResource::UniformType*) & std::get<0>(gn->GetShaderResource()->uniforms[i]);

		// Uniform name.
		std::string un = *(std::string*) & std::get<1>(gn->GetShaderResource()->uniforms[i]);

		// Uniform location.
		GLint uniformLocation = *(GLint*)&std::get<3>(gn->GetShaderResource()->uniforms[i]);

		// Switch the type of the uniform union.
		if (ut == ResourceLib::ShaderResource::UniformType::Mat4) {
			/*MathLib::Mat4 culvert = MathLib::Mat4::Identity;// = (cameraMatrix * gn->transform.GetTransform());

			MathLib::Mat4 m = **(std::shared_ptr<MathLib::Mat4>*)sp;

			if (un == "projection") {
				culvert = cameraProjection;
			}
			else if (un == "modelView") {
				culvert = cameraView * gn->transform.GetTransform();
			}
			else if (un == "normalMat") {
				MathLib::Mat4 inverted;
				if (MathLib::Mat4::Inverse(gn->transform.GetTransform(), &inverted)) {
					culvert = MathLib::Mat4::Transpose(inverted);
				};
    			//mat4 normalMat = transpose(inverse(model))
			}*/

			MathLib::Mat4 culvert = **(std::shared_ptr<MathLib::Mat4>*)sp;

			// Bind uniform to shader.
			glUniformMatrix4fv(
				uniformLocation, // i when looping.
				1,
				GL_TRUE,
				// &** address of double dereferenced double pointer.
				(GLfloat*)&culvert // PV*M = MVP
			);
		}
		else if (ut == ResourceLib::ShaderResource::UniformType::Int) {
			int val = **(std::shared_ptr<int>*)sp;
			glUniform1i(uniformLocation, val);
		}
		else if (ut == ResourceLib::ShaderResource::UniformType::Float) {
			// Bind uniform to shader.
			float f = **(std::shared_ptr<float>*)sp;
			glUniform1f(uniformLocation, f);
		}
		else if (ut == ResourceLib::ShaderResource::UniformType::Vec2) {
			// Bind uniform to shader.
			MathLib::Vec4 v = **(std::shared_ptr<MathLib::Vec4>*)sp;
			glUniform2f(uniformLocation, v[0], v[1]);
		}
		else if (ut == ResourceLib::ShaderResource::UniformType::Vec3) {
			// Bind uniform to shader.
			MathLib::Vec4 v = **(std::shared_ptr<MathLib::Vec4>*)sp;
			glUniform3f(uniformLocation, v[0], v[1], v[2]);
		}
		else if (ut == ResourceLib::ShaderResource::UniformType::Vec4) {
			// Bind uniform to shader.
			MathLib::Vec4 v = **(std::shared_ptr<MathLib::Vec4>*)sp;
			glUniform4f(uniformLocation, v[0], v[1], v[2], v[3]);
		}
		else if (ut == ResourceLib::ShaderResource::UniformType::Sam2) {
			if (gn->GetTextureResource() != nullptr) {
				/*glActiveTexture(GL_TEXTURE0 + tex);
				glBindTexture(GL_TEXTURE_2D, textureHandle);
				glUniform1i(1, tex);*/

				//std::string uniformName = *(std::string*)&std::get<1>(gn->sr->uniforms[i]);
				//std::string texName = (**(std::shared_ptr<ResourceLib::TextureResource>*)sp).filename;

				// Since we don't have other textures we can ignore this at all times tbh.
				//if (uniformName == "wakeMeUpInside") {
					// Activate and bind texture.
					glActiveTexture(GL_TEXTURE0 + tex);
					glBindTexture(handles[gn->GetTextureResource()->filename + "_TEX"].first, handles[gn->GetTextureResource()->filename + "_TEX"].second);
					//glBindTexture(handles[(**(std::shared_ptr<ResourceLib::TextureResource>*)sp).filename + "_TEX"].first, handles[(**(std::shared_ptr<ResourceLib::TextureResource>*)sp).filename + "_TEX"].second);
					// Give texture to uniform location 1 in shader.
					glUniform1i(uniformLocation, tex);
				//}

				tex++;
			}
		}
		else if (ut == ResourceLib::ShaderResource::UniformType::Lights) {

		}
	}

	/////////////////////////////
	// DRAW USING INDEX BUFFER //
	/////////////////////////////

	// Additionally bind the index buffer.
	glBindBuffer(handles[gn->GetMeshResource()->filename + "_IBO"].first, handles[gn->GetMeshResource()->filename + "_IBO"].second);
	glDrawElements(GL_TRIANGLES, gn->GetMeshResource()->indicesCount, GL_UNSIGNED_INT, (void*)0);


	/////////////////////////////
	// UNBIND BUFFERS/TEXTURES //
	/////////////////////////////

	// Unbind index buffer.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	// Unbind texture.
	glBindTexture(GL_TEXTURE_2D, 0);
	// Unbind vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


MathLib::Mat4 GG::ResourceHandler::GetCameraProjection() {
	return cameraProjection;
}

MathLib::Mat4 GG::ResourceHandler::GetCameraView() {
	return cameraView;
}

void GG::ResourceHandler::GPUClean() {
	/*for (size_t i = 0; i < handles.size(); i++) {
		if (handles[i].handle != 0) {
			switch (handles[i].type) {
			case GLHandle::Type::VBO:
			case GLHandle::Type::IBO:
				glDeleteBuffers(1, &handles[i].handle);
				break;
			case GLHandle::Type::TEX:
				glDeleteTextures(1, &handles[i].handle);
				break;
			}
		}
	}*/
	// Also zero out the vector when done.
	handles.clear();
	//handles.shrink_to_fit();
}

// Initialize map.
std::map<std::string, std::pair<GLenum, GLint>> GG::ResourceHandler::handles = std::map<std::string, std::pair<GLenum, GLint>>();