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

int main( int argc, char* argv[] ) {
	QApplication a(argc, argv);
    return a.exec();
}

