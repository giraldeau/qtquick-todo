#ifndef TODOLIST_H
#define TODOLIST_H

#include <QObject>
#include <QVector>


struct TodoItem {
   bool done;
   QString description;
};

class TodoList : public QObject
{
   Q_OBJECT
public:
   explicit TodoList(QObject *parent = nullptr);

   bool setItemAt(int index, const TodoItem &item);
   QVector<TodoItem> items();

signals:
   void preItemAppended();
   void postItemAppended();

   void preItemRemoved(int index);
   void postItemRemoved();

public slots:
   void appendItem();
   void removeCompletedItems();

private:
   QVector<TodoItem> m_items;
};

#endif // TODOLIST_H
