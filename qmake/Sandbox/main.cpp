#include "thistleproxywidget.h"

#include <QApplication>
#include <QQuickView>
#include <QStandardItemModel>
#include <qmath.h>

#include <charts/linear/linearchartwidget.h>
#include <charts/kiviat/kiviatchartwidget.h>

#define LINEAR

int main(int argc, char **argv)
{
#if 0
	QApplication app(argc, argv);
	QQuickView view;
	//qmlRegisterType<PushButtonItem>("PushButton", 1, 0, "PushButtonItem");
	view.setSource(QUrl::fromLocalFile("../../../src/qmake/Sandbox/Sandbox.qml"));
	view.show();
	return app.exec();
#endif
	QApplication app(argc, argv);
	int rows = 10;
	int cols = 2;
	QStandardItemModel* m = new QStandardItemModel( rows, cols );
	for ( int r = 0; r < rows; ++r )
	{
		for ( int c = 0; c < cols; ++c )
		{
			double value = qSin( r * 0.1 + (c % 2? 3.14159/2.0: 0) );
			m->setData( m->index( r, c ), value );
		}
	}

#ifdef KIVIAT
	Thistle::KiviatChartWidget* kiviat = new Thistle::KiviatChartWidget();
	kiviat->setModel( m );
	kiviat->show();
	kiviat->setTitle( "A Scatter Chart" );
#endif

#ifdef LINEAR
	Thistle::LinearChartWidget* kiviat = new Thistle::LinearChartWidget();
	kiviat->setModel( m );
	kiviat->show();
	kiviat->setTitle( "A Line Chart" );
#endif
	/*Thistle::SerieFormat f = kiviat->serieFormat( 0 );
	f.setType( Thistle::Spline );
	kiviat->setSerieFormat( 0, f );
	f = kiviat->serieFormat( 1 );
	f.setType( Thistle::Dot );
	kiviat->setSerieFormat( 1, f );*/
	return app.exec();
}
