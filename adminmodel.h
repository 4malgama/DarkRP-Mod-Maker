#ifndef ADMINMODEL_H
#define ADMINMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include "classes.h"

class AdminModel : public QAbstractListModel
{
    Q_OBJECT
    
    QList<std::pair<QString, AdminValue>> m_list;
public:
    explicit AdminModel(QObject *parent = nullptr);
    virtual QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    virtual int rowCount( const QModelIndex & parent = QModelIndex() ) const;
};

#endif // ADMINMODEL_H
