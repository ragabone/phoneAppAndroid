import QtQuick 2.4
//import QtQuick.Window 2.0
//import QtQuick.Layouts 1.0

//import QtQuick 2.1
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.0
import QtMultimedia 5.0
import VideoGL 1.0

Item {
    id: mainWindow
    width: Screen.width
    height: Screen.height
    visible: true
    state: "READY"
    property string indicator: "grey"
    property string regState: "Registering"
    property string regSettingFile: "register.xml"
    property double btmOpacity: 1
    property double activeButtonOpacity: 0






    Squircle {
        id: videoPlay
        isPlaying: false

    }





    function setCallRinging() {
        mainWindow.state = "RINGING"
        setCallState()


    }
    function playRingTone() {
        playMusic.loops = Audio.Infinite
        playMusic.play()

    }

    function stopRingTone() {
        playMusic.loops = 1
        playMusic.stop()

    }

    Audio {
        id: playMusic
        source: "ringtones/Pleasant1"

    }



    states: [

        State {
            name: "READY"
            // PropertyChanges { target: videoLoader; source: "qrc:///make_call.qml" }

        },
        State {
            name: "INCOMING"
            //PropertyChanges { target: videoLoader; source: "qrc:///incoming.qml" }

        },
        State {
            name: "ACTIVE"
            //PropertyChanges { target: videoLoader; source: "qrc:///active.qml" }

        }
    ]
    function setCallState(){
        videoLoader.item.setCallState()
    }
    function setReadyState(){
        videoLoader.item.setReadyState()
    }

    function setDoorFlash() {
        videoPlay.isPlaying = false;
        doorIcon.opacity = 1
        doorIconflash.start()
    }

    Timer {
        id: timer
    }

    function delay(delayTime, cb) {
        timer.interval = delayTime;
        timer.repeat = false;
        timer.triggered.connect(cb);
        timer.start();
    }

    function setBusyDefaultMain(text) {
        if(text === "Telephone")
            updateUI("User Busy ");
        else
            updateUI("Panel Busy ");
        delay(3000,setDefaultMain);

    }
    function setDefaultMain() {
        doorIcon.opacity = 0
        doorIconflash.stop()
        recDoorIcon.color = barBtm.color
        videoPlay.isPlaying = false;
        recSkin.opacity = 1.0;
        barBtm.opacity = 1.0;
        mainWindow.activeButtonOpacity = 0;

        //        hangupIcon.opacity = 0
        //        hangupIconflash.stop()
        //        recHangupIcon.color = barBtm.color
        console.log("setDefaultMain Ready ")
        stopRingTone()
        mainWindow.state = "READY"

        if(videoLoader.source != "qrc:///make_call.qml")
            videoLoader.setSource("qrc:///make_call.qml")
        setReadyState();
        updateUI("");


    }
    function setIncoming(text) {
        mainWindow.state = "INCOMING"
        videoLoader.setSource("qrc:///incoming.qml")
        videoLoader.item.setCaller(text);
        //ideoLoader.setSource("qrc:///incoming.qml")
    }
    function setActive() {
        mainWindow.state = "ACTIVE"
        stopRingTone()
        videoPlay.isPlaying = true;
        setActiveWindows()
        //videoLoader.setSource("qrc:///active.qml")

    }

    function setActiveWindows(){
        mainWindow.activeButtonOpacity = 1.0;
        mainWindow.btmOpacity=0;
        recSkin.opacity = 0;
        //barBtm.opacity = 0;

        //mainWindow.opacity = 0;


    }

    function setActiveTele() {
        mainWindow.state = "ACTIVE"
        stopRingTone()
        videoLoader.setSource("qrc:///activeTeleCall.qml")
    }

    function updateUI(text){
        console.log("updateUI " + text)
        videoLoader.item.setUIText(text)
    }

    function clrSite(){
        console.log("clrSite ")
        videoLoader.item.clrSite()
    }
    function addSite(text){
        console.log("addSite " + text)
        videoLoader.item.addSite(text)
    }
    function addSiteBuffer(text){
        console.log("addSiteBuffer " + text)
        videoLoader.item.addSite(text)
    }



    SystemPalette{
        id: pallette
    }

    function onMakeCallLoaded(siteText){
        addSite(siteText)
    }

    Rectangle{
        id: recSkin
        width: mainWindow.width
        height: mainWindow.height - barBtm.height
        radius: 20
        anchors.horizontalCenter: parent.horizontalCenter
        //        anchors.top: rowTop.bottom
        anchors.topMargin: 20
        property int rowHeight: recSkin.height/6
        property int buttonWidth: recSkin.width/5
        color: "Pink"
        //opacity: 0


        Loader {
            id: videoLoader
            anchors.fill: recSkin
            source: "qrc:///make_call.qml"
            //source: "qrc:///incoming.qml"
            //source: "qrc:///active.qml"
            //source: "qrc:///activeTeleCall.qml"

            //onLoaded: onMakeCallLoaded("Site2")
            //onStatusChanged: onMakeCallLoaded("Site4")

        }

        Loader {
            id: settingsLoader
            anchors.centerIn:recSkin
            width: parent.width/2
            height: parent.width/2
            source: ""
            opacity: 0.9

        }


    }


    //Bottom BAR
    Rectangle {
        id: barBtm
        width: mainWindow.width
        height: mainWindow.height/12
        color: Qt.darker(pallette.window,1.2)
        anchors.bottom: mainWindow.bottom
        anchors.left: mainWindow.left
//        opacity: btmOpacity
//        opacity: mainWindow.btmOpacity
        onFocusChanged: console.log("barBtm Focus changed " + focus)
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
                //anchors.left : btmRow.left
                height: barBtm.height
                width: regLabel.width
                color: barBtm.color


                Text {
                    id: regLabel
                    text: regState


                    color: Qt.darker(barBtm.color,2.4)
                    state: "home"
                    anchors.centerIn: recRegLabel
                    font.pointSize: 16

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
                        if (regLabel.state == "edit"){
                            regLabel.state = "home"

                            settingsLoader.source = ""

                        }
                        else{
                            regLabel.state = "edit"
                            settingsLoader.source = "qrc:///RegSettings.qml"
                            settingsLoader.item.setXmlSource(regSettingFile)

                        }
                    }

                    anchors.fill: regLabel
                    anchors.margins: -5 // Make MouseArea bigger than the rectangle, itself
                }
            }
            Rectangle {

                id: barIndictor
                width: barBtm.height/2
                height: barBtm.height/2
                radius: barBtm.height/2
                gradient: Gradient {
                    GradientStop { position: 0.0; color: Qt.lighter(mainWindow.indicator,2) }
                    GradientStop { position: 1.0; color: mainWindow.indicator }
                }
                anchors.verticalCenter: btmRow.verticalCenter
                //anchors.left: recRegLabel.right
            }


        }


        Rectangle {
            id: exitBtn

            anchors.verticalCenter: barBtm.verticalCenter
            anchors.right: barBtm.right
            anchors.rightMargin: 10

            height: barBtm.height
            width: exitText.width*2

            Text {
                id: exitText
                anchors.centerIn: exitBtn
                text: "Exit -->"

            }
            MouseArea{
                anchors.fill: exitBtn
                onClicked: {
                    if(mainWindow.state == "READY"){
                        exitBtn.color="blue"
                        Qt.quit()

                    }
                }


            }



            color: barBtm.color


        }




        //        Image {
        //            id: settingImage
        //            anchors.verticalCenter: barBtm.verticalCenter
        //            anchors.right: barBtm.right
        //            anchors.rightMargin: 20
        //            source: "qrc:/images/settings.png"
        //            height: 60
        //            width: 60
        //        }

    }



    //Notification Bar
    //    Rectangle {
    //        id: recNotBar
    //        anchors.verticalCenter: barBtm.verticalCenter
    //        anchors.horizontalCenter: barBtm.horizontalCenter
    //        state: "hidden"
    //        color: "grey"
    //        height: barBtm.height
    //        opacity: 1.0
    //        width: barBtm.width*2/3
    //        Text {
    //            id: notText
    //            anchors.top: recNotBar.top
    //            anchors.topMargin: 20
    //            anchors.left: recNotBar.left
    //            anchors.leftMargin: 20

    //            text: "notifications"

    //        }
    //        MouseArea {
    //            hoverEnabled: true
    //            onEntered: {
    //                //recNotBar.state = "visible"

    //            }

    //            onExited: {
    //                recNotBar.state = "hidden"


    //            }
    //            anchors.fill: recNotBar

    //        }
    //        states: [

    //            State {
    //                name: "visible"
    //                PropertyChanges { target: recNotBar; opacity: 1.0; }

    //            },
    //            State {
    //                name: "hidden"
    //                PropertyChanges { target: recNotBar; opacity: 0; }

    //            }
    //        ]
    //        transitions: [
    //            Transition {
    //                to: "visible"
    //                NumberAnimation { properties: "opacity"; from: 0; to: 1.0; duration: 1000 }
    //            },
    //            Transition {
    //                to: "hidden"
    //                NumberAnimation { properties: "opacity"; from: 1.0; to: 0; duration: 1000 }
    //            }
    //        ]
    //    }






    Item {
        id: rootActive
        width: Screen.width/2
        height: Screen.height/4
        anchors.bottom: barBtm.top
        anchors.bottomMargin: 40
        opacity: 1


        anchors.horizontalCenter: parent.horizontalCenter
        Item {
            id: endCall
            width: rootActive.height/2
            height: endCall.width
            anchors.left: rootActive.left

            Image {
                id: declinepng
                anchors.fill: parent
                width: parent.width
                height: parent.width
                opacity: mainWindow.activeButtonOpacity
                source: "qrc:///images/reject.png"
            }
            Text {
                id: declineText
                text: "End Call"
                opacity: declinepng.opacity
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
        Item {
            id: openDoor
            width: endCall.width
            height: endCall.width
            anchors.right: rootActive.right


            Image {
                id: openpng
                anchors.fill: parent
                width: parent.width
                height: parent.width
                opacity: mainWindow.activeButtonOpacity
                source: "qrc:///images/logout13.png"
            }
            Text {
                id: openText
                text: "Open Door"
                opacity: openpng.opacity

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
    Rectangle {
        id: recDoorIcon
        height: barBtm.height
        width: barBtm.height + 20
        anchors.centerIn: barBtm
        //anchors.right: barBtm.right
        //anchors.leftMargin: barBtm.width/2 - btmRow.width
        //color: "blue"
        color: barBtm.color



        SequentialAnimation on color {
            id: doorIconflash
            running: false
            loops: Animation.Infinite
            ColorAnimation { to: "lightgreen"; duration: 1000 }
            ColorAnimation { from: "lightgreen"; to: barBtm.color; duration: 1000 }
        }

        Image {
            id: doorIcon
            height: recDoorIcon.height -5
            width: recDoorIcon.width - 5
            source: "images/logout13.png"
            opacity: 0
            anchors.centerIn: recDoorIcon
            //            Text {
            //                id: doortxt
            //                text: "Open Door"
            //                color: Qt.darker(barBtm.color,2.4)
            //                opacity: doorIcon.opacity
            //                anchors.left: doorIcon.right
            //                anchors.leftMargin: 20
            //                anchors.verticalCenter: doorIcon.verticalCenter

            //            }

        }
    }

}
