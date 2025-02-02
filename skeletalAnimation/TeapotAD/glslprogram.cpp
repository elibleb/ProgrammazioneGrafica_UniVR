
#include "glslprogram.h"


#include <fstream>
using std::ifstream;
using std::ios;

#include <sstream>
#include <sys/stat.h>

//gestione programmi GLSL. Permette di caricare shader, compilare e collegare programmi, configurare varibili uniform, gestire attributi
//e uniform e fare devug delle risorse shader

namespace GLSLShaderInfo {
  struct shader_file_extension {
    const char *ext;
    GLSLShader::GLSLShaderType type;
  };

  struct shader_file_extension extensions[] = 
  {
    {".vs", GLSLShader::VERTEX},
    {".vert", GLSLShader::VERTEX},
    {".gs", GLSLShader::GEOMETRY},
    {".geom", GLSLShader::GEOMETRY},
    {".tcs", GLSLShader::TESS_CONTROL},
    {".tes", GLSLShader::TESS_EVALUATION},
    {".fs", GLSLShader::FRAGMENT},
    {".frag", GLSLShader::FRAGMENT},
    {".cs", GLSLShader::COMPUTE}   
  };
}

// Costruttore di GLSLProgram
GLSLProgram::GLSLProgram() : handle(0), linked(false) { }

// Distruttore di GLSLProgram
GLSLProgram::~GLSLProgram() {
  if(handle == 0) return;

  // Interroga il numero di shader allegati
  GLint numShaders = 0;
  gl::GetProgramiv(handle, gl::ATTACHED_SHADERS, &numShaders);

  // Ottieni i nomi degli shader
  GLuint * shaderNames = new GLuint[numShaders];
  gl::GetAttachedShaders(handle, numShaders, NULL, shaderNames);

  // Elimina gli shader
  for (int i = 0; i < numShaders; i++)
    gl::DeleteShader(shaderNames[i]);

  // Elimina il programma
  gl::DeleteProgram (handle);

  delete[] shaderNames;
}


//Compila lo shader dato il nome del file
void GLSLProgram::compileShader( const char * fileName )
  throw( GLSLProgramException ) {
    int numExts = sizeof(GLSLShaderInfo::extensions) / sizeof(GLSLShaderInfo::shader_file_extension);

    // Controlla l'estensione del nome del file per determinare il tipo di shader
    string ext = getExtension( fileName );
    GLSLShader::GLSLShaderType type = GLSLShader::VERTEX;
    bool matchFound = false;
    for( int i = 0; i < numExts; i++ ) {
      if( ext == GLSLShaderInfo::extensions[i].ext ) {
        matchFound = true;
        type = GLSLShaderInfo::extensions[i].type;
        break;
      }
    }

    // Se non viene trovata una corrispondenza, lancia un'eccezione
    if( !matchFound ) {
      string msg = "Unrecognized extension: " + ext;
      throw GLSLProgramException(msg);
    }

    // Passa il tipo di shader scoperto
    compileShader( fileName, type );
  }

//Ottieni l'estensione di un file
string GLSLProgram::getExtension( const char * name ) {
  string nameStr(name);

  size_t loc = nameStr.find_last_of('.');
  if( loc != string::npos ) {
    return nameStr.substr(loc, string::npos);
  }
  return "";
}

//Compila lo shader dato il nome del file e il tipo di shader
void GLSLProgram::compileShader( const char * fileName,
    GLSLShader::GLSLShaderType type )
throw( GLSLProgramException )
{
  if( ! fileExists(fileName) )
  {
    string message = string("Shader: ") + fileName + " not found.";
    throw GLSLProgramException(message);
  }

  if( handle <= 0 ) {
    handle = gl::CreateProgram();
    if( handle == 0) {
      throw GLSLProgramException("Unable to create shader program.");
    }
  }

  ifstream inFile( fileName, ios::in );
  if( !inFile ) {
    string message = string("Unable to open: ") + fileName;
    throw GLSLProgramException(message);
  }

  // Ottieni il contenuto del file
  std::stringstream code;
  code << inFile.rdbuf();
  inFile.close();

  compileShader(code.str(), type, fileName);
}

//Compila lo shader dato il codice sorgente, il tipo di shader e il nome del file
void GLSLProgram::compileShader( const string & source, 
    GLSLShader::GLSLShaderType type,
    const char * fileName )
throw(GLSLProgramException)
{
  if( handle <= 0 ) {
    handle = gl::CreateProgram();
    if( handle == 0) {
      throw GLSLProgramException("Unable to create shader program.");
    }
  }

  GLuint shaderHandle = gl::CreateShader(type);

  const char * c_code = source.c_str();
  gl::ShaderSource( shaderHandle, 1, &c_code, NULL );

  // Compila lo shader
  gl::CompileShader(shaderHandle);

  // Verifica la presenza di errori
  int result;
  gl::GetShaderiv( shaderHandle, gl::COMPILE_STATUS, &result );
  if( FALSE == result ) {
    //Compilazione fallita, ottieni il log
    int length = 0;
    string logString;
    gl::GetShaderiv(shaderHandle, gl::INFO_LOG_LENGTH, &length );
    if( length > 0 ) {
      char * c_log = new char[length];
      int written = 0;
      gl::GetShaderInfoLog(shaderHandle, length, &written, c_log);
      logString = c_log;
      delete [] c_log;
    }
    string msg;
    if( fileName ) {
      msg = string(fileName) + ": shader compliation failed\n";
    } else {
      msg = "Shader compilation failed.\n";
    }
    msg += logString;

    throw GLSLProgramException(msg);

  } else {
    // Compilazione riuscita, allega lo shader
    gl::AttachShader(handle, shaderHandle);
  }
}

//Collega il programma shader
void GLSLProgram::link() throw(GLSLProgramException)
{
  if( linked ) return;
  if( handle <= 0 ) 
    throw GLSLProgramException("Program has not been compiled.");

  gl::LinkProgram(handle);

  int status = 0;
  gl::GetProgramiv( handle, gl::LINK_STATUS, &status);
  if( FALSE == status ) {
    // Memorizza il log e restituisce false
    int length = 0;
    string logString;

    gl::GetProgramiv(handle, gl::INFO_LOG_LENGTH, &length );

    if( length > 0 ) {
      char * c_log = new char[length];
      int written = 0;
      gl::GetProgramInfoLog(handle, length, &written, c_log);
      logString = c_log;
      delete [] c_log;
    }

    throw GLSLProgramException(string("Program link failed:\n") + logString);
  } else {
    uniformLocations.clear();
    linked = true;
  }    
}

// Usa il programma shader collegato
void GLSLProgram::use() throw(GLSLProgramException)
{
  if( handle <= 0 || (! linked) ) 
    throw GLSLProgramException("Shader has not been linked");
  gl::UseProgram( handle );
}

// Ottieni il handle del programma shader
int GLSLProgram::getHandle()
{
  return handle;
}

// Verifica se il programma shader è stato collegato correttamente
bool GLSLProgram::isLinked()
{
  return linked;
}

// Associa una posizione di attributo al programma shader
void GLSLProgram::bindAttribLocation( GLuint location, const char * name)
{
  gl::BindAttribLocation(handle, location, name);
}

// Associa una posizione di dati di frammento al programma shader
void GLSLProgram::bindFragDataLocation( GLuint location, const char * name )
{
  gl::BindFragDataLocation(handle, location, name);
}

// Imposta un valore uniform di tipo float con 3 componenti
void GLSLProgram::setUniform( const char *name, float x, float y, float z)
{
  GLint loc = getUniformLocation(name);
  gl::Uniform3f(loc,x,y,z);
}

// Imposta un valore uniform di tipo vec3
void GLSLProgram::setUniform( const char *name, const vec3 & v)
{
  this->setUniform(name,v.x,v.y,v.z);
}

// Imposta un valore uniform di tipo vec4
void GLSLProgram::setUniform( const char *name, const vec4 & v)
{
  GLint loc = getUniformLocation(name);
  gl::Uniform4f(loc,v.x,v.y,v.z,v.w);
}

// Imposta un valore uniform di tipo vec2
void GLSLProgram::setUniform( const char *name, const vec2 & v)
{
  GLint loc = getUniformLocation(name);
  gl::Uniform2f(loc,v.x,v.y);
}

// Imposta un valore uniform di tipo mat4
void GLSLProgram::setUniform( const char *name, const mat4 & m)
{
  GLint loc = getUniformLocation(name);
  gl::UniformMatrix4fv(loc, 1, FALSE, &m[0][0]);
}

// Imposta un valore uniform di tipo mat3
void GLSLProgram::setUniform( const char *name, const mat3 & m)
{
  GLint loc = getUniformLocation(name);
  gl::UniformMatrix3fv(loc, 1, FALSE, &m[0][0]);
}

// Imposta un valore uniform di tipo float
void GLSLProgram::setUniform( const char *name, float val )
{
  GLint loc = getUniformLocation(name);
  gl::Uniform1f(loc, val);
}

// Imposta un valore uniform di tipo int
void GLSLProgram::setUniform( const char *name, int val )
{
  GLint loc = getUniformLocation(name);
  gl::Uniform1i(loc, val);
}

// Imposta un valore uniform di tipo GLuint
void GLSLProgram::setUniform( const char *name, GLuint val )
{
  GLint loc = getUniformLocation(name);
  gl::Uniform1ui(loc, val);
}

// Imposta un valore uniform di tipo booleano
void GLSLProgram::setUniform( const char *name, bool val )
{
  int loc = getUniformLocation(name);
  gl::Uniform1i(loc, val);
}

// Passa una matrice di trasformazione alla posizione uniform indicata
void GLSLProgram::setUniformIndex(unsigned int Index, const Matrix4f& matIn)
{
	// Passa la matrice di trasformazione all'array uniform.  
	gl::UniformMatrix4fv(m_boneLocation[Index], 1, TRUE, (const GLfloat*)matIn.m);
}

// Inizializza gli uniform delle ossa
void GLSLProgram::initialiseBoneUniforms()
{
	for (unsigned int i = 0; i < ui_BoneArraySize; i++) {

		char Name[128];
		memset(Name, 0, sizeof(Name));
		_snprintf_s(Name, sizeof(Name), "gBones[%d]", i);
		m_boneLocation[i] = gl::GetUniformLocation(handle, Name);
	}
}


// Stampa gli uniform attivi
void GLSLProgram::printActiveUniforms() {
  GLint numUniforms = 0;
  gl::GetProgramInterfaceiv( handle, gl::UNIFORM, gl::ACTIVE_RESOURCES, &numUniforms);

  GLenum properties[] = {gl::NAME_LENGTH, gl::TYPE, gl::LOCATION, gl::BLOCK_INDEX};

  printf("Active uniforms:\n");
  for( int i = 0; i < numUniforms; ++i ) {
    GLint results[4];
    gl::GetProgramResourceiv(handle, gl::UNIFORM, i, 4, properties, 4, NULL, results);

    if( results[3] != -1 ) continue;  // Salta gli uniform nei blocchi 
    GLint nameBufSize = results[0] + 1;
    char * name = new char[nameBufSize];
    gl::GetProgramResourceName(handle, gl::UNIFORM, i, nameBufSize, NULL, name);
    printf("%-5d %s (%s)\n", results[2], name, getTypeString(results[1]));
    delete [] name;
  }
}

// Stampa i blocchi di uniform attivi
void GLSLProgram::printActiveUniformBlocks() {
  GLint numBlocks = 0;

  gl::GetProgramInterfaceiv(handle, gl::UNIFORM_BLOCK, gl::ACTIVE_RESOURCES, &numBlocks);
  GLenum blockProps[] = {gl::NUM_ACTIVE_VARIABLES, gl::NAME_LENGTH};
  GLenum blockIndex[] = {gl::ACTIVE_VARIABLES};
  GLenum props[] = {gl::NAME_LENGTH, gl::TYPE, gl::BLOCK_INDEX};

  for(int block = 0; block < numBlocks; ++block) {
    GLint blockInfo[2];
    gl::GetProgramResourceiv(handle, gl::UNIFORM_BLOCK, block, 2, blockProps, 2, NULL, blockInfo);
    GLint numUnis = blockInfo[0];

    char * blockName = new char[blockInfo[1]+1];
    gl::GetProgramResourceName(handle, gl::UNIFORM_BLOCK, block, blockInfo[1]+1, NULL, blockName);
    printf("Uniform block \"%s\":\n", blockName);
    delete [] blockName;

    GLint * unifIndexes = new GLint[numUnis];
    gl::GetProgramResourceiv(handle, gl::UNIFORM_BLOCK, block, 1, blockIndex, numUnis, NULL, unifIndexes);

    for( int unif = 0; unif < numUnis; ++unif ) {
      GLint uniIndex = unifIndexes[unif];
      GLint results[3];
      gl::GetProgramResourceiv(handle, gl::UNIFORM, uniIndex, 3, props, 3, NULL, results);

      GLint nameBufSize = results[0] + 1;
      char * name = new char[nameBufSize];
      gl::GetProgramResourceName(handle, gl::UNIFORM, uniIndex, nameBufSize, NULL, name);
      printf("    %s (%s)\n", name, getTypeString(results[1]));
      delete [] name;
    }

    delete [] unifIndexes;
  }
}

// Stampa gli attributi attivi
void GLSLProgram::printActiveAttribs() {
  GLint numAttribs;
  gl::GetProgramInterfaceiv( handle, gl::PROGRAM_INPUT, gl::ACTIVE_RESOURCES, &numAttribs);

  GLenum properties[] = {gl::NAME_LENGTH, gl::TYPE, gl::LOCATION};

  printf("Active attributes:\n");
  for( int i = 0; i < numAttribs; ++i ) {
    GLint results[3];
    gl::GetProgramResourceiv(handle, gl::PROGRAM_INPUT, i, 3, properties, 3, NULL, results);

    GLint nameBufSize = results[0] + 1;
    char * name = new char[nameBufSize];
    gl::GetProgramResourceName(handle, gl::PROGRAM_INPUT, i, nameBufSize, NULL, name);
    printf("%-5d %s (%s)\n", results[2], name, getTypeString(results[1]));
    delete [] name;
  }
}

// Ottieni il tipo di dato come stringa
const char * GLSLProgram::getTypeString( GLenum type ) {
  // Esempi comuni, ce ne sono molti di +
  switch(type) {
  case gl::FLOAT:
      return "float";
  case gl::FLOAT_VEC2:
      return "vec2";
    case gl::FLOAT_VEC3:
      return "vec3";
    case gl::FLOAT_VEC4:
      return "vec4";
    case gl::DOUBLE:
      return "double";
    case gl::INT:
      return "int";
    case gl::UNSIGNED_INT:
      return "unsigned int";
    case gl::BOOL:
      return "bool";
    case gl::FLOAT_MAT2:
      return "mat2";
    case gl::FLOAT_MAT3:
      return "mat3";
	case gl::FLOAT_MAT4:
      return "mat4";
    default:
      return "?";
  }
}

// Verifica la validità del programma shader
void GLSLProgram::validate() throw(GLSLProgramException)
{
  if( ! isLinked() ) 
    throw GLSLProgramException("Program is not linked");

  GLint status;
  gl::ValidateProgram( handle );
  gl::GetProgramiv( handle, gl::VALIDATE_STATUS, &status );

  if( FALSE == status ) {
    // memorizza il log e ritorna false
    int length = 0;
    string logString;

    gl::GetProgramiv(handle, gl::INFO_LOG_LENGTH, &length );

    if( length > 0 ) {
      char * c_log = new char[length];
      int written = 0;
      gl::GetProgramInfoLog(handle, length, &written, c_log);
      logString = c_log;
      delete [] c_log;
    }

    throw GLSLProgramException(string("Program failed to validate\n") + logString);

  }
}

// Ottieni la posizione di un uniform
int GLSLProgram::getUniformLocation(const char * name )
{
  std::map<string, int>::iterator pos;
  pos = uniformLocations.find(name);

  if( pos == uniformLocations.end() ) {
    uniformLocations[name] = gl::GetUniformLocation(handle, name);
  }

  return uniformLocations[name];
}


// Verifica se il file esiste
bool GLSLProgram::fileExists( const string & fileName )
{
  struct stat info;
  int ret = -1;

  ret = stat(fileName.c_str(), &info);
  return 0 == ret;
}
