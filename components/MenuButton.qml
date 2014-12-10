import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import QtQuick.Layouts 1.1

Button {
    Layout.fillHeight: true
    Layout.fillWidth: true

    style: ButtonStyle {
        background: Rectangle {
            color: "black"
            opacity: control.pressed ? 0.1 : 0
        }

        label: Text {
            renderType: Text.NativeRendering
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: parent.height * 0.8
            text: control.text
        }
    }
}
