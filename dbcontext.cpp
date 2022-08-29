#include "dbcontext.h"
#include <QString>
#include <QDebug>
#include <QJsonObject>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>

#include <iostream>

using namespace std;

dbContext::dbContext(QString dbPath)
    : m_dbPath(dbPath)
{

}

QString dbContext::dbPath() {
    return m_dbPath;
}

void dbContext::setDbPath(QString &dbPath) {
    if(dbPath == m_dbPath) return;
    m_dbPath = dbPath;
    emit dbPathChanged(m_dbPath);
}

QJsonObject dbContext::createDbContext(QString &dbPath) {
    QString dbContent = R"({
        "vrijemeNeaktivnosti": 300,
        "cijenaSata" : 5,
        "stranke" : [
            {
                "id": 1,
                "ime" : "stranka 1",
                "vrijeme" : "4000"
            },
            {
                "id": 2,
                "ime" : "stranka 2",
                "vrijeme" : "0"
            },
        {
                "id": 3,
                "ime" : "stranka 3",
                "vrijeme" : "6666"
            }
        ]
    })";
    QDir dbDir = QDir();
    dbDir.mkpath(dbPath);
    dbPath += "/baza.json";
    QFile file(dbPath);

    if(!file.exists()) {
        if(file.open(QIODevice::ReadWrite | QIODevice::Text)) {
            QTextStream stream(&file);
            stream << dbContent;
            file.close();
        }
    }

    file.open(QIODevice::ReadOnly | QIODevice::Text);
    const QByteArray val = file.readAll();
    file.close();
    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(val, &jsonError);

    if( jsonError.error != QJsonParseError::NoError ) {
        std::cout << "fromJson failed: " << jsonError.errorString().toStdString() << std::endl;
        return QJsonObject();
    }else {
        QJsonObject dbContext = doc.object();
        return dbContext;
    }
}

QJsonArray dbContext::getStrankeArray(QJsonObject &dbContext) {
    QJsonValue strankeJson = dbContext.value(QString("stranke"));
    QJsonArray stranke = strankeJson.toArray();
    return stranke;
}

QStringList dbContext::getStrankeNames(QJsonArray &stranke) {
    QStringList strankeNames;
    for(int i = 0; i < stranke.size(); ++i) {
        QJsonObject name = stranke[i].toObject();
        strankeNames<<name.value(QString("ime")).toString();
    }

    return strankeNames;
}

void dbContext::mySlot(QString message)
{
    qInfo() << message;
}
