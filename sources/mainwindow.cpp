#include "mainwindow.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "openfiledialog.h"


#include <QMessageBox>
#include <QFileDialog>
#include <QGuiApplication>
#include <QScreen>


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
    ui->frameCountToolBar->addWidget(frameLabel);
    ui->frameCountToolBar->addWidget(frameIdxSpinBox);

    ui->frameCountToolBar->addSeparator();

    ui->frameCountToolBar->addWidget(speedLabel);
    ui->frameCountToolBar->addWidget(fpsSpinBox);

    connect(frameIdxSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),  ui->glWidget, &GLWidget::setFrameIdx);
    connect(ui->glWidget, &GLWidget::frameIdxChanged, frameIdxSpinBox,&QSpinBox::setValue);

    connect(fpsSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),  ui->glWidget, &GLWidget::setFps);


    tabifyDockWidget( ui->lightsDock,ui->cameraDock);

    ui->menuView->addAction(ui->cameraDock->toggleViewAction());
    ui->menuView->addAction(ui->lightsDock->toggleViewAction());
    ui->menuView->addAction(ui->materialDock->toggleViewAction());
    ui->menuView->addSeparator();
    ui->menuView->addAction(ui->loadFileToolbar->toggleViewAction());
    ui->menuView->addAction(ui->frameCountToolBar->toggleViewAction());
    ui->menuView->addAction(ui->mediControlToolBar->toggleViewAction());

    ui->solidColorButton->setVisible(true);
    ui->solidColorLabel->setVisible(true);
    ui->startColorLabel->setVisible(false);
    ui->startColorButton->setVisible(false);
    ui->endColorButton->setVisible(false);
    ui->endColorLabel->setVisible(false);

    ui->materialColoringComboBox->setAvailableColorModes(true,
                                                         ui->glWidget->velocityDataExist(),
                                                         ui->glWidget->areaDataExist()
                                                         );
    ui->glWidget->updateSignal();

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    int h = screenGeometry.height();
    int w = screenGeometry.width();

    this->resize(5*w/8,h/2);
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

QString getDesktopLocation(){
    QString desktopPath= QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).first();

    if(desktopPath.at(desktopPath.size() -1) != QChar('/')){
        return desktopPath + QChar('/');
    }
    return desktopPath;
}
void MainWindow::on_actionOpen_triggered()
{
//    OpenFilesDialog fileDialog(0,Qt::WindowCloseButtonHint | Qt::WindowSystemMenuHint | Qt::WindowTitleHint);
//    fileDialog.setModal(false);

//    int res=fileDialog.exec();
//    if(res== QDialog::Accepted){
//        QString name=fileDialog.getDirectoryName();
//        qDebug() << name.toLatin1().data();
//    }
//    else{
//        qDebug() << "rejected";
//    }
    //fileDialog.exec();
//    QString directoryName =
//        QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this, tr("Directory"), QDir::rootPath()));

    QStringList extensions;
    extensions << QString("*.vtp");

    QStringList filePaths=OpenFileDialog::getFiles(extensions,getDesktopLocation());


//    QString directoryName =
//            QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this, tr("Directory"),QString("C:/Users/THe_KinG/Desktop/000"),
//                                                                       QFileDialog::ShowDirsOnly|
//                                                                       QFileDialog::DontResolveSymlinks |
//                                                                       QFileDialog::DontUseNativeDialog));
    if(!filePaths.isEmpty()){
        int framesCount = ui->glWidget->loadFrameDirectory(filePaths);

        ui->materialColoringComboBox->setCurrentIndex(0);

        ui->materialColoringComboBox->setAvailableColorModes(true,
                                                             ui->glWidget->velocityDataExist(),
                                                             ui->glWidget->areaDataExist()
                                                             );
        ui->materialColoringComboBox->setCurrentIndex(0);


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
        ui->solidColorButton->setVisible(true);
        ui->solidColorLabel->setVisible(true);
        ui->startColorLabel->setVisible(false);
        ui->startColorButton->setVisible(false);
        ui->endColorButton->setVisible(false);
        ui->endColorLabel->setVisible(false);

    }
    else{
        ui->solidColorButton->setVisible(false);
        ui->solidColorLabel->setVisible(false);
        ui->startColorLabel->setVisible(true);
        ui->startColorButton->setVisible(true);
        ui->endColorButton->setVisible(true);
        ui->endColorLabel->setVisible(true);

    }
    ui->glWidget->setColorMode(mode);
}

void MainWindow::on_glWidget_colorModeChanged(ColorMode mode)
{
    if(mode == SOLID){
        ui->solidColorButton->setVisible(true);
        ui->solidColorLabel->setVisible(true);
        ui->startColorLabel->setVisible(false);
        ui->startColorButton->setVisible(false);
        ui->endColorButton->setVisible(false);
        ui->endColorLabel->setVisible(false);
    }
    else{
        ui->solidColorButton->setVisible(false);
        ui->solidColorLabel->setVisible(false);
        ui->startColorLabel->setVisible(true);
        ui->startColorButton->setVisible(true);
        ui->endColorButton->setVisible(true);
        ui->endColorLabel->setVisible(true);
    }

    ui->materialColoringComboBox->setColorMode(mode);
}
