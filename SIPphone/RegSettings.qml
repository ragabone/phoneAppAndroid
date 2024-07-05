import QtQuick 2.4
import QtQuick.Window 2.0
import QtQuick.Layouts 1.1
import QtQuick.XmlListModel 2.0



Rectangle {
    id: regsettings

    //width: parent.width-40; height: parent.height-20;
    color: "grey"
    anchors.centerIn: parent
    property string xmlSource: "register.xml"


    function setXmlSource(filename){
        xmlSource = "file://" + filename;

        console.log("setXmlSource: "  + xmlSource )
    }



    BorderImage {
        id: border

        source: "images/lineedit-bg.png"
        width: regsettings.width; height: regsettings.height
        border { left: 4; top: 4; right: 4; bottom: 4 }
    }
    //anchors.centerIn: parent
    SystemPalette{
        id: pallette
    }

    //    ListModel {
    //        id: regSettingList
    //        ListElement { name: "Domain"; holdingText: "Domain Name or IP Address" }
    //        ListElement { name: "Name";  holdingText: "ID Name"}

    //    }
    XmlListModel{
        id: xmlList
        source: xmlSource
        query: "/Data/Param"

        XmlRole {
            name: "ParamName"
            query:"ParamName/string()"
        }
        XmlRole {
            name: "ParamHoldingText"
            query:"ParamHoldingText/string()"
        }
        XmlRole {
            name: "ParamValue"
            query:"ParamValue/string()"
        }
        XmlRole {
            name: "ParamDesc"
            query:"ParamDesc/string()"
        }
    }

    Component {
        id: delegate

        //property string domain: ""
        //property string clientName: ""

        Row {
            x: 20
            spacing: parent.height/10
            Text {
                id: fieldName
                text: ParamName
                width: parent.width/5
                color: "grey"
                font.italic: true
                font.pointSize: 8

            }

            MyTextBox {
                id: textbox
                placeholder: ParamHoldingText
                placeholderStatic: ParamHoldingText
                value: ParamValue

                fieldName: fieldName.text

                onActivated: {
                    console.log("Activated at " + fieldParam + "," + fieldText)
                    if(fieldText != ""){
                        if(fieldParam === "Name")
                            registrar.client = fieldText
                        if(fieldParam === "Domain")
                            registrar.domain = fieldText
                    }

                }

            }
        }
    }

    Component {
        id: delegate1


        Row {
            x: 20
            spacing: 10
            Text {
                id: descName
                text: ParamName + ":"
                width: 50
                color: "grey"
                font.bold: true
                font.italic: true
                font.pointSize: 8

            }
            Rectangle{
                id: recDescBox
                width: regsettings.width - descName.width - 40
                height: 40
                Text {
                    anchors.fill: recDescBox
                    text: ParamDesc
                    wrapMode: "WordWrap"
                    //width: parent.width/5
                    color: "grey"
                    font.italic: true
                    font.pointSize: 8

                }
            }


        }


    }
    //    ListView{
    //        height: 50
    //        model: regSettingList
    //        delegate: delegate
    //    }
    Rectangle{
        id: recHolding
        width: border.width
        height: border.height


        Rectangle {
            id: rec1
            width: recHolding.width
            height: recHolding.height
            anchors.top: recHolding.top
            anchors.topMargin: 40
            clip: true

            Text{

                id: versionTxt
                anchors.top: rec1.top

                text: "Version No: 1.2"
                color: "gray"
                font.italic: true
                font.pointSize: 10
            }
            Text {
                id: versionVer
                anchors.top: versionTxt.bottom
                text: "Version Date: 14/11/2016"
                color: "gray"
                font.italic: true
                font.pointSize: 10
            }
            Column {
                anchors.top: versionVer.bottom
                anchors.topMargin: 40
                spacing: 40

                Repeater {

                    model: xmlList
                    delegate: delegate

                }
            }
//            Column {
//                y:recHolding.height
//                Repeater {
//                    model: xmlList
//                    delegate: delegate1

//                }
//            }
        }
    }
}
