#include "adminmodel.h"

AdminModel::AdminModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_list << std::make_pair<QString, AdminValue>("All", AdminValue::ALL)
            << std::make_pair<QString, AdminValue>("Admin or higher", AdminValue::ADMIN_OR_HEIGHER)
            << std::make_pair<QString, AdminValue>("Superadmin or rcon", AdminValue::SUPERADMIN_OR_RCON)
            << std::make_pair<QString, AdminValue>("Rcon only", AdminValue::RCON_ONLY);
}

QVariant AdminModel::data(const QModelIndex &index, int role) const
{
    const auto& data = m_list.at(index.row());
    
    QVariant value;
    
    switch (role)
    {
    case Qt::DisplayRole:
    {
        value = data.first;
    }
        break;
    case Qt::UserRole:
    {
        value = static_cast<int>(data.second);
    }
        break;
    default:
        break;
    }
    return value;
}

int AdminModel::rowCount(const QModelIndex &parent) const
{
    return m_list.count();
}
