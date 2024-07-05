import QtQuick 2.4
import QtQuick.Window 2.0
import QtQuick.Layouts 1.0
//import QmlVlc 0.1
import QtMultimedia 5.0


import "./"
Rectangle {
    id: rootIncoming
    width: Screen.width/1.5
    height: Screen.height/1.5
    property string caller: "James"


    function setCaller(text){

            rootIncoming.caller = text


    }

    SystemPalette{
        id: pallette
    }

    Text{

        id: versionTxt
        anchors.bottom: incomingBtns.top
        anchors.bottomMargin: 40
        anchors.horizontalCenter: incomingBtns.horizontalCenter
        text: "Incoming Call From\n" + rootIncoming.caller
        font.pointSize: 24; font.bold: true
    }

    Column {
        id: incomingBtns
        anchors.centerIn: rootIncoming
       Row {

            spacing: 30
            Rectangle {
                id: answer
                width: rootIncoming.width/5
                height: rootIncoming.height/5
                color: "Green"
                Image {
                    id: answerpng
                    anchors.fill: parent
                    width: parent.width
                    height: parent.height
                    source: "qrc:///images/answer.png"
                }
                MouseArea {
                    anchors.fill: parent
                     onClicked: {
                         answer.color = "Blue";
                         sip.answerCurrentCall();
                     }
                }

                Text {
                    id: answerText
                    text: "Answer"

                    anchors.horizontalCenter: answerpng.horizontalCenter
                    anchors.top: answerpng.bottom
                    anchors.topMargin: 5
                    font.pointSize: 24; font.bold: true
                }
            }
            Rectangle {
                id: decline
                width: rootIncoming.width/5
                height: rootIncoming.height/5
                color: "white"

                MouseArea {
                    anchors.fill: parent
                     onClicked: {
                         //decline.color = "Yellow";
                         //videoLoader.source = "videoVLC.qml"
                         sip.endCurrentCallDecline()
                     }
                }
                Image {
                    id: declinepng
                    anchors.fill: parent
                    width: parent.width
                    height: parent.height
                    source: "qrc:///images/reject.png"
                }
                Text {
                    id: declineText
                    text: "Decline"

                    anchors.horizontalCenter: declinepng.horizontalCenter
                    anchors.top: declinepng.bottom
                    anchors.topMargin: 5
                    font.pointSize: 24; font.bold: true
                }

            }
        }
    }
}

