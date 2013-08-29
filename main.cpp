#include <QApplication>
#include <QStandardItemModel>
#include <QTableView>
#include <QHBoxLayout>
#include <QTreeView>
#include <QTreeView>
#include <QPushButton>
#include <QScrollBar>

#include "trees/horizontaltree.h"
#include "trees/verticaltree.h"
#include "Trees/radialtree.h"
#include "Trees/treeitemdelegate.h"
#include "Trees/tree.h"
#include "Graphs/graph.h"

#include "charts/linearchart.h"
#include "charts/piechart3D.h"

#include <QPropertyAnimation>
#include <QLabel>

#include <QGraphicsView>
#include <QGraphicsScene>

int main( int argc, char* argv[] ) {
  QApplication a(argc, argv);

  QStandardItemModel* model = new QStandardItemModel(0, 2);

  Tree* tree = new HorizontalTree();
  /*VerticalTree* tree = new VerticalTree();*/
  tree->setModel( model );
  //tree->setItemSpacing( 50, 10 );
  tree->setItemSize( QSize( 60, 30 ) );
  tree->setConnectionPen( QPen( QColor( 190, 190, 190 ), 2 ) );
  tree->setMinimumSize( 500, 500 );

  /*RadialTree* radial = new RadialTree();
  radial->setModel( model );
  radial->setItemSize( QSize(40, 40) );
  radial->setConnectionPen( QPen( QColor( 190, 190, 190 ), 2 ) );*/

  QStandardItem* root = new QStandardItem( "Root" );
  QStandardItem* it1 = new QStandardItem( "0" );
  for( int i = 0; i < 2; ++i ) {
    it1->appendRow( new QStandardItem( QString( "0%1" ).arg( i ) ) );
  }

  root->appendRow( it1 );
  it1 = new QStandardItem( "1" );
  for( int i = 0; i < 2; ++i ) {
    it1->appendRow( new QStandardItem( QString( "1%1" ).arg( i ) ) );
  }

  root->appendRow( it1 );
  for ( int i = 2; i < 4; ++i ) {
    it1 = new QStandardItem( QString("%1").arg( i ) );
    root->appendRow( it1 );
  }

  it1 = new QStandardItem( "2" );

  for( int i = 0; i < 2; ++i ) {
    QStandardItem* it2 = new QStandardItem( QString( "2%1" ).arg( i ) );
    it1->appendRow( it2 );
    for ( int j = 0; j < 3; ++j ) {
      QStandardItem* it3 = new QStandardItem( QString( "2%1%2" ).arg( i ).arg( j ) );
      for ( int k = 0; k < 3; ++k ) {
        QStandardItem* it4 = new QStandardItem( QString( "2%1%2%3" ).arg( i ).arg( j ).arg( k ) );
        it3->appendRow( it4 );
      }
      it2->appendRow( it3 );
    }
  }
  //root->appendRow( it1 );

  model->appendRow( root );

  /*radial->resolvePositions();
  vertical->resolvePositions();
  horizontal->resolvePositions();*/

  model = new QStandardItemModel( 0, 1 );
  for( int i = 0; i < 6; ++i  ) {
    model->appendRow( new QStandardItem( QChar( i + 65 ) ) );
  }


  Graph graph;
  graph.setModel( model );

  graph.addEdge( 0, 0, 1, 0, Edge::Unilateral );
  graph.addEdge( 1, 0, 2, 0, Edge::Unilateral );
  graph.addEdge( 1, 0, 3, 0, Edge::Unilateral );
//  graph.addEdge( 3, 0, 2, 0, Edge::Unilateral );
  graph.addEdge( 5, 0, 2, 0, Edge::Unilateral );
  graph.addEdge( 2, 0, 4, 0, Edge::Unilateral );
  graph.addEdge( 1, 0, 5, 0, Edge::Unilateral );
  /*graph.addEdge( 4, 0, 7, 0 );
  graph.addEdge( 7, 0, 6, 0 );
  graph.addEdge( 6, 0, 8, 0 );
  graph.addEdge( 3, 0, 8, 0 );
  graph.addEdge( 8, 0, 9, 0 );
  graph.addEdge( 2, 0, 9, 0 );*/


  //graph.show();
  //graph.updateValues();



  QStandardItemModel* model2 = new QStandardItemModel( 10, 2 );
  for ( int i = 0; i < 10; ++i ) {
    model2->setData( model2->index( i, 0 ), i + 10 );
    model2->setData( model2->index( i, 1 ), -i );
  }

  LinearChart linear;
  linear.setModel( model2 );
  ChartStyle style = linear.columnStyle( 0 );
  style.setType( Marb::Bar );
  linear.setColumnStyle( 0, style );
  //linear.show();

  PieChart3D pie;
  pie.setModel( model2 );
  pie.show();

  RadialTree t;
  t.setModel( model );
  //t.show();

  QPropertyAnimation animation( &pie, "startAngle" );
  animation.setLoopCount( 2 );
  animation.setStartValue( 0 );
  animation.setEndValue( 360 );
  animation.setDuration( 6000 );
  //animation.start();

  return a.exec();
}
