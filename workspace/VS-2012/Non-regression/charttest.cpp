#include "charttest.h"


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

QStandardItemModel* ChartTest::make( QList<QList<double> > values )
{
	QStandardItemModel* model = new QStandardItemModel( values.first().count(), values.count() );
	for ( int i = 0; i != values.count(); ++i )
		for ( int j = 0; j != values.at(i).count(); ++j )
			model->setData( model->index( j, i ), values[i][j] );
	chart->setModel( model );
	return model;
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
	make( QList<QList<double> >() << (QList<double>() << 0 << 10 << 12 << 15 << 17 << 20 << 22 << 25 << 12 << 10) );
	chart->setTitle( QString( Q_FUNC_INFO ).split(':').last().split('(').first() );
	save( *chart, QApplication::applicationDirPath() + chart->title() + ".bmp" );
}

void ChartTest::test2()
{
	make( QList<QList<double> >() << (QList<double>() << -0 << -10 << -12 << -15 << -17 << -20 << -22 << -25 << -12 << -10) );
	chart->setTitle( QString( Q_FUNC_INFO ).split(':').last().split('(').first() );
	save( *chart, QApplication::applicationDirPath() + chart->title() + ".bmp" );
}

void ChartTest::test3()
{
	make( QList<QList<double> >() << (QList<double>() << 0 << -10 << 10 << 15 << -15 << 20 << -20 << 25 << -25 << 10) );
	chart->setTitle( QString( Q_FUNC_INFO ).split(':').last().split('(').first() );
	save( *chart, QApplication::applicationDirPath() + chart->title() + ".bmp" );
}

void ChartTest::test4()
{
	make( QList<QList<double> >() << (QList<double>() << 0 << -1000000 << 1000000 << 1500000 << -1500000 << 2000000 << -2000000 << 2500000 << -2500000 << 1000000) );
	chart->setTitle( QString( Q_FUNC_INFO ).split(':').last().split('(').first() );
	save( *chart, QApplication::applicationDirPath() + chart->title() + ".bmp" );
}

void ChartTest::test5()
{
	make( QList<QList<double> >() << (QList<double>() << 0.000000 << -0.0000010 << 0.0000010 
		<< 0.0000015 << -0.0000015 << 0.0000020 << -0.0000020 << 0.0000025 << -0.0000025 << 0.0000010) );
	chart->setTitle( QString( Q_FUNC_INFO ).split(':').last().split('(').first() );
	save( *chart, QApplication::applicationDirPath() + chart->title() + ".bmp" );
}

void ChartTest::test6()
{
	QList<double> values1, values2;
	values1 << 0 << -10 << 10 << 15 << -15 << 20 << -20 << 25 << -25 << 10;
	values2 << 0 << 44 << -20 << 35 << 5 << -10 << -15 << 12 << 12 << 21;
	make( QList<QList<double> >() << values1 << values2 );
	chart->setTitle( QString( Q_FUNC_INFO ).split(':').last().split('(').first() );
	save( *chart, QApplication::applicationDirPath() + chart->title() + ".bmp" );
}

void ChartTest::test7()
{
	QList<double> values1, values2;
	values1 << 0 << -10 << 10 << 15 << -15 << 20 << -20 << 25 << -25 << 10;
	values2 << 0 << 44 << -20 << 35 << 5 << -10 << -15 << 12 << 12 << 21;
	make( QList<QList<double> >() << values1 << values2 );
	chart->setTitle( QString( Q_FUNC_INFO ).split(':').last().split('(').first() );
	save( *chart, QApplication::applicationDirPath() + chart->title() + ".bmp" );
}


void ChartTest::test8()
{
	QList<double> values1, values2;
	values1 << 0 << -10 << 10 << 15 << -15 << 20 << -20 << 25 << -25 << 10 << 13 << -7;
	values2 << 0 << 44 << -20 << 35 << 5 << -10 << -15 << 12 << 12 << 21 << -8 << 20;
	QStandardItemModel* model = make( QList<QList<double> >() << values1 << values2 );
	model->setHeaderData( 0, Qt::Horizontal, QString(" Serie 1") );
	model->setHeaderData( 1, Qt::Horizontal, QString(" Serie 2") );
	model->setVerticalHeaderLabels( QStringList() 
		<< "January" << "February" << "March"
		<< "April" << "May" << "June" 
		<< "July" << "August" << "September" 
		<< "October" << "November" << "December" );
	chart->setTitle( QString( Q_FUNC_INFO ).split(':').last().split('(').first() );
	save( *chart, QApplication::applicationDirPath() + chart->title() + ".bmp" );
}

void ChartTest::test9()
{
	QList<double> values1, values2;
	values1 << 0 << -10 << 10 << 15 << -15 << 20 << -20 << 25 << -25 << 10 << 13 << -7;
	values2 << 0 << 44 << -20 << 35 << 5 << -10 << -15 << 12 << 12 << 21 << -8 << 20;
	QStandardItemModel* model = make( QList<QList<double> >() << values1 << values2 );
	model->setHeaderData( 0, Qt::Horizontal, QString(" Serie 1") );
	model->setHeaderData( 1, Qt::Horizontal, QString(" Serie 2") );
	model->setVerticalHeaderLabels( QStringList() 
		<< "January" << "February" << "March"
		<< "April" << "May" << "June" 
		<< "July" << "August" << "September" 
		<< "October" << "November" << "December" );
	Thistle::SerieFormat format = chart->chart()->serieFormat( 0 );
	format.setBrush( Qt::red );
	format.setPen( QPen( QBrush(Thistle::Colors::DarkRed), 2, Qt::DashLine ) );
	chart->chart()->setSerieFormat( 0, format );
	chart->setTitle( QString( Q_FUNC_INFO ).split(':').last().split('(').first() );
	save( *chart, QApplication::applicationDirPath() + chart->title() + ".bmp" );
}

void ChartTest::test10()
{
	QList<double> values1, values2;
	values1 << 0 << -10 << 10 << 15 << -15 << 20 << -20 << 25 << -25 << 10 << 13 << -7;
	values2 << 0 << 44 << -20 << 35 << 5 << -10 << -15 << 12 << 12 << 21 << -8 << 20;
	QStandardItemModel* model = make( QList<QList<double> >() << values1 << values2 );
	model->setHeaderData( 0, Qt::Horizontal, QString(" Serie 1") );
	model->setHeaderData( 1, Qt::Horizontal, QString(" Serie 2") );
	model->setVerticalHeaderLabels( QStringList() 
		<< "January" << "February" << "March"
		<< "April" << "May" << "June" 
		<< "July" << "August" << "September" 
		<< "October" << "November" << "December" );
	Thistle::SerieFormat format = chart->chart()->serieFormat( 0 );
	format.setType( Thistle::Bar );
	chart->chart()->setSerieFormat( 0, format );
	format = chart->chart()->serieFormat( 1 );
	format.setType( Thistle::Spline | Thistle::Dot );
	chart->chart()->setSerieFormat( 1, format );
	chart->setTitle( QString( Q_FUNC_INFO ).split(':').last().split('(').first() );
	save( *chart, QApplication::applicationDirPath() + chart->title() + ".bmp" );
}

void ChartTest::test11()
{
	QList<double> values1, values2;
	values1 << 0 << -10 << 10 << 15 << -15 << 20 << -20 << 25 << -25 << 10 << 13 << -7;
	values2 << 0 << 44 << -20 << 35 << 5 << -10 << -15 << 12 << 12 << 21 << -8 << 20;
	QStandardItemModel* model = make( QList<QList<double> >() << values1 << values2 );
	model->setHeaderData( 0, Qt::Horizontal, QString(" Serie 1") );
	model->setHeaderData( 1, Qt::Horizontal, QString(" Serie 2") );
	model->setVerticalHeaderLabels( QStringList() 
		<< "January" << "February" << "March"
		<< "April" << "May" << "June" 
		<< "July" << "August" << "September" 
		<< "October" << "November" << "December" );
	chart->setModel( model );
	Thistle::SerieFormat format = chart->chart()->serieFormat( 0 );
	format.setType( Thistle::Bar );
	chart->chart()->setSerieFormat( 0, format );
	format = chart->chart()->serieFormat( 1 );
	format.setType( Thistle::Spline | Thistle::Dot );
	chart->chart()->setSerieFormat( 1, format );
	chart->setTitle( QString( Q_FUNC_INFO ).split(':').last().split('(').first() );
	save( *chart, QApplication::applicationDirPath() + chart->title() + ".bmp" );
}

QTEST_MAIN(ChartTest)