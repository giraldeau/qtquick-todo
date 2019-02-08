#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QQmlContext>
#include <todomodel.h>
#include <todolist.h>

int main(int argc, char *argv[])
{
   QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
   QQuickStyle::setStyle("Material");

   QGuiApplication app(argc, argv);

   // The list of todos
   TodoList todoList;

   // expose the model to QML
   qmlRegisterType<TodoModel>("TodoModel", 1, 0, "TodoModel");
   qmlRegisterUncreatableType<TodoList>("TodoList", 1, 0, "TodoList",
                                        QStringLiteral("TodoList cannot be created"));

   QQmlApplicationEngine engine;
   engine.rootContext()->setContextProperty("todolist", &todoList);
   engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
   if (engine.rootObjects().isEmpty())
      return -1;

   return app.exec();
}
