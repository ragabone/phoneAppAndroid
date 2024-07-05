import QtQuick 2.1
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.0





Rectangle {
    id: makeCall
    width: Screen.width
    height: Screen.height
    visible: true
    focus: true
    property string indicator: "grey"
    property string regState: "Registering"
    state: "idle"






    SystemPalette{
        id: pallette
    }

    function setCallState(){
        makeCall.state = "calling"
        dialSkin.state = "calling"
    }

    function setReadyState(){
        makeCall.state = "idle"
    }

    function callButtonClicked(){

        if(dialSkin.state == "ready"){
            inputField.state = "Active"
            sip.make_call(siteBox.currentText,typeBox.currentText, inputField.text)

            //setUIText("CALLING..")

        }
        else if(dialSkin.state == "calling"){

            setUIText("CANCELLING")
            makeCall.state = "cancelling"
            sip.endCurrentCall();

        }
        console.log("callButtonClicked " + inputField.length)
        inputField.focus = true
    }

    function setUIText(text){
        console.log("setUIText")
        if(inputField.state == "Active"){
            inputField.text = text
        }

    }

    function textChanged(){
        //console.log("textChanged " + inputField.length)
        if(inputField.length > 0){
            if(makeCall.state == "idle"){
                dialSkin.state = "ready"
            }
            else if(makeCall.state == "calling"){
                dialSkin.state = "calling"
            }
            else
                dialSkin.state = "notready"
        }
        else
            dialSkin.state = "notready"
    }

    function textFocusChanged(key){

        inputField.forceActiveFocus()
        keyEmitter.emitKey(key)
    }
    function clrSite(){
        model.clear()
        addSite("Local")
    }


    function addSite(text){

        model.append({"text": text})
    }




    Rectangle {
        id:rowTop
        anchors.horizontalCenter: dialSkin.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 20

        anchors.leftMargin: 100
        height: Screen.height/15
        width: Screen.width
        color: "white"


        ComboBox {
            id: typeBox
            editable: false
            height: rowTop.height
            width: rowTop.width/6
            anchors.right: inputField.left
            anchors.rightMargin: 20
            anchors.verticalCenter: rowTop.verticalCenter
            model: ListModel {
                id: model

                ListElement { text: "Telephone"}
                ListElement { text: "Entrance" }


            }
            onAccepted: {
                if (find(currentText) === -1) {
                    model.append({text: editText})
                    currentIndex = find(editText)

                }
            }
        }
        ComboBox {
            id: siteBox
            editable: false
            height: rowTop.height
            width: rowTop.width/8
            anchors.right: typeBox.left
            anchors.rightMargin: 20

            anchors.verticalCenter: rowTop.verticalCenter
            model: myRegisterModel.comboList
//            model: ListModel {
//                id: model

//                ListElement { text: "Local"}


//            }


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

            width: rowTop.width/2
            anchors.horizontalCenter: rowTop.horizontalCenter
            anchors.horizontalCenterOffset: rowTop.width/6
            anchors.rightMargin: 20
            anchors.verticalCenter: rowTop.verticalCenter
            placeholderText: "Enter Extension No"
            state: "Idle"

            focus: true
            //            onAccepted: focus = false

            onFocusChanged: keyEmitter.disableAndroidKB()
            onTextChanged: makeCall.textChanged()

            states: [

                State {
                    name: "Idle"
                    PropertyChanges { target: inputField; maximumLength: 12; }

                },
                State {
                    name: "Active"
                    PropertyChanges { target: inputField; maximumLength: 24; }

                }
            ]


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
    Component {
        id: callButtonStyle_active
        ButtonStyle {

            background: Rectangle {
                implicitWidth: 100
                implicitHeight: 25
                border.width: control.activeFocus ? 2 : 1
                border.color: "#fff"
                radius: 4
                gradient: Gradient {
                    GradientStop { position: 0 ; color: control.pressed ? "#ccc" : "#0f0" }
                    GradientStop { position: 1 ; color: control.pressed ? "#aaa" : "#0c0" }
                }
            }
        }


    }

    Component {
        id: callButtonStyle_calling
        ButtonStyle {

            background: Rectangle {
                id: recBGCalling
                implicitWidth: 100
                implicitHeight: 25
                border.width: control.activeFocus ? 2 : 1
                border.color: "#fff"
                radius: 4
                gradient: Gradient {
                    GradientStop { position: 0 ; color: control.pressed ? "#ccc" : "#f00" }
                    GradientStop { position: 1 ; color: control.pressed ? "#aaa" : "#c00" }
                }
                Text{
                    anchors.centerIn: recBGCalling
                    text: "Cancel"
                }
            }
        }


    }

    Rectangle{
        id: dialSkin
        width: makeCall.width * 4/5
        height: makeCall.height * 2/3
        radius: 20
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: rowTop.bottom
        anchors.topMargin: 20
        property int rowHeight: dialSkin.height/6
        property int buttonWidth: dialSkin.width/5
        color: "light blue"
        state: "notready"


        states: [

            State {
                name: "notready"
                PropertyChanges { target: buttonCall; style: myButtonStyle; }
                PropertyChanges { target: buttonCall; text: "CALL"; }

            },
            State {
                name: "ready"
                PropertyChanges { target: buttonCall; style: callButtonStyle_active; }
                PropertyChanges { target: buttonCall; text: "CALL"; }

            },
            State {
                name: "calling"
                PropertyChanges { target: buttonCall; style: callButtonStyle_calling; }
                PropertyChanges { target: buttonCall; text: ""; }

            }
        ]


        Row {
            id: keyRow1
            anchors.horizontalCenter: dialSkin.horizontalCenter
            anchors.top: dialSkin.top
            anchors.topMargin: 20
            height: dialSkin.rowHeight
            spacing: 20
            Button {
                id: button1
                text: qsTr("1")
                style: myButtonStyle
                onClicked: textFocusChanged(Qt.Key_1)
                height: dialSkin.rowHeight
                width: dialSkin.buttonWidth

            }
            Button {
                id: button2
                style: myButtonStyle
                text: qsTr("2")
                onClicked: textFocusChanged(Qt.Key_2)
                height: dialSkin.rowHeight
                width: dialSkin.buttonWidth
            }
            Button {
                id: button3
                style: myButtonStyle
                text: qsTr("3")
                onClicked: textFocusChanged(Qt.Key_3)
                height: dialSkin.rowHeight
                width: dialSkin.buttonWidth
            }
        }
        Row {
            id: keyRow2
            anchors.horizontalCenter: dialSkin.horizontalCenter
            anchors.top: keyRow1.bottom
            anchors.topMargin: 20
            spacing: 20
            height: dialSkin.rowHeight
            Button {
                id: button4
                style: myButtonStyle
                text: qsTr("4")
                onClicked: textFocusChanged(Qt.Key_4)
                height: dialSkin.rowHeight
                width: dialSkin.buttonWidth
            }
            Button {
                id: button5
                text: qsTr("5")
                style: myButtonStyle
                onClicked: textFocusChanged(Qt.Key_5)
                height: dialSkin.rowHeight
                width: dialSkin.buttonWidth
            }
            Button {
                id: button6
                text: qsTr("6")
                style: myButtonStyle
                onClicked: textFocusChanged(Qt.Key_6)
                height: dialSkin.rowHeight
                width: dialSkin.buttonWidth
            }
        }
        Row {
            id: keyRow3
            anchors.horizontalCenter: dialSkin.horizontalCenter
            anchors.top:  keyRow2.bottom
            anchors.topMargin: 20
            height: dialSkin.rowHeight

            spacing: 20
            Button {
                id: button7
                style: myButtonStyle
                text: qsTr("7")
                onClicked: textFocusChanged(Qt.Key_7)
                height: dialSkin.rowHeight
                width: dialSkin.buttonWidth

            }
            Button {
                id: button8
                style: myButtonStyle
                text: qsTr("8")
                onClicked: textFocusChanged(Qt.Key_8)
                height: dialSkin.rowHeight
                width: dialSkin.buttonWidth
            }
            Button {
                id: button9
                style: myButtonStyle
                text: qsTr("9")
                onClicked: textFocusChanged(Qt.Key_9)
                height: dialSkin.rowHeight
                width: dialSkin.buttonWidth
            }
        }
        Row {
            id: keyRow4

            anchors.horizontalCenter: dialSkin.horizontalCenter
            anchors.top: keyRow3.bottom
            anchors.topMargin: 20
            height: dialSkin.rowHeight
            spacing: 20
            Button {
                id: buttonCall
                style: callButtonStyle_active
                text: qsTr("CALL")
                onClicked: callButtonClicked()
                height: dialSkin.rowHeight
                width: dialSkin.buttonWidth
                state: "disabled"

            }
            Button {
                id: buttonEnter
                style: myButtonStyle
                text: qsTr("0")
                onClicked: textFocusChanged(Qt.Key_0)
                height: dialSkin.rowHeight
                width: dialSkin.buttonWidth
            }
            Button {
                id: buttonEsc
                text: qsTr("DEL")
                style: myButtonStyle
                onClicked: textFocusChanged(Qt.Key_Backspace)
                height: dialSkin.rowHeight
                width: dialSkin.buttonWidth
                onVisibleChanged: textFocusChanged()

            }
        }
    }
}
