import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import i2c_stmp_demo 1.0

ApplicationWindow {
    id: root
    readonly property bool embeddedDev: Qt.platform.pluginName === "eglfs"
    width: embeddedDev ? 480 : 800
    height: embeddedDev ? 800 : 480
    visible: true
    title: qsTr("AS5600L - 3 Sensors")

    FontLoader { id: goRegular; source: Qt.resolvedUrl("go_fonts/Go-Regular.ttf") }

    // === SENSORS ===
    AS5600Sensor { id: sensor1; busAddress: 0; deviceAddress: 0x40 }
    AS5600Sensor { id: sensor2; busAddress: 0; deviceAddress: 0x41 }
    AS5600Sensor { id: sensor3; busAddress: 0; deviceAddress: 0x42 }

    Component {
        id: sensorPanel

        ColumnLayout {
            spacing: 12
            Layout.fillWidth: true

            required property AS5600Sensor sensor
            required property string title

            // Each panel manages its own zero offset (no root clutter)
            property real zeroOffset: 0

            // Normalized signed angle (-180° … +180°)
            property real displayAngle: {
                const a = ((sensor.angle - zeroOffset) % 360 + 360) % 360
                return a > 180 ? a - 360 : a
            }

            Label {
                text: title
                font { pixelSize: 19; bold: true; family: goRegular.name }
                Layout.alignment: Qt.AlignHCenter
            }

            Rectangle {
                id: errorBox
                Layout.fillWidth: true
                Layout.preferredHeight: sensor.valid ? 0 : 25
                Layout.alignment: Qt.AlignHCenter

                color: "#d32f2f"
                radius: 8
                clip: true

                // Smooth hide/show animation
                Behavior on Layout.preferredHeight {
                    NumberAnimation { duration: 250; easing.type: Easing.OutCubic }
                }

                Text {
                    anchors.centerIn: parent
                    text: sensor.errorMessage
                    color: "white"
                    font.pixelSize: 15
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    padding: 10
                    wrapMode: Text.Wrap
                }
            }

            Item {
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredWidth: 192
                Layout.preferredHeight: 192
                opacity: sensor.valid ? 1.0 : 0.45
                enabled: sensor.valid

                Image {
                    anchors.centerIn: parent
                    source: Qt.resolvedUrl("assets/dial_no_background.png")
                    width: 192; height: 192
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    antialiasing: true
                    rotation: displayAngle + 140
                }

                Label {
                    text: displayAngle.toFixed(1) + "°"
                    font { pixelSize: 24; family: goRegular.name }
                    color: sensor.valid ? "#555" : "#999"
                    anchors {
                        top: parent.top
                        topMargin: 5
                        horizontalCenter: parent.horizontalCenter
                    }
                }
            }

            RoundButton {
                text: "ZERO"
                font { pixelSize: 24; bold: true; family: goRegular.name }
                Layout.alignment: Qt.AlignHCenter
                implicitWidth: 130
                implicitHeight: 48
                opacity: sensor.valid ? 1.0 : 0.45

                background: Rectangle {
                    radius: width / 2
                    color: parent.pressed ? "#b91c1c" : (sensor.valid ? "#ef4444" : "#999999")
                    border { color: "blue"; width: 4 }
                }
                contentItem: Text {
                    text: parent.text
                    font: parent.font
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                onClicked: zeroOffset = sensor.angle
            }
        }
    }

    Item {
            id: contentContainer
            anchors.fill: parent
            width:  embeddedDev ? root.height : root.width
            height: embeddedDev ? root.width  : root.height
            rotation: embeddedDev ? 270 : 0
            anchors.centerIn: parent

            ToolBar {
                id: rotatedHeader
                width: parent.width
                anchors.top: parent.top
                Label {
                    anchors.centerIn: parent
                    text: "I2C Demo - AS5600L (3 Sensors)"
                    font { pixelSize: 22; bold: true; family: goRegular.name }
                }
            }

            RowLayout {
                anchors.fill: parent
                anchors.margins: 16
                spacing: 18

                Repeater {
                    model: [
                        { sensor: sensor1, title: "Sensor 1 (0x40)" },
                        { sensor: sensor2, title: "Sensor 2 (0x41)" },
                        { sensor: sensor3, title: "Sensor 3 (0x42)" }
                    ]
                    delegate: sensorPanel
                }
            }
            ToolBar {
                id: rotatedFooter
                width: parent.width
                anchors.bottom: parent.bottom
                Label {
                    anchors.centerIn: parent
                    text: "Stelth Robotics 2026"
                }
            }
        }
}
