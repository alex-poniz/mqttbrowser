import QtQuick 2.15
import QtQuick.Controls 2.15

Dialog {
    id: topicDialogID
    signal closedText(string text)
    property alias text : textContainer.text


    standardButtons: DialogButtonBox.Ok
    modal: true

    TextEdit {
        id: textContainer

        text: ""
        font.family: "Helvetica"
        font.pointSize: 20
        color: "blue"
        focus: true
    }

    onClosed: closedText(textContainer.text)
}
