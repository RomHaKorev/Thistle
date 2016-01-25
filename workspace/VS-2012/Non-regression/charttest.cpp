#include "charttest.h"

#include <qstandarditemmodel.h>
#include <qpainter.h>
#include <Thistle/Charts/line/linechart.h>

bool ChartTest::compare( const QImage& img1, const QImage& img2 )
{
	int i = 0;
	if ( img1.size() != img2.size() )
		return false;
	for ( int x = 0; x != img1.width(); ++x )
		for ( int y = 0; y != img1.height(); ++y )
			if ( img1.pixel(x,y) != img2.pixel(x,y) )
				++i;
	return i < 20;
}

QPixmap ChartTest::snapshot( QWidget& w )
{
	QPixmap pix( w.size() );
	pix.fill( Qt::transparent );
	QPainter painter( &pix );
	w.render( &painter, QPoint(), QRegion(), QWidget::DrawChildren );
	painter.end();
	return pix;
}

void ChartTest::save( QWidget& w, const QString& name )
{
	ChartTest::snapshot( w ).save( name );
}


void ChartTest::init()
{
	chart = new Thistle::LineChartWidget();
	chart->setFixedSize( 600, 600 );
}

void ChartTest::cleanup()
{
	chart->deleteLater();
}

void ChartTest::test1()
{
	QList<double> values;
	values << 0 << 10 << 12 << 15 << 17 << 20 << 22 << 25 << 12 << 10;
	QStandardItemModel* model = new QStandardItemModel( 10, 1 );
	for ( int i = 0; i != values.count(); ++i )
		model->setData( model->index( i, 0 ), values[i] );
	chart->setModel( model );
	chart->setTitle( "w:/" + QString( Q_FUNC_INFO ).split(':').last().split('(').first() + ".bmp" );
	save( *chart, chart->title() );
}

void ChartTest::test2()
{
	QList<double> values;
	values << 0 << 10 << 12 << 15 << 17 << 20 << 22 << 25 << 12 << 10;
	QStandardItemModel* model = new QStandardItemModel( 10, 1 );
	for ( int i = 0; i != values.count(); ++i )
		model->setData( model->index( i, 0 ), -1 * values[i] );
	chart->setModel( model );
	chart->setTitle( "w:/" + QString( Q_FUNC_INFO ).split(':').last().split('(').first() + ".bmp" );
	save( *chart, chart->title() );
}

void ChartTest::test3()
{
	QList<double> values;
	values << 0 << -10 << 10 << 15 << -15 << 20 << -20 << 25 << -25 << 10;
	QStandardItemModel* model = new QStandardItemModel( 10, 1 );
	for ( int i = 0; i != values.count(); ++i )
		model->setData( model->index( i, 0 ), values[i] );
	chart->setModel( model );
	chart->setTitle( "w:/" + QString( Q_FUNC_INFO ).split(':').last().split('(').first() + ".bmp" );
	save( *chart, chart->title() );
}

void ChartTest::test4()
{
	QList<double> values;
	values << 0 << -10 << 10 << 15 << -15 << 20 << -20 << 25 << -25 << 10;
	QStandardItemModel* model = new QStandardItemModel( 10, 1 );
	for ( int i = 0; i != values.count(); ++i )
		model->setData( model->index( i, 0 ), values[i] * 100000 );
	chart->setModel( model );
	chart->setTitle( "w:/" + QString( Q_FUNC_INFO ).split(':').last().split('(').first() + ".bmp" );
	save( *chart, chart->title() );
}

void ChartTest::test5()
{
	QList<double> values;
	values << 0 << -10 << 10 << 15 << -15 << 20 << -20 << 25 << -25 << 10;
	QStandardItemModel* model = new QStandardItemModel( 10, 1 );
	for ( int i = 0; i != values.count(); ++i )
		model->setData( model->index( i, 0 ), values[i] * 0.000001 );
	chart->setModel( model );
	chart->setTitle( "w:/" + QString( Q_FUNC_INFO ).split(':').last().split('(').first() + ".bmp" );
	save( *chart, chart->title() );
}

void ChartTest::test6()
{
	QList<double> values1, values2;
	values1 << 0 << -10 << 10 << 15 << -15 << 20 << -20 << 25 << -25 << 10;
	values2 << 0 << 44 << -20 << 35 << 5 << -10 << -15 << 12 << 12 << 21;
	QStandardItemModel* model = new QStandardItemModel( 10, 2 );
	for ( int i = 0; i != values1.count(); ++i )
	{
		model->setData( model->index( i, 0 ), values1[i] );
		model->setData( model->index( i, 1 ), values2[i] );
	}
	chart->setModel( model );
	chart->setTitle( "w:/" + QString( Q_FUNC_INFO ).split(':').last().split('(').first() + ".bmp" );
	save( *chart, chart->title() );
}

void ChartTest::test7()
{
	QList<double> values1, values2;
	values1 << 0 << -10 << 10 << 15 << -15 << 20 << -20 << 25 << -25 << 10;
	values2 << 0 << 44 << -20 << 35 << 5 << -10 << -15 << 12 << 12 << 21;
	QStandardItemModel* model = new QStandardItemModel( 10, 2 );
	model->setHeaderData( 0, Qt::Horizontal, QString(" Serie 1") );
	model->setHeaderData( 1, Qt::Horizontal, QString(" Serie 2") );
	for ( int i = 0; i != values1.count(); ++i )
	{
		model->setData( model->index( i, 0 ), values1[i] );
		model->setData( model->index( i, 1 ), values2[i] );
	}
	chart->setModel( model );
	chart->setTitle( "w:/" + QString( Q_FUNC_INFO ).split(':').last().split('(').first() + ".bmp" );
	save( *chart, chart->title() );
}


void ChartTest::test8()
{
	QList<double> values1, values2;
	values1 << 0 << -10 << 10 << 15 << -15 << 20 << -20 << 25 << -25 << 10 << 13 << -7;
	values2 << 0 << 44 << -20 << 35 << 5 << -10 << -15 << 12 << 12 << 21 << -8 << 20;
	QStandardItemModel* model = new QStandardItemModel( 12, 2 );
	model->setHeaderData( 0, Qt::Horizontal, QString(" Serie 1") );
	model->setHeaderData( 1, Qt::Horizontal, QString(" Serie 2") );
	model->setVerticalHeaderLabels( QStringList() 
		<< "January" << "February" << "March"
		<< "April" << "May" << "June" 
		<< "July" << "August" << "September" 
		<< "October" << "November" << "December" );
	for ( int i = 0; i != values1.count(); ++i )
	{
		model->setData( model->index( i, 0 ), values1[i] );
		model->setData( model->index( i, 1 ), values2[i] );
	}
	chart->setModel( model );
	chart->setTitle( "w:/" + QString( Q_FUNC_INFO ).split(':').last().split('(').first() + ".bmp" );
	save( *chart, chart->title() );
}

void ChartTest::test9()
{
	QList<double> values1, values2;
	values1 << 0 << -10 << 10 << 15 << -15 << 20 << -20 << 25 << -25 << 10 << 13 << -7;
	values2 << 0 << 44 << -20 << 35 << 5 << -10 << -15 << 12 << 12 << 21 << -8 << 20;
	QStandardItemModel* model = new QStandardItemModel( 12, 2 );
	model->setHeaderData( 0, Qt::Horizontal, QString(" Serie 1") );
	model->setHeaderData( 1, Qt::Horizontal, QString(" Serie 2") );
	model->setVerticalHeaderLabels( QStringList() 
		<< "January" << "February" << "March"
		<< "April" << "May" << "June" 
		<< "July" << "August" << "September" 
		<< "October" << "November" << "December" );
	for ( int i = 0; i != values1.count(); ++i )
	{
		model->setData( model->index( i, 0 ), values1[i] );
		model->setData( model->index( i, 1 ), values2[i] );
	}
	chart->setModel( model );
	Thistle::SerieFormat format = chart->chart()->serieFormat( 0 );
	format.setBrush( Qt::red );
	format.setPen( QPen( QBrush(Thistle::Colors::DarkRed), 2, Qt::DashLine ) );
	chart->chart()->setSerieFormat( 0, format );
	chart->setTitle( "w:/" + QString( Q_FUNC_INFO ).split(':').last().split('(').first() + ".bmp" );
	save( *chart, chart->title() );
}

void ChartTest::test10()
{
	QList<double> values1, values2;
	values1 << 0 << -10 << 10 << 15 << -15 << 20 << -20 << 25 << -25 << 10 << 13 << -7;
	values2 << 0 << 44 << -20 << 35 << 5 << -10 << -15 << 12 << 12 << 21 << -8 << 20;
	QStandardItemModel* model = new QStandardItemModel( 12, 2 );
	model->setHeaderData( 0, Qt::Horizontal, QString(" Serie 1") );
	model->setHeaderData( 1, Qt::Horizontal, QString(" Serie 2") );
	model->setVerticalHeaderLabels( QStringList() 
		<< "January" << "February" << "March"
		<< "April" << "May" << "June" 
		<< "July" << "August" << "September" 
		<< "October" << "November" << "December" );
	for ( int i = 0; i != values1.count(); ++i )
	{
		model->setData( model->index( i, 0 ), values1[i] );
		model->setData( model->index( i, 1 ), values2[i] );
	}
	chart->setModel( model );
	Thistle::SerieFormat format = chart->chart()->serieFormat( 0 );
	format.setType( Thistle::Bar );
	chart->chart()->setSerieFormat( 0, format );
	format = chart->chart()->serieFormat( 1 );
	format.setType( Thistle::Spline | Thistle::Dot );
	chart->chart()->setSerieFormat( 1, format );
	chart->setTitle( "w:/" + QString( Q_FUNC_INFO ).split(':').last().split('(').first() + ".bmp" );
	save( *chart, chart->title() );
}



QTEST_MAIN(ChartTest)