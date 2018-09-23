#include "camerawidget.h"


CameraWidget::CameraWidget(Camera* camera,QWidget *parent) : QWidget(parent),camera(camera)
{


//    QHBoxLayout *container = new QHBoxLayout;
//    QWidget *w = new QWidget;
//    w->setLayout(container);
//    mainLayout->addWidget(w);



    xRotationSlider = new QSlider (Qt::Horizontal);
    xRotationSlider->setRange(0, 359);
    xRotationSlider->setSingleStep(2);
    xRotationSlider->setPageStep(2);
    xRotationSlider->setTickInterval(2);
    xRotationSlider->setTickPosition(QSlider::TicksRight);

    yRotationSlider = new QSlider (Qt::Horizontal);
    yRotationSlider->setRange(20, 160);
    yRotationSlider->setSingleStep(2);
    yRotationSlider->setPageStep(2);
    yRotationSlider->setTickInterval(2);
    yRotationSlider->setTickPosition(QSlider::TicksRight);

    distanceSlider = new QSlider (Qt::Horizontal);
    distanceSlider->setRange(15, 100);
    distanceSlider->setSingleStep(2);
    distanceSlider->setPageStep(2);
    distanceSlider->setTickInterval(2);
    distanceSlider->setTickPosition(QSlider::TicksRight);

    viewAngleSlider = new QSlider (Qt::Horizontal);
    viewAngleSlider->setRange(20, 179);
    viewAngleSlider->setSingleStep(2);
    viewAngleSlider->setPageStep(2);
    viewAngleSlider->setTickInterval(2);
    viewAngleSlider->setTickPosition(QSlider::TicksRight);


    xRotationSpinBox = new QSpinBox();
    xRotationSpinBox->setRange(0, 359);
    xRotationSpinBox->setSingleStep(5);

    yRotationSpinBox = new QSpinBox();
    yRotationSpinBox->setRange(20,160);
    yRotationSpinBox->setSingleStep(5);

    distanceSpinBox = new QSpinBox();
    distanceSpinBox->setRange(15, 100);
    distanceSpinBox->setSingleStep(5);

    viewAngleSpinBox = new QSpinBox();
    viewAngleSpinBox->setRange(20, 179);
    viewAngleSpinBox->setSingleStep(5);

    connect(xRotationSlider, &QSlider::valueChanged, camera, &Camera::setXRotation);
    connect(camera, &Camera::xRotationChanged, xRotationSlider, &QSlider::setValue);
    connect(xRotationSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            camera, &Camera::setXRotation);
    connect(camera, &Camera::xRotationChanged, xRotationSpinBox, &QSpinBox::setValue);



    connect(yRotationSlider, &QSlider::valueChanged, camera, &Camera::setYRotation);
    connect(camera, &Camera::yRotationChanged, yRotationSlider, &QSlider::setValue);
    connect(yRotationSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            camera, &Camera::setYRotation);
    connect(camera, &Camera::yRotationChanged, yRotationSpinBox, &QSpinBox::setValue);


    connect(distanceSlider, &QSlider::valueChanged, camera, &Camera::setDistance);
    connect(camera, &Camera::distanceChanged, distanceSlider, &QSlider::setValue);
    connect(distanceSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            camera, &Camera::setDistance);
    connect(camera, &Camera::distanceChanged, distanceSpinBox, &QSpinBox::setValue);

    connect(viewAngleSlider, &QSlider::valueChanged, camera, &Camera::setViewAngle);
    connect(camera, &Camera::viewAngleChanged, viewAngleSlider, &QSlider::setValue);
    connect(viewAngleSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            camera, &Camera::setViewAngle);
    connect(camera, &Camera::viewAngleChanged, viewAngleSpinBox, &QSpinBox::setValue);

    camera->updateSignal();


    QFormLayout *mainLayout = new QFormLayout;
    mainLayout->addRow(new QLabel(tr("horizontal angle:")), xRotationSpinBox);
    mainLayout->addRow(xRotationSlider);

    mainLayout->addRow(new QLabel(tr("vertical angle:")), yRotationSpinBox);
    mainLayout->addRow(yRotationSlider);

    mainLayout->addRow(new QLabel(tr("distance:")), distanceSpinBox);
    mainLayout->addRow(distanceSlider);

    mainLayout->addRow(new QLabel(tr("angle of view:")), viewAngleSpinBox);
    mainLayout->addRow(viewAngleSlider);

    setLayout(mainLayout);
}

