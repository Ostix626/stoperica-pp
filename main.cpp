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


QQmlContext *keylogerContext;

int main(int argc, char *argv[])
{
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
    QQmlContext *context = engine.rootContext();

    qmlRegisterType<ToDoModel>("ToDo", 1, 0, "ToDoModel");
    qmlRegisterUncreatableType<ToDoList>("ToDo", 1, 0, "ToDoList",
        QStringLiteral("ToDoList should not be created in QML"));

    ToDoList toDoList;
    engine.rootContext()->setContextProperty(QStringLiteral("toDoList"), &toDoList);

//models
    Satnica satnicaData(satnica, vrijemeMirovanja);
//    QQmlContext *context = engine.rootContext();
    context->setContextProperty("_satnicaData", &satnicaData);
    keylogerContext = context;
    //qWarning() << (&Satnica::mySlot) << "kkklas " << satnicaData.cijenaSata();
//models


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
        keylogerContext->setContextProperty("_source", &oSource);
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
