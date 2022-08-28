#ifndef SATNICA_H
#define SATNICA_H

#include <QObject>

class Satnica : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float cijenaSata READ cijenaSata WRITE setCijenaSata NOTIFY cijenaSataChanged)

public:
    explicit Satnica(float cijenaSata = 77);

    float cijenaSata();
    void setCijenaSata(float& cijenaSata);

//public slots:
//    void changeCijenaSata(float cijena);

signals:
    void cijenaSataChanged(float cijena);

private:
    float m_cijenaSata;
};

#endif // SATNICA_H
