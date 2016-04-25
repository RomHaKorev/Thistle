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

#include "Thistle/Charts/pie/piechart3dwidget.h"
#include "Thistle/Charts/pie/piechartwidget.h"
#include "Thistle/Charts/line/linechartwidget.h"

int main( int argc, char* argv[] ) {
	QApplication a(argc, argv);
	Thistle::PieChart3DWidget pie;
	Thistle::LineChartWidget line;
	{
		QStandardItemModel* m = new QStandardItemModel( 6, 2 );

		for ( qreal r = 0; r != m->rowCount(); ++r )
			for ( qreal c = 0; c != m->columnCount(); ++c )
				m->setData( m->index( r, c ),  r / ( c + 1) + 10 );
		line.setModel( m );
		line.show();
	}

	{
		QStandardItemModel* m = new QStandardItemModel( 6, 1 );

		for ( qreal r = 0; r != m->rowCount(); ++r )
			for ( qreal c = 0; c != m->columnCount(); ++c )
				m->setData( m->index( r, c ),  r / ( c + 1) + 10 );
		pie.setModel( m );
		pie.show();
	}
	return a.exec();
}

