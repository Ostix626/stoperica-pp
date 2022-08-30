#include <QJsonObject>
#include <QJsonArray>
#include <QDir>
#include "todolist.h"
#include "dbcontext.h"


ToDoList::ToDoList(QObject *parent) : QObject(parent)
{
    QString dbPath = QDir::currentPath() + "/Baza";
    dbContext ctx(dbPath);
    QJsonObject dbContext = ctx.createDbContext(dbPath);
    QJsonArray stranke = ctx.getStrankeArray(dbContext);
    QStringList strankeNames = ctx.getStrankeNames(stranke);
    QJsonObject vrijeme = stranke[0].toObject();
    /*qWarning() << vrijeme.value(QString("vrijeme")).toString();
    qWarning() << strankeNames[0];
    qWarning() << strankeNames[1];
    qWarning() << strankeNames[2];*/

    for(int i = 0; i < stranke.size(); ++i) {
        QJsonObject stranka = stranke[i].toObject();
        strankeNames<<stranka.value(QString("ime")).toString();
        mItems.append({ false, stranka.value(QString("ime")).toString(), stranka.value(QString("vrijeme")).toString().toInt()});
    }
//    qWarning() << mItems.data()->time;
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
