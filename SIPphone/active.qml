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
        anchors.horizontalCenter: rootActive.horizontalCenter
        anchors.top: rootActive.top
        anchors.topMargin: 50
        spacing: 40
        Rectangle {
            id: videoView
//            width: 480
//            height: 272
            width: rootActive.width*2/3
            height: rootActive.height/2
            color: "Black"
            anchors.horizontalCenter: rootActive.horizontalCenter



//            MediaPlayer {
//                    id: player
//                    source: "http://download.blender.org/peach/bigbuckbunny_movies/big_buck_bunny_480p_stereo.avi"
//                    //source: "/data/data/org.qtproject.example.SIPphone/files/test.m4v"
//                    //source: "file:///home/james/.local/share/test.m4v"
//                    autoPlay: true
//                    //metaData.videoCodec : "video/MPEG4-ES 9000"
//                }

//                VideoOutput {
//                    id: videoOutput
//                    source: player
//                    anchors.fill: parent
//                }
//            Loader {
//                id: videoLoader
//                width: videoView.width
//                height: videoView.height
//                //sourceComponent: compDrag
//                anchors.fill: videoView



//           }

        }
        Row {
            id: activeBtms
            spacing: 40
            anchors.horizontalCenter: parent.horizontalCenter
            Rectangle {
                id: endCall
                width: rootActive.width/5
                height: rootActive.height/5
                color: "white"

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
                    font.pixelSize: declinepng.width/6; font.bold: true
                }

                MouseArea {
                    anchors.fill: endCall
                     onClicked: {
                         //endCall.color = "Blue";
                         sip.endCurrentCall();
                         //sip.endCurrentCallMsg();
                     }
                }


            }
            Rectangle {
                id: openDoor
                width: rootActive.width/5
                height: rootActive.height/5
                color: "white"


                Image {
                    id: openpng
                    anchors.fill: parent
                    width: parent.width
                    height: parent.height
                    source: "qrc:///images/logout13.png"
                }
                Text {
                    id: openText
                    text: "Open Door"

                    anchors.horizontalCenter: openpng.horizontalCenter
                    anchors.top: openpng.bottom
                    anchors.topMargin: 5
                    font.pixelSize: openpng.width/6; font.bold: true
                }
                MouseArea {
                    anchors.fill: parent
                     onClicked: {
                         //openDoor.color = "Yellow";
                         //videoLoader.source = "videoVLC.qml"
                         sip.opendoorCurrentCall()
                     }
                }


            }
        }
    }
}

