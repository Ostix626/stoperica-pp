import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

import ToDo 1.0

ColumnLayout {
    Frame {
        Layout.fillWidth: true
        anchors.fill: parent

        RowLayout {
            id: rowlayout_buttons
            Button {
                text: qsTr("Add new item")
                onClicked: toDoList.appendItem()
                Layout.fillWidth: true
            }
            Button {
                text: qsTr("Remove completed")
                onClicked: toDoList.removeCompletedItems()
                Layout.fillWidth: true
            }
        }

        ListView {
            anchors.top: rowlayout_buttons.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            clip: true

            model: ToDoModel {
                list: toDoList
            }

            delegate: RowLayout {
                width: parent.width


                TextField {
                    text: model.description
                    onEditingFinished: model.description = text
                    Layout.fillWidth: true
                }
                TextField {
                    text: model.time
                    readOnly : true
                    onEditingFinished: model.time = text
                    Layout.fillWidth: true
                }
                CheckBox {
                    checked: model.done
                    onClicked: model.done = checked
                }
            }
        }
    }


}
