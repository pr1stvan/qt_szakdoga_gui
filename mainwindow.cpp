#include "mainwindow.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    frameIdxSpinBox = new QSpinBox(this);
    fpsSpinBox = new QSpinBox(this);


    frameIdxSpinBox->setEnabled(false);
    fpsSpinBox->setEnabled(false);

    frameIdxSpinBox->setSingleStep(1);

    fpsSpinBox->setSingleStep(1);

    fpsSpinBox->setMinimum(1);
    fpsSpinBox->setMaximum(1000);
    fpsSpinBox->setValue(6000);

    QLabel *frameLabel = new QLabel(this);
    QLabel *speedLabel = new QLabel(this);

    frameLabel->setText(QString("Frame: "));
    speedLabel->setText(QString("Fps: "));
    ui->toolBar_3->addWidget(frameLabel);
    ui->toolBar_3->addWidget(frameIdxSpinBox);

    ui->toolBar_3->addSeparator();

    ui->toolBar_3->addWidget(speedLabel);
    ui->toolBar_3->addWidget(fpsSpinBox);

    connect(frameIdxSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),  ui->glWidget, &GLWidget::setFrameIdx);
    connect(ui->glWidget, &GLWidget::frameIdxChanged, frameIdxSpinBox,&QSpinBox::setValue);

    connect(fpsSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),  ui->glWidget, &GLWidget::setFps);


    tabifyDockWidget( ui->lightsDock,ui->cameraDock);

    ui->menuView->addAction(ui->cameraDock->toggleViewAction());
    ui->menuView->addAction(ui->lightsDock->toggleViewAction());
    ui->menuView->addAction(ui->materialDock->toggleViewAction());


    ui->solidColorWidget->setVisible(true);
    ui->startEndColorWidget->setVisible(false);

    ui->glWidget->updateSignal();
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("About this project"),
                       tr("It's an OpenGL sample application."));
}

void MainWindow::on_actionOpen_triggered()
{
//    QString directoryName =
//        QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this, tr("Directory"), QDir::rootPath()));

       QString directoryName =
            QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this, tr("Directory"),QString("C:/Users/THe_KinG/Desktop/")));

    int framesCount = ui->glWidget->loadFrameDirectory(directoryName);

    if(framesCount){
        frameIdxSpinBox->setEnabled(true);
        frameIdxSpinBox->setMinimum(0);
        frameIdxSpinBox->setMaximum(framesCount-1);
        frameIdxSpinBox->setValue(0);

        fpsSpinBox->setEnabled(true);
        fpsSpinBox->setValue(60);
    }
    else{
        frameIdxSpinBox->setEnabled(false);
        frameIdxSpinBox->setMinimum(0);
        frameIdxSpinBox->setValue(0);

        fpsSpinBox->setEnabled(false);
        fpsSpinBox->setValue(60);
    }

}

void MainWindow::on_actionplay_triggered()
{
    ui->glWidget->play();
}

void MainWindow::on_actionstop_triggered()
{
    ui->glWidget->stop();
}

void MainWindow::on_actionpause_triggered()
{
    ui->glWidget->pause();

}

void MainWindow::on_actionnext_triggered()
{
   ui->glWidget->nextFrame();
}

void MainWindow::on_actionprev_triggered()
{
    ui->glWidget->prevFrame();
}



void MainWindow::on_actionreplay_triggered()
{
    ui->glWidget->setAutoReplay(ui->actionreplay->isChecked());
}


void MainWindow::on_materialColoringComboBox_colorModeChanged(ColorMode mode)
{
    if(mode == SOLID){
        ui->solidColorWidget->setVisible(true);
        ui->startEndColorWidget->setVisible(false);
    }
    else{
         ui->solidColorWidget->setVisible(false);
         ui->startEndColorWidget->setVisible(true);
    }
    ui->glWidget->setColorMode(mode);
}

void MainWindow::on_glWidget_colorModeChanged(ColorMode mode)
{
    if(mode == SOLID){
        ui->solidColorWidget->setVisible(true);
        ui->startEndColorWidget->setVisible(false);
    }
    else{
         ui->solidColorWidget->setVisible(false);
         ui->startEndColorWidget->setVisible(true);
    }

    ui->materialColoringComboBox->setColorMode(mode);
}
