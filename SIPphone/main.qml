import QtQuick 2.1
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.0

Rectangle {
    id: mainWindow
    width: Screen.width
    height: Screen.height
    visible: true

    property string indicator: "grey"
    property string regState: "Registering"

    SystemPalette{
        id: pallette
    }

    Row{
        id:rowTop
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 20
        height: 50
        spacing: 20
        ComboBox {
            editable: false
            height: rowTop.height
            width: recSkin.width/5
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
            height: rowTop.height
            width: 200
            placeholderText: "Enter Flat Number"

            focus: true
            //            onAccepted: focus = false
            onFocusChanged: keyEmitter.disableAndroidKB()
            //            Keys.onEscapePressed: undo()
            //            Keys.onEnterPressed: {
            //                    //...
            //                    inputMethod.hide()
            //                }
            //            Keys.onReturnPressed: {
            //                //...
            //                inputMethod.hide()
            //            }
            //onFocusChanged:  Qt.inputMethod.hide()
            inputMethodHints: Qt.ImhDigitsOnly

        }
    }
    Component {
      id: myButtonStyle
      ButtonStyle {

          background: Rectangle {
              implicitWidth: 100
              implicitHeight: 25
              border.width: control.activeFocus ? 2 : 1
              border.color: "#fff"
              radius: 4
              gradient: Gradient {
                  GradientStop { position: 0 ; color: control.pressed ? "#ccc" : "#fff" }
                  GradientStop { position: 1 ; color: control.pressed ? "#aaa" : "#ccc" }
              }
          }
      }


    }

    Rectangle{
        id: recSkin
        width: mainWindow.width * 2/3
        height: mainWindow.height * 2/3
        radius: 20
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: rowTop.bottom
        anchors.topMargin: 20
        property int rowHeight: recSkin.height/6
        property int buttonWidth: recSkin.width/5
        color: "light blue"

        Row {
            id: keyRow1
            anchors.horizontalCenter: recSkin.horizontalCenter
            anchors.top: recSkin.top
            anchors.topMargin: 20
            height: recSkin.rowHeight
            spacing: 20
            Button {
                id: button1
                text: qsTr("1")
                style: myButtonStyle
                onClicked: keyEmitter.emitKey(Qt.Key_1)
                height: recSkin.rowHeight
                width: recSkin.buttonWidth
            }
            Button {
                id: button2
                style: myButtonStyle
                text: qsTr("2")
                onClicked: keyEmitter.emitKey(Qt.Key_2)
                height: recSkin.rowHeight
                width: recSkin.buttonWidth
            }
            Button {
                id: button3
                style: myButtonStyle
                text: qsTr("3")
                onClicked: keyEmitter.emitKey(Qt.Key_3)
                height: recSkin.rowHeight
                width: recSkin.buttonWidth
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
                style: myButtonStyle
                text: qsTr("4")
                onClicked: keyEmitter.emitKey(Qt.Key_4)
                height: recSkin.rowHeight
                width: recSkin.buttonWidth
            }
            Button {
                id: button5
                text: qsTr("5")
                style: myButtonStyle
                onClicked: keyEmitter.emitKey(Qt.Key_5)
                height: recSkin.rowHeight
                width: recSkin.buttonWidth
            }
            Button {
                id: button6
                text: qsTr("6")
                style: myButtonStyle
                onClicked: keyEmitter.emitKey(Qt.Key_6)
                height: recSkin.rowHeight
                width: recSkin.buttonWidth
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
                style: myButtonStyle
                text: qsTr("7")
                onClicked: keyEmitter.emitKey(Qt.Key_7)
                height: recSkin.rowHeight
                width: recSkin.buttonWidth

            }
            Button {
                id: button8
                style: myButtonStyle
                text: qsTr("8")
                onClicked: keyEmitter.emitKey(Qt.Key_8)
                height: recSkin.rowHeight
                width: recSkin.buttonWidth
            }
            Button {
                id: button9
                style: myButtonStyle
                text: qsTr("9")
                onClicked: keyEmitter.emitKey(Qt.Key_9)
                height: recSkin.rowHeight
                width: recSkin.buttonWidth
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
                style: myButtonStyle
                text: qsTr("CALL")
                onClicked: keyEmitter.emitKey(Qt.Key_Backspace)
                height: recSkin.rowHeight
                width: recSkin.buttonWidth
            }
            Button {
                id: buttonEnter
                style: myButtonStyle
                text: qsTr("0")
                onClicked: keyEmitter.emitKey(Qt.Key_0)
                height: recSkin.rowHeight
                width: recSkin.buttonWidth
            }
            Button {
                id: buttonEsc
                text: qsTr("DEL")
                style: myButtonStyle
                onClicked: keyEmitter.emitKey(Qt.Key_Backspace)
                height: recSkin.rowHeight
                width: recSkin.buttonWidth
            }
        }
    }


    //Bottom BAR
    Rectangle {
        id: barBtm
        width: recSkin.width
        height: recSkin.height/10
        color: Qt.darker(pallette.window,1.2)
        anchors.bottom: recSkin.bottom
        anchors.left: recSkin.left
        Row {
            id: btmRow
            x: barBtm.x+10
            //y: barBtm.y+10
            spacing: 10
            anchors.verticalCenter: barBtm.verticalCenter
            anchors.left: barBtm.left
            anchors.leftMargin: 20
            Rectangle{
                id: recRegLabel
                anchors.verticalCenter: btmRow.verticalCenter
                height: barBtm.height
                width: regLabel.width
                color: barBtm.color
                Text {
                    id: regLabel
                    text: regState
                    color: Qt.darker(barBtm.color,2.4)
                    state: "home"
                    anchors.centerIn: recRegLabel

                }
                MouseArea {
//                    hoverEnabled: true
                    onEntered: {
                        recRegLabel.color = Qt.lighter(barBtm.color,1.1)
                    }

                    onExited: {
                        recRegLabel.color = barBtm.color

                    }
                    onClicked: {
                            console.log("onClicked")
                            registrar.registerClient()
                            //pageLoader.source = ""

                    }
                    anchors.fill: regLabel
                    anchors.margins: -5 // Make MouseArea bigger than the rectangle, itself
                }
            }
            Rectangle {

                id: barIndictor
                width: 20
                height: 20
                radius: 20
                gradient: Gradient {
                        GradientStop { position: 0.0; color: Qt.lighter(mainWindow.indicator,2) }
                        GradientStop { position: 1.0; color: mainWindow.indicator }
                    }
                anchors.verticalCenter: btmRow.verticalCenter
            }

        }


        Text {
            id: settingsTxt

            anchors.verticalCenter: settingImage.verticalCenter
            anchors.right: settingImage.left
            anchors.rightMargin: 20
            text: "Settings"

            color: Qt.darker(barBtm.color,2.4)


        }




        Image {
            id: settingImage
            anchors.verticalCenter: barBtm.verticalCenter
            anchors.right: barBtm.right
            anchors.rightMargin: 20
            source: "qrc:/images/settings.png"
            height: 60
            width: 60
        }

    }


    //Notification Bar
    Rectangle {
        id: recNotBar
        anchors.verticalCenter: barBtm.verticalCenter
        anchors.horizontalCenter: barBtm.horizontalCenter
        state: "hidden"
        color: "grey"
        height: barBtm.height
        opacity: 1.0
        width: barBtm.width*2/3
        Text {
            id: notText
            anchors.top: recNotBar.top
            anchors.topMargin: 20
            anchors.left: recNotBar.left
            anchors.leftMargin: 20

            text: "notifications"

        }
        MouseArea {
//            hoverEnabled: true
            onEntered: {
                recNotBar.state = "visible"
            }

            onExited: {
                recNotBar.state = "hidden"

            }
            anchors.fill: recNotBar

        }
        states: [

            State {
                name: "visible"
                PropertyChanges { target: recNotBar; opacity: 1.0; }

            },
            State {
                name: "hidden"
                PropertyChanges { target: recNotBar; opacity: 0; }

            }
        ]
        transitions: [
            Transition {
                to: "visible"
                NumberAnimation { properties: "opacity"; from: 0; to: 1.0; duration: 1000 }
            },
            Transition {
                to: "hidden"
                NumberAnimation { properties: "opacity"; from: 1.0; to: 0; duration: 1000 }
            }
        ]
    }



}
