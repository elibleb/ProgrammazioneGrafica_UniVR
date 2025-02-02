
#include "glutils.h"
#include "gl_core_4_3.hpp"


#include <cstdio>

//codice utilizzato per il debug, funzioni utili per rilevare e stampare gli errori e per ottenere informazioni sull'installazione
//di OpenGl e le relative estensioni

GLUtils::GLUtils() {}

int GLUtils::checkForOpenGLError(const char * file, int line) {
    //
    // ritorna 1 se si è verificato un errore OpenGL, 0 altrimenti.
    //
    GLenum glErr;
    int    retCode = 0;

    glErr = gl::GetError();
    while (glErr != NO_ERROR)
    {
		const char * message = "";
		switch( glErr )
		{
		case gl::INVALID_ENUM:
			message = "Invalid enum";
			break;
		case gl::INVALID_VALUE:
			message = "Invalid value";
			break;
		case gl::INVALID_OPERATION:
			message = "Invalid operation";
			break;
		case gl::INVALID_FRAMEBUFFER_OPERATION:
			message = "Invalid framebuffer operation";
			break;
		case gl::OUT_OF_MEMORY:
			message = "Out of memory";
			break;
		default:
			message = "Unknown error";
		}

        printf("glError in file %s @ line %d: %s\n", file, line, message);
        retCode = 1;
        glErr = gl::GetError();
    }
    return retCode;

}

void GLUtils::dumpGLInfo(bool dumpExtensions) {
    const GLubyte *renderer = gl::GetString( gl::RENDERER );
    const GLubyte *vendor = gl::GetString( gl::VENDOR );
    const GLubyte *version = gl::GetString( gl::VERSION );
    const GLubyte *glslVersion = gl::GetString( gl::SHADING_LANGUAGE_VERSION );

    GLint major, minor;
    gl::GetIntegerv(gl::MAJOR_VERSION, &major);
    gl::GetIntegerv(gl::MINOR_VERSION, &minor);

    printf("GL Vendor    : %s\n", vendor);
    printf("GL Renderer  : %s\n", renderer);
    printf("GL Version   : %s\n", version);
    printf("GL Version   : %d.%d\n", major, minor);
    printf("GLSL Version : %s\n", glslVersion);

    if( dumpExtensions ) {
        GLint nExtensions;
        gl::GetIntegerv(gl::NUM_EXTENSIONS, &nExtensions);
        for( int i = 0; i < nExtensions; i++ ) {
            printf("%s\n", gl::GetStringi(gl::EXTENSIONS, i));
        }
    }
}
