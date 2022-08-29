#ifndef DBCONTEXT_H
#define DBCONTEXT_H

#include <QString>
#include <QObject>
#include "todolist.h"

class dbContext : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString dbPath READ dbPath WRITE setDbPath NOTIFY dbPathChanged)
public:
    explicit dbContext(QString dbPath = "");

    QString dbPath();
    void setDbPath(QString& dbPath);

    const QJsonObject createDbContext(QString& dbPath);

    QJsonArray getStrankeArray(const QJsonObject& dbContext);

    QStringList getStrankeNames(QJsonArray& stranke);

    void updateDB(QJsonObject& dbContext, QString date, double satnica, double vrijemeMirovanja, QVector<ToDoItem> mItems);

public slots:
    void mySlot(QString message);

signals:
    void dbPathChanged(QString path);

private:
    QString m_dbPath;
};

#endif // DBCONTEXT_H
