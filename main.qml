import QtQuick 2.3
import QtQuick.Controls 1.2
//import QtWebKit 3.0
import QtQuick.Layouts 1.1
import "components" 1.0 as Components
import Api 1.0
import AppStates 1.0
import Utils 1.0
import "platform" 1.0 as Platform

Item {
    id: app
    width: 320
    height: 480
    state: StateMaster.state

    Messenger {
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 70
        height: 40
        z: 999;

        ListModel {
            id: msgQueue
        }

        Timer {
            id: hidder
            interval: 3000
            running: false
            repeat: false
            onTriggered: {
                if (msgQueue.count > 0) {
                    start()
                    msgQueue.remove(0, 1);
                }
            }
        }

        onPrintableMessage: {
            msgQueue.append({desc: pritable});

            if (!hidder.running) {
                hidder.start()
            }
        }

        ListView {
            id: lstMsgs
            anchors.fill: parent
            visible: true
            interactive: false
            verticalLayoutDirection: ListView.BottomToTop
            model:  msgQueue
            delegate: Rectangle {
                height: dlgLabel.height
                width: dlgLabel.width + 10
                color: "white"
                anchors.horizontalCenter: parent.horizontalCenter

                Text {
                    id: dlgLabel
                    text: desc
                    anchors.horizontalCenter: parent.horizontalCenter
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }

    User {
        id: currentUser
        onLoginRequired: StateMaster.consumeEvent(StateEvents.LoginRequired)
        onLoginSucessful: StateMaster.consumeEvent(StateEvents.LoginSuccessful)
    }

    Rectangle {
        id: splashView
        visible: false
        anchors.fill: parent

        onVisibleChanged: {
            if (visible) {
                currentUser.loadData();
            }
        }

        Label {
            id: label1
            text: qsTr("Loading...")
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
    }

    Platform.LoginView {
        id: loginView
        anchors.fill: parent
        visible: false
    }

//    WebView {
//        id: loginView
//        anchors.fill: parent
//        visible: false

//        onLoadingChanged: {
//            if (loading == false && loadRequest.url == currentUser.loginUrl) {
//                StateMaster.consumeEvent(StateEvents.LoginCompleted)
//            }
//        }
//    }

    ColumnLayout {
        id: loggedInView
        anchors.fill: parent
        visible: false
        spacing: 0

        ColumnLayout {
            id: userView
            visible: false
            Layout.fillHeight: true
            Layout.fillWidth: true

            onVisibleChanged: {
                if (visible) {
                    currentUser.loadData();
                }
            }

            Image {
                id: profile_img
                fillMode: Image.PreserveAspectFit
                source: "static/profile_placeholder.png"
                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            Label {
                text: currentUser.name
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
            }

            Label {
                text: currentUser.founds
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
            }
        }

        ColumnLayout {
            id: bankView
            anchors.fill: parent
            visible: false

            onVisibleChanged: {
                if (visible) {
                    bankTags.model.loadData();
                }
                bankTags.delegate = visible ? tagDelegate : null;
            }

            Components.TagInList {
                id: tagDelegate
            }

            ListView {
                id: bankTags
                currentIndex: -1
                Layout.fillHeight: true
                Layout.fillWidth: true

                model: TagModel {}

                delegate: tagDelegate
                highlight: Rectangle { color: "lightsteelblue"; radius: 2 }
            }
        }

        ColumnLayout {
            id: orderView
            anchors.fill: parent
            Layout.fillWidth: true
            visible: false
            spacing: 0

            onVisibleChanged: {
                if (visible) {
                    pendingOrders.model.loadData();
                }
                pendingOrders.delegate = visible ? orderDelegate : null;
            }

            Components.OrderInList {
                id: orderDelegate
            }

            Text {
                id: noPendingOrders
                visible: !pendingOrders.visible
                Layout.fillHeight: true
                Layout.fillWidth: true
                text: "No pending orders"
            }

            ListView {
                id: pendingOrders
                currentIndex: -1
                Layout.fillHeight: true
                Layout.fillWidth: true

                model: OrderModel {}

                delegate: orderDelegate
                highlight: Rectangle { color: "lightsteelblue"; radius: 2 }
            }
        }

        Components.Menu {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignBottom
            Layout.preferredHeight: 40

            onBankClicked: StateMaster.consumeEvent(StateEvents.GoToBankView)
            onUserClicked: StateMaster.consumeEvent(StateEvents.GoToUserView)
            onOrdersClicked: StateMaster.consumeEvent(StateEvents.GoToOrdersView)
            onHistoryClicked: StateMaster.consumeEvent(StateEvents.GoToHistoryView)
        }
    }

    states: [
        State {
            name: "splashState"
            PropertyChanges {target: splashView; visible: true }
        },
        State {
            name: "loginState"
            PropertyChanges {target: loginView; visible: true; url: currentUser.loginUrl; }
        },
        State {
            name: "_loggedInBaseState"
            PropertyChanges {target: loggedInView; visible: true; }
        },
        State {
            name: "userState"
            extend: "_loggedInBaseState"
            PropertyChanges {target: userView; visible: true }
        },
        State {
            name: "bankState"
            extend: "_loggedInBaseState"
            PropertyChanges {target: bankView; visible: true }
        },
        State {
            name: "orderState"
            extend: "_loggedInBaseState"
            PropertyChanges {target: orderView; visible: true }
        },
        State {
            name: "historyState"
            extend: "_loggedInBaseState"
            PropertyChanges {target: historyView; visible: true }
        }
    ]
}
