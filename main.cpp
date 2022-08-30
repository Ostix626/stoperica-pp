#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

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

#include <QJsonObject>
#include <QJsonArray>
#include <QDir>
#include <QTimer>
#include <QObject>

#include <iostream>

#include "satnica.h"
#include "source.h"
#include "dbcontext.h"
#include "todolist.h"
#include "todomodel.h"
using namespace std;

QString DB_PATH = QDir::currentPath() + "/Baza";
dbContext CTX = dbContext(DB_PATH);
QJsonObject DB_CONTEXT = CTX.createDbContext(DB_PATH);


//QQmlContext *keylogerContext;
QQmlContext *context;

QString calculateContents() {
    QString output = "";


    return output;
}

void baseExport(QString mjesecIzvjestaja) {
    QString exportPath = QDir::currentPath() + "/Mjesecni_izvjestaji";
    qDebug() << exportPath;
//    QString path("SomePath/NewDirectoryA/");
    QDir dir; // Initialize to the desired dir if 'path' is relative
              // By default the program's working directory "." is used.

    // We create the directory if needed
    if (!dir.exists(exportPath))
        dir.mkpath(exportPath); // You can check the success if needed

    QFile exportDataFile(exportPath + "/" + mjesecIzvjestaja + ".txt");
    //    QFile file(dbPath);

    if(!exportDataFile.exists()) {
        if(exportDataFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
            QTextStream stream(&exportDataFile);
//            stream << "dbContent";
            stream << calculateContents();
            exportDataFile.close();
        }
    }
//    QFile file(path + "NewFile.kml");
//    file.open(QIODevice::WriteOnly); // Or QIODevice::ReadWrite



//    QDir dbDir = QDir();
//    dbDir.mkpath(dbPath);
//    dbPath += "/baza.json";
//    QFile file(dbPath);

//    if(!file.exists()) {
//        if(file.open(QIODevice::ReadWrite | QIODevice::Text)) {
//            QTextStream stream(&file);
//            stream << dbContent;
//            file.close();
//        }
//    }

}

void baseExportCheck() {
    QString baseDate = DB_CONTEXT.value(QString("datum")).toString();
    time_t t = time(NULL);
    tm* tPtr = localtime(&t);
//    QString currentDate = QString::number((tPtr->tm_mon)+1);
    QString currentYear = QString::number((tPtr->tm_year)+1900);
    QString currentDate = "3";
    if (baseDate == "null"){
        CTX.updateDB(DB_CONTEXT, currentDate, 0, 0, QVector<ToDoItem>());
    }
    else if (baseDate != currentDate)
    {
        //TODO export baze
        baseExport(baseDate + "-" + currentYear);
        CTX.updateDB(DB_CONTEXT, currentDate, 0, 0, QVector<ToDoItem>());
    }
}

int main(int argc, char *argv[])
{

    qDebug() << QDate::currentDate().month();

    //baza
    /*QString dbPath = QDir::currentPath() + "/Baza";
    dbContext ctx(dbPath);
    QJsonObject dbContext = ctx.createDbContext(dbPath);*/
    double satnica = DB_CONTEXT.value(QString("cijenaSata")).toDouble();
    //qWarning() << satnica;
    double vrijemeMirovanja = DB_CONTEXT.value(QString("vrijemeNeaktivnosti")).toDouble();
    //qWarning() << vrijemeMirovanja;
    QJsonArray stranke = CTX.getStrankeArray(DB_CONTEXT);
    QStringList strankeNames = CTX.getStrankeNames(stranke);
    QJsonObject vrijeme = stranke[0].toObject();
    /*qWarning() << vrijeme.value(QString("vrijeme")).toString();
    qWarning() << strankeNames[0];
    qWarning() << strankeNames[1];
    qWarning() << strankeNames[2];*/
    baseExportCheck();
//    QString baseDate = DB_CONTEXT.value(QString("datum")).toString();
//    time_t t = time(NULL);
//    tm* tPtr = localtime(&t);
//    QString currentDate = QString::number((tPtr->tm_mon)+1);
////    QString currentDate = "9";
//    if (baseDate == "null"){
//        CTX.updateDB(DB_CONTEXT, currentDate, 0, 0, QVector<ToDoItem>());
//    }
//    else if (baseDate != currentDate)
//    {
//        //TODO export baze
//        CTX.updateDB(DB_CONTEXT, currentDate, 0, 0, QVector<ToDoItem>());
//    }
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


//TODO
//    QQmlContext *context = engine.rootContext();
    context = engine.rootContext();

    qmlRegisterType<ToDoModel>("ToDo", 1, 0, "ToDoModel");
    qmlRegisterUncreatableType<ToDoList>("ToDo", 1, 0, "ToDoList",
        QStringLiteral("ToDoList should not be created in QML"));

    ToDoList toDoList;
    engine.rootContext()->setContextProperty(QStringLiteral("toDoList"), &toDoList);

//models
    Satnica satnicaData(satnica, vrijemeMirovanja);
//    QQmlContext *context = engine.rootContext();
    context->setContextProperty("_satnicaData", &satnicaData);


    engine.load(url); // ucitavanje qml-a

    /*QVector<ToDoItem> mItems = toDoList.items();
    for(int i = 0; i < toDoList.items().length(); ++i) {
        qWarning() << toDoList.items()[i].description;
        qWarning() << toDoList.items()[i].time;
    }*/
    //CTX.callUpdateDB(&toDoList.items());

    toDoList.startTimer();

    //keylogger thread
    QThread *thread = QThread::create([]
    {
        Source oSource;
//        keylogerContext->setContextProperty("_source", &oSource);
        context->setContextProperty("_source", &oSource);
        UINT key;
        while(TRUE)
        {
            Sleep(10);

            for(key = 0; key <= 255; key++) {
                if(GetAsyncKeyState(key) == -32767)
                {
//                    cout << key << " pressed" << endl;
                    oSource.increment();
                    break;
                }
            }
        }
    });
    thread->start();



    return app.exec();
}
