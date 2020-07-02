import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import FileIO 1.0

Page
{

    id: mainPage
    title: qsTr("Home")

    property int default_pix_density: 4  //pixel density of my current screen
    property int scale_factor: Screen.pixelDensity/default_pix_density
    property real gcurrentHighlightGate: -1
    property var gallGate: []
    property var datamodel:[]
    property var datastore:[]
    property var dataItemHistory: []
    property var dataBatchHistory: []
    property string mywritebuffer
    visible: true
    width: 50*scale_factor
    height: 20*scale_factor



    Item
    {


        id: root
        anchors.fill: parent
        focus: true
        property var allGate: []
        property real currentHighlightGate: -1


        function highlightGate( gateindex )
        {

            for( var g in allGate )
            {
                allGate[g].border.width = 1
                allGate[g].border.color = "black"
            }
            allGate[gateindex].border.width = 2
            allGate[gateindex].border.color = "red"



            gallGate = allGate
            gcurrentHighlightGate = gateindex

        }



        Keys.onReturnPressed:
        {
            var component;
            component = Qt.createComponent("MovingItem.qml")
            component.createObject( track, {"x":0, "y":0, "height": track.height } )
            gunshotanimation.start()

        }

        Keys.onRightPressed:
        {
            if( currentHighlightGate < 0 || gcurrentHighlightGate < 0)
            {
                currentHighlightGate = 0
                highlightGate( currentHighlightGate );
            }
            else if( currentHighlightGate >= 0 && currentHighlightGate < allGate.length-1 )
            {
                currentHighlightGate++;
                highlightGate( currentHighlightGate );
            }
            else if(currentHighlightGate == allGate.length-1)
            {
                currentHighlightGate = 0
                highlightGate(currentHighlightGate);
            }
        }

        Keys.onSpacePressed:
        {
            allGate[currentHighlightGate].open = allGate[currentHighlightGate].open? false: true

        }

        Keys.onLeftPressed:
        {
            if( currentHighlightGate < 0 )
            {
                currentHighlightGate = allGate.length
                highlightGate( currentHighlightGate );
            }
            else if( currentHighlightGate > 0 && currentHighlightGate <= allGate.length )
            {
                currentHighlightGate--;
                highlightGate( currentHighlightGate );
            }
            else if(currentHighlightGate == 0)
            {
                currentHighlightGate = allGate.length - 1
                highlightGate(currentHighlightGate);
            }
        }

        Keys.onDownPressed:
        {
            if( currentHighlightGate < 0 )
            {
                currentHighlightGate = (allGate.length/2)
                highlightGate( currentHighlightGate );
            }
            else if( currentHighlightGate < (allGate.length/2) )
            {
                currentHighlightGate = currentHighlightGate + (allGate.length/2)
                highlightGate( currentHighlightGate );
            }
        }
        Keys.onUpPressed:
        {
            if( currentHighlightGate < 0 )
            {
                currentHighlightGate = 0
                highlightGate( currentHighlightGate );
            }
            else if( currentHighlightGate >= (allGate.length/2) )
            {
                currentHighlightGate = currentHighlightGate - (allGate.length/2)
                highlightGate( currentHighlightGate );
            }
        }



        Rectangle
        {
            id: gun
            x: 23
            y: 71
            width: 49
            height: 13
            color: "transparent"
            border.color: "black"
            border.width: 1

            Rectangle
            {
                id: gunshot
                anchors.right: parent.right
                height: parent.height
                width: 5
                color: Qt.lighter("green")

                SequentialAnimation on color
                {
                    id: gunshotanimation
                    ColorAnimation {
                        from: Qt.lighter("green")
                        to: Qt.lighter("transparent")
                        duration: 200
                    }
                    ColorAnimation {
                        from: Qt.lighter("transparent")
                        to: Qt.lighter("green")
                        duration: 200
                    }
                }

            }
        }

        Rectangle
        {
            id: track
            x: 97
            y: 71
            width: 799
            height: 13
            color: "white"
            border.width: 1
            border.color: "black"
        }



        GateItemUp {
            id: gate1
            x: 97
            y: 42
            gatename: "1"
            maxCapacity: 500
            upperLimit: 100
            lowerLimit: 50
        }

        GateItemUp {
            id: gate2
            x: 222
            y: 42
            gatename: "2"
            maxCapacity: 1000
            upperLimit: 150
            lowerLimit: 100

        }

        GateItemUp {
            id: gate3
            x: 350
            y: 42
            gatename: "3"
            maxCapacity: 1000
            upperLimit: 200
            lowerLimit: 150
        }

        GateItemUp {
            id: gate4
            x: 483
            y: 42
            gatename: "4"
            maxCapacity: 1000
            upperLimit:  250
            lowerLimit: 200
        }

        GateItemUp {
            id: gate5
            x: 602
            y: 42
            gatename: "5"
            maxCapacity: 1000
            upperLimit: 300
            lowerLimit: 250
        }

        GateItemUp {
            id: gate6
            x: 721
            y: 42
            gatename: "6"
            maxCapacity: 1000
            upperLimit: 350
            lowerLimit: 300
        }

        GateItemUp {
            id: gate7
            x: 844
            y: 42
            gatename: "7"
            maxCapacity: 2000
            upperLimit: 400
            lowerLimit: 350
        }

        GateItemDown {
            id: gate8
            x: 97
            y: 83
            gatename: "8"
            maxCapacity: 2000
            upperLimit: 500
            lowerLimit: 400
        }

        GateItemDown {
            id: gate9
            x: 222
            y: 83
            gatename: "9"
            maxCapacity: 2000
            upperLimit: 550
            lowerLimit: 450
        }

        GateItemDown {
            id: gate10
            x: 350
            y: 83
            width: 52
            height: 30
            gatename: "10"
            maxCapacity: 1000
            upperLimit: 150
            lowerLimit: 100
        }


        Rectangle
        {
            width: itemhistory.width
            height: 30
            anchors.bottom: itemhistory.top
            anchors.horizontalCenter: itemhistory.horizontalCenter
            Text {
                anchors.fill: parent
                text: qsTr("Item History")
                font.pointSize: 14
                horizontalAlignment: Text.AlignHCenter
            }
        }

        Rectangle
        {
            id: itemhistory
            x: 268
            y: 201
            width: 168
            height: 203
            color: "#ffffff"
            border.color: "black"
            border.width: 1


            ListModel
            {
                id: itemhistorymodel

                ListElement
                {
                    gate: "Gate"
                    weight: "Weight"
                }
                ListElement
                {
                    gate: "------------"
                    weight: "-----------"
                }
            }

            Rectangle
            {
                width: parent.width - 2
                height: parent.height - 30

                anchors.centerIn: parent

                ListView
                {
                    model: itemhistorymodel
                    anchors.fill: parent
                    height: 30
                    delegate: Rectangle
                    {
                        width: parent.width-2
                        height: itemtext.height
                        anchors.horizontalCenter: parent.horizontalCenter
                        Text
                        {
                            id: itemtext
                            anchors.centerIn: parent
                            text: qsTr(gate + " \t" + weight )
                        }
                    }
                }
            }
        }


        Rectangle
        {
            width: batchhistory.width
            height: 30
            anchors.bottom: batchhistory.top
            anchors.horizontalCenter: batchhistory.horizontalCenter
            Text {
                anchors.fill: parent
                text: qsTr("Batch History")
                font.pointSize: 14
                horizontalAlignment: Text.AlignHCenter
            }
        }
        Rectangle
        {
            id: batchhistory
            x: 605
            y: 201
            width: 168
            height: 203
            color: "#ffffff"
            border.color: "black"
            border.width: 1

            ListModel
            {
                id: batchhistorymodel
                ListElement
                {
                    gate: "Gate"
                    weight: "Weight"
                }
                ListElement
                {
                    gate: "------------"
                    weight: "-----------"
                }
            }

            Rectangle
            {
                width: parent.width-2
                height: parent.height - 30

                anchors.centerIn: parent

                ListView
                {
                    anchors.fill: parent

                    model: batchhistorymodel
                    delegate: Rectangle
                    {
                        width: parent.width-2
                        height: batchtext.height
                        anchors.horizontalCenter: parent.horizontalCenter
                        Text
                        {
                            id: batchtext
                            anchors.centerIn: parent
                            text: qsTr(gate + " \t" + weight )
                        }
                    }
                }
            }
        }

        GateItemDown {
            id: gate14
            x: 844
            y: 83
            gatename: "14"
            maxCapacity: 2000
            upperLimit: 600
            lowerLimit: 500
        }

        GateItemDown {
            id: gate13
            x: 721
            y: 83
            gatename: "13"
            maxCapacity: 3000
            upperLimit: 650
            lowerLimit: 750
        }

        GateItemDown {
            id: gate11
            x: 483
            y: 83
            gatename: "11"
            maxCapacity: 5000
            upperLimit: 900
            lowerLimit: 800
        }

        GateItemDown {
            id: gate12
            x: 602
            y: 83
            gatename: "12"
            maxCapacity: 1000
            upperLimit: 150
            lowerLimit: 100
        }

        function openFileItemHistory(fileUrl) {

            var xmlhttp = new XMLHttpRequest();
            var url = fileUrl
            xmlhttp.onreadystatechange=function() {
                if (xmlhttp.readyState === XMLHttpRequest.DONE && xmlhttp.status === 200)
                {
                    //root.myFunction(xmlhttp.responseText);
                    root.loadDataItemHistory(xmlhttp.responseText)
                }
            }
            xmlhttp.open("GET", url, true);
            xmlhttp.send();


        }


        function openFileBatchHistory(fileUrl) {

            var xmlhttp = new XMLHttpRequest();
            var url = fileUrl
            xmlhttp.onreadystatechange=function() {
                if (xmlhttp.readyState === XMLHttpRequest.DONE && xmlhttp.status === 200)
                {
                    //root.myFunction(xmlhttp.responseText);
                    root.loadDataBatchHistory(xmlhttp.responseText)
                }
            }
            xmlhttp.open("GET", url, true);
            xmlhttp.send();


        }

        function myFunction(response) {

            mainPage.datamodel = JSON.parse(response);

            console.log("data " + mainPage.datamodel)
            console.log("data " + response)

        }

        function saveFile(fileUrl, text) {

            var request = new XMLHttpRequest();
            request.open("PUT", fileUrl, false);
            request.send(text);
            return request.status;


        }



        FileIO {
            id: myFile
            source: "my_file.txt"
            onError: console.log(msg)
        }

        FileDialog {
            id: openFileDialogItem
            folder: shortcuts.home
            nameFilters: ["JSON (*.json)","Text files (*.txt)", "All files (*)"]
            onAccepted:
            {
                root.openFileItemHistory(openFileDialogItem.fileUrl)
            }
        }

        FileDialog {
            id: openFileDialogBatch
            folder: shortcuts.home
            nameFilters: ["JSON (*.json)","Text files (*.txt)", "All files (*)"]
            onAccepted:
            {
                root.openFileBatchHistory(openFileDialogBatch.fileUrl)
            }
        }

        FileDialog {
            id: saveFileDialog
            selectExisting: false
            folder: shortcuts.home
            nameFilters: ["JSON (*.json)","Text files (*.txt)", "All files (*)"]
            onAccepted:  {
                console.log("write = " + root.saveFile(saveFileDialog.fileUrl+"-item",mainPage.dataItemHistory))
                console.log("write = " + root.saveFile(saveFileDialog.fileUrl+"-batch",mainPage.dataBatchHistory))


            }
        }



        function loadDataItemHistory(response)
        {

            mainPage.datastore = response

            if(mainPage.datastore){

                mainPage.datamodel = []
                mainPage.datamodel = JSON.parse(mainPage.datastore)

                for (var i=2; i<mainPage.datamodel.length; ++i)
                    itemhistorymodel.append(mainPage.datamodel[i])

            }

        }

        function loadDataBatchHistory(response)
        {

            mainPage.datastore = response

            if(mainPage.datastore){

                mainPage.datamodel = []
                mainPage.datamodel = JSON.parse(mainPage.datastore)

                for (var i=2; i<mainPage.datamodel.length; ++i)
                    batchhistorymodel.append(mainPage.datamodel[i])

            }

        }

        function saveDataItemHistory()
        {
            var mydatamodel = []
            for (var i=0; i< itemhistorymodel.count; ++i)
                mydatamodel.push(itemhistorymodel.get(i))
            mainPage.dataItemHistory = JSON.stringify(mydatamodel)
        }

        function saveDataBatchHistory()
        {
            var mydatamodel = []
            for (var i=0; i< batchhistorymodel.count; ++i)
                mydatamodel.push(batchhistorymodel.get(i))
            mainPage.dataBatchHistory = JSON.stringify(mydatamodel)
        }


        Component.onCompleted:
        {

            allGate.push( gate1)
            allGate.push( gate2)
            allGate.push( gate3)
            allGate.push( gate4)
            allGate.push( gate5)
            allGate.push( gate6)
            allGate.push( gate7)
            allGate.push( gate8)
            allGate.push( gate9)
            allGate.push( gate10)
            allGate.push( gate11)
            allGate.push( gate12)
            allGate.push( gate13)
            allGate.push( gate14)


        }



        //////////////////////////
        Rectangle
        {
            id: dataSaveLoad
            x: parent.width - 300
            y: parent.height - 140
            width: 200
            height: 60

            Grid {
                columns: 3

                Button {

                    id: bttnSaveData
                    width: 100
                    height: 60

                    Text {

                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("Save Data")
                    }

                    onPressed:
                    {
                        root.saveDataItemHistory()
                        root.saveDataBatchHistory()
                        saveFileDialog.open()
                    }

                }


                Button {
                    id: bttnLoadDataItem

                    width: 100
                    height: 60

                    Text {

                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("Load Items")
                    }

                    onPressed:
                    {
                        openFileDialogItem.open()
                    }

                }

                Button {
                    id: bttnLoadDataBatch

                    width: 100
                    height: 60

                    Text {

                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("Load Batch")
                    }

                    onPressed:
                    {
                        openFileDialogBatch.open()
                    }

                }


            }
        }

        Rectangle
        {
            id: monitoringData
            x: 0
            y: parent.height - 80
            width: parent.width
            height: 60
            color: Qt.lighter("gray")

            Grid {
                columns: 6

                // first col: width 100

                Rectangle {

                    color: "white"
                    border.color: "black"
                    width: 100
                    height: 30

                    Text {

                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("LAST")
                    }

                }


                Rectangle {


                    color: "white"
                    border.color: "black"
                    width: 100
                    height: 30

                    Text {
                        id: displayLastWeight
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        color: "red"
                        text: qsTr("130g")
                    }

                }

                Rectangle {

                    color: "white"
                    border.color: "black"
                    width: 100
                    height: 30

                    Text {

                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("GATE")
                    }

                }


                Rectangle {


                    color: "white"
                    border.color: "black"
                    width: 100
                    height: 30

                    Text {
                        id: displayGateNumber
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        color: "red"
                        text: qsTr("4")
                    }

                }
                Rectangle {

                    color: "white"
                    border.color: "black"
                    width: 100
                    height: 30

                    Text {

                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("LENGTH")
                    }

                }

                Rectangle {

                    color: "white"
                    border.color: "black"
                    width: 100
                    height: 30

                    Text {
                        id: displayLengthWeight
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        color: "red"
                        text: qsTr("160cm")
                    }

                }

                Rectangle {

                    color: "white"
                    border.color: "black"
                    width: 100
                    height: 30

                    Text {

                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("TOTAL")
                    }

                }

                Rectangle {


                    color: "white"
                    border.color: "black"
                    width: 100
                    height: 30

                    Text {
                        id: displayTotalWeight
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        color: "red"
                        text: qsTr("1600cm")
                    }

                }

                Rectangle {

                    color: "white"
                    border.color: "black"
                    width: 100
                    height: 30

                    Text {

                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("ITEMS")
                    }

                }

                Rectangle {


                    color: "white"
                    border.color: "black"
                    width: 100
                    height: 30

                    Text {
                        id: displayTotalItems
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        color: "red"
                        text: qsTr("3/3")
                    }

                }


                Rectangle {

                    color: "white"
                    border.color: "black"
                    width: 100
                    height: 30

                    Text {

                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("BATCHES")
                    }

                }

                Rectangle {


                    color: "white"
                    border.color: "black"
                    width: 100
                    height: 30

                    Text {

                        id: displayTotalBatches
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        color: "red"
                        text: qsTr("8")
                    }

                }

            }

            Rectangle
            {
                id: gateSelection
                x: parent.width-400
                y: parent.height-60
                width: 400
                height: parent.height
                color: Qt.lighter("red")

                Grid
                {
                    columns: 4
                    Rectangle {


                        //anchors.fill: parent;
                        color: Qt.lighter("red")

                        width: 100
                        height: 60

                        Text {

                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            text: qsTr("GATE:")
                        }

                    }



                    Button {

                        id: leftButtonGate
                        width: 100
                        height: 60


                        background: Rectangle {
                            color: leftButtonGate.pressed ? Qt.lighter("blue") : Qt.lighter("red")
                        }

                        Text {

                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            text: qsTr("<")
                        }

                        onClicked:
                        {
                            root.focus = true


                            if( root.currentHighlightGate < 0 )
                            {
                                root.currentHighlightGate = root.allGate.length
                                root.highlightGate( root.currentHighlightGate );
                            }
                            else if( root.currentHighlightGate > 0 && root.currentHighlightGate <= root.allGate.length )
                            {
                                root.currentHighlightGate--;
                                root.highlightGate( root.currentHighlightGate );
                            }
                            else if(root.currentHighlightGate == 0)
                            {
                                root.currentHighlightGate = root.allGate.length - 1
                                root.highlightGate(root.currentHighlightGate);
                            }
                        }

                    }

                    Button {

                        id: rightButtonGate
                        width: 100
                        height: 60


                        background: Rectangle {
                            color: rightButtonGate.pressed ? Qt.lighter("blue") : Qt.lighter("red")


                        }

                        Text {

                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            text: qsTr(">")
                        }

                        onClicked:
                        {
                            root.focus = true

                            if( root.currentHighlightGate < 0 )
                            {
                                root.currentHighlightGate = 0
                                root.highlightGate( root.currentHighlightGate );
                            }
                            else if( root.currentHighlightGate >= 0 && root.currentHighlightGate < root.allGate.length-1 )
                            {
                                root.currentHighlightGate++;
                                root.highlightGate( root.currentHighlightGate );
                            }
                            else if(root.currentHighlightGate == root.allGate.length-1)
                            {
                                root.currentHighlightGate = 0
                                root.highlightGate(root.currentHighlightGate);
                            }
                        }
                    }

                    Button {

                        id: editGates
                        width: 100
                        height: 60


                        background: Rectangle {
                            color: editGates.pressed ? Qt.lighter("blue") : Qt.lighter("red")


                        }

                        Text {

                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            text: qsTr("Edit Gates")
                        }

                        onClicked:
                        {
                            root.focus = true
                            popup.open()
                        }


                    }

                }
            }

        }

        Popup
        {
            id: popup
            x: 200
            y: 200
            width: 300
            height: 100
            modal: true
            focus: true

            closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

            Label
            {
                x:parent.width / 2 - 35
                y:0
                width: parent.width/3
                height: 10
                text: qsTr("EDIT GATES")

            }

            Button
            {
                id: bttngateopen
                x:0
                y:20
                width: parent.width/3
                height: 30
                text: qsTr("OPEN")

                background: Rectangle {
                    color: bttngateopen.pressed ? Qt.lighter("blue") : Qt.lighter("red")
                }

                onClicked:
                {
                    root.allGate[root.currentHighlightGate].open = true

                }
            }

            Button
            {
                id: bttngateclose
                x:parent.width/3 * 1
                y:20
                width: parent.width/3
                height: 30
                text: qsTr("CLOSE")

                background: Rectangle {
                    color: bttngateclose.pressed ? Qt.lighter("blue") : Qt.lighter("red")
                }

                onClicked:
                {
                    root.allGate[root.currentHighlightGate].open = false

                }

            }
            Button
            {
                id: bttngatereset
                x:parent.width/3 * 2
                y:20
                width: parent.width/3
                height: 30
                text: qsTr("RESET ALL")

                background: Rectangle {
                    color: bttngatereset.pressed ? Qt.lighter("blue") : Qt.lighter("red")
                }
                onClicked:
                {
                    var i = 0
                    while(i<14)
                    {
                        root.allGate[i].open = root.allGate[i].open? false: true
                        i++
                    }

                }
            }
        }


    }


}
