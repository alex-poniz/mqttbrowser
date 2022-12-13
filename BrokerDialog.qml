import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

Dialog {
    id: brokerDialogID
    signal closedDialog(string hostname, int port)
    //property alias text : textContainer.text


    standardButtons: DialogButtonBox.Ok
    modal: true

    RowLayout {
        Text {
            text: "Host: "
        }

        TextEdit {
            id: hostEdit

            text: appController.hostname
            font.family: "Helvetica"
            font.pointSize: 20
            focus: true
            }

        Text {
            text: "Port: "
        }

        TextEdit {
            id: portEdit

            text: "1883"
            font.family: "Helvetica"
            font.pointSize: 20
            focus: true
        }
    }

    onClosed: {
        appController.hostname = hostEdit.text
        closedDialog(hostEdit.text, portEdit.text)
    }
}
