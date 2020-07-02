import QtQuick 2.12
import QtQuick.Controls 2.5

Page {
    id: page
    width: 1000
    height: 800
    title: qsTr("Unit")
    Button{
        id: butGrams
        x:200
        y: 50
        width: 600
        height: 50
        text: qsTr("Grams")
        font.pointSize: 11
    }
    Button{
        id: butKg
        x: 200
        y: 110
        width: 600
        height: 50
        text: "Kilogram"
        font.pointSize: 11
    }
    Button{
        id: butPou
        x: 200
        y: 170
        width: 600
        height: 50
        text: "Ounces"
        font.pointSize: 11
    }
    Button{
        id: butOun
        x: 200
        y: 230
        width: 600
        height: 50
        text: "Pounds"
        font.pointSize: 11
    }

}
