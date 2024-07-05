import QtQuick 2.4
import QtQuick.Window 2.0
import QtQuick.Layouts 1.0
//import QmlVlc 0.1
import QtMultimedia 5.0


import "./"
Rectangle {
    id: rootActive
    width: Screen.width/1.5
    height: Screen.height/1.5


    SystemPalette{
        id: pallette
    }


    Column {
        anchors.centerIn: rootActive

        Row {
            spacing: 20
            Rectangle {
                id: endCall
                width: rootActive.width/3
                height: rootActive.height/3
                color: "white"

                MouseArea {
                    anchors.fill: endCall
                     onClicked: {
                         //endCall.color = "Blue";
                         sip.endCurrentCall();

                         //sip.endCurrentCallMsg();
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
                    text: "End Call"

                    anchors.horizontalCenter: declinepng.horizontalCenter
                    anchors.top: declinepng.bottom
                    anchors.topMargin: 5
                    font.pointSize: 24; font.bold: true
                }
            }

        }
    }
}

