#ifndef MATERIALCOLORINGCOMBOBOX_H
#define MATERIALCOLORINGCOMBOBOX_H

#include <QtGui>
#include <QComboBox>
#include <QDebug>
#include "functions.h"

class MaterialColoringComboBox : public QComboBox
{
    Q_OBJECT
private:
    ColorMode colorMode;

public slots:
    void setColorMode(ColorMode mode);
    void changeColorMode(int idx);

signals:
    void colorModeChanged(ColorMode mode);

public:
    MaterialColoringComboBox(QWidget *parent);
    ColorMode getColorMode();
    void setAvailableColorModes(bool solid, bool velocity, bool area);
};

#endif // MATERIALCOLORINGCOMBOBOX_H
