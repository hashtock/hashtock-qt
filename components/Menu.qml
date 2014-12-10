import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtPositioning 5.3
import QtQuick.Controls 1.2
import QtQuick.Window 2.1
import QtQuick.Controls.Styles 1.2
import "fontawesome.js" as FontAwesome

Rectangle {
    signal userClicked
    signal ordersClicked
    signal historyClicked
    signal bankClicked

    FontLoader {
        source: "fonts/fontawesome-webfont.ttf"
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0

        MenuButton {
            text: FontAwesome.Icon.User
            onClicked: userClicked()
        }

        MenuButton {
            text: FontAwesome.Icon.Exchange
            onClicked: ordersClicked()
        }

        MenuButton {
            text: FontAwesome.Icon.History
            onClicked: historyClicked()
        }

        MenuButton {
            text: FontAwesome.Icon.Bank
            onClicked: bankClicked()
        }
    }
}
