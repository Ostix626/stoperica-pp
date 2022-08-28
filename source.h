#ifndef SOURCE_H
#define SOURCE_H

#include <QObject>

class Source : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int press READ press WRITE setPress NOTIFY pressChanged)
public:
    explicit Source(int press = 0);

    int press();
    void setPress(int& press);
    void increment();

signals:
    void pressChanged(int num);
private:
    int m_press;
};

#endif // SOURCE_H


