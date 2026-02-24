import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

// Add this import (use the exact URI from your CMake)
import i2c_stmp_demo 1.0

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("I2C AS5600L Demo")

    FontLoader { id: goRegular; source: Qt.resolvedUrl("go_fonts/Go-Regular.ttf") }

    AS5600Sensor {
        id: sensor
        active: sw.checked          // Switch now controls live polling
    }

    Label {
        id: title
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        text: "AS5600L Magnetic Angle Sensor"
        font.pixelSize: 28
        font.bold: true
        color: "black"
        font.family: goRegular.name
    }

    Switch {
        id: sw
        scale: 2
        anchors.top: title.bottom
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        text: checked ? "Live Sensor (Rotation Enabled)" : "Sensor Paused"
        checked: true
    }

    RowLayout {
        anchors.top: sw.bottom
        anchors.topMargin: 40
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 35
        spacing: 65

        Dial {
            id: dial
            Layout.preferredWidth: 190
            Layout.preferredHeight: 190
            from: 0
            to: 360
            value: sensor.angle
            enabled: sw.checked
            Text {
                anchors.centerIn: parent
                text: Math.round(dial.value) + "°"
                font.pixelSize: 22
                font.bold: true
                color: sw.checked ? "black" : "#777"
                font.family: goRegular.name
            }
        }

        Image {
            source: Qt.resolvedUrl("assets/dial_no_background.png")
            fillMode: Image.PreserveAspectFit
            smooth: true
            antialiasing: true
            rotation: sensor.angle + 140
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumWidth: 230
            Layout.alignment: Qt.AlignVCenter
        }
    }

    Label {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 20
        text: "Raw: " + sensor.rawAngle + " | Mag: " + sensor.magnitude
        font.pixelSize: 16
        font.family: goRegular.name
    }
}
