#include <QtMath>
#include "camera.h"

int normalizeAngle(int angle)
{
    int angle_normalized= angle % 360;
    while (angle_normalized < 0)
    {
        angle_normalized += 360;
    }

    return angle_normalized;
}

Camera::Camera()
{
    canvasWidth=640;
    canvasHeight=480;

    distanceVelocity=1;

    setDistance(30);
    setViewAngle(60);
}

QMatrix4x4 Camera::V()
{
    QMatrix4x4 vMatrix;
    vMatrix.setToIdentity();
    vMatrix.lookAt(position,lookAtPoint,up);

    return vMatrix;
}

QMatrix4x4 Camera::P()
{
    QMatrix4x4 pMatrix;

    pMatrix.setToIdentity();
    pMatrix.perspective(viewAngle,(float)canvasWidth/(float)canvasHeight,0.1f,1000);

    return pMatrix;
}

void Camera::setPosition(QVector3D pos)
{
    float cosalphaX=QVector3D::dotProduct(QVector3D(pos.x(),0,pos.z()).normalized(),QVector3D(0,0,1));
    float cosalphaY=QVector3D::dotProduct(pos.normalized(),QVector3D(0,1,0));

    setHorizontalAngleDegrees(nearbyint(qRadiansToDegrees(acos(cosalphaX))));
    setVerticalAngleDegrees(nearbyint(qRadiansToDegrees(acos(cosalphaY))));
}

void Camera::setLookAtPoint(QVector3D lookAtPoint)
{
    this -> lookAtPoint=lookAtPoint;
    rotateToNewPosition();
}

void Camera::rotateHorizontal(int dh_degrees)
{
    float d = dh_degrees/2.0f;
    setHorizontalAngleDegrees(horizontalAngle_degrees + (int)roundf(d));

}

void Camera::rotateVertical(int dv_degrees)
{
    float d = dv_degrees/2.0f;

    setVerticalAngleDegrees(verticalAngle_degrees - (int)roundf(d));
}

void Camera::increaseDistance(int dDistance)
{
    setDistance(distance+distanceVelocity*dDistance);
}

void Camera::rotateToNewPosition()
{
    QMatrix4x4 rotationMatrix;

    rotationMatrix.setToIdentity();
    rotationMatrix.rotate((float)horizontalAngle_degrees,QVector3D(0,1,0));
    position=rotationMatrix*QVector3D(0,0,(float)distance);
    up=QVector3D(0,1,0);
    right=rotationMatrix*QVector3D(1,0,0);

    rotationMatrix.setToIdentity();
    rotationMatrix.rotate((float)-verticalAngle_degrees,right);
    position = rotationMatrix*position;
    up=rotationMatrix*up;

    position += lookAtPoint;
}

void Camera::setHorizontalAngleDegrees(int degrees)
{
    if(degrees!=horizontalAngle_degrees)
    {
       horizontalAngle_degrees=normalizeAngle(degrees);
       rotateToNewPosition();
       emit horizontalAngleChanged(horizontalAngle_degrees);
    }

}

void Camera::setVerticalAngleDegrees(int degrees)
{
    if(degrees!=verticalAngle_degrees)
    {
        verticalAngle_degrees = normalizeAngle(degrees);
        rotateToNewPosition();
        emit verticalAngleChanged(verticalAngle_degrees);

    }
}

void Camera::setCanvas(int width,int height)
{
    canvasWidth=width;
    canvasHeight=height;
}

void Camera::setDistance(int dist)
{
    if(distance!=dist)
    {
        if(dist < 15)
        {
            distance=15;
            emit distanceChanged(distance);
        }
        else if(dist > 100)
        {
            distance = 100;
            emit distanceChanged(distance);
        }
        else
        {
            distance=dist;
            emit distanceChanged(distance);
        }
        rotateToNewPosition();
    }
}

void Camera::setViewAngle(int degrees)
{
    if(degrees < 20 || degrees > 179 )
    {
        emit viewAngleChanged(viewAngle);
    }
    viewAngle = degrees;
    emit viewAngleChanged(viewAngle);
}

void Camera::updateSignal()
{
    emit horizontalAngleChanged(horizontalAngle_degrees);
    emit verticalAngleChanged(verticalAngle_degrees);
    emit distanceChanged(distance);
    emit viewAngleChanged(viewAngle);
}


