import QtQuick 2.0

Rectangle
{
    property alias label: title
    property alias icon: illustration

    id: item
    width: 180
    height: 100
    /*gradient: Gradient {
            GradientStop { position: 0.0; color: "#50606060" }
            GradientStop { position: 1.0; color: "transparent"; }
        }*/
    color: "#555555"
    Row
    {
        id: layout
        transformOrigin: Item.Center
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 10
        anchors.topMargin: 0
        anchors.fill: parent

        Image
        {
            id: illustration
            x: 0
            y: 0
            width: 90
            height: 60
            anchors.verticalCenter: parent.verticalCenter
            clip: false
            antialiasing: true
            source: ""
        }

        Text
        {
            id: title
            width: 96
            height: 14
            anchors.verticalCenter: parent.verticalCenter
            text: qsTr("Text")
            font.pixelSize: 12
            color: "white"
        }

    }
}

