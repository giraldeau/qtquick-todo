#include "todolist.h"
#include <QDebug>

TodoList::TodoList(QObject *parent) : QObject(parent)
{
   qDebug() << "TodoList::TodoList";
   // some mock data
   m_items.push_back({true, "foo"});
   m_items.push_back({false, "bar"});
}

bool TodoList::setItemAt(int index, const TodoItem &item)
{
   qDebug() << "TodoList::setItemAt";
   if (index < 0 || index >= m_items.size()) {
      return false;
   }

   const TodoItem &oldItem = m_items.at(index);
   if (oldItem.done == item.done && oldItem.description == item.description) {
      return false;
   }
   m_items[index] = item;
   return true;
}

QVector<TodoItem> TodoList::items()
{
   return m_items;
}

void TodoList::appendItem()
{
   emit preItemAppended();
   m_items.push_back({false, ""});
   emit postItemAppended();
}

void TodoList::removeCompletedItems()
{
   for (int i = 0; i < m_items.size(); ) {
      if (m_items.at(i).done) {
         emit preItemRemoved(i);
         m_items.remove(i);
         emit postItemRemoved();
      } else {
         i++;
      }
   }
}
