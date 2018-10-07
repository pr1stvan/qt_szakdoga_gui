#include "openglinterface.h"
#include "glwidget.h"

GLWidget *w;
OpenGLInterface::OpenGLInterface(GLWidget *widget)
{
    w = widget;
}
void glLinkProgram(GLuint program)
{
    w->glLinkProgram(program);
}

void glGetShaderiv(GLuint shader, GLenum pname, GLint* params)
{
    w->glGetShaderiv(shader,pname,params);
}

void glGetShaderInfoLog(GLuint shader, GLsizei bufsize, GLsizei* length, char* infolog)
{
    w->glGetShaderInfoLog(shader, bufsize, length, infolog);
}
void glGetProgramiv(GLuint program, GLenum pname, GLint* params)
{
    w->glGetProgramiv(program, pname, params);
}
void glShaderSource(GLuint shader, GLsizei count, const char** string, const GLint* length)
{
    w->glShaderSource(shader, count, string, length);
}
void glCompileShader(GLuint shader)
{
    w->glCompileShader(shader);
}
void glAttachShader(GLuint program, GLuint shader)
{
    w->glAttachShader(program, shader);
}

void glDetachShader(GLuint program, GLuint shader)
{
    w->glDetachShader(program,shader);
}
void glDeleteShader(GLuint shader)
{
    w->glDeleteShader(shader);
}
GLuint glCreateShader(GLenum type)
{
    return w->glCreateShader(type);
}
GLuint glCreateProgram()
{
    return w->glCreateProgram();
}
