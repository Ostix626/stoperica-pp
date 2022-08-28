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
                text: "Satnica (EUR): " + _satnicaData.cijenaSata
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
                width: 50
                height: 32
                anchors.left: textField_satnica.right
                anchors.top: label_satnica.bottom
                leftPadding: 4
                text: qsTr("Save")
                font.pointSize: 14
                anchors.leftMargin: 5
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
                width: 50
                height: 32
                anchors.left: textField_vrijeme_mirovanja.right
                anchors.top: label_vrijeme_mirovanja.bottom
                leftPadding: 4
                text: qsTr("Save")
                font.pointSize: 14
                anchors.leftMargin: 5
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
            text: qsTr("START/STOP")
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
        color: "#eff3ff"
        anchors {
            fill: parent
            topMargin: rectangle_postavke.height + 20
            margins: 10
        }

        ListModel {
            id: model_stranke
            ListElement { ime: "goldfren" ; vrijeme: "4:34:33" }
            ListElement { ime: "goldfren" ; vrijeme: "4:34:33" }
            ListElement { ime: "goldfren" ; vrijeme: "4:34:33" }
            ListElement { ime: "goldfren" ; vrijeme: "4:34:33" }
            ListElement { ime: "goldfren" ; vrijeme: "4:34:33" }
            ListElement { ime: "goldfren" ; vrijeme: "4:34:33" }
            ListElement { ime: "goldfren" ; vrijeme: "4:34:33" }
            ListElement { ime: "goldfren" ; vrijeme: "4:34:33" }
        }

        Component {
            id: component_stranke_delegate
            Row {
                Text {
                    text: model.ime
                }
                Text {
                    text: model.vrijeme
                }
            }
        }

        Column {
            anchors.fill: parent
            Repeater {
                model: model_stranke
                delegate: component_stranke_delegate
            }
        }

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
        running: true
        onTriggered: {

//            console.log(label_broj_preostalog_vremena_mirovanja.preostalo_vrijeme_mirovanja)
            if (label_broj_preostalog_vremena_mirovanja.preostalo_vrijeme_mirovanja > 0) {
                label_broj_preostalog_vremena_mirovanja.preostalo_vrijeme_mirovanja -= 1
            }
            else {
                //TODO: funkcija oduzet vrijeme na aktivnoj stranci
            }

            if (last_reset != _source.press)
            {
                label_broj_preostalog_vremena_mirovanja.preostalo_vrijeme_mirovanja = _satnicaData.idleTimeSeconds
                last_reset = _source.press
            }
        }
    }


}
