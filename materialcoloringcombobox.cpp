#include "materialcoloringcombobox.h"

MaterialColoringComboBox::MaterialColoringComboBox(QWidget *parent = Q_NULLPTR)
    :QComboBox(parent),
     colorMode(VERTEX_AREA)
{

    this->addItem(QString("solid"));
    this -> addItem(QString("velocity"));
    this -> addItem(QString("vertex area"));
    this -> addItem(QString("projected vertex area"));
    this -> addItem(QString("vertex mass"));

    this -> setCurrentIndex(0);



    connect(this, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this, &MaterialColoringComboBox::changeColorMode);
}

void MaterialColoringComboBox::setColorMode(ColorMode mode)
{
    colorMode=mode;



    switch(mode){
    case SOLID:
         this->setCurrentIndex(0);
        break;
    case VELOCITY:
         this->setCurrentIndex(1);
        break;
    case VERTEX_AREA:
         this->setCurrentIndex(2);
        break;
    case PROJECTED_VERTEX_AREA:
         this->setCurrentIndex(3);
        break;
    case VERTEX_MASS:
         this->setCurrentIndex(4);
        break;
    default:
        return;
    }
}

void MaterialColoringComboBox::changeColorMode(int idx)
{
    switch(idx){
    case 0:
        setColorMode(SOLID);
        break;
    case 1:
        setColorMode(VELOCITY);
        break;
    case 2:
        setColorMode(VERTEX_AREA);
        break;
    case 3:
        setColorMode(PROJECTED_VERTEX_AREA);
        break;
    case 4:
        setColorMode(VERTEX_MASS);
        break;
    default:
        return;
    }

    emit colorModeChanged(colorMode);
}

void MaterialColoringComboBox::setAvailableColorModes(bool solid, bool velocity, bool area)
{
    this->clear();

    if(solid)this->addItem(QString("solid"));
    if(velocity)this -> addItem(QString("velocity"));
    if(area){
        this -> addItem(QString("vertex area"));
        this -> addItem(QString("projected vertex area"));
        this -> addItem(QString("vertex mass"));
    }

    this -> setCurrentIndex(0);
}
