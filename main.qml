import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

ApplicationWindow {
    id: mainWindow
    width: 800
    height: 600
    visible: true
    title: qsTr("Test")

    Component {
           id: myTabButton
           TabButton {
               text: qsTr("Activity")
          }
    }

    Component {
           id: mySubLayout
           SubLayout {y: 100; width: parent.width}
    }

    function topicDialogClosed(topicname)
    {
        console.log("topicDialogClosed: begin")

        if (appController.subscribe(topicname)) {
            var newSubLayout = mySubLayout.createObject(tabsStack);
            newSubLayout.y = 100;
            tabsStack.data.push(newSubLayout);
            appData.addTopic(topicname);


            var newTab = myTabButton.createObject(bar, {"text": topicname} );
            //bar.addItem(newTab);
            //newTab.clicked();
            //newtab.toggled();
            //newTab.activeFocus = true;
            //bar.currentIndexChanged();
            bar.setCurrentIndex(1)
            //bar.
            tabsStack.update();
            console.log("topicDialogClosed: end")
        }
    }

    function brokerDialogClosed(hostname, port)
    {
        console.log("brokerDialogClosed: host: " + hostname + " port: " + port)
        //appController.setHostname(hostname);
        appController.setPort(port);
        appController.connectToBroker();

            /*
        const newTab = myTabButton.createObject(bar, {"text": topicname} );
        bar.addItem(newTab);

        var newSubLayout = mySubLayout.createObject(tabsStack);
        newSubLayout.y = 100;
        tabsStack.data.push(newSubLayout);
        */
    }

    function showTopicDialog(message) {
        var component = Qt.createComponent("TopicDialog.qml")
        if (component.status === Component.Ready) {
            var dialog = component.createObject(mainWindow)

            dialog.title = qsTr("MQTT topic");
            dialog.text = message;
            dialog.closedText.connect(topicDialogClosed)

            dialog.open()
        } else
            console.error(component.errorString())
    }

    function showBrokerDialog() {
        var component = Qt.createComponent("BrokerDialog.qml")
        if (component.status === Component.Ready) {
            var dialog = component.createObject(mainWindow)

            dialog.title = qsTr("MQTT broker");
            //dialog.text = message;
            dialog.closedDialog.connect(brokerDialogClosed)

            dialog.open()
        } else
            console.error(component.errorString())
    }

    menuBar: MenuBar {
           Menu {
               title: qsTr("&File")
               Action {
                   text: qsTr("&Connect...")
                   onTriggered: showBrokerDialog();
               }

               Action {
                   text: qsTr("&Disconnect...")
                   onTriggered: disconnect('');
                   enabled: appController.isConnected
               }

               Action {
                   text: qsTr("&Add...")
                   onTriggered: showTopicDialog('');
                   enabled: appController.isConnected
               }
               MenuSeparator { }
               Action {
                   text: qsTr("&Quit")
                   onTriggered: mainWindow.close()
               }
           }
           Menu {
               title: qsTr("&Help")
               Action { text: qsTr("&About") }
           }
       }

    ColumnLayout {
        width: parent.width
        height: parent.height

        RowLayout {
            spacing: 10
            width: parent.width

            Text {
                text: "MQTT broker: "
            }

            Text {
                text: appController.hostname
            }

            Rectangle {
                width: 20
                height:20

                anchors.right: tabsStack.right
                radius: 10
                color: appController.isConnected ? "green" : "red"
            }
        }

    ListModel {
        id:listModel
    }

    TabBar {
        id: bar
        Layout.fillWidth: true
        width: parent.width

        /*
        TabButton {
            text: qsTr("Home")
        }
        TabButton {
            text: qsTr("Discover")
        }
        TabButton {
            text: qsTr("Activity")
        } */
    }

    StackLayout {
        id: tabsStack
        //y: bar.height

        width: parent.width
        height: parent.height

        currentIndex: bar.currentIndex
        //currentIndex: 0
        //data: [

        //    SubLayout{y: bar.height; width: parent.width},
        //    SubLayout{y: bar.height; width: parent.width},
        //    SubLayout{y: bar.height; width: parent.width}
        //    ]
        Repeater {
            model: appData.responses
            //model: appController.responsesList
            //SubLayout{y: bar.height; width: parent.width; textOutput: modelData}
            ColumnLayout{
                spacing: 2
                Layout.fillWidth: true
                width: parent.width
                height: parent.height

                    TextArea {
                        id: input
                        //Layout.alignment: Qt.AlignCenter
                        Layout.preferredHeight: 100
                        Layout.fillWidth: true
                        width: parent.width
                        wrapMode: TextEdit.Wrap
                        text: "<b>Hello</b> <i>World!</i>"
                        //font.family: "Helvetica"
                        //font.pointSize: 20
                        //color: "blue"
                        focus: true
                        background: Rectangle {
                            border.color: "black"
                        }
                    }

                    TextArea {
                        id: output
                        //Layout.alignment: Qt.AlignCenter
                        //Layout.preferredHeight: 40
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        width: parent.width
                        //height: 500
                        readOnly: true
                        wrapMode: TextEdit.Wrap
                        textFormat: "RichText"
                        text: modelData

                        //font.family: "Helvetica"
                        //font.pointSize: 20

                        //color: "blue"
                        focus: false

                        background: Rectangle {
                            border.color: "black"
                        }
                    }
            }

        }
    }

    }
}
