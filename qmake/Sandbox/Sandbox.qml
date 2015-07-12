import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2

import "."

ScrollView
{
    width: 245
    height: 700
    Rectangle
    {
        id: rectangle1
        /*gradient: Gradient {
            GradientStop { position: 0.0; color: "#ffffff" }
            GradientStop { position: 1.0; color: "#3E3E3E" }
        }*/
        height: 700
        width: 300
        ColumnLayout
        {
            id: columnLayout
            height: 100
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter
            antialiasing: true
            transformOrigin: Item.Top
            //width: 100

            ItemTypeView
            {
                id: linearItem
                label.text: "Linear Chart"
                icon.source: "res/linear.png"
            }
            ItemTypeView
            {
                id: pieItem
                label.text: "Pie Chart"
                icon.source: "res/pie.png"
            }
        }
    }
}
