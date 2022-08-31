#include "satnica.h"
#include "dbcontext.h"
#include "source.h"
#include <QDebug>
#include <QDir>
#include <QJsonObject>


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
    if (cijenaSata == m_cijenaSata) return;
    m_cijenaSata = cijenaSata;
    CTX.updateDB(DB_CONTEXT, "null", m_cijenaSata, 0, QVector<ToDoItem>());
    emit cijenaSataChanged(m_cijenaSata);
}

int Satnica::idleTimeSeconds()
{
    return m_idleTimeSeconds;
}

void Satnica::setIdleTimeSeconds(int &idleTimeSeconds)
{
    if (idleTimeSeconds == m_idleTimeSeconds) return;
    m_idleTimeSeconds = idleTimeSeconds;
    CTX.updateDB(DB_CONTEXT, "null", 0, m_idleTimeSeconds, QVector<ToDoItem>());
    emit cijenaSataChanged(m_idleTimeSeconds);
}

void Satnica::mySlot(QString message)
{
    qInfo() << message;
}



