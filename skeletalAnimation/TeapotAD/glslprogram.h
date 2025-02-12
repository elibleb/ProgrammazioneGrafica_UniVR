#pragma once

#ifndef GLSLPROGRAM_H
#define GLSLPROGRAM_H

#ifdef WIN32
#pragma warning( disable : 4290 )
#endif


#include "gl_core_4_3.hpp"

#include <glm/glm.hpp>
//

#include <string>
using std::string;
#include <map>
#include "Math3D.h"




using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::mat3;



#include <stdexcept>

//gestione programmi GLSL. Permette di caricare shader, compilare e collegare programmi, configurare varibili uniform, gestire attributi
//e uniform e fare devug delle risorse shader

class GLSLProgramException : public std::runtime_error {
  public:
    GLSLProgramException( const string & msg ) :
      std::runtime_error(msg) { }
};

namespace GLSLShader {
  enum GLSLShaderType {
    VERTEX = gl::VERTEX_SHADER, 
    FRAGMENT = gl::FRAGMENT_SHADER, 
    GEOMETRY = gl::GEOMETRY_SHADER,
    TESS_CONTROL = gl::TESS_CONTROL_SHADER, 
    TESS_EVALUATION = gl::TESS_EVALUATION_SHADER, 
    COMPUTE = gl::COMPUTE_SHADER
  };
};

class GLSLProgram
{
  private:

	static const unsigned int ui_BoneArraySize = 70;

	GLint m_boneLocation[ui_BoneArraySize]; // Bone uniform locations 

    int  handle;
    bool linked;
    std::map<string, int> uniformLocations;

    GLint  getUniformLocation(const char * name );
    bool fileExists( const string & fileName );
    string getExtension( const char * fileName );

    // Make these private in order to make the object non-copyable
    GLSLProgram( const GLSLProgram & other ) { }
    GLSLProgram & operator=( const GLSLProgram &other ) { return *this; }

  public:
    GLSLProgram();
    ~GLSLProgram();

    void   compileShader( const char *fileName ) throw (GLSLProgramException);
    void   compileShader( const char * fileName, GLSLShader::GLSLShaderType type ) throw (GLSLProgramException);
    void   compileShader( const string & source, GLSLShader::GLSLShaderType type, 
        const char *fileName = NULL ) throw (GLSLProgramException);

    void   link() throw (GLSLProgramException);
    void   validate() throw(GLSLProgramException);
    void   use() throw (GLSLProgramException);

    int    getHandle();
    bool   isLinked();

    void   bindAttribLocation( GLuint location, const char * name);
    void   bindFragDataLocation( GLuint location, const char * name );

    void   setUniform( const char *name, float x, float y, float z);
    void   setUniform( const char *name, const vec2 & v);
    void   setUniform( const char *name, const vec3 & v);
    void   setUniform( const char *name, const vec4 & v);
    void   setUniform( const char *name, const mat4 & m);
    void   setUniform( const char *name, const mat3 & m);
    void   setUniform( const char *name, float val );
    void   setUniform( const char *name, int val );
    void   setUniform( const char *name, bool val );
    void   setUniform( const char *name, GLuint val );
	void   setUniformIndex(unsigned int Index, const Matrix4f& matIn);

	void   initialiseBoneUniforms();

    void   printActiveUniforms();
    void   printActiveUniformBlocks();
    void   printActiveAttribs();

    const char * getTypeString( GLenum type );
};

#endif // GLSLPROGRAM_H
