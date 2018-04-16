#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <cmath>
#include "camera.h"
#include <QSlider>
#include <QSpinBox>
#include <QLabel>
#include <QFormLayout>

class CameraWidget : public QWidget
{
    Q_OBJECT
private:
    QSlider  *xRotationSlider;
    QSlider  *yRotationSlider;
    QSlider  *distanceSlider;
    QSlider *viewAngleSlider;

    QSpinBox* xRotationSpinBox;
    QSpinBox* yRotationSpinBox;
    QSpinBox* distanceSpinBox;
    QSpinBox* viewAngleSpinBox;

    Camera* camera;
public:
    explicit CameraWidget(Camera* camera,QWidget *parent = nullptr);



};

#endif // CAMERAWIDGET_H
