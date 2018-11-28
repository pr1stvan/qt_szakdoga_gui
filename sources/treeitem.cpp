/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include <QStringList>
#include "treeitem.h"

TreeItem::TreeItem(const QList<QVariant> &data,FileEntry file, TreeItem *parent)
{
    m_parentItem = parent;
    m_itemData = data;
    m_file=file;
}

TreeItem::~TreeItem()
{
    qDeleteAll(m_childItems);
}

void TreeItem::appendChild(TreeItem *item)
{
    m_childItems.append(item);
}

TreeItem *TreeItem::child(int row)
{
    return m_childItems.value(row);
}

int TreeItem::childCount() const
{
    return m_childItems.count();
}

int TreeItem::columnCount() const
{
    return m_itemData.count();
}

QVariant TreeItem::data(int column) const
{
    return m_itemData.value(column);
}

TreeItem *TreeItem::parentItem()
{
    return m_parentItem;
}

bool TreeItem::operator<(const TreeItem &other) const
{
    return m_file < other.m_file;
}

void TreeItem::setSortingType(SortingType type)
{
    m_file.setSortingType(type);
}

void TreeItem::setChildsSortingType(SortingType type)
{
    for(int i=0; i<m_childItems.size(); i++)
    {
        m_childItems[i]->setSortingType(type);
    }
}

template <typename T> bool PComp(const T * const & a, const T * const & b)
{
   return *a < *b;
}

template <typename T> bool PComp_reverse(const T * const & a, const T * const & b)
{
   return *b < *a;
}

typedef bool (*fvptr)(const TreeItem* const & a,const TreeItem* const & b);

void TreeItem::sort(SortingType sortingType,Qt::SortOrder order)
{
    QList<TreeItem*> directories;
    QList<TreeItem*> files;

    for(int i=0; i< m_childItems.size(); i++)
    {
        FileEntry entry=m_childItems.at(i)->file();

        switch(entry.entryType())
        {
        case EntryType::directory:
            switch(sortingType)
            {
            case SortingType::name:
                m_childItems.at(i)->setSortingType(SortingType::name);
                break;
            case SortingType::type:
                m_childItems.at(i)->setSortingType(SortingType::name);
                break;
            case SortingType::size:
                m_childItems.at(i)->setSortingType(SortingType::name);
                break;
            case SortingType::dateModified:
                m_childItems.at(i)->setSortingType(SortingType::dateModified);
                break;
            }
            directories << m_childItems.at(i);
            break;
        case EntryType::file:
            m_childItems.at(i)->setSortingType(sortingType);
            files << m_childItems.at(i);
            break;
        case EntryType::glob:
            m_childItems.at(i)->setSortingType(sortingType);

            m_childItems.at(i)->sort(type,order);
            files << m_childItems.at(i);
            break;
        default:
            //Do nothing
            break;
        }
    }

    fvptr compareFunction=PComp<TreeItem>;

    if(order==Qt::DescendingOrder)
    {
        compareFunction=PComp_reverse<TreeItem>;
    }

    switch(sortingType)
    {
    case SortingType::name:
        qSort(directories.begin(),directories.end(),compareFunction);
        qSort(files.begin(),files.end(),compareFunction);
        break;
    case SortingType::type:
        qSort(directories.begin(),directories.end(),PComp<TreeItem>);
        qSort(files.begin(),files.end(),compareFunction);
        break;
    case SortingType::size:
        qSort(directories.begin(),directories.end(),PComp<TreeItem>);
        qSort(files.begin(),files.end(),compareFunction);
        break;
    case SortingType::dateModified:
        qSort(directories.begin(),directories.end(),compareFunction);
        qSort(files.begin(),files.end(),compareFunction);
        break;
    }
    m_childItems=directories + files;
}

FileEntry TreeItem::file()
{
    return m_file;
}

QList<TreeItem *> TreeItem::getChildItems()
{
    return m_childItems;
}

int TreeItem::row() const
{
    if (m_parentItem)
    {
        return m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this));
    }
    return 0;
}
