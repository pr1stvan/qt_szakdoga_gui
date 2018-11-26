#ifndef FILEENTRY_H
#define FILEENTRY_H

#include <QString>
#include <QDateTime>
#include <QDebug>

enum EntryType
{
   none,
   directory,
   file,
   glob
};

enum SortingType
{
    name,
    type,
    size,
    dateModified
};

class FileEntry
{
public:
    FileEntry();
    FileEntry(QString name, QString type, int size, QDateTime dateModified, EntryType entryType, QString path);

    bool isValid();
    QString name()const;
    QString type()const;
    QDateTime dateModified()const;

    void setSortingType(SortingType type);

    bool operator<(const FileEntry& other)const;

    int size();
    EntryType entryType();
    SortingType sortingType();

    QString path();
private:
    SortingType m_sortingType;
    bool valid;

    QString m_name;
    QString m_type;
    int m_size;
    QDateTime m_dateModified;
    EntryType m_entryType;

    QString m_path;
};

#endif // FILEENTRY_H
