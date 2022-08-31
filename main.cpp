#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QThread>
#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <winuser.h>
#include <fstream>
#include <string>

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
#include "export.h"
using namespace std;

QString DB_PATH = QDir::currentPath() + "/Baza";
dbContext CTX = dbContext(DB_PATH);
QJsonObject DB_CONTEXT = CTX.createDbContext(DB_PATH);

QQmlContext *context;



int main(int argc, char *argv[])
{

    double satnica = DB_CONTEXT.value(QString("cijenaSata")).toDouble();
    double vrijemeMirovanja = DB_CONTEXT.value(QString("vrijemeNeaktivnosti")).toDouble();
    baseExportCheck();

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

    context = engine.rootContext();

    qmlRegisterType<ToDoModel>("ToDo", 1, 0, "ToDoModel");
    qmlRegisterUncreatableType<ToDoList>("ToDo", 1, 0, "ToDoList",
        QStringLiteral("ToDoList should not be created in QML"));

    ToDoList toDoList;
    engine.rootContext()->setContextProperty(QStringLiteral("toDoList"), &toDoList);

    Satnica satnicaData(satnica, vrijemeMirovanja);
    context->setContextProperty("_satnicaData", &satnicaData);

    engine.load(url); // ucitavanje qml-a
    toDoList.startTimer();

    //keylogger thread
    QThread *thread = QThread::create([]
    {
        Source oSource;
        context->setContextProperty("_source", &oSource);
        UINT key;
        while(TRUE)
        {
            Sleep(10);

            for(key = 0; key <= 255; key++) {
                if(GetAsyncKeyState(key) == -32767)
                {
                    oSource.increment();
                    break;
                }
            }
        }
    });
    thread->start();



    return app.exec();
}
