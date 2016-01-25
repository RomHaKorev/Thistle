#include <QApplication>
#include <QStandardItemModel>
#include <QTableView>
#include <QHBoxLayout>
#include <QTreeView>
#include <QTreeView>
#include <QListView>
#include <QScrollBar>
#include <QDebug>
#include <QStringListModel>
#include <QLayout>
#include <QLabel>
#include <QListWidget>
#include <QPointer>
#include <QTime>

#include "Thistle/Trees/treeview.h"
#include "Thistle/Trees/radialtreelayout.h"

#include "Thistle/Core/itemdelegate.h"
#include "Thistle/Graphs/graphview.h"
#include "Thistle/Graphs/graphmodel.h"

#include "Thistle/Charts/line/linechart.h"
#include "Thistle/Charts/line/linechartwidget.h"
#include "Thistle/Charts/scatter/scatterchart.h"
#include "Thistle/Charts/kiviat/kiviatchart.h"
#include "Thistle/Charts/pie/piechart.h"
#include "Thistle/Charts/pie/piechart3d.h"
#include "Thistle/Charts/pie/pielegendview.h"
#include "Thistle/Charts/base/abstractcoordinatesystemview.h"
#include "Thistle/Charts/line/linechartwidget.h"
#include "Thistle/Charts/line/linelegendview.h"

#include <iostream>

bool compare( const QImage& img1, const QImage& img2 )
{
	int i = 0;
	if ( img1.size() != img2.size() )
		return false;
	for ( int x = 0; x != img1.width(); ++x )
	{
		for ( int y = 0; y != img1.height(); ++y )
		{
			if ( img1.pixel(x,y) != img2.pixel(x,y) )
				++i;
		}
	}
	qDebug() << i;
	return i < 20;
}

QPixmap snapshot( QWidget& w )
{
	QPixmap pix( w.size() );
	pix.fill( Qt::transparent );
	QPainter painter( &pix );
	w.render( &painter, QPoint(), QRegion(), QWidget::DrawChildren );
	painter.end();
	return pix;
}

void save( QWidget& w, const QString& name )
{
	snapshot( w ).save( name );
}


#define LINE

int main( int argc, char* argv[] ) {
	QApplication a(argc, argv);
#ifdef TEST
	int high = 40;
	int low = 10;

	qsrand( QTime::currentTime().msec() );

	QStandardItemModel* model2 = new QStandardItemModel( 5, 2 );
	//model2->setHorizontalHeaderLabels( QStringList() << "My values" << "My second values" );
	model2->setHorizontalHeaderLabels( QStringList() << "My values" );
	//model2->setVerticalHeaderLabels( QStringList() << "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun" << "Jul" << "Aug" << "Sept" << "Oct" << "Nov" << "Dec" );
	for ( int i = 0; i < model2->rowCount(); ++i )
	{
		for ( int j = 0; j < model2->columnCount(); ++j )
		{
			int v = qrand() % ((high + 1) - low) + low;
			model2->setData( model2->index( i, j ), v );
		}
		/*model2->setData( model2->index( i, 1 ), 5 );
		if ( i%2 ) model2->setData( model2->index( i, 2 ), 1 );
		else model2->setData( model2->index( i, 2 ), 3 );*/
	}

	model2->setData( model2->index( 1, 0 ), QLineF( 10, 50, 50, 65 ), Qt::UserRole );

	
	Thistle::LineChartWidget widget;
	widget.setModel( model2 );
	widget.show();

#endif
#ifdef PIECHART
	int high = 40;
	int low = 10;

	qsrand( QTime::currentTime().msec() );

	QStandardItemModel* model = new QStandardItemModel( 6, 1 );
	model->setHorizontalHeaderLabels( QStringList() << "My values" );
	model->setVerticalHeaderLabels( QStringList() << "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun" );
	for ( int i = 0; i < model->rowCount(); ++i )
	{
		int v = qrand() % ((high + 1) - low) + low;
		model->setData( model->index( i, 0 ), 2 );
	}

	Thistle::PieChart3D pie;
	pie.setFrameShape( QFrame::NoFrame );
	pie.setModel( model );
	pie.show();
	pie.resize( 400, 275 );

#endif
#ifdef POINTCHART
    int high_x = 10;
    int low_x = -10;
    int high_y = 100;
    int low_y = -200;

    qsrand( QTime::currentTime().msec() );

    QStandardItemModel* model = new QStandardItemModel( 15, 1 );
    for ( int i = 0; i < model->rowCount(); ++i )
    {
        for ( int j = 0; j < model->columnCount(); ++j )
        {
            int x = qrand() % ((high_x + 1) - low_x) + low_x;
            int y = qrand() % ((high_y + 1) - low_y) + low_y;

            model->setData( model->index( i, j ), QPointF( x, y ) );
        }
    }

    model->setData( model->index( 1, 0 ), QLineF( 10, 50, 50, 65 ), Qt::UserRole );


    Thistle::PointChart xy;
    xy.setFrameShape( QFrame::NoFrame );
    xy.setModel( model );
    xy.show();
    xy.resize( 400, 275 );
#endif
#ifdef KIVIATCHART
	int high_x = 10;
	int low_x = -10;
	int high_y = 0;
	int low_y = 200;

	qsrand( QTime::currentTime().msec() );

	QStandardItemModel* model = new QStandardItemModel( 5, 2 );
	for ( int i = 0; i < model->rowCount(); ++i )
	{
		for ( int j = 0; j < model->columnCount(); ++j )
		{
			qreal x = qrand() % ((high_x + 1) - low_x) + low_x;
			qreal y = qrand() % ((high_y + 1) - low_y) + low_y;

			model->setData( model->index( i, j ), y );
		}
	}

	model->setData( model->index( 1, 0 ), QLineF( 10, 50, 50, 65 ), Qt::UserRole );
	model->setVerticalHeaderLabels( QStringList() << "Et maintenant, si on testait avec un titre beaucoup beaucoup trop long." << "Et maintenant, si on testait avec un titre beaucoup beaucoup trop long." );

	Thistle::KiviatChart kiviat;
	kiviat.setFrameShape( QFrame::NoFrame );
	kiviat.setModel( model );

	Thistle::SerieFormat style = kiviat.serieFormat( 0 );
	style.setType( Thistle::Area | Thistle::Line /*Thistle::Spline /*| Thistle::Dot*/ /*| Thistle::Thistle::Area*/ );
	QPen pen = style.pen();
	pen.setWidth( 2 );
	style.setPen( pen );
	QColor br = style.brush().color();
	br.setAlpha( 100 );
	style.setBrush( br );
	
	kiviat.setSerieFormat( 0, style );

	kiviat.show();
	kiviat.resize( 400, 275 );
#endif
#ifdef LINE
    int high = 40;
    int low = 10;

    qsrand( QTime::currentTime().msec() );

	QList<int> values;
	values << 10 << 0 << 20 << 0 << 2 << 40 << 30 << 15 << 8 << 23 << 6 << 17;
    QStandardItemModel* model2 = new QStandardItemModel( values.count(), 1 );
    model2->setHorizontalHeaderLabels( QStringList() << "My values" << "My second values" );
    model2->setHorizontalHeaderLabels( QStringList() << "My values" );
    model2->setVerticalHeaderLabels( QStringList() << "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun" << "Jul" << "Aug" << "Sept" << "Oct" << "Nov" << "Dec" );
    for ( int i = 0; i < model2->rowCount(); ++i )
    {
		model2->setData( model2->index( i, 0 ), values[i] );
    }

    model2->setData( model2->index( 1, 0 ), QLineF( 10, 50, 50, 65 ), Qt::UserRole );

	Thistle::LineChartWidget w1;
	w1.setFixedSize( 600, 600 );
	w1.setModel( model2 );
	w1.setTitle( "Test title" );
	w1.show();

	Thistle::LineChartWidget w2;
	w2.setFixedSize( 600, 600 );
	w2.setModel( model2 );
	w2.setTitle( "Test title" );
	w2.show();

	QPixmap pix1 = snapshot( w1 );
	QPixmap pix2( "w:/test1.bmp");

	qDebug() << compare(pix1.toImage(), pix2.toImage());

	//save( w, "w:/test1.png" );

    Thistle::SerieFormat style = w1.chart()->serieFormat( 0 );
    style.setType( /*Thistle::Area | Thistle::Line */Thistle::Line | Thistle::Dot /*| Thistle::Thistle::Area*/ );
    QPen pen = style.pen();
    pen.setWidth( 3 );
	pen.setBrush( QBrush(Thistle::Colors::Blue) );
    style.setPen( pen );
	style.setBrush( Qt::red );
    w1.chart()->setSerieFormat( 0, style );
    style = w1.chart()->serieFormat( 1 );
    style.setType( Thistle::Bar );
    w1.chart()->setSerieFormat( 1, style );
	//save( w, "w:/test2.png" );
#endif

#ifdef CHARTWIDGET
	int high = 40;
	int low = 10;

	qsrand( QTime::currentTime().msec() );

	QStandardItemModel* model = new QStandardItemModel( 6, 1 );
	model->setHorizontalHeaderLabels( QStringList() << "My values" );
	model->setVerticalHeaderLabels( QStringList() << "Jan" << "Feb" << "Mar" << "Apr" << "May" /*<< "Jun" << "Jul" << "Aug" << "Sept" << "Oct" << "Nov" << "Dec"*/ );
	for ( int i = 0; i < model->rowCount(); ++i )
	{
		for ( int j = 0; j < model->columnCount(); ++j )
		{
			int v = qrand() % ((high + 1) - low) + low;
			model->setData( model->index( i, j ), v );
		}
	}

	model->setData( model->index( 1, 0 ), QLineF( 10, 50, 50, 65 ), Qt::UserRole );

	Thistle::LinearChartWidget* c = new Thistle::LinearChartWidget();
	Thistle::LinearChart& linear = c->chart();

	Thistle::SerieFormat style = linear.serieFormat( 1 );
	style.setType( /*Thistle::Area | Thistle::Line */Thistle::Spline /*| Thistle::Dot*/ /*| Thistle::Thistle::Area*/ );
	QPen pen = style.pen();
	pen.setWidth( 3 );
	style.setPen( pen );
	linear.setSerieFormat( 1, style );
	style = linear.serieFormat( 0 );
	style.setType( Thistle::Bar );
	style.setShape( Thistle::Shape::Triangle );
	linear.setSerieFormat( 0, style );

	c->setModel( model );
	c->show();
#endif

#ifdef GRAPH
    Thistle::GraphModel* model = new Thistle::GraphModel(0,1);
    for( int i = 0; i < 9; ++i  ) {
        model->appendRow( new QStandardItem( QString::number( i ) ) );
    }

    Thistle::GraphView graph;
    //graph.setFixedSize( 500, 500 );
    graph.setModel( model );
    model->addEdge( 0, 0, 1, 0 );
    model->addEdge( 0, 0, 5, 0 );
    model->addEdge( 0, 0, 7, 0 );
    model->addEdge( 1, 0, 4, 0 );
    model->addEdge( 1, 0, 9, 0 );
    model->addEdge( 2, 0, 3, 0 );
    model->addEdge( 3, 0, 6, 0 );
    model->addEdge( 4, 0, 7, 0 );
    model->addEdge( 2, 0, 5, 0 );
    model->addEdge( 6, 0, 8, 0 );
    model->addEdge( 0, 0, 8, 0 );
    model->addEdge( 6, 0, 5, 0 );
    model->addEdge( 2, 0, 1, 0 );
    graph.show();

	
	Thistle::ItemDelegate* d = new Thistle::ItemDelegate();
	Thistle::ItemStyle style = d->itemStyle();
	style.setShape( Thistle::Ellipse );


	style.setBrush( QBrush( Thistle::Colors::Blue ), true );
	style.setBrush( QBrush( Thistle::Colors::Gray ), false );
	style.setBorderPen( QPen( QColor( Thistle::Colors::DarkBlue ), 1.5 ), true );
	style.setBorderPen( QPen( QColor( Thistle::Colors::DarkGray ), 1.5 ), false );
	d->setItemStyle( style );
	
	graph.setItemDelegate( d );

#endif

#ifdef TREE
    QStandardItemModel* model = new QStandardItemModel( 0, 1 );
    QStandardItem* root = new QStandardItem( "root" );
    QStandardItem* A = new QStandardItem( "A" );
    QStandardItem* B = new QStandardItem( "B" );
    QStandardItem* C = new QStandardItem( "C" );
    QStandardItem* D = new QStandardItem( "D" );
    QStandardItem* E = new QStandardItem( "E" );
    QStandardItem* F = new QStandardItem( "F" );
    QStandardItem* G = new QStandardItem( "G" );
    QStandardItem* H = new QStandardItem( "H" );
    QStandardItem* I = new QStandardItem( "I" );
    QStandardItem* J = new QStandardItem( "J" );

    root->appendRow( A );
    root->appendRow( B );
    root->appendRow( C );
    
    A->appendRow( D );
    A->appendRow( E );

    C->appendRow( F );

    D->appendRow( G );
    D->appendRow( H );

    E->appendRow( I );

    I->appendRow( J );

    model->appendRow( root );

    Thistle::TreeView* tree = new Thistle::TreeView();

    Thistle::ItemDelegate* d = new Thistle::ItemDelegate();
    Thistle::ItemStyle style = d->itemStyle();
    style.setShape( Thistle::Ellipse );
    d->setItemStyle( style );
    
    tree->layout()->setOrientation( Qt::Horizontal );
    tree->layout()->deleteLater();
    tree->setLayout( new Thistle::RadialTreeLayout() );
    tree->setItemDelegate( d );
    tree->setModel( model );

    tree->show();

#endif
    return a.exec();
}

