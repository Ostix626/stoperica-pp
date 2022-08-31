#ifndef EXPORT_H
#define EXPORT_H
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>
#include <QTimer>
#include <QObject>

#include "dbcontext.h"

#include <QString>
#include <QJsonDocument>

#endif // EXPORT_H

extern dbContext CTX;
extern QJsonObject DB_CONTEXT;
extern QString DB_PATH;

QString conv(int x)
{
    QString out = "0";
    if (x < 10){
         out = "0" + QString::number(x);
    }
    else out = QString::number(x);
    return out;
}

QString secondsToTime(long sekunde) {
    int h = sekunde / 3600;
    int m = (sekunde - (h * 3600))/ 60;
    int s = sekunde % 60;

    return (conv(h) + ":" + conv(m) + ":" + conv(s) + " h");
}

QString calculateEarning(long sekunde, float satnica){
    float rez = sekunde * (satnica / 3600);
    return QString::number(rez, 'f', 2) + " €";
}

QString calculateContents() {
    // zapis u bazu
    QJsonArray resetStranke;
    QJsonDocument doc;
    QFile file(DB_PATH);
    QJsonObject obj;

    // zapis u export
    float satnica = DB_CONTEXT.value(QString("cijenaSata")).toDouble();
    QString output = "Cijena radnog sata: " + QString::number(satnica) + " €\n";
    QJsonArray stranke = CTX.getStrankeArray(DB_CONTEXT);
    long ukVrijeme = 0;

    for(int i = 0; i < stranke.size(); i++){
        // zapis u export
        QJsonObject stranka = stranke[i].toObject();
        QString imeStranke = stranka.value(QString("ime")).toString();
        int vrijeme = stranka.value(QString("vrijeme")).toInt();
        ukVrijeme += vrijeme;
        output += "\n" + imeStranke + " ; " + secondsToTime(vrijeme) + " ; " + calculateEarning(vrijeme, satnica);

        //zapis u bazu
        obj.insert("ime", imeStranke);
        obj.insert("vrijeme", 0);
        resetStranke.insert(resetStranke.size(), obj);
    }
    // zapis u export
    output += "\n\nUkupno vrijeme rada: " + secondsToTime(ukVrijeme) + "\nUkupna mjesecna zarada: " + calculateEarning(ukVrijeme, satnica);

    //zapis u bazu
    DB_CONTEXT.insert("stranke", resetStranke);
    doc.setObject(DB_CONTEXT);
    file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
    file.write(doc.toJson());
    file.close();

    return output;
}

void baseExport(QString mjesecIzvjestaja) {
    QString exportPath = QDir::currentPath() + "/Mjesecni_izvjestaji";
    qDebug() << exportPath;
    QDir dir;

    if (!dir.exists(exportPath))
        dir.mkpath(exportPath);
    QFile exportDataFile(exportPath + "/" + mjesecIzvjestaja + ".txt");


    if(!exportDataFile.exists()) {
        if(exportDataFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
            QTextStream stream(&exportDataFile);
            stream << calculateContents();
            exportDataFile.close();
        }
    }

}


void baseExportCheck() {
    QString baseDate = DB_CONTEXT.value(QString("datum")).toString();
    time_t t = time(NULL);
    tm* tPtr = localtime(&t);
    QString currentDate = QString::number((tPtr->tm_mon)+1);
    QString currentYear = QString::number((tPtr->tm_year)+1900);
//  currentDate = "8"; // za testiranje mjesecnog exporta staviti drugi broj
    if (baseDate == "null"){
        CTX.updateDB(DB_CONTEXT, currentDate, 0, 0, QVector<ToDoItem>());
    }
    else if (baseDate != currentDate)
    {
        baseExport(baseDate + "-" + currentYear);
        CTX.updateDB(DB_CONTEXT, currentDate, 0, 0, QVector<ToDoItem>());
    }
}
