#include "fileentry.h"

FileEntry::FileEntry()
{
    valid=false;
}

FileEntry::FileEntry(QString name, QString type, int size, QDateTime dateModified, EntryType entryType, QString path)
{
    m_sortingType=SortingType::name;
    m_name=name;
    m_type=type;
    m_size=size;
    m_dateModified=dateModified;
    m_entryType=entryType;

    m_path = path;
    valid=true;
}

void FileEntry::setSortingType(SortingType type)
{
    m_sortingType=type;
}

bool FileEntry::operator<(const FileEntry &other)const
{
    switch(m_sortingType) {
    case SortingType::name:
        return m_name < other.m_name;
        break;
    case SortingType::type:
        if(m_type == other.m_type){
            return m_name < other.m_name;
        }
        return m_type < other.m_type;
        break;
    case SortingType::size:
        if(m_size == other.m_size){
            return m_name < other.m_name;
        }
        return m_size < other.m_size;
        break;
    case SortingType::dateModified:
        if(m_dateModified == other.m_dateModified){
            return m_name < other.m_name;
        }
        return m_dateModified < other.m_dateModified;
        break;
    }
    return m_name < other.m_name;
}

int FileEntry::size(){
    return m_size;
}

EntryType FileEntry::entryType(){
    return m_entryType;
}

SortingType FileEntry::sortingType()
{
    return m_sortingType;
}

QString FileEntry::path()
{
    return m_path;
}
