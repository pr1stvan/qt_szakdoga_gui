#ifndef MATERIALCOLORINGCOMBOBOX_H
#define MATERIALCOLORINGCOMBOBOX_H

#include <QtGui>
#include <QComboBox>
#include <QDebug>
#include "functions.h"

class MaterialColoringComboBox : public QComboBox
{
    Q_OBJECT
public:
    MaterialColoringComboBox(QWidget *parent);
    ColorMode getColorMode();

signals:
    void colorModeChanged(ColorMode mode);

public slots:
    void setColorMode(ColorMode mode);

    void changeColorMode(int idx);
    void setAvailableColorModes(bool solid, bool velocity, bool area);
private:

    ColorMode colorMode;
};

#endif // MATERIALCOLORINGCOMBOBOX_H
