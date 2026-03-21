/*
* Copyright 2025 Xanytka Devs
*   This file contains embedded data for Firesteel.
*
* Content list:
* * Embedded::defaultShaderVert - Vertex shader if some other shader fails (failsafe)
* * Embedded::defaultShaderFrag - Fragment shader if some other shader fails (failsafe)
* * Embedded::primitiveShaderVert - Vertex shader for rendering basic primitives
* * Embedded::primitiveShaderFrag - Fragment shader for rendering basic primitives
*/

namespace Firesteel {
    namespace Embedded {
        static const char* defaultShaderVert = "\
#version 330 core\n\
layout(location = 0) in vec3 aPos;\n\
uniform mat4 model;\n\
uniform mat4 view;\n\
uniform mat4 projection;\n\
void main() {\n\
	mat4 projView=projection*view;\n\
	if(projView==mat4(0)) projView=mat4(1);\n\
	gl_Position=projView*model*vec4(aPos,1.0);\n\
}\
";
	    static const char* defaultShaderFrag = "\
#version 330 core\n\
out vec4 frag_COLOR;\n\
void main() {\n\
	frag_COLOR=vec4(vec3(1.0, 0.25, 0.0),1.0);\n\
}\
";
        static const char* primitiveShaderVert = "\
#version 330 core\n\
layout(location = 0) in vec3 aPos;\n\
uniform mat4 view;\n\
uniform mat4 projection;\n\
void main() {\n\
	gl_Position=projection*view*vec4(aPos, 1.0);\n\
}\
";
	    static const char* primitiveShaderFrag = "\
#version 330 core\n\
out vec4 frag_COLOR;\n\
uniform vec4 color;\n\
void main() {\n\
	frag_COLOR=color;\n\
}\
";
    }
}