import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import i2c_stmp_demo 1.0

Window {
    width: 720; height: 540
    visible: true
    title: qsTr("AS5600L Magnetic Angle Sensor")

    FontLoader { id: goRegular; source: Qt.resolvedUrl("go_fonts/Go-Regular.ttf") }

    AS5600Sensor { id: sensor1; active: true }
    AS5600Sensor { id: sensor2; active: true }

    property real zeroOffset: 0
    property real displayAngle: {
        let a = ((sensor1.angle - zeroOffset) % 360 + 360) % 360
        return a > 180 ? a - 360 : a
    }

    ColumnLayout {
        anchors { fill: parent; margins: 20 }
        spacing: 12

        Label {
            text: "AS5600L Magnetic Angle Sensor"
            font { pixelSize: 22; bold: true; family: goRegular.name }
            Layout.alignment: Qt.AlignHCenter
        }

        RoundButton {
            text: "ZERO"
            font { pixelSize: 28; bold: true; family: goRegular.name }
            Layout.alignment: Qt.AlignHCenter
            padding: 0
            implicitWidth: 150; implicitHeight: 150
            background: Rectangle {
                radius: width / 2
                color: parent.pressed ? "#b91c1c" : "#ef4444"
                border { color: "#ffffff"; width: 5 }
            }
            contentItem: Text {
                text: parent.text; font: parent.font; color: "white"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            onClicked: zeroOffset = sensor1.angle
        }

        RowLayout {
            spacing: 50
            Layout.alignment: Qt.AlignHCenter

            Dial {
                from: -180; to: 180; value: displayAngle
                Layout.preferredWidth: 175; Layout.preferredHeight: 175
                Text {
                    anchors.centerIn: parent
                    text: Math.round(displayAngle) + "°"
                    font { pixelSize: 36; bold: true; family: goRegular.name }
                    color: "#1f2937"
                }
            }
            Item {
                Layout.preferredWidth: 250; Layout.preferredHeight: 250
                Image {
                    source: Qt.resolvedUrl("assets/dial_no_background.png")
                    fillMode: Image.PreserveAspectFit
                    smooth: true; antialiasing: true
                    width: 250; height: 250
                    rotation: displayAngle + 140
                }
                Label {
                    text: displayAngle.toFixed(1) + "°"
                    font { pixelSize: 25; family: goRegular.name }
                    color: "#555"
                    anchors {
                        top: parent.top; topMargin: 12
                        horizontalCenter: parent.horizontalCenter
                    }
                }
                Label {
                    text: "Magnet: " +sensor1.status
                    font { pixelSize: 25; family: goRegular.name }
                    color: "#ff0000"
                    anchors {
                        bottom: parent.bottom; bottomMargin: 12
                        horizontalCenter: parent.horizontalCenter
                    }
                }
            }
        }
    }
}
