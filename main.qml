import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.5
import QtQuick.Window 2.3
import QtQuick.Layouts 1.3

Window {
    id: window
    width: 700
    height: 960
    minimumHeight: 600
    minimumWidth: 650
    visible: true
    color: "#e9d5a7"
    title: qsTr("Štoperica++")

    Rectangle {
        id: rectangle_postavke
        y: 10
        width: 200
        height: 180
        color: "#00ffffff"
        border.width: 2

        anchors.left: parent.left
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.leftMargin: 10

        Rectangle {
            id: rectangle_satnica
            width: 300
            height: rectangle_postavke.height
            color: "#00ffffff"

            Label{
                id: label_satnica
//                x: 0
//                y: 0
                text: "Satnica (EUR): "
                focus: true
                font.styleName: "Bold"
                font.pointSize: 16
                anchors.top: rectangle_satnica.top
                bottomPadding: 10
                topPadding: 10
                anchors.left: rectangle_satnica.left
                anchors.leftMargin: 10
            }

            TextField {
                id: textField_satnica
                width: 240
                height: 32
                anchors.top: label_satnica.bottom
                text: ""
                hoverEnabled: false
                font.pointSize: 14
                leftPadding: 17
                leftInset: 10
                placeholderText: qsTr("Napisi cijenu radnog sata")
            }

            Button {
                id: button_save_satnica
                width: 50
                height: 32
                anchors.left: textField_satnica.right
                anchors.top: label_satnica.bottom
                leftPadding: 4
                text: qsTr("Save")
                font.pointSize: 14
                anchors.leftMargin: 5
                onClicked: {
                    label_satnica.text = "Satnica (EUR): " + textField_satnica.text
                }
            }

            // VRIJEME
            Label{
                id: label_vrijeme_mirovanja
                text: "Vrijeme mirovanja (min): "
                focus: true
                font.styleName: "Bold"
                font.pointSize: 16
                anchors.top: textField_satnica.bottom
                bottomPadding: 10
                topPadding: 10
                anchors.left: rectangle_satnica.left
                anchors.leftMargin: 10
            }

            TextField {
                id: textField_vrijeme_mirovanja
                width: 240
                height: 32
                anchors.top: label_vrijeme_mirovanja.bottom
                text: ""
                hoverEnabled: false
                font.pointSize: 14
                leftPadding: 17
                leftInset: 10
                placeholderText: qsTr("Napisi minute mirovanja")
            }

            Button {
                id: button_save_vrijeme_mirovanja
                width: 50
                height: 32
                anchors.left: textField_vrijeme_mirovanja.right
                anchors.top: label_vrijeme_mirovanja.bottom
                leftPadding: 4
                text: qsTr("Save")
                font.pointSize: 14
                anchors.leftMargin: 5
                onClicked: {
                    label_vrijeme_mirovanja.text = "Vrijeme mirovanja (min): " + textField_vrijeme_mirovanja.text
                }
            }


        }

        ////////////////////////////////////////

        Rectangle {
            id: rectangle_vrijeme
            height: rectangle_postavke.height
            color: "#00ffffff"
            anchors.left: rectangle_satnica.right
            anchors.right: parent.right
            anchors.leftMargin: 10

        }
        Label{
            id: label_proteklo_vrijeme_mirovanja
            text: "Proteklo vrijeme mirovanja: "
            focus: true
            font.styleName: "Bold"
            font.pointSize: 16
            anchors.top: rectangle_postavke.top
            horizontalAlignment: Text.AlignHCenter
            bottomPadding: 10
            topPadding: 10
            anchors.left: rectangle_vrijeme.left
            anchors.right: rectangle_vrijeme.right
            anchors.leftMargin: 10
        }
        Label{
            id: label_broj_preostalog_vremena_mirovanja
            text: "14:22 min"
            focus: true
            font.styleName: "Bold"
            font.pointSize: 20
            anchors.top: label_proteklo_vrijeme_mirovanja.bottom
            horizontalAlignment: Text.AlignHCenter
            anchors.left: rectangle_vrijeme.left
            anchors.right: rectangle_vrijeme.right
            bottomPadding: 10
            topPadding: 5
        }

        Button {
            id: button_start_stop
            width: 80
            height: 60
            anchors.left: rectangle_vrijeme.left
            anchors.right: rectangle_vrijeme.right
            anchors.bottom: rectangle_vrijeme.bottom
            leftPadding: 4
            text: qsTr("START/STOP")
            font.pointSize: 20
            anchors.leftMargin: 30
            anchors.rightMargin: 30
            anchors.bottomMargin: 20
            onClicked: {
                //TODO: funkcija koja ce priomijenit start i stop tekst i upravaljt vremenom
            }
        }


    }




}
