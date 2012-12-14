#include <QtGui/QApplication>

#include <QStandardItemModel>
#include <QTableView>
#include <QHBoxLayout>
#include <QTreeView>
#include <QDebug>
#include <QDateTime>
#include <QPushButton>
#include <QScrollBar>


#include "charts/chart.h"

int main( int argc, char* argv[] ) {
  QApplication a(argc, argv);

  QStandardItemModel* model = new QStandardItemModel(8, 2);
  model->setHeaderData(0, Qt::Horizontal, ("Label"));
  model->setHeaderData(1, Qt::Horizontal, ("Quantity"));
  model->removeRows(0, model->rowCount(QModelIndex()), QModelIndex());
  return a.exec();
}
