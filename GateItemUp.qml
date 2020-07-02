import QtQuick 2.0

Rectangle
{
    id: gate

    width: 52
    height: 30
    color: Qt.lighter("transparent")
    border.color: "black"
    border.width: 1

    property bool up: false
    property double currentCapacity: 0
    property string gatename: "1"
    property bool open: true
    property int maxCapacity: 1
    property int captured: 0
    property double upperLimit: 0
    property double lowerLimit: 0
    property int capturedWeights: 0

    Rectangle
    {
        SequentialAnimation on color
        {
            id: blinkanimation

            loops: 1

            ColorAnimation
            {
                to: Qt.lighter("green")
                duration: 200
            }
            ColorAnimation
            {
                to: "transparent"
                duration: 100
            }
        }

        width: parent.width
        height: parent.height * (2/3)
        anchors.top: parent.top

        color: "transparent"
        Text {
            anchors.centerIn: parent
            text: qsTr( gate.gatename )
        }
    }
    Rectangle
    {
        width: parent.width
        height: parent.height/3
        anchors.bottom: parent.bottom
        border.color: "black"
        border.width: 0.5

        color: "white"
        Rectangle
        {

            id: progressbar
            height: parent.height
            anchors.bottom: parent.bottom
            color: "blue"
        }
    }
    onOpenChanged:
    {
        if( open == true )
        {
            color = "transparent"
        }
        else
        {
            color = "gray"
        }
    }
    onCurrentCapacityChanged:
    {
        blinkanimation.start()
        if( currentCapacity >= maxCapacity )
        {
            batchhistorymodel.insert(2, {gate:gatename, weight: currentCapacity.toString()} )
            open = true
            progressbar.width = 1 * width
            color = "gray"
        }
        else
        {
            progressbar.width = (currentCapacity/maxCapacity) * width
        }
    }
}

