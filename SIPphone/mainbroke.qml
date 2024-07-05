import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2

Window {
    id: mainWindow
    width: Screen.width
    height: Screen.height
    visible: true



    Rectangle{
        id: recSkin
        width: mainWindow.width * 2/3
        height: mainWindow.height * 2/3
        radius: 20
        anchors.centerIn: parent
        property int rowHeight: recSkin.height/8
        gradient: Gradient {
            GradientStop { position: 0.0; color: "lightsteelblue" }
            GradientStop { position: 1.0; color: "blue" }
        }

        Row{
            id:rowTop
            anchors.horizontalCenter: recSkin.horizontalCenter
            anchors.top: recSkin.top
            anchors.topMargin: 20
            height: recSkin.rowHeight
            spacing: 20
            ComboBox {
                editable: false
                height: recSkin.rowHeight
                model: ListModel {
                    id: model
                    ListElement { text: "Site1"}
                    ListElement { text: "Site2"}
                    ListElement { text: "Site3" }
                }
                onAccepted: {
                    if (find(currentText) === -1) {
                        model.append({text: editText})
                        currentIndex = find(editText)
                    }
                }
            }

            TextField {
                id: inputField
                anchors.horizontalCenter: parent.horizontalCenter

                height: recSkin.rowHeight
                readOnly: true
                anchors.topMargin: 20
                text: "123"
                onAccepted: focus = false

                Keys.onEscapePressed: undo()
            }
        }

        Row {
            id: keyRow1
            anchors.horizontalCenter: recSkin.horizontalCenter
            anchors.top: rowTop.bottom
            anchors.topMargin: 20
            height: recSkin.rowHeight
            spacing: 20
            Button {
                id: button1
                text: qsTr("182")
                onClicked: keyEmitter.emitKey(Qt.Key_1)
                height: recSkin.rowHeight
            }
            Button {
                id: button2
                text: qsTr("2")
                onClicked: keyEmitter.emitKey(Qt.Key_2)
                height: recSkin.rowHeight
            }
            Button {
                id: button3
                text: qsTr("3")
                onClicked: keyEmitter.emitKey(Qt.Key_3)
                height: recSkin.rowHeight
            }
        }
        Row {
            id: keyRow2
            anchors.horizontalCenter: recSkin.horizontalCenter
            anchors.top: keyRow1.bottom
            anchors.topMargin: 20
            spacing: 20
            height: recSkin.rowHeight
            Button {
                id: button4
                text: qsTr("4")
                onClicked: keyEmitter.emitKey(Qt.Key_4)
                height: recSkin.rowHeight
            }
            Button {
                id: button5
                text: qsTr("5")
                onClicked: keyEmitter.emitKey(Qt.Key_5)
                height: recSkin.rowHeight
            }
            Button {
                id: button6
                text: qsTr("6")
                onClicked: keyEmitter.emitKey(Qt.Key_6)
                height: recSkin.rowHeight
            }
        }
        Row {
            id: keyRow3
            anchors.horizontalCenter: recSkin.horizontalCenter
            anchors.top:  keyRow2.bottom
            anchors.topMargin: 20
            height: recSkin.rowHeight
            spacing: 20
            Button {
                id: button7
                text: qsTr("7")
                onClicked: keyEmitter.emitKey(Qt.Key_7)
                height: recSkin.rowHeight

            }
            Button {
                id: button8
                text: qsTr("8")
                onClicked: keyEmitter.emitKey(Qt.Key_8)
                height: recSkin.rowHeight
            }
            Button {
                id: button9
                text: qsTr("9")
                onClicked: keyEmitter.emitKey(Qt.Key_9)
                height: recSkin.rowHeight
            }
        }
        Row {
            id: keyRow4
            anchors.horizontalCenter: recSkin.horizontalCenter
            anchors.top: keyRow3.bottom
            anchors.topMargin: 20
            height: recSkin.rowHeight
            spacing: 20
            Button {
                id: buttonBack
                text: qsTr("CALL")
                onClicked: keyEmitter.emitKey(Qt.Key_Backspace)
                height: recSkin.rowHeight
            }
            Button {
                id: buttonEnter
                text: qsTr("0")
                onClicked: keyEmitter.emitKey(Qt.Key_0)
                height: recSkin.rowHeight
            }
            Button {
                id: buttonEsc
                text: qsTr("DEL")
                onClicked: keyEmitter.emitKey(Qt.Key_Backspace)
                height: recSkin.rowHeight
            }
        }
    }
}
