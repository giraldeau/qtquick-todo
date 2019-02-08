#include "todomodel.h"
#include <QHash>
#include <QByteArray>
#include <todolist.h>

#include <QDebug>

TodoModel::TodoModel(QObject *parent)
   : QAbstractListModel(parent),
     m_list(nullptr)
{
}

int TodoModel::rowCount(const QModelIndex &parent) const
{
   // invalid model index indicates the root of the model
   // for a list, all items have the root as parent
   if (parent.isValid() || !m_list)
      return 0;
   return m_list->items().size();
}

int TodoModel::columnCount(const QModelIndex &parent) const
{
   if (parent.isValid())
      return 0;
   return 2;
}

QVariant TodoModel::data(const QModelIndex &index, int role) const
{
   qDebug() << "TodoModel::data" << index << role;
   if (!index.isValid() || !m_list)
      return QVariant();

   const TodoItem item = m_list->items().at(index.row());

   switch(role) {
   case DoneRole:
      return QVariant(item.done);
   case DescriptionRole:
      return QVariant(item.description);
   }

   return QVariant();
}

bool TodoModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
   if (!m_list) {
      return false;
   }

   TodoItem item = m_list->items().at(index.row());

   switch(role) {
   case DoneRole:
      item.done = value.toBool();
      break;
   case DescriptionRole:
      item.description = value.toString();
      break;
   }

   if (m_list->setItemAt(index.row(), item)) {
      emit dataChanged(index, index, QVector<int>() << role);
      return true;
   }
   return false;
}

Qt::ItemFlags TodoModel::flags(const QModelIndex &index) const
{
   if (!index.isValid())
      return Qt::NoItemFlags;
   return Qt::ItemIsEditable;
}

QHash<int, QByteArray> TodoModel::roleNames() const
{
   // the role names must exactly match the QML property name
   // i.e. TextField { text: model.description }
   // otherwise, you will get a runtime error:
   //
   //  qrc:/ToDoList.qml:36:17: Unable to assign [undefined] to QString
   auto ret = QHash<int, QByteArray>{
      { DoneRole, "done" },
      { DescriptionRole, "description" }
   };
   qDebug() << ret;
   return ret;
}

TodoList *TodoModel::list() const
{
   return m_list;
}

void TodoModel::setList(TodoList *list)
{
   qDebug() << "TodoModel::setList" << list->items().size();

   beginResetModel();
   if (m_list) {
      m_list->disconnect(this);
   }
   m_list = list;

   if (m_list) {
      connect(m_list, &TodoList::preItemAppended, this, [=](){
         const int index = m_list->items().size();
         beginInsertRows(QModelIndex(), index, index);
      });
      connect(m_list, &TodoList::postItemAppended, this, [=](){
         endInsertRows();
      });

      connect(m_list, &TodoList::preItemRemoved, this, [=](int index){
         beginRemoveRows(QModelIndex(), index, index);
      });
      connect(m_list, &TodoList::postItemRemoved, this, [=](){
         endRemoveRows();
      });
   }

   endResetModel();
}
