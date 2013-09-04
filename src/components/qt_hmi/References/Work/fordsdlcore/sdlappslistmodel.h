#ifndef SDLAPPSLISTMODEL_H
#define SDLAPPSLISTMODEL_H

#include <QAbstractListModel>
#include <QStringList>

class SdlAppsListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit SdlAppsListModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const { return m_data.size(); }

    QVariant data(const QModelIndex &index, int role) const {
        return m_data.values().at(index.row());
    }

    virtual QHash<int, QByteArray>	roleNames() const {
        QHash<int, QByteArray> roles;
        roles['t'] = "text";
        return roles;
    }

    void append(int id, QString string) {
        beginResetModel();
        m_data.clear();
        m_data[id] = string;
        endResetModel();
    }

    void clear() {
        beginResetModel();
        m_data.clear();
        endResetModel();
    }
    
signals:
    
public slots:

private:
    QHash<int, QString> m_data;
    
};

#endif // SDLAPPSLISTMODEL_H
