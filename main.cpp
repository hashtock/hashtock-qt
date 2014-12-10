#include <QtGui>
#include <QtQuick>

#include "user.h"
#include "qmlnetfactory.h"
#include "webkitcookiejar.h"
#include "statemaster.h"
#include "tag.h"
#include "order.h"
#include "hashtockapi.h"
#include "qmlmessenger.h"

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);
    qmlRegisterType<User>("Api", 1, 0, "User");
    qmlRegisterType<TagModel>("Api", 1, 0, "TagModel");
    qmlRegisterType<OrderModel>("Api", 1, 0, "OrderModel");
    qmlRegisterType<StateEvents>("AppStates", 1, 0, "StateEvents");
    qmlRegisterType<QmlMessenger>("Utils", 1, 0, "Messenger");

    QQuickView view;
    QQmlContext *ctx = view.rootContext();
    view.engine()->setNetworkAccessManagerFactory(new QMLNetFactory);
    view.setResizeMode(QQuickView::SizeRootObjectToView);

    StateMaster state_master;
    ctx->setContextProperty("StateMaster", &state_master);

    view.setSource(QUrl(QStringLiteral("qrc:/main.qml")));
    view.show();

    return app.exec();
}

