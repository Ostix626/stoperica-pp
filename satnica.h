#ifndef SATNICA_H
#define SATNICA_H

#include <QObject>

class Satnica : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float cijenaSata READ cijenaSata WRITE setCijenaSata NOTIFY cijenaSataChanged)
    Q_PROPERTY(int idleTimeSeconds READ idleTimeSeconds WRITE setIdleTimeSeconds NOTIFY idleTimeSecondsChanged)

public:
    explicit Satnica(float cijenaSata = 77, int idleTimeSeconds = 120);

    float cijenaSata();
    void setCijenaSata(float& cijenaSata);

    int idleTimeSeconds();
    void setIdleTimeSeconds(int& idleTimeSeconds);

public slots:
    void mySlot(QString message);

signals:
    void cijenaSataChanged(float cijena);
    void idleTimeSecondsChanged(int seconds);

private:
    float m_cijenaSata;
    int m_idleTimeSeconds;
};

#endif // SATNICA_H
