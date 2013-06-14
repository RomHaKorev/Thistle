#include <QApplication>

#include <QStandardItemModel>
#include <QTableView>
#include <QHBoxLayout>
#include <QTreeView>
#include <QDebug>
#include <QDateTime>
#include <QPushButton>
#include <QScrollBar>

#include <QtNetwork/QNetworkInterface>


#include "charts/linearchart.h"
#include "charts/radialchart.h"
#include "charts/piechart.h"
#include "charts/piechart3D.h"


#include <QFile>

int main( int argc, char* argv[] ) {
  QApplication a(argc, argv);

  QStandardItemModel* model = new QStandardItemModel(7,2);
  model->setHeaderData(0, Qt::Horizontal, ("Label"));
  model->setHeaderData(1, Qt::Horizontal, ("Value"));
  qsrand( QDateTime::currentDateTime().toTime_t() );
  QColor color( 0x083991 );
  for (int i = 0; i < model->rowCount(); ++i ) {
    qreal v = (qrand() % ( 30 - 10) + 10);
    model->setData( model->index( i, 0 ), v, Qt::DisplayRole );
    v = (qrand() % ( 50 - 10) + 10);
    model->setData( model->index( i, 1 ), v, Qt::DisplayRole );
    //model->setData( model->index( i, 0 ), color, Qt::DecorationRole );
    //color = color.lighter( 120 );
//    v = (qrand() % ( 25));
//    model->setData( model->index( i, 1 ), v, Qt::DisplayRole );
//    v = (qrand() % ( 25));
//    model->setData( model->index( i, 2 ), v, Qt::DisplayRole );
//    model->setData( model->index( i, 3 ), v - v/2, Qt::DisplayRole );
//    v = (qrand() % ( 25));
//    model->setData( model->index( i, 4 ), 3, Qt::DisplayRole );
  }

  /*QVariant(double, 20)
  QVariant(double, 12)
  QVariant(double, 20)
  QVariant(double, 24)
  QVariant(double, 17)
  QVariant(double, 14) */

//  model->setData( model->index( 0, 0 ), 20, Qt::DisplayRole );
//  model->setData( model->index( 1, 0 ), 20, Qt::DisplayRole );
//  model->setData( model->index( 2, 0 ), 20, Qt::DisplayRole );
//  model->setData( model->index( 3, 0 ), 24, Qt::DisplayRole );
//  model->setData( model->index( 4, 0 ), 17, Qt::DisplayRole );
//  model->setData( model->index( 5, 0 ), 14, Qt::DisplayRole );

  qDebug() << "Values";
  for (int i = 0; i < model->rowCount(); ++i ) {
    qDebug() << model->data( model->index( i, 0 ) );
  }
  qDebug() << "!!!Values";


  /*QColor c( 50, 50, 90 );
  for (int i = 0; i < model->rowCount(); ++i ) {
    model->setData( model->index( i, 0 ), c, Qt::DecorationRole );
    c = c.lighter( 120 );
  }*/

  //model->setData( model->index( 0, 0 ), -24, Qt::DisplayRole );
  //model->setData( model->index( 1, 0 ), 80, Qt::DisplayRole );
  /*PieChart ring;
  ring.setModel( model );
  ring.setRing( true );
  ring.show();
  ring.setGeometry( 500, 100, 500, 500 );

  PieChart pie;
  pie.setSplitted( true );
  pie.setModel( model );
  pie.show();
  pie.setFixedSize( 200, 200 );
  qDebug() << pie.save( "bluePie.png" );*/

  LinearChart chart;
  chart.setModel( model );
  ChartStyle style = chart.columnStyle( 0 );
  style.setType( Marb::Bar );
  chart.setColumnStyle( 0, style );
  chart.setTitle( "Test title" );
  chart.show();

  return a.exec();
}
