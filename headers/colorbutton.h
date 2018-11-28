//Source:
//https://stackoverflow.com/questions/282938/show-result-of-color-selection-in-qt?utm_medium=organic&utm_source=google_rich_qa&utm_campaign=google_rich_qa

#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <QtGui>
#include <QPushButton>
#include <QColorDialog>

class ColorButton : public QPushButton
{
    Q_OBJECT

private:
    QColor currentColor;

public slots:
    void setColor(const QColor);
    void chooseColor();
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

signals:
    void colorChanged(QColor);

public:
    explicit ColorButton(QWidget *parent = 0,const QColor & color = Qt::yellow);
    QColor getColor();

};

#endif // COLORBUTTON_H
