#include "satnica.h"
#include <QDebug>
//Laptop::Laptop(QObject *parent, QString name)
//    : QObject{parent}
//{
//    this -> name = name;
//    qInfo() << this << "constructor" << name;
//}

Satnica::Satnica(float cijenaSata, int idleTimeSeconds)
    : m_cijenaSata(cijenaSata), m_idleTimeSeconds(idleTimeSeconds)
{
}

float Satnica::cijenaSata()
{
    return m_cijenaSata;
}

void Satnica::setCijenaSata(float &cijenaSata)
{
    qDebug() << cijenaSata;
    if (cijenaSata == m_cijenaSata) return;
    m_cijenaSata = cijenaSata;
    emit cijenaSataChanged(m_cijenaSata);
}

int Satnica::idleTimeSeconds()
{
    return m_idleTimeSeconds;
}

void Satnica::setIdleTimeSeconds(int &idleTimeSeconds)
{
    qDebug() << idleTimeSeconds;
    if (idleTimeSeconds == m_idleTimeSeconds) return;
    m_idleTimeSeconds = idleTimeSeconds;
    emit cijenaSataChanged(m_idleTimeSeconds);
}

void Satnica::mySlot(QString message)
{
    qInfo() << message;
}



