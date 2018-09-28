/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QMatrix4x4>
#include <QTimer>
#include <QColor>
#include <QMouseEvent>
#include <QCoreApplication>
#include <QGuiApplication>
#include <QScreen>
#include <math.h>
#include <QDebug>

#include "camera.h"
#include "framesystem.h"
#include "openglinterface.h"
#include "shaderloader.h"

QT_FORWARD_DECLARE_CLASS(OpenGLInterface)
QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    void updateSignal();
    int loadFrameDirectory(QStringList filePaths);
    bool velocityDataExist();
    bool areaDataExist();

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

public slots:
    void cameraSetHorizontalAngleDegrees(int degrees);
    void cameraSetVerticalAngleDegrees(int degrees);
    void cameraSetDistance(int distance);
    void cameraSetViewAngle(int degrees);

    void setLightX(int x);
    void setLightY(int y);
    void setLightZ(int z);
    void setLightPower(int power);
    void setSpecularPower(int power);
    void setSmoothness(int smoothnessValue);
    void setDrawingTriangles(bool state);

    void setLightColor(QColor color);
    void setBackgroundColor(QColor color);

    void setColorMode(ColorMode mode);
    void setSolidColor(QColor color);
    void setStartColor(QColor color);
    void setEndColor(QColor color);

    void setLightPosition(QVector3D position);

    void setFrameIdx(int idx);
    void setFps(int fps);
    void play();
    void pause();
    void stop();
    void nextFrame();
    void prevFrame();
    void setAutoReplay(bool autoReplay);

    void cleanup();
    void timer_tick();

private slots:
    void cameraHorizontalAngleChangedEmitter(int degrees);
    void cameraVerticalAngleChangedEmitter(int degrees);
    void cameraDistanceChangedEmitter(int distance);
    void cameraViewAngleChangedEmitter(int degrees);

signals:
    void cameraHorizontalAngleChanged(int degrees);
    void cameraVerticalAngleChanged(int degrees);
    void cameraDistanceChanged(int distance);
    void cameraViewAngleChanged(int degrees);

    void lightPowerChanged(int power);
    void specularPowerChanged(int power);
    void smoothnessChanged(int smoothness);
    void lightColorChanged(QColor color);
    void backgroundColorChanged(QColor color);

    void colorModeChanged(ColorMode mode);
    void solidColorChanged(QColor color);
    void startColorChanged(QColor color);
    void endColorChanged(QColor color);

    void lightXChanged(int x);
    void lightYChanged(int y);
    void lightZChanged(int z);

    void frameIdxChanged(int idx);

    void drawingTrianglesChanged(bool state);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    QString loadFile(QString path);
    void lightPositionChanged();
    void loadFrameToBuffers();
    void setupVertexAttribs();
    void allocateBuffers();
    void animate(int frameCount);

//    void getErrorInfo(unsigned int handle);
//    void checkShader(unsigned int shader, const char *message);
//    void checkLinking(unsigned int program);
//    unsigned int createShaderProgramFromSource(const char * vertexSource, const char* fragmentSource);

    int elapsed;
    bool m_core;
    int m_xRot;
    int m_yRot;
    int m_zRot;
    QPoint m_lastPos;

    unsigned int m_vaoID;
    unsigned int m_vertexBufferID;
    unsigned int m_ambientBufferID;
    unsigned int m_diffuseBufferID;
    unsigned int m_specularBufferID;
    unsigned int m_indexBufferID;

    unsigned int m_programID;

    int m_MVPMatrixLoc;
    int m_VMatrixLoc;
    int m_MMatrixLoc;
    int m_lightPosLoc;
    int m_lightPowerLoc;
    int m_specularPowerLoc;
    int m_ambientPowerLoc;
    int m_smoothnessLoc;
    int m_lightColorLoc;
    QMatrix4x4 M;
    QMatrix4x4 V;
    QMatrix4x4 P;
    bool m_transparent;

    Camera camera;

    QVector3D lightPosition;
    int lightPower;
    int specularPower;
    int smoothness;
    int ambientPower;

    ColorMode colorMode;
    QColor lightColor;
    QColor backgroundColor;
    QColor solidColor;
    QColor startColor;
    QColor endColor;

    FrameSystem frameSystem;
    bool autoReplay;
    bool paused;
    int frameIdx;
    int fps;
    bool areFramesLoaded;
    bool drawingTriangles;
    OpenGLInterface openGLInterface;
    ShaderLoader loader;
};

#endif
