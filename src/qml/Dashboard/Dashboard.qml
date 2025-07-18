import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import app.ui 1.0


Item {
    //width: parent.width
    //height: parent.height
    DashboardBackend {
        id: backend
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        Label { 
            text: qsTr("System Dashboard (QML)")
            color: "black" // <--- 添加这一行
            }
    Text {
        text: "Hello World!"
        font.family: "Helvetica"
        font.pointSize: 24
        color: "red"
    }

    ProgressBar {
        id: cpuBar
        value: backend.cpuUsage / 100.0
        Layout.fillWidth: true
    }
    Label {
        color: "black" // <--- 添加这一行
        text: qsTr("CPU Usage: %1%").arg(backend.cpuUsage)
        }

        ProgressBar {
            id: memBar
            value: backend.memUsage / 100.0
            Layout.fillWidth: true
        }
        Label {
            text: "Memory: " + backend.memUsage + "%" 
            color: "black" // <--- 添加这一行


        }

        Button {
            text: qsTr("Reset")
            onClicked: backend.resetValues()
        }
    }
}
