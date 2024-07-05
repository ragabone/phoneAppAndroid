import QtQuick 2.2
import QtMultimedia 5.0
import Qt.labs.folderlistmodel 2.1




Rectangle {
    id:comboBox
    width: chosenItemText.width
    property color homeColor: "grey"
    property string ringtone: ""
    color: homeColor
    signal comboClicked;
    z: 0
    smooth:true;

    function playRingTone() {
        playMusic.loops = Audio.Infinite
        playMusic.play()

    }

    function stopRingTone() {
        playMusic.loops = 1
        playMusic.stop()

    }

    MouseArea {
        anchors.fill: comboBox
        hoverEnabled: true
        onEntered: {
            comboBox.color = Qt.lighter(comboBox.homeColor,1.1)
        }

        onExited: {
            comboBox.color = comboBox.homeColor

        }
    }

    Audio {
        id: playMusic
        source: "ringtones/default"

    }


    Text {
        id:chosenItemText
        anchors.top: parent.top;
        anchors.margins: 8;
        color: Qt.darker(comboBox.color,2.4)
        text:"Ring Tone";
        anchors.centerIn: parent
        smooth:true


        MouseArea {
            anchors.bottomMargin: 0
            anchors.fill: parent;
            onClicked: {
                if(comboBox.state==="dropDown"){
                    filehandle.copyFileChangeName1(comboBox.ringtone,"default")
                }
                comboBox.state = comboBox.state==="dropDown"?"":"dropDown"
            }
        }
    }

    Rectangle {
        id:dropDown
        radius:4;
        width:colModel.width;
        height: listView.height
        anchors.right: chosenItemText.left;
        anchors.bottom: chosenItemText.bottom;
        anchors.margins: 2;
        //clip: true
        color: "lightblue"
        Flickable {
            id:listView
            width:colModel.width;
            height: 0
            contentHeight: listView.height*6
            anchors.bottom: dropDown.bottom;

            clip: true
            z:dropDown.z+1
            FolderListModel {
                id: folderModel
                folder: "ringtones/"
                nameFilters: ["*"]
            }
            Component {
                id: fileDelegate

                Text {
                    id: audioTxt
                    text: fileName
                    MouseArea {
                        id: textArea
                        anchors.fill: parent;
                        hoverEnabled: true
                        onEntered: {
                            audioTxt.color = "white"
                        }

                        onExited: {
                            audioTxt.color = "black"

                        }

                        onClicked: {
                            playMusic.source = folderModel.folder + audioTxt.text
                            playMusic.play()
                            comboBox.ringtone = playMusic.source;
                        }
                    }

                }


            }
            Column {
                id: colModel
                spacing: 10
                Repeater {

                    model: folderModel
                    delegate: fileDelegate

                }
            }
        }
    }

    states: State {
        name: "dropDown";
        PropertyChanges { target: listView; height:200 }
    }

    transitions: Transition {
        NumberAnimation { target: listView; properties: "height"; easing.type: Easing.OutExpo; duration: 1000 }
    }
}
