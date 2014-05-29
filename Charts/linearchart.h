/*
This file is part of Thistle.

Thistle is free software: you can redistribute it and/or modify
it under the terms of the Lesser GNU General Public License as published by
the Free Software Foundation, either version 3 of the License.

Thistle is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
Lesser GNU General Public License for more details.

You should have received a copy of the Lesser GNU General Public License
along with Thistle.    If not, see <http://www.gnu.org/licenses/>.

Thistle    Copyright (C) 2013    Dimitry Ernot & Romha Korev
*/

#ifndef LINEARCHART_H
#define LINEARCHART_H

#include "axischart.h"
//#include "serieformat.h"

#include <QItemDelegate>

#include "linearchart_p.h"

namespace Thistle {

    class DotDelegate;
    class BarDelegate;

    class OrthogonalAxis;

    /*!
    \class LinearChart
    \brief LinearChart class provides a default implementation for linear charts such as barcharts and line charts.

    The LinearChart class considers that each column from the model is a serie of values. It represents a serie with dots, lines or bars.
    The horizontal header contains the name of series. The vertical headers defines the x-axis data.

    QStandardItemModel* model = new QStandardItemModel( 10, 2 );
    model->setHorizontalHeaderLabels( QStringList() << "My values" );
    model->setVerticalHeaderLabels( QStringList() << "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun" << "Jul" << "Aug" << "Sept" << "Oct" );
    for ( int i = 0; i < model2->rowCount(); ++i ) {
    int v = qrand() % ((high + 1) - low) + low;
    model->setData( model->index( i, 0 ), v );
    v = qrand() % ((high + 1) - low) + low;
    model->setData( model->index( i, 1 ), v );
    }
    Thistle::LinearChart linear;
    linear.setModel( model );
    Thistle::SerieFormat style = linear.serieFormat( 1 );
    style.setType( Thistle::Thistle::Bar );
    linear.setSerieFormat( 1, style );
    linear.setTitle( "A line chart example" );
    linear.show();
    */


    class LinearChart : public AxisChart {
        Q_OBJECT
            Q_DECLARE_PRIVATE( LinearChart );
    public:

        explicit LinearChart(QWidget *parent = 0);
        virtual AbstractChartDelegate* delegate() const { 
            const Q_D( LinearChart );
            return d->delegate();
        }
    protected:
        QList<int> calculateColumnsOrder() const;
        virtual void updateRects();
        Thistle::Types columnType( int column ) const;
        QList<int> barStyleColumns() const;
        virtual QRectF itemRect( const QModelIndex& index ) const;
        virtual QRectF itemRect( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
        void paintValues( QPainter& painter, int column);
        virtual void paintSerieLegend(QPainter &painter, int column, QPoint pos, int maxHeight) const;
        virtual void paintChart( QPainter& painter );
    };

}

#endif // LINEARCHART_H
