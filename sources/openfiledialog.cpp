#include "openfiledialog.h"
#include "ui_openfiledialog.h"

QStringList OpenFileDialog::getFiles(QStringList extensions,QString startDirectory)
{
    OpenFileDialog openFileDialog(extensions,startDirectory);
    openFileDialog.setModal(true);
    if(openFileDialog.exec())
    {
        return openFileDialog.selectedFiles();
    }

    return QStringList();
}

OpenFileDialog::OpenFileDialog(QStringList extensions,QString directoryPath,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenFileDialog)
{
    ui->setupUi(this);

    pathStackIdx=-1;
    treeViewModel=new CustomFileSystemModel(this);
    treeViewModel->setExtensions(extensions);

    QFileInfo file(directoryPath);
    if(file.exists())
    {
        treeViewModel->setup(directoryPath);
        setupCombobox(directoryPath);
        addPathToStack(directoryPath);
    }
    else
    {
        treeViewModel->setup(QDir::rootPath());
        setupCombobox(QDir::rootPath());
        addPathToStack(QDir::rootPath());
    }

    QFileInfoList drives=QDir::drives();
    QFileIconProvider iconProvider;

    QStringList directoryList=QStandardPaths::standardLocations(QStandardPaths::HomeLocation) +
            QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)+
            QStandardPaths::standardLocations(QStandardPaths::DesktopLocation) +
            QStandardPaths::standardLocations(QStandardPaths::DownloadLocation);

    for(const QString & dir : directoryList)
    {
        QString displayedName=dir.split('/').last();

        QListWidgetItem *newItem = new QListWidgetItem(iconProvider.icon(QFileIconProvider::Folder),
                                                       QDir::toNativeSeparators(displayedName));
        newItem->setData(Qt::UserRole, dir + QString("/"));

        ui->listWidget->addItem(newItem);
    }

    for(const QFileInfo & dir : drives)
    {
         QString displayedName=dir.absoluteFilePath();

         QListWidgetItem *newItem = new QListWidgetItem(iconProvider.icon(QFileIconProvider::Drive),
                                                        QDir::toNativeSeparators(displayedName));
         QVariant fullFilePathData(dir.absoluteFilePath());
         newItem->setData(Qt::UserRole, fullFilePathData);

         ui->listWidget->addItem(newItem);
    }

//    QStringList list=QStandardPaths::standardLocations(QStandardPaths::GenericCacheLocation);


    ui->treeView->setModel(treeViewModel);
    ui->treeView->setSortingEnabled(true);
    ui->treeView->sortByColumn(0, Qt::AscendingOrder);
//    ui->treeView->resizeColumnToContents(0);

//    ui->toolButton->setDefaultAction(ui->prevAction);
    ui->prevButton->setDefaultAction(ui->actionPrev);
    ui->nextButton->setDefaultAction(ui->actionNext);
    ui->upDirectoryButton->setDefaultAction(ui->actionUpDirectory);


    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int h = screenGeometry.height();
    int w = screenGeometry.width();

    this->resize(w/3,h/3);

    QItemSelectionModel *selectionModel = ui->treeView->selectionModel();

    if(!QObject::connect(selectionModel, &QItemSelectionModel::currentChanged,
                          this, &OpenFileDialog::mySelectionChanged))
    {
        qDebug() << "can't connect slot";
    }
}

OpenFileDialog::~OpenFileDialog()
{
    delete ui;
}

void OpenFileDialog::addPathToStack(QString path)
{
    while(pathStackIdx < pathStack.size() -1)
    {
        pathStack.removeLast();
    }

    if(pathStack.size() == 8)
    {
       pathStack.removeFirst();
    }
    else
    {
        pathStackIdx++;
    }
    pathStack << path;

    if( pathStackIdx > 0)
    {
        ui->actionPrev->setEnabled(true);
    }
    ui->actionNext->setEnabled(false);
}

void OpenFileDialog::setupCombobox(QString path)
{
    ui->pathComboBox->clear();
    QStringList pathFragments=path.split("/");

    QString pathString("");
    for(int i=0; i<pathFragments.size()-1; i++)
    {
        pathString=pathString + pathFragments.at(i) + QString("/");
        ui->pathComboBox->addItem(pathString);
    }
    ui->pathComboBox->setCurrentIndex(pathFragments.size()-2);

    if(path.count('/')==1)
    {
        ui->actionUpDirectory->setEnabled(false);
    }
    else
    {
        ui->actionUpDirectory->setEnabled(true);
    }
}

void OpenFileDialog::openDirectory(QString directoryPath)
{
    treeViewModel->setup(directoryPath);
    setupCombobox(directoryPath);
    addPathToStack(directoryPath);
}

void OpenFileDialog::on_okButton_clicked()
{
    QItemSelectionModel *selectionModel = ui->treeView->selectionModel();
    QModelIndexList selectedIndexes=selectionModel->selectedIndexes();

    if(selectedIndexes.isEmpty())
    {
        return;
    }
    QModelIndex index = selectedIndexes.first();

    if (!index.isValid())
    {
         qDebug() << "ok button clicked";
         qDebug() << "not valid index";
         return;
    }

    TreeItem *item=static_cast<TreeItem*>(index.internalPointer());
    FileEntry entry=item->getFile();

    if(!entry.isValid())
    {
         qDebug() << "not valid file";
         return;
    }

    m_selectedFiles=QStringList();
    switch(entry.entryType()){
    case EntryType::file:
    {
        m_selectedFiles << entry.path();
        if(!m_selectedFiles.isEmpty())accept();
        break;
    }
    case EntryType::glob:
    {
        QList<TreeItem*> childItems=item->getChildItems();
        for(int i=0; i<childItems.size(); i++)
        {
            m_selectedFiles << childItems.at(i)->getFile().path();
        }
        if(!m_selectedFiles.isEmpty())accept();
        break;
    }
    case EntryType::directory:
    {
        QString directoryPath=entry.path() + QChar('/');
        openDirectory(directoryPath);
        break;
    }
    default:
        break;
    }
}

void OpenFileDialog::on_pathComboBox_activated(const QString &directoryPath)
{
    treeViewModel->setup(directoryPath);
    setupCombobox(directoryPath);
    addPathToStack(directoryPath);
}

void OpenFileDialog::on_actionUpDirectory_triggered()
{
    QString path=treeViewModel->getDirectoryPath();

    if(path.count('/') > 1)
    {
        QString directoryPath;
        QStringList fragments=path.split('/');
        for(int i =0; i<fragments.size() -2;i++)
        {
            directoryPath=directoryPath+fragments.at(i) + '/';
        }
        treeViewModel->setup(directoryPath);
        setupCombobox(directoryPath);

        addPathToStack(directoryPath);
    }
}


void OpenFileDialog::on_actionPrev_triggered()
{
    if(pathStackIdx-1 >= 0)
    {
        pathStackIdx--;
        QString directoryPath=pathStack.at(pathStackIdx);

        treeViewModel->setup(directoryPath);
        setupCombobox(directoryPath);

        if(pathStackIdx == 0)ui->actionPrev->setEnabled(false);
        ui->actionNext->setEnabled(true);
    }
}

void OpenFileDialog::on_actionNext_triggered()
{
    if(pathStackIdx+1 < pathStack.size())
    {
        pathStackIdx++;

        QString directoryPath=pathStack.at(pathStackIdx);

        treeViewModel->setup(directoryPath);
        setupCombobox(directoryPath);

        if(pathStackIdx == pathStack.size() -1)ui->actionNext->setEnabled(false);
        ui->actionPrev->setEnabled(true);

    }
}

void OpenFileDialog::on_listWidget_itemActivated(QListWidgetItem *item)
{
    if (item != nullptr)
    {
         QVariant data = item->data(Qt::UserRole);
         QString directoryPath = data.toString();

         treeViewModel->setup(directoryPath);
         setupCombobox(directoryPath);
         addPathToStack(directoryPath);
    }
}

void OpenFileDialog::on_cancelButton_clicked()
{
    this->reject();
}

QStringList OpenFileDialog::selectedFiles()
{
    return m_selectedFiles;
}

void OpenFileDialog::on_textBox1_textEdited(const QString &text)
{
    ui->treeView->clearSelection();
    QModelIndexList list=treeViewModel->match(treeViewModel->index(0, 0),
                         Qt::DisplayRole,
                         QVariant::fromValue(text), -1,
                         Qt::MatchExactly);

    QItemSelectionModel *selectionModel = ui->treeView->selectionModel();
    for(int i=0; i<list.size(); i++)
    {
        QModelIndex index=list.at(i);

        selectionModel->select(index, QItemSelectionModel::Select | QItemSelectionModel::Rows);
    }

}

void OpenFileDialog::on_treeView_doubleClicked(const QModelIndex &index)
{
        if (!index.isValid())
        {
            qDebug() << "not valid index";
            return;
        }

        TreeItem *item=static_cast<TreeItem*>(index.internalPointer());
        FileEntry entry=item->getFile();

        if(!entry.isValid())
        {
             qDebug() << "not valid file";
             return;
        }

        m_selectedFiles=QStringList();
        switch(entry.entryType()){
        case EntryType::file:
            m_selectedFiles << treeViewModel->getDirectoryPath()+entry.name();
            if(!m_selectedFiles.isEmpty())accept();
            break;
        case EntryType::glob:
        {
            QList<TreeItem*> childItems=item->getChildItems();
            for(int i=0; i<childItems.size(); i++)
            {
                m_selectedFiles <<  treeViewModel->getDirectoryPath()+childItems.at(i)->getFile().name();
            }
            if(!m_selectedFiles.isEmpty())accept();
            break;
        }
        case EntryType::directory:
        {
            QString directoryPath=treeViewModel->getDirectoryPath()+entry.name()+QChar('/');
            openDirectory(directoryPath);
            break;
        }
        default:
            break;
        }
}

void OpenFileDialog::mySelectionChanged(const QModelIndex &index, const QModelIndex &)
{
    TreeItem * item=static_cast<TreeItem*>(index.internalPointer());
    FileEntry entry=item->getFile();
    ui->textBox1->setText(entry.name());
}

