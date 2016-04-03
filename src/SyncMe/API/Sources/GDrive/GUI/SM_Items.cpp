#include "SM_Items.h"
#include <QtAlgorithms>

SM_Items::SM_Items()
{
}

void SM_Items::setFileSize(const QString &size, int index)
{
    items[index].fileSize = size;
}

const QList<SM_Items::Data> &SM_Items::getItems() const
{
    return items;
}

void SM_Items::sort(QList<SM_Items::Data> &sortItems, Data::ESortOrder itemSortOrder, Qt::SortOrder sortOrder)
{
    setItemsSortOrder(sortItems, itemSortOrder);

    QList<SM_Items::Data> folderItems, fileItems;

    for(int i = 0; i < sortItems.count(); ++i)
    {
        if(sortItems[i].type == FOLDER_TYPE_STR) folderItems.push_back(sortItems[i]);
        if(sortItems[i].type == FILE_TYPE_STR) fileItems.push_back(sortItems[i]);
    }

    if(sortOrder == Qt::AscendingOrder)
    {
        qSort(folderItems.begin(), folderItems.end());
        qSort(fileItems.begin(), fileItems.end());
    }

    if(sortOrder == Qt::DescendingOrder)
    {
        qSort(folderItems.begin(), folderItems.end(), qGreater<SM_Items::Data>());
        qSort(fileItems.begin(), fileItems.end(), qGreater<SM_Items::Data>());
    }

    for(int i = 0; i < folderItems.count(); ++i)
    {
        sortItems[i] = folderItems[i];
    }

    for(int i = folderItems.count(), j = 0; i < sortItems.count(); ++i, ++j)
    {
        sortItems[i] = fileItems[j];
    }
}

void SM_Items::setItemsSortOrder(QList<SM_Items::Data> &sortItems, Data::ESortOrder itemSortOrder)
{
    for(int i = 0; i < sortItems.count(); ++i)
    {
        sortItems[i].setSortOrder(itemSortOrder);
    }
}

void SM_Items::pushBack(SM_Items::Data &data)
{
    items.push_back(data);
}

void SM_Items::setAccountOwner(const QString &name)
{
    accountOwner = name;
}

QString SM_Items::getAccountOwner(void) const
{
    return accountOwner;
}

void SM_Items::setDataOwner(const QString &name, int index)
{
    items[index].owner = name;
}