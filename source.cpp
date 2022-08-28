#include "source.h"
#include <QDebug>

Source::Source(int press) : m_press(press)
{
}

int Source::press()
{
    return m_press;
}

void Source::setPress(int &press)
{
    if (press == m_press) return;
    m_press = press;
    emit pressChanged(m_press);
}

void Source::increment()
{
    m_press += 1;
    emit pressChanged(m_press);
}



//void Source::test()
//{
//    qDebug() << "QString message key is pressed";
//    emit mySignal("key is pressed");
//}
