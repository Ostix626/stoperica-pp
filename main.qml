import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.5
import QtQuick.Window 2.3
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.3

import ToDo 1.0



Window {
    id: window
    width: 700
    height: 960
    minimumHeight: 600
    minimumWidth: 650
    visible: true
//    color: "#e9d5a7"
//    color: "#dee6fc"

    title: qsTr("Štoperica++")
    function time(sekunde) {
        var h = parseInt(sekunde / 3600)
        var m = parseInt((sekunde - (h * 3600))/ 60)
        var s = sekunde % 60
        if (h < 10) h = "0" + h
        if (m < 10) m = "0" + m
        if (s < 10) s = "0" + s
        var formated = h + ":" + m + ":" + s
        return formated;
    }

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
            width: 320
            height: rectangle_postavke.height
            color: "#00ffffff"

            Label{
                id: label_satnica
//                x: 0
//                y: 0
                text: "Satnica (EUR): " + _satnicaData.cijenaSata
                font.styleName: "Bold"
                font.pointSize: 16
                anchors.top: rectangle_satnica.top
//                bottomPadding: 10
                topPadding: 10
                anchors.left: rectangle_satnica.left
                anchors.leftMargin: 10
            }

            TextField {
                id: textField_satnica
                width: 240
//                height: 32
                anchors.top: label_satnica.bottom
                validator: DoubleValidator {bottom: 0.01; top: 9999.0;}
                text: "" + _satnicaData.cijenaSata.toFixed(2)
                hoverEnabled: false
                font.pointSize: 14
                leftPadding: 17
                leftInset: 10
                placeholderText: qsTr("Napisi cijenu radnog sata")
            }

            Button {
                id: button_save_satnica
//                width: 50
//                height: 32
                anchors.left: textField_satnica.right
                anchors.top: label_satnica.bottom
                leftPadding: 4
                text: qsTr("Save")
                font.pointSize: 14
                anchors.leftMargin: 20
                Material.background: "#9cf5ff"
                onClicked: {
                    if(textField_satnica.text != "")
                    {
                        _satnicaData.cijenaSata = parseFloat(textField_satnica.text).toFixed(2)
                    }
                    label_satnica.text = "Satnica (EUR): " + _satnicaData.cijenaSata.toFixed(2)
//                    label_satnica.text = "Satnica (EUR): " + textField_satnica.text
                }
            }

            // VRIJEME
            Label{
                id: label_vrijeme_mirovanja
                text: "Vrijeme mirovanja (min): " + (_satnicaData.idleTimeSeconds / 60)
                font.styleName: "Bold"
                font.pointSize: 16
                anchors.top: textField_satnica.bottom
//                bottomPadding: 10
                topPadding: 4
                anchors.left: rectangle_satnica.left
                anchors.leftMargin: 10
            }

            TextField {
                id: textField_vrijeme_mirovanja
                width: 240
//                height: 32
                anchors.top: label_vrijeme_mirovanja.bottom
                validator: IntValidator {bottom: 1;}
                text: "" + (_satnicaData.idleTimeSeconds / 60)
                hoverEnabled: false
                font.pointSize: 14
                leftPadding: 17
                leftInset: 10
                placeholderText: qsTr("Napisi minute mirovanja")
            }

            Button {
                id: button_save_vrijeme_mirovanja
//                width: 50
//                height: 32
                anchors.left: textField_vrijeme_mirovanja.right
                anchors.top: label_vrijeme_mirovanja.bottom
                leftPadding: 4
                text: qsTr("Save")
                font.pointSize: 14
                anchors.leftMargin: 20
                Material.background: "#9cf5ff"
                onClicked: {
                    if(textField_vrijeme_mirovanja.text != "")
                    {
                        _satnicaData.idleTimeSeconds = parseInt(textField_vrijeme_mirovanja.text) * 60
                    }
                    label_vrijeme_mirovanja.text = "Vrijeme mirovanja (min): " + _satnicaData.idleTimeSeconds / 60

                    if (textField_vrijeme_mirovanja.text !== "" ) {
                        label_broj_preostalog_vremena_mirovanja.preostalo_vrijeme_mirovanja = _satnicaData.idleTimeSeconds
                    }
                    else {
                        label_broj_preostalog_vremena_mirovanja.preostalo_vrijeme_mirovanja = 66
                    }
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
            property int preostalo_vrijeme_mirovanja: _satnicaData.idleTimeSeconds
            text: parseInt(preostalo_vrijeme_mirovanja / 60) + ":" + preostalo_vrijeme_mirovanja % 60 + " min"
//            text: "14:22"
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
            anchors.leftMargin: 30
            anchors.rightMargin: 30
            anchors.bottomMargin: 20
            leftPadding: 4
            text: qsTr("START")
            Material.background: button_start_stop.text == qsTr("START")? "#86f9b9" : "#fe6778"
            font.pointSize: 20
            onClicked: {
                if (timer_odbrojaavnje.running === true)
                {
                    timer_odbrojaavnje.running = false
                    button_start_stop.text = qsTr("START")
                }
                else if (timer_odbrojaavnje.running === false)
                {
                    timer_odbrojaavnje.running = true
                    button_start_stop.text = qsTr("STOP")

                }
            }
        }
    }

    Rectangle {
        id: rectangle_stranke
//        color: "#eff3ff"
//        color: "#d2e7e8"
        anchors {
            fill: parent
            topMargin: rectangle_postavke.height + 20
            margins: 10
        }

//        ToDoList {
//            anchors {
//                fill: parent
//                margins: 10
//            }
//        }
        ColumnLayout {
            anchors {
                fill: parent
                margins: 10
            }
            Frame {
                Layout.fillWidth: true
                anchors.fill: parent

                Dialog {
                    anchors.centerIn: parent
                    id: confirm_dialog
                    width: 400
                    height: 100
                    title: "Želite li sigurno izbrisati označene stranke?"
                    standardButtons: Dialog.No | Dialog.Yes
                    onAccepted: toDoList.removeCompletedItems()
                }


                RowLayout {
                    id: rowlayout_buttons
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: 50
                    spacing: 10
                    anchors.leftMargin: 10
                    anchors.rightMargin: 10

                    Button {
                        text: qsTr("    Dodaj novu stranku    ")
                        onClicked: toDoList.appendItem()
                        Layout.fillWidth: true
                        font.pointSize: 12
                        Material.background: "#86f9b9"
                    }
                    Button {
                        text: qsTr("Izbriši označene stranke")
//                        onClicked: toDoList.removeCompletedItems()
                        onClicked: confirm_dialog.open()
                        Layout.fillWidth: true
                        font.pointSize: 12
                        Material.background: "#fe6778"
                    }

                }

                ListView {
                    anchors.top: rowlayout_buttons.bottom
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.leftMargin: 10
                    clip: true

                    model: ToDoModel {
                        list: toDoList
                    }

                    delegate: RowLayout {
                        width: parent.width
//                        height: 34



                        TextField {
                            text: model.description
                            onEditingFinished: model.description = text
                            Layout.fillWidth: true
                            font.pointSize: 14
                            leftPadding: 10
                            color: checkbox_aktivna_stranka.checked ? "#e91e63" : ""
                        }
                        TextField {
                            id: textfield_izracunato_vrijeme
                            visible:false
                            text: model.time
                            readOnly : true
                            onEditingFinished: model.time = text
//                            Layout.fillWidth: true
                        }
                        Label {
                            id: label_formatirano_vrijeme
                            text: window.time(model.time)
                            font.pointSize: 14
                            color: checkbox_aktivna_stranka.checked ? "#e91e63" : ""

                            leftPadding: 50
                            rightPadding: 50

                        }

                        CheckBox {
                            id: checkbox_aktivna_stranka

                            checked: model.done
                            onClicked: model.done = checked
                        }
                        Timer {
                            id: timer_racunanj_vremena_stranke
                            interval: 1000
                            repeat: true
                            running: true
                            property int oduzmi_vrijeme: 1;
                            onTriggered: {

//                                console.log(textfield_izracunato_vrijeme.text)
                                if (checkbox_aktivna_stranka.checked == true
                                        && timer_odbrojaavnje.running == true
                                        && label_broj_preostalog_vremena_mirovanja.preostalo_vrijeme_mirovanja > 0)
                                {
                                    textfield_izracunato_vrijeme.text = parseInt(textfield_izracunato_vrijeme.text) + 1
                                    label_formatirano_vrijeme.text = window.time(textfield_izracunato_vrijeme.text)
                                    oduzmi_vrijeme = 1
                                }
                                else if(checkbox_aktivna_stranka.checked == true
                                        && timer_odbrojaavnje.running == true
                                        && label_broj_preostalog_vremena_mirovanja.preostalo_vrijeme_mirovanja == 0
                                        && oduzmi_vrijeme == 1)

                                {
                                    textfield_izracunato_vrijeme.text = parseInt(textfield_izracunato_vrijeme.text) - _satnicaData.idleTimeSeconds
                                    label_formatirano_vrijeme.text = window.time(textfield_izracunato_vrijeme.text)
                                    oduzmi_vrijeme = 0
                                }

                            }
                        }

                    }
                }
            }


        }


//        ListModel {
//            id: model_stranke
//            ListElement { ime: "goldfren" ; vrijeme: "4:34:33" }
//            ListElement { ime: "goldfren" ; vrijeme: "4:34:33" }
//            ListElement { ime: "goldfren" ; vrijeme: "4:34:33" }
//            ListElement { ime: "goldfren" ; vrijeme: "4:34:33" }
//            ListElement { ime: "goldfren" ; vrijeme: "4:34:33" }
//            ListElement { ime: "goldfren" ; vrijeme: "4:34:33" }
//            ListElement { ime: "goldfren" ; vrijeme: "4:34:33" }
//            ListElement { ime: "goldfren" ; vrijeme: "4:34:33" }
//        }

//        Component {
//            id: component_stranke_delegate
//            Row {
//                Text {
//                    text: model.ime
//                }
//                Text {
//                    text: model.vrijeme
//                }
//            }
//        }

//        Column {
//            anchors.fill: parent
//            Repeater {
//                model: model_stranke
//                delegate: component_stranke_delegate
//            }
//        }

       ////////////////

//        Keys.onPressed: (event)=> { if (event.key == Qt.Key_Enter) console.log("event.8key"); }


//        Keys.onPressed: {
//            console.log(".key")
//            if (event.key === Qt.Key_Left){

//                console.log("event.key");
//            }
//        }
    }


    Timer {
        id: timer_odbrojaavnje
        property int last_reset: 0;
        interval: 1000
        repeat: true
        running: false
        onTriggered: {

//            console.log(label_broj_preostalog_vremena_mirovanja.preostalo_vrijeme_mirovanja)
            if (label_broj_preostalog_vremena_mirovanja.preostalo_vrijeme_mirovanja > 0) {
                label_broj_preostalog_vremena_mirovanja.preostalo_vrijeme_mirovanja -= 1
            }
//            else {
//                //TODO: funkcija oduzet vrijeme na aktivnoj stranci
//            }

            if (last_reset != _source.press)
            {
                label_broj_preostalog_vremena_mirovanja.preostalo_vrijeme_mirovanja = _satnicaData.idleTimeSeconds
                last_reset = _source.press
            }
        }
    }


}
