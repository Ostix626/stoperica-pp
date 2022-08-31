#include <QJsonObject>
#include <QJsonArray>
#include <QDir>
#include "todolist.h"
#include "dbcontext.h"
#include "source.h"


ToDoList::ToDoList(QObject *parent) : QObject(parent)
{
    QJsonArray stranke = CTX.getStrankeArray(DB_CONTEXT);
    QStringList strankeNames = CTX.getStrankeNames(stranke);
    QJsonObject vrijeme = stranke[0].toObject();

    for(int i = 0; i < stranke.size(); ++i) {
        QJsonObject stranka = stranke[i].toObject();
        strankeNames<<stranka.value(QString("ime")).toString();
        mItems.append({ false, stranka.value(QString("ime")).toString(), stranka.value(QString("vrijeme")).toInt()});
    }

}

QVector<ToDoItem> ToDoList::items() const
{
    return mItems;
}

bool ToDoList::setItemAt(int index, const ToDoItem &item)
{
    if (index < 0 || index >= mItems.size())
        return false;

    const ToDoItem &oldItem = mItems.at(index);
    if (item.done == oldItem.done && item.description == oldItem.description && item.time == oldItem.time)
        return false;

    mItems[index] = item;
    return true;
}

void ToDoList::appendItem()
{
    emit preItemAppended();

    ToDoItem item;
    item.done = false;
    item.description = "";
    item.time = 0;
    mItems.append(item);

    emit postItemAppended();
}

void ToDoList::removeCompletedItems()
{
    for (int i = 0; i < mItems.size(); ) {
        if (mItems.at(i).done) {
            emit preItemRemoved(i);

            mItems.removeAt(i);

            emit postItemRemoved();
        } else {
            ++i;
        }
    }
}

void ToDoList::startTimer() {
    connect(&m_timer, &QTimer::timeout, this, [=]() { dbContext::updateDB(DB_CONTEXT, "null", 0, 0, mItems); });
    m_timer.setInterval(5000);
    m_timer.start();
}
