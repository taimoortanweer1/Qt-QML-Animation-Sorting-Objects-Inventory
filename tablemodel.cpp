#include "tablemodel.h"

TableModel::TableModel(QObject *parent) : QAbstractTableModel(parent)
{

}

int TableModel::rowCount(const QModelIndex &) const
{
    return table.size();//Number of rows
}

int TableModel::columnCount(const QModelIndex &) const
{
    return table.at(0).size();//Columns
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case TableDataRole:
    {
        return  table.at(index.row()).at(index.column());
    }
    case HeadingRole:
    {
        if( index.row()==0){
            return true;
        }else{
            return false;
        }
    }
    default:
        break;
    }

    return QVariant();
}

QHash<int, QByteArray> TableModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TableDataRole] = "tabledata";
    roles[HeadingRole] = "heading";
    return roles;
}
