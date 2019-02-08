import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3
import TodoModel 1.0

Item {

    ListView {
        anchors.fill: parent

        // Cannot create nested lists
        model: TodoModel {
            list: todolist
        }

        delegate: RowLayout {
            width: parent.width

            CheckBox {
                checked: model.done
                onClicked: model.done = checked

            }
            TextField {
                Layout.fillWidth: parent
                Layout.rightMargin: 20

                // Accessing fields directly and using model seems to work
                // but model is not the complete model, but the current item which is weird
                // what is the difference with modelData i've seen elsewhere?

                //text: description
                //onEditingFinished: description = text

                text: model.description
                onEditingFinished: model.description = text
            }
        }
    }

}
