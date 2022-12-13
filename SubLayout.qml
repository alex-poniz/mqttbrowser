import QtQuick 2.15
import QtQuick.Layouts 2.15
import QtQuick.Controls 2.15

Item {


    ColumnLayout{
        spacing: 2
        Layout.fillWidth: true
        width: parent.width



            TextArea {
                id: input
                //Layout.alignment: Qt.AlignCenter
                Layout.preferredHeight: 40
                Layout.fillWidth: true
                width: parent.width
                wrapMode: TextEdit.Wrap
                text: "<b>Hello</b> <i>World!</i>"
                //font.family: "Helvetica"
                //font.pointSize: 20
                color: "blue"
                focus: true
                background: Rectangle {
                    border.color: "black"
                }
            }

            TextArea {
                id: output
                //Layout.alignment: Qt.AlignCenter
                Layout.preferredHeight: 40
                Layout.fillWidth: true
                width: parent.width
                wrapMode: TextEdit.Wrap
                text: "<b>Hello</b> <i>World!</i>"
                //font.family: "Helvetica"
                //font.pointSize: 20
                color: "blue"
                focus: true
                background: Rectangle {
                    border.color: "black"
                }
            }
    }
}

