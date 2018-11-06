#include <algorithm>
#include "customfilesystemmodel.h"


CustomFileSystemModel::CustomFileSystemModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    m_lastClickState=none;
    QList<QVariant> rootData;
    rootData << "Name" << "Type" << "Size" << "Date modified";
    rootItem = new TreeItem(rootData,FileEntry());

    sortingTypes[QString("Name")]=SortingType::name;
    sortingTypes[QString("Type")]=SortingType::type;
    sortingTypes[QString("Size")]=SortingType::size;
    sortingTypes[QString("Date modified")]=SortingType::dateModified;

    extensions=QStringList();
}

CustomFileSystemModel::~CustomFileSystemModel()
{
    delete rootItem;
}

int CustomFileSystemModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

void CustomFileSystemModel::setup(QString directoryPath)
{
    if(m_lastClickState==directory)m_lastClickState=none;
    delete rootItem;

    QList<QVariant> rootData;
    rootData << "Name" << "Type" << "Size" << "Date modified";
    rootItem = new TreeItem(rootData,FileEntry());

    this->fileNames=QStringList();
    this->directoryPath=directoryPath;

    QDir dir(directoryPath);

    //directories
    QFileInfoList directoryInfoList =dir.entryInfoList(QStringList(), QDir::NoDotAndDotDot | QDir::Dirs | QDir::NoSymLinks,QDir::Name);

    for(int i=0; i<directoryInfoList.size(); i++){
        QFileInfo directoryInfo=directoryInfoList.at(i);

        FileEntry directory(directoryInfo.fileName(),
                            QString("File folder"),
                            directoryInfo.size(),
                            directoryInfo.lastModified(),
                            EntryType::directory,
                            directoryInfo.filePath());

        QList<QVariant> list;
        list << directory.name() << directory.type() << QString("") << directory.dateModified();
        rootItem->appendChild(new TreeItem(list,directory,rootItem));
    }

    //files
    QMap<QString, QVector<FileEntry>> fileGlobHash;

    QFileInfoList fileInfoList =dir.entryInfoList(extensions, QDir::Files | QDir::NoSymLinks);

    for(int i=0; i<fileInfoList.size(); i++){
        QFileInfo fileInfo=fileInfoList.at(i);

        QString extension=fileInfo.fileName();
        extension=extension.split(".").last();
        FileEntry file=FileEntry(fileInfo.fileName(),
                                 extension + QString(" file"),
                                 fileInfo.size(),
                                 fileInfo.lastModified(),
                                 EntryType::file,
                                 fileInfo.filePath());

        //Has number at the end, e.g out_001.vtp
        QRegExp rx(".*_[0-9]*\\..*$");

        if(rx.exactMatch(file.name())){
            //remove numbers from the end
            QStringList list1 = file.name().split('_');
            list1.pop_back();
            QString fileNameWithoutEndNumbers=list1.join("_");

            QStringList list2 = file.name().split('.');
            QString extension=list2.last();

            fileNameWithoutEndNumbers=fileNameWithoutEndNumbers +
                    QChar('_')+
                    QString("**")+
                    QChar('.')+
                    extension;

            fileGlobHash[fileNameWithoutEndNumbers].push_back(file);
        }
        else{
            fileGlobHash[file.name()].push_back(file);
        }
    }

    QMapIterator<QString, QVector<FileEntry>> it(fileGlobHash);
    while (it.hasNext())
    {
        it.next();

        QVector<FileEntry> files=it.value();

        if(files.size() > 1)
        {
            qSort(files);

            QList<QVariant> list;
            list << it.key() << QString("Group") << QString("") << QDateTime();
            TreeItem *fileGlobItem=new TreeItem(list,FileEntry(it.key(),
                                                              QString("Group"),
                                                              0,
                                                              QDateTime(),
                                                              EntryType::glob,
                                                               QString()),rootItem);
            for(int i=0; i< files.size(); i++)
            {
                FileEntry file=files.at(i);
                QString sizeNumberString=QString::number(file.size()/1000.0, 'f', 1) + QString(" KB");
                list=QList<QVariant>();

                list << file.name() << file.type() << sizeNumberString << file.dateModified();
                fileGlobItem->appendChild(new TreeItem(list,file,fileGlobItem));
            }
            rootItem->appendChild(fileGlobItem);

        }
        else if(files.size() == 1)
        {
            FileEntry file=files.at(0);
            QList<QVariant> list;
            QString sizeNumberString=QString::number(file.size()/1000.0, 'f', 1) + QString(" KB");
            list << file.name() << file.type() << sizeNumberString << file.dateModified();

            rootItem->appendChild(new TreeItem(list,file,rootItem));
        }
        else
        {
            qDebug() << "!!!!No files!!!!!";
        }
    }
    endResetModel();
}

void CustomFileSystemModel::sort(int column, Qt::SortOrder order)
{
    QString label= rootItem->data(column).toString();
    SortingType stype=sortingTypes[label];

    rootItem->sort(stype,order);

    endResetModel();
}

QString CustomFileSystemModel::getDirectoryPath()
{
    return directoryPath;
}

void CustomFileSystemModel::setExtensions(QStringList extensions)
{
    this->extensions=extensions;
}

QVariant CustomFileSystemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    if (role == Qt::DisplayRole){
        return item->data(index.column());
    }
    else if ( role == Qt::DecorationRole && index.column()==0) {
        FileEntry file=item->getFile();
        if(file.entryType() == EntryType::directory){
            return iconProvider.icon(QFileIconProvider::Folder);
        }
        return iconProvider.icon(QFileIconProvider::File);

    }
    return QVariant();
}

Qt::ItemFlags CustomFileSystemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant CustomFileSystemModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex CustomFileSystemModel::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex CustomFileSystemModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int CustomFileSystemModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}
