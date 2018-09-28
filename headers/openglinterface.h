#ifndef OPENGLINTERFACE_H
#define OPENGLINTERFACE_H

#include <QOpenGLExtraFunctions>

class GLWidget;

class OpenGLInterface
{
public:
    OpenGLInterface(GLWidget *widget);
};

void glGetShaderiv(GLuint shader, GLenum pname, GLint* params);
void glGetShaderInfoLog(GLuint shader, GLsizei bufsize, GLsizei* length, char* infolog);
void glGetProgramiv(GLuint program, GLenum pname, GLint* params);
void glShaderSource(GLuint shader, GLsizei count, const char** string, const GLint* length);
void glCompileShader(GLuint shader);
void glAttachShader(GLuint program, GLuint shader);
void glLinkProgram(GLuint program);
void glDetachShader(GLuint program, GLuint shader);
void glDeleteShader(GLuint shader);
GLuint glCreateShader(GLenum type);
GLuint glCreateProgram();

#endif // OPENGLINTERFACE_H
