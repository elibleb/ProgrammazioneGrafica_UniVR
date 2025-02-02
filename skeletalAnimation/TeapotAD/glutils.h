#ifndef GLUTILS_H
#define GLUTILS_H

//codice utilizzato per il debug, funzioni utili per rilevare e stampare gli errori e per ottenere informazioni sull'installazione
//di OpenGl e le relative estensioni

class GLUtils
{
public:
    GLUtils();

    static int checkForOpenGLError(const char *, int);
    static void dumpGLInfo(bool dumpExtensions = false);
};

#endif // GLUTILS_H
