import QtQuick 2.4
import QtQuick.Window 2.0


import "./"
Rectangle {
    id: root
    width: Screen.width
    height: Screen.height

    property string indicator: "grey"
    property string regState: "Registering"

    property string currentEntry: ""

    function startVideoStreamUI(){
        pageLoader.item.startVideo()

    }

    function setDefaultMain() {
        doorIcon.opacity = 0
        doorIconflash.stop()
        recDoorIcon.color = barBtm.color
        hangupIcon.opacity = 0
        hangupIconflash.stop()
        recHangupIcon.color = barBtm.color
        pageLoader.source = ""

    }
    function playRingTone() {
        ring.playRingTone()
    }

    function stopRingTone() {
        ring.stopRingTone()
    }

    SystemPalette{
        id: pallette
    }

    Column {
        anchors.centerIn: root
        Row {
            spacing: 10
            Rectangle {
                id: mainPane
                width: root.width * 2/3
                height: root.height * 2/3
                color: Qt.darker(pallette.window,1.4)


                Loader {
                    id: pageLoader
                    width: mainPane.width-40
                    height: mainPane.height-20
                    anchors.centerIn: mainPane
                    source: "qrc:///make_call.qml"

                }

                Connections {
                    target: pageLoader.item

                    onCallActive :{
                        doorIcon.opacity = 1
                        hangupIcon.opacity = 1
                        sideLoader.sourceComponent = compDrag
                        root.currentEntry = ""
                        recParam.state = ""
                        console.log("sip.answerCurrentCall()")
                        sip.answerCurrentCall()

                    }
                }
            }
            //Bottom BAR
            Rectangle {
                id: barBtm
                width: root.width
                height: root.height/10
                color: Qt.darker(pallette.window,1.2)
                anchors.top: root.bottom
                //anchors.left: root.left
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
    }
}

