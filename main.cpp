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

#include <iostream>

#include "satnica.h"
#include "source.h"
#include "dbcontext.h"
#include "todolist.h"
#include "todomodel.h"
using namespace std;



QQmlContext *keylogerContext;

int main(int argc, char *argv[])
{

    //baza
    QString dbPath = QDir::currentPath() + "/Baza";
    dbContext ctx(dbPath);
    QJsonObject dbContext = ctx.createDbContext(dbPath);
    double satnica = dbContext.value(QString("cijenaSata")).toDouble();
    qWarning() << satnica;
    double vrijemeMirovanja = dbContext.value(QString("vrijemeNeaktivnosti")).toDouble();
    qWarning() << vrijemeMirovanja;
    QJsonArray stranke = ctx.getStrankeArray(dbContext);
    QStringList strankeNames = ctx.getStrankeNames(stranke);
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
    qWarning() << (&Satnica::mySlot) << "kkklas " << satnicaData.cijenaSata();
//models



    engine.load(url); // ucitavanje qml-a


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
                    cout << key << " pressed" << endl;
                    oSource.increment();
                    break;
                }
            }
        }
    });
    thread->start();



    return app.exec();
}
