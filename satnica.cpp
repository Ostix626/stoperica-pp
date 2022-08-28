#include "satnica.h"
#include <QDebug>
//Laptop::Laptop(QObject *parent, QString name)
//    : QObject{parent}
//{
//    this -> name = name;
//    qInfo() << this << "constructor" << name;
//}

Satnica::Satnica(float cijenaSata) : m_cijenaSata(cijenaSata)
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


