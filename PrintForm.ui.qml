import QtQuick 2.12
import QtQuick.Controls 2.5

Page {
    width: 1000
    height: 800
    title: qsTr("Print")
    Button{
        id: butGrams
        x:200
        y: 50
        width: 600
        height: 50
        text: qsTr("ON/Off")
        font.pointSize: 11
    }
    Button{
        id: butKg
        x: 200
        y: 110
        width: 600
        height: 50
        text: qsTr("Print Preview")
        font.pointSize: 11
    }
    Button{
        id: butPou
        x: 200
        y: 170
        width: 600
        height: 50
        text: qsTr("Print")
        font.pointSize: 11
    }
}
