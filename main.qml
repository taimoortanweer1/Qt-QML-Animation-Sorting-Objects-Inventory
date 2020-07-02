import QtQuick 2.12
import QtQuick.Controls 2.5


ApplicationWindow {
    property var pagesName : ["HomeForm.qml","UnitForm.ui.qml","ConfigForm.ui.qml","PrintForm.ui.qml",
        "GateOpenDistanceForm.ui.qml","DiagForm.ui.qml","SupervisorForm.ui.qml",
        "HelpForm.ui.qml"]

    property int currentIndexPage: 0
    property int totalPages: 8

    id: window
    visible: true
    width: 1000
    height: 800
    title: qsTr("Sorting")

    header: ToolBar {
        contentHeight: toolButton.implicitHeight

        ToolButton {
            id: toolButton
            text: stackView.depth > 1 ? "\u25C0" : "\u2630"
            font.pixelSize: Qt.application.font.pixelSize * 1.6
            onClicked: {
                if (stackView.depth > 1) {
                    stackView.pop()
                } else {
                    drawer.open()
                }
            }
        }

        Label {
            text: stackView.currentItem.title
            anchors.centerIn: parent
        }
    }

    Drawer {
        id: drawer
        width: window.width * 0.3
        height: window.height

        Column {
            anchors.fill: parent

            Button {
                text: qsTr("Home")
                width: parent.width
                onClicked: {
                    //stackView.pop("HomeForm.qml")

//                    while(currentIndexPage != -1)
//                    {
//                        switchPages(-1)
//                        stackView.pop("HomeForm.qml")
//                    }
                    while(stackView.depth > 0)
                    {
                        //switchPages(-1)

                        currentIndexPage = 0
                        stackView.pop("HomeForm.qml")


                    }

                    drawer.close()
                    //focus: true
                }
            }
            Button {
                text: qsTr("Units")
                width: parent.width
                onClicked: {
                    stackView.push("UnitForm.ui.qml")
                    drawer.close()
                }
            }
            Button {
                text: qsTr("Configuration")
                width: parent.width
                onClicked: {
                    stackView.push("ConfigForm.ui.qml")
                    drawer.close()
                }
            }
            Button {
                text: qsTr("Printing")
                width: parent.width
                onClicked: {
                    stackView.push("PrintForm.ui.qml")
                    drawer.close()
                }
            }
            Button {
                text: qsTr("Gate Open Distance")
                width: parent.width
                onClicked: {
                    stackView.push("GateOpenDistanceForm.ui.qml")
                    drawer.close()
                }
            }
            Button {
                text: qsTr("Diagostic")
                width: parent.width
                onClicked: {
                    stackView.push("DiagForm.ui.qml")
                    drawer.close()
                }
            }
            Button {
                text: qsTr("Supervisor Menu")
                width: parent.width
                onClicked: {
                    stackView.push("SupervisorForm.ui.qml")
                    drawer.close()
                }
            }
            Button {
                text: qsTr("Help/About")
                width: parent.width
                onClicked: {
                    stackView.push("HelpForm.ui.qml")
                    drawer.close()
                }
            }
        }
    }

    StackView {
        id: stackView
        initialItem: "HomeForm.qml"
        anchors.fill: parent
        focus: true

        onCurrentItemChanged: {
            currentItem.forceActiveFocus()
        }
    }


    footer: TabBar{

        id: footerRoot
        width: parent.width
        height: 60

        TabButton{
            text:qsTr("HOME")
            height: parent.height
            onPressed:
            {
                //while(currentIndexPage != -1)
                while(stackView.depth > 1)
                {
                    //switchPages(-1)

                    currentIndexPage = 0
                    stackView.pop("HomeForm.qml")


                }
            }

        }

        TabButton{
            text:qsTr("<")
            height: parent.height

            onPressed:
            {
                switchPages(-1)
                //stackView.pop("HomeForm.qml")
                stackView.pop()
            }

        }

        TabButton{
            text: qsTr(">")
            height: parent.height

            onPressed:
            {
                switchPages(1)
                stackView.push(pagesName[currentIndexPage])
            }

        }


    }

    function switchPages(next)
    {
        if(next == 1)
        {
            currentIndexPage = currentIndexPage + 1
            currentIndexPage = currentIndexPage%totalPages
        }
        else if(next == -1)
        {
            currentIndexPage = currentIndexPage - 1
            currentIndexPage = currentIndexPage%totalPages
        }
    }


}
