/*
 This file is part of Marb.
    Marb is free software: you can redistribute it and/or modify
    it under the terms of the Lesser GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License.
    Marb is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    Lesser GNU General Public License for more details.
    You should have received a copy of the Lesser GNU General Public License
    along with Marb.    If not, see <http://www.gnu.org/licenses/>.
 Marb    Copyright (C) 2013    Dimitry Ernot & Romha Korev
*/
#include "linearchart.h"
#include "delegates.h"
#include <QItemDelegate>
#include <QPainter>
#include <QPaintEvent>
#include <QItemDelegate>
#include <QStyleOptionViewItem>

#include "Axis/orthogonalaxis.h"

LinearChart::LinearChart( QWidget* parent ) : AxisChart( parent ) {
        myOrigin = QPointF(20, 10);
        myX = 0;
        myMinBottomMargin = 0;
        myPointDelegate = new PointDelegate( this );
        myBarDelegate = new BarDelegate( this );
        myVerticalXAxis = false;
        myOrthoAxis = new OrthogonalAxis;
        this->setAxis( myOrthoAxis );
}


QList<int> LinearChart::barStyleColumns() const {
        QList<int> bars;
        for ( int c = 0;    c < this->model()->columnCount(); ++c ) {
            if ( columnType( c ) == Marb::Bar ) {
                bars.append( c );
            }
        }
        return bars;
}


QList<int> LinearChart::calculateColumnsOrder() const {
    QList<int> bars;
    QList<int> lines;
    QList<int> points;
    for ( int i = 0; i < this->model()->columnCount(); ++i ) {
        Marb::Types t = columnType( i );
        if ( t.testFlag( Marb::Bar ) ) {
            bars << i;
        } else if ( t.testFlag( Marb::Point ) ) {
            points << i;
        } else    {
            lines << i;
        }
    }
    bars << lines << points;
    return bars;
}


Marb::Types LinearChart::columnType( int column ) const {
        if ( myStyle.contains( column ) ) {
            return myStyle[ column ].type();
        }
        return ChartStyle().type();
}


QRectF LinearChart::itemRect( const QModelIndex& index ) const {
    /* Reimplemented from Chart.itemRect() */
    QRectF r;
    Marb::Types t = this->columnType( index.column() );
    bool ok = false;
    qreal value = index.data().toReal( &ok );
    if ( ok == false ) {
        return QRectF();
    }
    QPointF pos = myOrthoAxis->valueToPoint( value, index.row() );
    QList<int> orderedColumns = this->calculateColumnsOrder();
    if ( t == Marb::Bar ) {
        QList<int> bars = this->barStyleColumns();
        qreal margin = myOrthoAxis->stepSize() * 0.1;
        qreal w = float( myOrthoAxis->stepSize() - margin ) / bars.count();
        pos += QPointF( margin/2.0 + w * orderedColumns.indexOf( index.column() ), 0 );
        QPointF br( pos.x() + w, myOrthoAxis->origin().y() );
        r = QRectF( pos, br );
        if ( value < 0 ) {
            r.translate( 0, 1 );
        } else {
            r.translate( 0, -1 );
        }

        if ( myOrthoAxis->startOnAxis() == false ) {
            r.translate( -myOrthoAxis->stepSize()/2.0, 0 );
        }
    } else {
        r = QRectF( -7, -7, 14 ,14 ).translated( pos.x(), pos.y() );
    }
    return r.normalized().toRect();
}



void LinearChart::paintChart( QPainter& painter ) {
    painter.setRenderHints( QPainter::Antialiasing | QPainter::TextAntialiasing );
    myOrthoAxis->paintBack( painter );
    QList<int> ordered = this->calculateColumnsOrder();
    Q_FOREACH( int c, ordered ) {
        this->paintValues( painter, c );
    }
    myOrthoAxis->paintFront( painter );
    this->paintLegend(painter);
    painter.drawText( myTitleRect, Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap, myTitle );
}


void LinearChart::paintColumnLegend(QPainter &painter, int column, QPoint pos, int maxHeight) {
        QPoint p1 = pos + QPoint( 10, - maxHeight/2 );
        QPoint p2 = pos + QPoint( 40, - maxHeight/2 );
        QPoint posText = pos + QPoint( 45, 0 );
        QString s(model()->headerData( column, Qt::Horizontal ).toString() );
        ChartStyle style = columnStyle( column );
        Marb::Types t = style.type();
        painter.drawText( posText, s );
        painter.save();
        painter.setPen( style.pen() );
        if ( t.testFlag( Marb::Line ) ) {
            painter.drawLine( p1, p2 );
        }
        if ( t.testFlag( Marb::Bar ) ) {
            int j = 0;
            Q_FOREACH( int i, QList<int>() << 15 << 8 << 17 << 5 ) {
                painter.setPen( Qt::NoPen );
                painter.setBrush( style.brush() );
                QRect r( 10 + pos.x() + j * 8, pos.y() - i, 5, i );
                painter.drawRect( r );
                j += 1;
            }
        } else if ( t.testFlag( Marb::Point ) ) {
            QStyleOptionViewItem option;
            option.rect = QRect( p1.x() + abs(p1.x() - p2.x())/2 - 5, p1.y() - 5, 10, 10 );
            myPointDelegate->paint( &painter, option, this->model()->index( 0, column ) );
        }
        painter.restore();
}


void LinearChart::paintValues( QPainter& painter, int column ) {
    Marb::Types t = this->columnType( column );
    QStyledItemDelegate* delegate = 0;
    if ( t.testFlag( Marb::Point ) ) {
        delegate = myPointDelegate;
    } else if ( t.testFlag( Marb::Bar ) ) {
        delegate = myBarDelegate;
    }
    int rows = this->model()->rowCount();
    painter.save();
    ChartStyle style = this->columnStyle( column );
    painter.setBrush( style.brush() );
    painter.setPen( style.pen() );

    bool isActive = true;
    if ( this->selectionModel() != 0 ) {
        QModelIndexList selectedIndexes = this->selectionModel()->selectedIndexes();
        if ( !selectedIndexes.isEmpty() ) {
            isActive = false;
            Q_FOREACH( QModelIndex idx, selectedIndexes ) {
                if ( idx.column() == column ) {
                    isActive = true;
                    break;
                }
            }
        }
    }

    for ( int r = 0; r < rows; ++r ) {
        QModelIndex index = this->model()->index( r, column );
        QStyleOptionViewItem option;
        qreal value = index.data().toReal();
        if ( value < 0 ) {
            option.decorationPosition = QStyleOptionViewItem::Bottom;
        } else {
            option.decorationPosition = QStyleOptionViewItem::Top;
        }
        if ( isActive == false ) {
            option.state = QStyle::State_Off;
        } else {
            option.state = QStyle::State_Selected;
        }
        option.rect = this->itemRect( index ).toRect();
        if ( ( (t | Marb::Line) == t) && ( r < (rows - 1) ) ) {
            if ( isActive == false ) {
                QPointF p1 = option.rect.center();
                QPointF p2 = this->itemRect( this->model()->index( r + 1, column ) ).center();
                QLineF line( p1, p2 );
                QLineF l = QLineF( line.pointAt( 0.5 ), line.p2() ).normalVector();
                l.setLength( 4 );
                QLinearGradient gradient( l.p1(), l.p2() );
                QColor c( Qt::darkGray );
                c.setAlpha( 50 );
                gradient.setColorAt( 0, c );
                gradient.setColorAt( 1, Qt::transparent );
                gradient.setSpread( QLinearGradient::ReflectSpread );
                painter.save();
                QPen pen( QBrush( gradient ), 8 );
                painter.setPen( pen );
                painter.drawLine( p1, p2 );
                painter.restore();
            } else {
                QPointF p1 = option.rect.center();
                QPointF p2 = this->itemRect( this->model()->index( r + 1, column ) ).center();
                painter.drawLine( p1, p2 );
            }
        }
        if ( delegate != 0 ) {
            delegate->paint( &painter, option, index );
        }
    }
    painter.restore();
}


void LinearChart::updateRects() {
    if ( this->model() == 0 ) {
                return;
    }

    this->scan();
    this->defineRects();
    myOrthoAxis->valuesRect = QRect( myOrthoAxis->chartRect );
    myOrthoAxis->valuesRect.setX( myOrthoAxis->valuesRect.x() + myOrthoAxis->yLabelsLength );
    myTitleRect.moveTo( myOrthoAxis->chartRect.bottomLeft() );
    myTitleRect.translate( (myOrthoAxis->chartRect.width() - myTitleRect.width())/2, 20 );
    myOrthoAxis->update();
}
