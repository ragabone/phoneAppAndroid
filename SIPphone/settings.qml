import QtQuick 2.4
import QtQuick.Window 2.0
import QtQuick.Layouts 1.0
import QtMultimedia 5.0


import "./"
Rectangle {
    id: rootSettings
    width: Screen.width/1.5
    height: Screen.height/1.5


    SystemPalette{
        id: pallette
    }

    Row {
        x: 20
        spacing: parent.height/10
        anchors.centerIn: rootSettings
        Text {
            id: fieldName
            text: "Client Name"
            width: parent.width/5
            color: "grey"
            font.italic: true
            font.pointSize: 8

        }

        MyTextBox {
            id: textbox
            placeholder: "Enter Your Name "
            value: ""
            fieldName: fieldName.text


        }
    }

}

