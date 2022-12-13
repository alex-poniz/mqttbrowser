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
        console.log("topicDialogClosed")

        if (appController.subscribe(topicname)) {
            const newTab = myTabButton.createObject(bar, {"text": topicname} );
            bar.addItem(newTab);

            var newSubLayout = mySubLayout.createObject(tabsStack);
            newSubLayout.y = 100;
            tabsStack.data.push(newSubLayout);
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

    TabBar {
        id: bar
        width: parent.width

        TabButton {
            text: qsTr("Home")
        }
        TabButton {
            text: qsTr("Discover")
        }
        TabButton {
            text: qsTr("Activity")
        }
    }

    StackLayout {
        id: tabsStack
        //y: bar.height

        width: parent.width
        currentIndex: bar.currentIndex
        data: [
            SubLayout{y: bar.height; width: parent.width},
            SubLayout{y: bar.height; width: parent.width},
            SubLayout{y: bar.height; width: parent.width}
            ]
    }

    }
}
