#include "thistleproxywidget.h"
#include <QApplication>

#include <QQuickView>

int main(int argc, char **argv) {
QApplication app(argc, argv);
QQuickView view;
//qmlRegisterType<PushButtonItem>("PushButton", 1, 0, "PushButtonItem");
view.setSource(QUrl::fromLocalFile("../../../src/qmake/Sandbox/Sandbox.qml"));
view.show();
return app.exec();
}
