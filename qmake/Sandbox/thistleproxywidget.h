#ifndef THISTLEPROXYWIDGET_H
#define THISTLEPROXYWIDGET_H

#include <QQuickItem>
#include <QGraphicsProxyWidget>

class ThistleProxyWidget : public QQuickItem
{
  Q_OBJECT
private:
  QGraphicsProxyWidget proxy;
public:
  ThistleProxyWidget(QQuickItem *parent = 0);
  ~ThistleProxyWidget();
};


/*
class PushButtonItem : public QQuickItem {
public:
PushButtonItem(QQuickItem *parent =0) : QQuickItem(parent) {
pb = new QPushButton("text");
proxy = new QGraphicsProxyWidget();
proxy->setWidget(pb);
proxy->setPos(-pb->sizeHint().width()/2, -pb->sizeHint().height()/2);
}
private:
QPushButton *pb;
QGraphicsProxyWidget *proxy;
};
 */

#endif // THISTLEPROXYWIDGET_H
