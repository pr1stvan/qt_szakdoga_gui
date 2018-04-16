#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSpinBox>
#include "functions.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionQuit_triggered();

    void on_actionAbout_triggered();


    void on_actionOpen_triggered();

    void on_actionplay_triggered();

    void on_actionstop_triggered();

    void on_actionpause_triggered();

    void on_actionnext_triggered();

    void on_actionprev_triggered();



    void on_actionreplay_triggered();


    void on_materialColoringComboBox_colorModeChanged(ColorMode mode);

    void on_glWidget_colorModeChanged(ColorMode mode);

private:
    Ui::MainWindow *ui;
    QSpinBox *frameIdxSpinBox;
    QSpinBox *fpsSpinBox;
};

#endif // MAINWINDOW_H
