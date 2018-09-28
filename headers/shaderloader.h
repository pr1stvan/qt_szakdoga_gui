#ifndef SHADERLOADER_H
#define SHADERLOADER_H

#include <cstdio>
#include "openglinterface.h"

struct ShaderLoader{
    void getErrorInfo(unsigned int handle);
    void checkShader(unsigned int shader, const char *message);
    void checkLinking(unsigned int program);
    unsigned int createShaderProgramFromSource(const char * vertexSource, const char* fragmentSource);
};



#endif // SHADERLOADER_H
