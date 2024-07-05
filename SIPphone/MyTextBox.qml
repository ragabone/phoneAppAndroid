//import QtQuick 2.2
//import QtQuick.Window 2.0

import QtQuick 2.1
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.0

Rectangle {
    id: focusScope
    width: 250; height: 80

    property string text: textInput.text
    property string placeholder: ""
    property string value: ""
    property string fieldName: ""
    property string placeholderStatic: ""

    signal clear
    signal activated(string fieldParam, string fieldText)

    function setText(txt){
        textInput.text= txt
        placeholder = ""
        value= ""
        fieldName= ""
    }

    function clearText(){
        textInput.text=""
    }

    onClear: {
        textInput.text=""
    }

    BorderImage {
        source: "images/lineedit-bg.png"
        width: parent.width; height: parent.height
        border { left: 4; top: 4; right: 4; bottom: 4 }
    }

    // onActiveFocusChanged: { focusChanged(focus) }




    onActiveFocusChanged: {
        if (activeFocus){
            console.log("Active Focus " + fieldName)
            focusScope.state = ""
            typeSomething.text = ""
        }
        else{
            typeSomething.text =  placeholderStatic
            console.log("Active Not Focus " + fieldName + " state " + focusScope.state)
            if(focusScope.state == "hasText"){
//                if(Qt.application.active === Qt.ApplicationHidden)
//                    console.log("Application is Focus " + fieldName)
//                else
                console.log("Application is not Focus " + fieldName + " Text " + textInput.text)
                if(textInput.text != ""){
                    activated(fieldName,textInput.text)
                    typeSomething.text = textInput.text
                    typeSomething.opacity
                }
            }
            focusScope.state = "notActive"
        }

    }

    Text {
        id: typeSomething
        anchors.fill: parent;
        anchors.leftMargin: 8
        verticalAlignment: Text.AlignVCenter
        text: value ? value : placeholder
        color: "gray"
        font.italic: true
        font.pixelSize: parent.width/10


    }

    MouseArea {
        anchors.fill: parent
        onClicked: { focusScope.focus = true; focusScope.value = "Start Typing ..." }
    }

    TextField {
        id: textInput
        anchors.fill: parent;
//        anchors { left: parent.left; leftMargin: 8; right: clear.left; rightMargin: 8; verticalCenter: parent.verticalCenter }
        focus: true

        //selectByMouse: true
        font.pixelSize: parent.width/10

//        onEditingFinished: {
//            activated(fieldName,textInput.text)
//        }
        onActiveFocusChanged: {

            if (activeFocus){
//                if(focusScope.fieldName == "Domain")
//                    inputMethodHints=Qt.ImhDigitsOnly;

            }
        }
        onAccepted: {
            console.log("MyTextBox Activated " + textInput.text)
            activated(fieldName,textInput.text)
            typeSomething.text = textInput.text
            typeSomething.opacity = 0
        }


    }


    Image {
        id: clear
        anchors { right: parent.right; rightMargin: 8; verticalCenter: parent.verticalCenter }
        source: "images/clear.png"
        opacity: 0

        MouseArea {
            // allow area to grow beyond image size
            // easier to hit the area on high DPI devices
            anchors.centerIn: parent
            height:focusScope.height
            width: focusScope.height
            onClicked: {
                //toogle focus to be able to jump out of input method composer
                focusScope.focus = false;
                textInput.text = "";//focusScope.value;
                focusScope.focus = true;

            }
        }
    }

    states: State {
        name: "hasText"; when: (textInput.text != '' || textInput.inputMethodComposing)
        PropertyChanges { target: typeSomething; opacity: 0 }
        PropertyChanges { target: clear; opacity: 1 }
    }

    transitions: [
        Transition {
            from: ""; to: "hasText"
            NumberAnimation { exclude: typeSomething; properties: "opacity" }
        },
        Transition {
            from: "hasText"; to: ""
            NumberAnimation { properties: "opacity" }
        }
    ]
}
