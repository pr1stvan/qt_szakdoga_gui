#ifndef OPENFILEDIALOG_H
#define OPENFILEDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QFileSystemModel>
#include <QDir>
#include "treeitem.h"
#include "customfilesystemmodel.h"
#include <QStandardPaths>
#include <QStorageInfo>
#include <QListWidgetItem>
#include <QToolBar>
#include <QMainWindow>
#include <QRect>
#include <QScreen>

namespace Ui {
class OpenFileDialog;
}

class OpenFileDialog : public QDialog
{
    Q_OBJECT

public:
    static QStringList getFiles(QStringList extensions, QString startDirectory);
    explicit OpenFileDialog(QStringList extensions, QString directoryPath, QWidget *parent = 0);
    ~OpenFileDialog();

private slots:
    void addPathToStack(QString path);

    void on_okButton_clicked();
    void on_pathComboBox_activated(const QString &directoryPath);
    void on_actionUpDirectory_triggered();
    void on_actionPrev_triggered();
    void on_actionNext_triggered();
    void on_listWidget_itemActivated(QListWidgetItem *item);
    void on_cancelButton_clicked();
    void on_textBox1_textEdited(const QString &text);
    void on_treeView_doubleClicked(const QModelIndex &index);

    void mySelectionChanged(const QModelIndex &index, const QModelIndex &previous);

private:
    QStringList selectedFiles();
    void setupCombobox(QString path);
    void openDirectory(QString directoryPath);

    Ui::OpenFileDialog *ui;

    CustomFileSystemModel *treeViewModel;
    QFileSystemModel *listViewModel;

    QStringList m_selectedFiles;
    QString m_directoryPath;
    //for the next and prev buttons
    QStringList pathStack;
    int pathStackIdx;

};

#endif // OPENFILEDIALOG_H
