#ifndef TODOLIST_H
#define TODOLIST_H

#include <QObject>
#include <QVector>
#include <QTimer>

//Q_MOC_INCLUDE("dbcontext.h");
//class dbContext;

struct ToDoItem
{
    bool done;
    QString description;
    int time;
};

class ToDoList : public QObject
{
    Q_OBJECT
public:
    explicit ToDoList(QObject *parent = nullptr);

    QVector<ToDoItem> items() const;

    bool setItemAt(int index, const ToDoItem &item);

signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

public slots:
    void appendItem();
    void removeCompletedItems();
    void startTimer();

private:
    QVector<ToDoItem> mItems;
    QTimer m_timer;
};

#endif // TODOLIST_H
