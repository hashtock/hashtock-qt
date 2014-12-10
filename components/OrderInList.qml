import QtQuick 2.3
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2
import "fontawesome.js" as FontAwesome

Component {
    ColumnLayout {
        id: container
        spacing: 0
        height: orderInfo.height + (is_selected ? cancelTool.height : 0)

        anchors.left: parent.left
        anchors.right: parent.right

        readonly property bool is_buy: action == "buy"
        readonly property bool is_selected: ListView.view.currentIndex === index
        readonly property int action_width: 20
        readonly property int quantity_width: 60
        readonly property int from_width: 60

        FontLoader {
            source: "fonts/fontawesome-webfont.ttf"
        }

        RowLayout {
            id: orderInfo
            height: 30
            Layout.fillWidth: true;
            spacing: 0

            Item {
                Layout.fillHeight: true
                width: action_width

                Text {
                    text: is_buy ? FontAwesome.Icon.Plus : FontAwesome.Icon.Minus
                    color: is_buy ? "green" : "red"
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true

                Text {
                    text: hashtag
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
            }

            Item {
                width: quantity_width
                Layout.fillHeight: true

                Text {
                    text: quantity.toFixed(2)
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                }
            }

            Item {
                width: from_width
                Layout.fillHeight: true

                Text {
                    text: bank_order ? "bank" : "market";
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                }
            }

            MouseArea {
                id: mouseArea
                anchors.fill: parent

                onClicked: {
                    var new_value = index;
                    if (container.is_selected) {
                        new_value = -1;
                    }

                    container.ListView.view.currentIndex = new_value;
                }
            }
        }

        RowLayout {
            id: cancelTool
            anchors.right: parent.right
            anchors.left: parent.left
            visible: container.is_selected && !complete

            Button {
                width: 90
                text: "Cancel"
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: container.ListView.view.model.cancel(uuid)
            }
        }
    }
}
