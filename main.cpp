#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QThread>
//keylogger
#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <winuser.h>
#include <fstream>
#include <string>
#include <windows.h>
//keylogger

#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <filesystem>
#include <unistd.h>
#include <QDir>
#include <QTextStream>

#include <iostream>
using namespace std;
using std::filesystem::current_path;

QJsonObject createDbContext() {
    QString dbContent = R"({
        "vrijemeNeaktivnosti": 5,
        "cijenaSata" : 5,
        "stranke" : [
            {
                "id": 1,
                "ime" : "goldfren",
                "vrijeme" : "4:45:34"
            },
            {
                "id": 2,
                "ime" : "Montraker d.o.o.",
                "vrijeme" : "3:23:15"
            },
        {
                "id": 3,
                "ime" : "Bepi the barber",
                "vrijeme" : "2:16:48"
            }
        ]
    })";
    QString dbPath = QDir::currentPath() + "/Baza";
    QDir dbDir = QDir();
    dbDir.mkpath(dbPath);
    dbPath += "/baza.json";
    QFile file(dbPath);

    if(!file.exists()) {
        if(file.open(QIODevice::ReadWrite | QIODevice::Text)) {
            bool flag = true;
            qWarning() << flag;
            QTextStream stream(&file);
            stream << dbContent;
            file.close();
        }
    }else {
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
}

QJsonArray getStrankeArray(QJsonObject dbContext) {
    QJsonValue strankeJson = dbContext.value(QString("stranke"));
    QJsonArray stranke = strankeJson.toArray();
    return stranke;
}

QStringList getStrankeNames(QJsonArray stranke) {
    QStringList strankeNames;
    for(int i = 0; i < stranke.size(); ++i) {
        QJsonObject name = stranke[i].toObject();
        strankeNames<<name.value(QString("ime")).toString();
    }

    return strankeNames;
}

int main(int argc, char *argv[])
{

    //baza
    QJsonObject dbContext = createDbContext();
    double satnica = dbContext.value(QString("cijenaSata")).toDouble();
    qWarning() << satnica;
    double vrijemeMirovanja = dbContext.value(QString("vrijemeNeaktivnosti")).toDouble();
    qWarning() << vrijemeMirovanja;
    QJsonArray stranke = getStrankeArray(dbContext);
    QStringList strankeNames = getStrankeNames(stranke);
    QJsonObject vrijeme = stranke[0].toObject();
    qWarning() << vrijeme.value(QString("vrijeme")).toString();
    qWarning() << strankeNames[0];
    qWarning() << strankeNames[1];
    qWarning() << strankeNames[2];
    //baza

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);


    //keylogger thread
    QThread *thread = QThread::create([]
    {
        UINT key;

        while(TRUE)
        {
            Sleep(10);

            for(key = 0; key <= 255; key++) {
                if(GetAsyncKeyState(key) == -32767)
                {
                    cout << key << " pressed" << endl;
                }
            }
        }
    });
    thread->start();



    return app.exec();
}
