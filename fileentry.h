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
    FileEntry(QString name, QString type, int size, QDateTime dateModified, EntryType entryType);
    bool isValid(){
        return valid;
    }

    QString name() const{
        return m_name;
    }
    QString type() const{
        return m_type;
    }
    QDateTime dateModified() const{
        return m_dateModified;
    }

    void setSortingType(SortingType type);

    bool operator<(const FileEntry& other)const;

    int size();
    EntryType entryType();
    SortingType sortingType();


private:
    SortingType m_sortingType;
    bool valid;

    QString m_name,m_type;
    int m_size;
    QDateTime m_dateModified;
    EntryType m_entryType;
};

#endif // FILEENTRY_H
