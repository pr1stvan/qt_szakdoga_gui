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

#include "glwidget.h"
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <QTimer>
#include <math.h>
#include <qDebug>

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      m_xRot(0),
      m_yRot(0),
      m_zRot(0),
      m_vertexBuffer(QOpenGLBuffer::VertexBuffer),
      m_ambientBuffer(QOpenGLBuffer::VertexBuffer),
      m_diffuseBuffer(QOpenGLBuffer::VertexBuffer),
      m_specularBuffer(QOpenGLBuffer::VertexBuffer),
      m_indexBuffer(QOpenGLBuffer::IndexBuffer),
      m_program(0),
      autoReplay(false),
      paused(true),
      frameIdx(0),
      areFramesLoaded(false)
{
    fps=1;
    elapsed =0;
    m_core = QCoreApplication::arguments().contains(QStringLiteral("--coreprofile"));
    // --transparent causes the clear color to be transparent. Therefore, on systems that
    // support it, the widget will become transparent apart from the logo.
    m_transparent = QCoreApplication::arguments().contains(QStringLiteral("--transparent"));
    if (m_transparent) {
        QSurfaceFormat fmt = format();
        fmt.setAlphaBufferSize(8);
        setFormat(fmt);
    }

    camera.setPosition(QVector3D(20, 20, 10));
    camera.setLookAtPoint(QVector3D(0,0,0));
    camera.setCanvas(width(),height());

    lightPosition=QVector3D(0,10.0f,0);
    lightPower=50;
    specularPower=100;
    smoothness=5;

    lightColor=QColor(255,255,255);
    backgroundColor=QColor(130,130,130);

    colorMode=SOLID;
    solidColor=QColor(160, 160, 255);
    startColor=QColor(145, 255, 145);
    endColor=QColor(255, 80, 80);

    frameSystem.setSolidColor(solidColor);
    frameSystem.setStartColor(startColor);
    frameSystem.setEndColor(endColor);
    frameSystem.setColorMode(colorMode);

    M.setToIdentity();

    connect(&camera, &Camera::xRotationChanged, this, &GLWidget::cameraSetXRotationEmitter);
    connect(&camera, &Camera::yRotationChanged, this, &GLWidget::cameraSetYRotationEmitter);
    connect(&camera, &Camera::distanceChanged, this, &GLWidget::cameraSetDistanceEmitter);
    connect(&camera, &Camera::viewAngleChanged, this, &GLWidget::cameraSetViewAngleEmitter);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GLWidget::timer_tick);
    timer->start(16);
}

GLWidget::~GLWidget()
{
    cleanup();
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(400, 400);
}

void GLWidget::updateSignal()
{
    camera.updateSignal();

    lightPositionChanged();
    emit lightPowerChanged(lightPower);
    emit specularPowerChanged(specularPower);
    emit smoothnessChanged(smoothness);
    emit lightColorChanged(lightColor);
    emit backgroundColorChanged(backgroundColor);

    emit colorModeChanged(colorMode);
    emit solidColorChanged(solidColor);
    emit startColorChanged(startColor);
    emit endColorChanged(endColor);
}

int GLWidget::loadFrameDirectory(QString directoryName)
{
    paused=true;
    frameSystem.loadDirectory(directoryName);

    if(frameSystem.size()>0){
        areFramesLoaded=true;
        allocateBuffers();
        QVector3D centerPoint=frameSystem.getPointsAvgAfterLoading();
        camera.setLookAtPoint(centerPoint);
        setLightPosition(centerPoint + QVector3D(0,10,0));
    }
    else {
        areFramesLoaded=false;
        setLightPosition(QVector3D(0,10,0));
    }

    return frameSystem.size();
}

void GLWidget::cameraSetXRotation(int degrees)
{
    camera.setXRotation(degrees);
}

void GLWidget::cameraSetYRotation(int degrees)
{
    camera.setYRotation(degrees);
}

void GLWidget::cameraSetDistance(int distance)
{
    camera.setDistance(distance);
}

void GLWidget::cameraSetViewAngle(int degrees)
{
    camera.setViewAngle(degrees);
}

void GLWidget::setLightX(int x)
{
    setLightPosition(QVector3D(
                         (float)x,
                         lightPosition.y(),
                         lightPosition.z()
                         ));
}

void GLWidget::setLightY(int y)
{
    setLightPosition(QVector3D(
                         lightPosition.x(),
                         (float)y,
                         lightPosition.z()
                         ));
}

void GLWidget::setLightZ(int z)
{
    setLightPosition(QVector3D(
                         lightPosition.x(),
                         lightPosition.y(),
                         (float)z
                         ));
}

void GLWidget::setLightPower(int power)
{
    lightPower=power;
    emit lightPowerChanged(lightPower);
}

void GLWidget::setSpecularPower(int power)
{
    specularPower=power;
    emit specularPowerChanged(specularPower);
}
void GLWidget::setSmoothness(int smoothnessValue)
{
    smoothness=smoothnessValue;
    emit smoothnessChanged(smoothness);
}


void GLWidget::setLightColor(QColor color)
{
    lightColor=color;
    emit lightColorChanged(color);
}

void GLWidget::setBackgroundColor(QColor color)
{
    backgroundColor=color;
    emit backgroundColorChanged(color);
}

void GLWidget::setColorMode(ColorMode mode)
{
    colorMode=mode;
    frameSystem.setColorMode(colorMode);
    emit colorModeChanged(colorMode);
}

void GLWidget::setSolidColor(QColor color)
{
    solidColor=color;
    frameSystem.setSolidColor(color);
    emit solidColorChanged(color);
}

void GLWidget::setStartColor(QColor color)
{
    startColor=color;
    frameSystem.setStartColor(startColor);
    emit startColorChanged(color);
}

void GLWidget::setEndColor(QColor color)
{
    endColor=color;
    frameSystem.setEndColor(endColor);
    emit endColorChanged(color);
}

void GLWidget::setLightPosition(QVector3D position)
{
    lightPosition=position;
    lightPositionChanged();
}


void GLWidget::setFrameIdx(int idx)
{
    if(areFramesLoaded){
        frameIdx=idx;
        emit frameIdxChanged(frameIdx);
    }
}

void GLWidget::setFps(int fps)
{
    this->fps=fps;
}

void GLWidget::play()
{
    if(areFramesLoaded){
         paused=false;
    }
}

void GLWidget::pause()
{
    if(areFramesLoaded){
          paused=true;
    }
}

void GLWidget::stop()
{
    if(areFramesLoaded){
        setFrameIdx(0);
        paused=true;
    }
}

void GLWidget::nextFrame()
{
    if(areFramesLoaded){
        if(!paused){
            pause();
        }
        else{
            if(frameIdx +1 <= frameSystem.size() -1){
                setFrameIdx(frameIdx+1);
            }
        }
    }
}

void GLWidget::prevFrame()
{
    if(areFramesLoaded){
        if(!paused){
            pause();
        }
        else{
            if(frameIdx -1 >= 0){
                setFrameIdx(frameIdx-1);
            }
        }
    }

}

void GLWidget::setAutoReplay(bool autoReplay)
{
    this->autoReplay=autoReplay;
}

void GLWidget::cameraSetXRotationEmitter(int degrees)
{
    emit cameraXRotationChanged(degrees);
}

void GLWidget::cameraSetYRotationEmitter(int degrees)
{
    emit cameraYRotationChanged(degrees);
}

void GLWidget::cameraSetDistanceEmitter(int distance)
{
    emit cameraDistanceChanged(distance);
}

void GLWidget::cameraSetViewAngleEmitter(int degrees)
{
    emit cameraViewAngleChanged(degrees);
}

void GLWidget::timer_tick()
{
    elapsed = (elapsed + qobject_cast<QTimer*>(sender())->interval());
    int count=elapsed/(1000/fps);
    elapsed=elapsed % (1000/fps);

    animate(count);

    //redraw the scene
    update();
}

void GLWidget::cleanup()
{
    makeCurrent();
    m_vertexBuffer.destroy();
    m_ambientBuffer.destroy();
    m_diffuseBuffer.destroy();
    m_specularBuffer.destroy();
    m_indexBuffer.destroy();

    delete m_program;
    m_program = 0;
    doneCurrent();
}



void GLWidget::initializeGL()
{
    // In this example the widget's corresponding top-level window can change
    // several times during the widget's lifetime. Whenever this happens, the
    // QOpenGLWidget's associated context is destroyed and a new one is created.
    // Therefore we have to be prepared to clean up the resources on the
    // aboutToBeDestroyed() signal, instead of the destructor. The emission of
    // the signal will be followed by an invocation of initializeGL() where we
    // can recreate all resources.
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GLWidget::cleanup);

    initializeOpenGLFunctions();

    //set the background color
    glClearColor((float)backgroundColor.redF(), (float)backgroundColor.greenF(), (float)backgroundColor.blueF(), m_transparent ? 0 : 1);

    m_program = new QOpenGLShaderProgram;

    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex,":/shaders/vertexshader.vert");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragshader.frag");

    m_program->link();

    m_program->bind();
    m_MVPMatrixLoc = m_program->uniformLocation("MVP");
    m_VMatrixLoc = m_program->uniformLocation("V");
    m_MMatrixLoc = m_program->uniformLocation("M");
    m_lightPosLoc = m_program->uniformLocation("LightPosition_worldspace");
    m_lightColorLoc = m_program->uniformLocation("LightColor");
    m_lightPowerLoc= m_program->uniformLocation("LightPower");
    m_specularPowerLoc= m_program->uniformLocation("SpecularPower");
    m_smoothnessLoc= m_program->uniformLocation("Smoothness");

    // Create a vertex array object. In OpenGL ES 2.0 and OpenGL 2.x
    // implementations this is optional and support may not be present
    // at all. Nonetheless the below code works in all cases and makes
    // sure there is a VAO when one is needed.
    m_vao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

    // Setup our vertex buffer objects.
    m_vertexBuffer.create();
    m_vertexBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);

    m_ambientBuffer.create();
    m_ambientBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);

    m_diffuseBuffer.create();
    m_diffuseBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);

    m_specularBuffer.create();
    m_specularBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);

    m_indexBuffer.create();
    m_indexBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);

    // Store the vertex attribute bindings for the program.
    allocateBuffers();
    setupVertexAttribs();

    m_program->release();
}


void GLWidget::setupVertexAttribs(){
    m_vertexBuffer.bind();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(
                0,                  // attribute
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
            );
    m_vertexBuffer.release();

    m_ambientBuffer.bind();
    f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(
                1,                  // attribute
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
    );
    m_ambientBuffer.release();

    m_diffuseBuffer.bind();
    f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(2);
    f->glVertexAttribPointer(
                2,                  // attribute
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
    );
    m_diffuseBuffer.release();

    m_specularBuffer.bind();
    f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(3);
    f->glVertexAttribPointer(
                3,                  // attribute
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
    );
    m_specularBuffer.release();
}

void GLWidget::allocateBuffers()
{
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(frameSystem.getVertexAllocationSize()*sizeof(float));
    m_vertexBuffer.release();

    m_ambientBuffer.bind();
    m_ambientBuffer.allocate(frameSystem.getVertexAllocationSize()*sizeof(float));
    m_ambientBuffer.release();

    m_diffuseBuffer.bind();
    m_diffuseBuffer.allocate(frameSystem.getVertexAllocationSize()*sizeof(float));
    m_diffuseBuffer.release();

    m_specularBuffer.bind();
    m_specularBuffer.allocate(frameSystem.getVertexAllocationSize()*sizeof(float));
    m_specularBuffer.release();

    m_indexBuffer.bind();
    m_indexBuffer.allocate(frameSystem.getIndexAllocationSize()*sizeof(int));
    m_indexBuffer.release();

}

void GLWidget::animate(int frameCount)
{
    if(areFramesLoaded){
        for(int i = 1; i<=frameCount && !paused; i++){
            if(frameIdx+1 >= frameSystem.size() && autoReplay){
                setFrameIdx(0);
            }
            else if(frameIdx+1 >= frameSystem.size() && !autoReplay){
                pause();
            }
            else{
                setFrameIdx(frameIdx+1);
            }
        }
        loadFrameToBuffers();
    }
}
void GLWidget::loadFrameToBuffers()
{
    m_vertexBuffer.bind();
    m_vertexBuffer.write(0,frameSystem.getPoints(frameIdx).constData(),frameSystem.getVertexAllocationSize()*sizeof(float));
    m_vertexBuffer.release();

    m_ambientBuffer.bind();
    m_ambientBuffer.write(0, frameSystem.getAmbients(frameIdx).constData(),frameSystem.getVertexAllocationSize()*sizeof(float));
    m_ambientBuffer.release();

    m_diffuseBuffer.bind();
    m_diffuseBuffer.write(0,frameSystem.getDiffuses(frameIdx).constData(),frameSystem.getVertexAllocationSize()*sizeof(float));
    m_diffuseBuffer.release();

    m_specularBuffer.bind();
    m_specularBuffer.write(0,frameSystem.getSpeculars(frameIdx).constData(),frameSystem.getVertexAllocationSize()*sizeof(float));
    m_specularBuffer.release();

    m_indexBuffer.bind();
    m_indexBuffer.write(0,frameSystem.getIndices(frameIdx).constData(),frameSystem.getIndexAllocationSize()*sizeof(int));
    m_indexBuffer.release();

}

void GLWidget::paintGL()
{
    //set the background color
    glClearColor((float)backgroundColor.redF(), (float)backgroundColor.greenF(), (float)backgroundColor.blueF(), m_transparent ? 0 : 1);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);

    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

    m_program->bind();
    m_indexBuffer.bind();

    QMatrix4x4 P=camera.P();
    QMatrix4x4 V=camera.V();
    M.setToIdentity();

    m_program->setUniformValue(m_MVPMatrixLoc,P*V*M);
    m_program->setUniformValue(m_VMatrixLoc, V);
    m_program->setUniformValue(m_MMatrixLoc, M);

    m_program->setUniformValue(m_lightPosLoc, lightPosition);
    m_program->setUniformValue(m_lightPowerLoc,(float)lightPower);
    m_program->setUniformValue(m_specularPowerLoc,(1.0f*((float)specularPower)/100.0f));
    m_program->setUniformValue(m_smoothnessLoc,(float)smoothness);
    m_program->setUniformValue(m_lightColorLoc, QVector3D((float)lightColor.redF(),
                                                        (float)lightColor.greenF(),
                                                        (float)lightColor.blueF()));

//    glDrawArrays(GL_TRIANGLES, 0, frameSystem.getVertexAllocationSize());

    glDrawElements(
        GL_TRIANGLES,      // mode
        frameSystem.getIndexAllocationSize(),    // count
        GL_UNSIGNED_INT,            // type
        (void*)0           // element array buffer offset
                );
    m_indexBuffer.release();

    m_program->release();
}

void GLWidget::resizeGL(int w, int h)
{
    camera.setCanvas(w,h);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();


    if (event->buttons() & Qt::LeftButton) {
        camera.rotateHorizontal(dx);
        camera.rotateVertical(dy);

    } else if (event->buttons() & Qt::RightButton) {
        camera.rotateHorizontal(dx);
        camera.rotateVertical(dy);

    }
    m_lastPos = event->pos();
}

void GLWidget::wheelEvent(QWheelEvent *event){
     QPoint numDegrees = event->angleDelta() / 50.0f;
     float degrees=-numDegrees.y();
     camera.increaseDistance(nearbyint(degrees));

}

void GLWidget::lightPositionChanged()
{
    int x=(int)round(lightPosition.x());
    int y=(int)round(lightPosition.y());
    int z=(int)round(lightPosition.z());
    emit lightXChanged(x);
    emit lightYChanged(y);
    emit lightZChanged(z);
}
