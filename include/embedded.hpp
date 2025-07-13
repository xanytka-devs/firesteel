/*
/// Copyright 2025 Xanytka Devs
///   This file contains embedded data for Firesteel.

/// Content list:
/// * Embedded::defaultShaderVert - Vertex shader if some other shader fails (failsafe)
/// * Embedded::defaultShaderFrag - Fragment shader if some other shader fails (failsafe)
*/

#include "shader.hpp"

namespace Firesteel {
    namespace Embedded {
        static const char* defaultShaderVert = "\
#version 330 core\n\
layout(location = 0) in vec3 aPos;\n\
layout(location = 2) in vec2 aUV;\n\
out vec2 uv;\n\
uniform mat4 model;\n\
uniform mat4 view; \n\
uniform mat4 projection; \n\
void main() {\n\
	mat4 projView = projection * view;\n\
	if(projView==mat4(0)) projView=mat4(1);\n\
	gl_Position = projView * model * vec4(aPos, 1.0);\n\
	uv=aUV;\n\
}\
";
	    static const char* defaultShaderFrag = "\
#version 330 core\n\
out vec4 frag_COLOR;\n\
out vec4 frag_COLOR1;\n\
in vec2 uv;\n\
uniform float time;\n\
void main() {\n\
	float squareSize = 20.0;\n\
	float speedModifier = 1.5;\n\
	float offset = (time/100) * squareSize * speedModifier;\n\
	int xIndex = int((uv.x + offset) / squareSize);\n\
	int yIndex = int((uv.y + offset) / squareSize);\n\
	bool inAColorSquare = (xIndex % 2 == 0 && yIndex % 2 == 0) || (xIndex % 2 == 1 && yIndex % 2 == 1);\n\
	vec3 col = inAColorSquare ? vec3(1.0, 0.25, 0.0) : vec3(0.0, 0.0, 0.0);\n\
	frag_COLOR=vec4(col, 1.0);\n\
	frag_COLOR1=vec4(0);\n\
}\
";
    }
}