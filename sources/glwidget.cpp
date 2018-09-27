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
#include <QGuiApplication>
#include <QScreen>
#include <math.h>
#include <QDebug>

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      m_xRot(0),
      m_yRot(0),
      m_zRot(0),
      m_vertexBufferID(0),
      m_ambientBufferID(0),
      m_diffuseBufferID(0),
      m_specularBufferID(0),
      m_indexBufferID(0),
      m_programID(0),
      autoReplay(false),
      paused(true),
      frameIdx(0),
      areFramesLoaded(false),
      drawingTriangles(true)
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
    ambientPower=50;

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
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    int height = screenGeometry.height();
    int width = screenGeometry.width();

    return QSize(width/8, height/8);
}

QSize GLWidget::sizeHint() const
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    int height = screenGeometry.height();
    int width = screenGeometry.width();

    return QSize(width/8, height/8);
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

    emit drawingTrianglesChanged(drawingTriangles);
}

int GLWidget::loadFrameDirectory(QStringList filePaths)
{
    paused=true;

    frameSystem.loadDirectory(filePaths);
    setColorMode(frameSystem.getColorMode());


    if(frameSystem.size()>0){
        areFramesLoaded=true;
        setDrawingTriangles(!frameSystem.preferDrawingPoints());
        allocateBuffers();
        QVector3D centerPoint=frameSystem.getPointsAvgAfterLoading();
        camera.setLookAtPoint(centerPoint);

//        int rndx = rand() % 11 - 5;
//        int rndy= rand() % 11 - 5;
//        int rndz=rand() % 11 - 5;

        setLightPosition(centerPoint + QVector3D(2,10,-4));
    }
    else {
        areFramesLoaded=false;
        setLightPosition(QVector3D(0,10,0));
    }
//    drawingTriangles=!frameSystem.preferDrawingPoints();
    return frameSystem.size();

}

bool GLWidget::velocityDataExist()
{
    return frameSystem.velocityDataExist();
}

bool GLWidget::areaDataExist()
{
    return frameSystem.areaDataExist();
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

void GLWidget::setDrawingTriangles(bool state)
{

    if(state==false){
        drawingTriangles=false;
        ambientPower=100;
    }
    else{
        drawingTriangles=true;
        ambientPower=50;
    }
    emit drawingTrianglesChanged(state);
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

    glDeleteBuffers(1, &m_vertexBufferID);
    glDeleteBuffers(1, &m_ambientBufferID);
    glDeleteBuffers(1, &m_diffuseBufferID);
    glDeleteBuffers(1, &m_specularBufferID);
    glDeleteBuffers(1, &m_indexBufferID);

    glDeleteVertexArrays(1, &m_vaoID);

    glDeleteProgram(m_programID);
    m_programID = 0;
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

    QString vertexSource = loadFile(":/shaders/vertexshader.vert");
    QString fragmentSource = loadFile(":/shaders/fragshader.frag");

    m_programID = createShaderProgramFromSource(
                vertexSource.toLatin1().constData(),
                fragmentSource.toLatin1().constData());

    glUseProgram(m_programID);

    m_MVPMatrixLoc = glGetUniformLocation(m_programID, "MVP");
    m_VMatrixLoc = glGetUniformLocation(m_programID, "V");
    m_MMatrixLoc = glGetUniformLocation(m_programID, "M");
    m_lightPosLoc = glGetUniformLocation(m_programID, "LightPosition_worldspace");
    m_lightColorLoc = glGetUniformLocation(m_programID, "LightColor");
    m_lightPowerLoc = glGetUniformLocation(m_programID, "LightPower");
    m_specularPowerLoc = glGetUniformLocation(m_programID, "SpecularPower");
    m_smoothnessLoc = glGetUniformLocation(m_programID, "Smoothness");
    m_ambientPowerLoc = glGetUniformLocation(m_programID, "AmbientPower");

    // Create a vertex array object. In OpenGL ES 2.0 and OpenGL 2.x
    // implementations this is optional and support may not be present
    // at all. Nonetheless the below code works in all cases and makes
    // sure there is a VAO when one is needed.

    glGenVertexArrays(1, &m_vaoID);	// create 1 vertex array object
    glBindVertexArray(m_vaoID);		// make it active

    glGenBuffers(1, &m_vertexBufferID);
    glGenBuffers(1, &m_ambientBufferID);
    glGenBuffers(1, &m_diffuseBufferID);
    glGenBuffers(1, &m_specularBufferID);
    glGenBuffers(1, &m_indexBufferID);

    // Store the vertex attribute bindings for the program.
    allocateBuffers();
    setupVertexAttribs();
}


void GLWidget::setupVertexAttribs(){
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
                0,                  // attribute
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
            );


    glBindBuffer(GL_ARRAY_BUFFER, m_ambientBufferID);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
                1,                  // attribute
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
    );

    glBindBuffer(GL_ARRAY_BUFFER, m_diffuseBufferID);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
                2,                  // attribute
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
    );

    glBindBuffer(GL_ARRAY_BUFFER, m_specularBufferID);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(
                3,                  // attribute
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
    );
}

void GLWidget::allocateBuffers()
{

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

void GLWidget::getErrorInfo(unsigned int handle)
{
    int logLen;
    glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logLen);
    if (logLen > 0) {
        char * log = new char[logLen];
        int written;
        glGetShaderInfoLog(handle, logLen, &written, log);
        printf("Shader log:\n%s", log);
        delete[] log;
    }
}

void GLWidget::checkShader(unsigned int shader, const char *message)
{
    int OK;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &OK);
    if (!OK) {
        printf("%s!\n", message);
        getErrorInfo(shader);
    }
}

void GLWidget::checkLinking(unsigned int program)
{
    int OK;
    glGetProgramiv(program, GL_LINK_STATUS, &OK);
    if (!OK) {
        printf("Failed to link shader program!\n");
        getErrorInfo(program);
    }
}

unsigned int GLWidget::createShaderProgramFromSource(const char *vertexSource, const char *fragmentSource)
{
    // Create vertex shader from string
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    if (!vertexShader) {
        printf("Error in vertex shader creation\n");
        exit(1);
    }
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    checkShader(vertexShader, "Vertex shader error");

    // Create fragment shader from string
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    if (!fragmentShader) {
        printf("Error in fragment shader creation\n");
        exit(1);
    }
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    checkShader(fragmentShader, "Fragment shader error");

    // Attach shaders to a single program
    unsigned int programID = glCreateProgram();
    if (!programID) {
        printf("Error in shader program creation\n");
        exit(1);
    }

    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);

    glLinkProgram(programID);
    checkLinking(programID);

    glDetachShader(programID, vertexShader);
    glDetachShader(programID, fragmentShader);

    glDeleteShader(fragmentShader);
    glDeleteShader(fragmentShader);

    return programID;
}


void GLWidget::loadFrameToBuffers()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, frameSystem.getVertexAllocationSize()*sizeof(float), frameSystem.getPoints(frameIdx).constData(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_ambientBufferID);
    glBufferData(GL_ARRAY_BUFFER, frameSystem.getVertexAllocationSize()*sizeof(float),frameSystem.getAmbients(frameIdx).constData(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_diffuseBufferID);
    glBufferData(GL_ARRAY_BUFFER, frameSystem.getVertexAllocationSize()*sizeof(float), frameSystem.getDiffuses(frameIdx).constData(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_specularBufferID);
    glBufferData(GL_ARRAY_BUFFER,frameSystem.getVertexAllocationSize()*sizeof(float), frameSystem.getSpeculars(frameIdx).constData(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, frameSystem.getIndexAllocationSize()*sizeof(int), frameSystem.getIndices(frameIdx).constData(), GL_DYNAMIC_DRAW);
}

void GLWidget::paintGL()
{
    //set the background color
    glClearColor((float)backgroundColor.redF(), (float)backgroundColor.greenF(), (float)backgroundColor.blueF(), m_transparent ? 0 : 1);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);

    glUseProgram(m_programID);
    glBindVertexArray(m_vaoID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferID);

    QMatrix4x4 P=camera.P();
    QMatrix4x4 V=camera.V();
    M.setToIdentity();
    QMatrix4x4 PVM = P*V*M;

    glUniformMatrix4fv(m_MVPMatrixLoc,1,GL_FALSE,PVM.constData());
    glUniformMatrix4fv(m_VMatrixLoc,1,GL_FALSE,V.constData());
    glUniformMatrix4fv(m_MMatrixLoc,1,GL_FALSE,M.constData());

    glUniform3fv(m_lightPosLoc,1,&lightPosition[0]);
    glUniform1f(m_lightPowerLoc,(float)lightPower);
    glUniform1f(m_specularPowerLoc,specularPower/100.0f);
    glUniform1f(m_smoothnessLoc,(float)smoothness);
    glUniform1f(m_ambientPowerLoc,ambientPower/100.0f);
    glUniform3f(m_lightColorLoc,(float)lightColor.redF(),
                (float)lightColor.greenF(),
                (float)lightColor.blueF());

//    glDrawArrays(GL_TRIANGLES, 0, frameSystem.getVertexAllocationSize());
    glDrawElements(
        drawingTriangles? GL_TRIANGLES: GL_POINTS,      // mode
        frameSystem.getIndexAllocationSize(),    // count
        GL_UNSIGNED_INT,            // type
        (void*)0           // element array buffer offset
                );
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
        camera.rotateHorizontal(-dx);
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

QString GLWidget::loadFile(QString path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return QString("file not found");
    }
    QTextStream in(&file);
    return in.readAll();
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
