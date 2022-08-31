#include "dbcontext.h"
#include <QString>
#include <QDebug>
#include <QJsonObject>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QTimer>
#include "todolist.h"
#include "source.h"

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

const QJsonObject dbContext::createDbContext(QString &dbPath) {

    QDir dbDir = QDir();
    dbDir.mkpath(dbPath);
    dbPath += "/baza.json";
    QFile file(dbPath);

    if(!file.exists()) {
        QString dbContent = R"({
            "datum" : "null",
            "vrijemeNeaktivnosti": 300,
            "cijenaSata" : 5,
            "stranke" : [
                {
                    "ime" : "stranka 1",
                    "vrijeme" : 4000
                },
                {
                    "ime" : "stranka 2",
                    "vrijeme" : 0
                },
                {
                    "ime" : "stranka 3",
                    "vrijeme" : 6666
                }
            ]
        })";
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
//        dbContext.insert("datum", QDate::currentDate().month());
        return dbContext;
    }
}

QJsonArray dbContext::getStrankeArray(const QJsonObject &dbContext) {
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

void dbContext::updateDB(QJsonObject &dbContext,
                         QString date = "null",
                         double satnica = 0,
                         double vrijemeMirovanja = 0,
                         QVector<ToDoItem> mItems = QVector<ToDoItem>()){

    QJsonValue ref = dbContext.value(QString("stranke"));
    QJsonObject newVal;
    //QJsonArray arr = ref.toArray();
    QJsonArray stranke;
    QJsonDocument doc;
    QFile file(DB_PATH);
    if(date != "null") {
        dbContext.insert("datum", date);
    }

    if(satnica != 0) {
        int tmp = (int)((satnica * 100) + 0.5);
        satnica = (float)tmp/100.0;
//        cout << satnica;
        dbContext.insert("cijenaSata", satnica);
    }

    if(vrijemeMirovanja != 0) {
        dbContext.insert("vrijemeNeaktivnosti", vrijemeMirovanja);
    }

    if(!mItems.empty()) {
        for(int i = 0; i < mItems.length(); ++i) {
            QJsonObject obj;
            //QJsonObject stranka = arr[i].toObject();
            obj.insert("ime", mItems[i].description);
            obj.insert("vrijeme", mItems[i].time);
            stranke.insert(stranke.size(), obj);
//            qWarning() << mItems.length();
//            qWarning() << mItems[i].description;
//            qWarning() << mItems[i].time;
        }
        dbContext.insert("stranke", stranke);
    }

//    qWarning() << date << " " << satnica << " " << vrijemeMirovanja;
//    for(int i = 0; i < mItems.length(); ++i) {
//        qWarning() << mItems[i].description;
//        qWarning() << mItems[i].time;
//    }

    doc.setObject(dbContext);
    file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
    file.write(doc.toJson());
    file.close();

    qWarning() << "DB updated";
}
