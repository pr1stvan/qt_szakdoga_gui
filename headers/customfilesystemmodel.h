#ifndef CUSTOMFILESYSTEMMODEL_H
#define CUSTOMFILESYSTEMMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QFileIconProvider>
#include <QStringList>
#include <QDir>
#include <QDebug>
#include <QDateTime>
#include <QtAlgorithms>
#include <QTreeView>

#include "treeitem.h"

class TreeItem;


//! [0]
class CustomFileSystemModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit CustomFileSystemModel(QObject *parent = 0);
    ~CustomFileSystemModel();

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

    void setup(QString directoryPath);
    QString getDirectoryPath();
    EntryType lastClickState();
    void setExtensions(QStringList extensions);

private:
    void setupModelData(const QStringList &lines, TreeItem *parent);

    QStringList extensions;
    QMap<QString,SortingType> sortingTypes;
    QString directoryPath;
    QStringList fileNames;
    TreeItem *rootItem;
    QFileIconProvider iconProvider;
    EntryType m_lastClickState;

};
//! [0]

#endif // CUSTOMFILESYSTEMMODEL_H
