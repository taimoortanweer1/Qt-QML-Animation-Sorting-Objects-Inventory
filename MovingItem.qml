import QtQuick 2.0

Rectangle
{
    id: movingrect
    color: Qt.lighter("green")

    x: 0
    y: 0
    height: 20
    width: 20

    property int weight: Math.floor((Math.random() * 100) + 1)

    SequentialAnimation on x
    {
        id: forwardanimation
        running: true
        property int forwardX: movingrect.x

        // loops: Animation.Infinite

        NumberAnimation
        {
            to: x
            duration: 500
        }
        NumberAnimation
        {
            from: gun.x
            to: parent.width
            duration: 4000
        }
    }


    SequentialAnimation on x
    {
        id: capturedanimation
        loops: 1
        NumberAnimation
        {
            to: x
            duration: 5000
        }
    }

    onXChanged: checkGate()
    property var currentGate: null

    function checkGate()
    {
        var captured = false
        for( var g in root.allGate )
        {

            if( root.allGate[g].currentCapacity < root.allGate[g].maxCapacity &&
                    root.allGate[g].open &&
                    movingrect.x > root.allGate[g].x - 90 &&
                    movingrect.x < root.allGate[g].x+root.allGate[g].width )
            {
                if( currentGate !== root.allGate[g] )
                {
                    currentGate = root.allGate[g]
                    root.allGate[g].capturedWeights += weight

                    itemhistorymodel.insert(2, {gate:root.allGate[g].gatename, weight: weight.toString()} )
                    root.allGate[g].currentCapacity += weight
                    forwardanimation.stop();
                    capturedanimation.start();
                    visible = false
                    captured = true
                    break
                }
            }

            if( captured == true )
            {
                break
            }
        }
    }
}
