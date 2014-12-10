import QtQuick 2.3
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2

Component {
    ColumnLayout {
        id: container
        spacing: 0
        height: tagInfo.height + (is_selected ? buyTool.height : 0)

        readonly property bool is_selected: ListView.view.currentIndex === index;
        readonly property int in_bank_width: 60
        readonly property int value_width: 60

        anchors {
            left: parent.left;
            right: parent.right;
        }

        RowLayout {
            id: tagInfo
            height: 30
            spacing: 0

            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true

                Text {
                    text: hashtag
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
            }

            Item {
                width: in_bank_width
                Layout.fillHeight: true
                Text {
                    text: in_bank
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                }
            }

            Item {
                width: value_width
                Layout.fillHeight: true

                Text {
                    text: value
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
            id: buyTool
            visible: container.is_selected
            height: button1.height

            Button {
                id: button1
                width: 90
                text: "Buy"
                enabled: toBuy.value > 0

                onClicked: container.ListView.view.model.buy(hashtag, toBuy.value)
            }

            Slider {
                id: toBuy
                Layout.fillWidth: true
                minimumValue: 0
                maximumValue: in_bank
                stepSize: 0.1
            }

            Item {
                width: value_width
                height: parent.height

                Text {
                    clip: true
                    elide: Text.ElideRight
                    anchors.fill: parent
                    text: toBuy.value * value
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }
}
