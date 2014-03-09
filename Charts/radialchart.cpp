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
#include "radialchart.h"
#include "Axis/radialaxis.h"
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>

RadialChart::RadialChart( QWidget* parent ) : AxisChart( parent ) {
    myOrigin = QPointF(20, 20);
    myNbTicks = 5;
    myRadialAxis = new RadialAxis();

    this->setAxis( myRadialAxis );
}


QRectF RadialChart::itemRect(const QModelIndex &index) const {
    Q_UNUSED( index )
    return QRect();
}


QPainterPath RadialChart::itemPath( const QModelIndex& index ) const {
    /*Returns the bounding path for the item pointed by the [index].
    *index: QModelIndex*/
    QPainterPath path;
    float angle = float(myRadialAxis->stepSize() / this->model()->columnCount() );
    float delta = 0.1 * angle;
    float startAngle = angle * index.column();
    startAngle += index.row() * myRadialAxis->stepSize() + myRadialAxis->startAngle();
    QPainterPath pathCenter;
    QRect rectangle( -myRadialAxis->centerHoleDiam()/2, -myRadialAxis->centerHoleDiam()/2, myRadialAxis->centerHoleDiam(), myRadialAxis->centerHoleDiam() );
    rectangle.translate( myRadialAxis->valuesRect.center() );
    pathCenter.addEllipse( rectangle );
    qreal value = index.data().toReal();

    rectangle = myRadialAxis->valueToRect( value );

    path.moveTo( myRadialAxis->valuesRect.center() );
    path.arcTo( rectangle, (startAngle + delta), (angle - delta * 2) );
    path.closeSubpath();
    path = path.subtracted( pathCenter );
    return path;
}


void RadialChart::paintAxis( QPainter& painter ) const {
    painter.save();
    //paintTicks( painter );
    painter.setRenderHint( QPainter::Antialiasing, false );
    painter.setPen( QPen( Qt::darkGray, 1.5 ) );
    QPoint p1 = myRadialAxis->valuesRect.center();
    QPoint p2 = p1 + QPoint( myRadialAxis->valuesRect.width()/2, 0 );
    painter.drawLine( p1, p2 );
    painter.restore();
}


void RadialChart::paintChart(QPainter& painter) {
    if ( this->model() == 0 ) {
        return;
    }
    painter.setRenderHints( QPainter::Antialiasing | QPainter::TextAntialiasing );
    myRadialAxis->paintBack( painter );
    int cols = this->model()->columnCount();
    painter.save();
    for ( int c = 0; c < cols; ++c ) {
        ChartStyle style = columnStyle( c );
        painter.setPen( style.pen() );
        painter.setBrush( style.brush() );
        this->paintValues( painter, c );
    }
    painter.restore();
    myRadialAxis->paintFront( painter );
    this->paintLegend( painter );
    painter.drawText( myTitleRect, Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap, myTitle );
}


void RadialChart::paintTextAxis( QPainter& painter) const {
    QFontMetrics metrics( font() );
    int h = metrics.height();
    qreal x = myMinBound;
    while ( x <= myMaxBound ) {
        qreal v = valueToPx( x )/2;
        int w = metrics.width( QString::number( x ) );
        painter.save();
        painter.setPen( QPen( QColor( Marb::DarkGray ), 1.5 ) );
        painter.rotate( -90 );
        painter.translate( -myRadialAxis->valuesRect.center().y(), myRadialAxis->valuesRect.center().x() + v );
        painter.drawText( -w - 5, h / 2, QString::number(x) );
        x += myTickSize;
        painter.restore();
    }
}


void RadialChart::paintTicks( QPainter& painter ) const {
}


void RadialChart::paintValues( QPainter& painter, int column ) const {
    int rows = this->model()->rowCount();
    painter.save();
    bool isActive = false;
    QModelIndexList selectedIndexes = this->selectionModel()->selectedIndexes();
    if ( selectedIndexes.isEmpty() != false ) {
        Q_FOREACH( QModelIndex idx, selectedIndexes ) {
            if ( idx.column() == column ) {
                isActive = true;
                break;
            }
        }
    } else {
        isActive = true;
    }
    for ( int r = 0; r < rows; ++r ) {
        QModelIndex index = this->model()->index( r, column );
        painter.save();
        if ( !selectedIndexes.isEmpty() ) {
            if ( !selectedIndexes.contains( index ) ) {
                QPen pen = painter.pen();
                if ( isActive == false ) {
                    QColor c( Marb::Gray );
                    c.setAlpha( 125 );
                    pen.setColor( c );
                    painter.setPen( pen );
                    c = QColor( Marb::LightGray );
                    c.setAlpha( 150 );
                    painter.setBrush( QBrush( c ) );
                } else {
                    QColor c = pen.color();
                    c.setAlpha( c.alpha() * 0.5 );
                    pen.setColor( c );
                    painter.setPen( pen );
                    QBrush brush = painter.brush();
                    c = brush.color();
                    c.setAlpha( c.alpha() * 0.5 );
                    brush.setColor( c );
                    painter.setBrush( brush );
                }
            }
        }
        QPainterPath path = this->itemPath( index );
        painter.drawPath( path );
        painter.restore();
    }
    painter.restore();
}


/*void RadialChart::process() {
    if ( this->model() == 0 ) {
        return;
    }
    myMin = 0;
    myMax = 0;
    updateValues();
}*/


void RadialChart::setAlphaBeta() {
    qreal w = myRadialAxis->valuesRect.width();
    qreal m = w * 0.3;
    //myAlpha = float( w - m )    / float( myMaxBound - myMinBound );
    //myBeta = w - myAlpha * myMaxBound;
}


void RadialChart::updateRects() {
    if ( this->model() == 0 ) {
        return;
    }
    myRadialAxis->setModel( this->model() );
    this->scan();
    this->defineRects();
    qreal w = qMin( myRadialAxis->chartRect.width(), myRadialAxis->chartRect.height() );
    myRadialAxis->valuesRect = QRect( -w/2, -w/2, w, w );
    myRadialAxis->valuesRect.translate( myRadialAxis->chartRect.center().x(), myRadialAxis->chartRect.center().y() );
    myTitleRect.moveTo( myRadialAxis->chartRect.bottomLeft() );
    myTitleRect.translate( (myRadialAxis->chartRect.width() - myTitleRect.width())/2, 10 );
    myRadialAxis->valuesRect = myRadialAxis->valuesRect;
    myRadialAxis->chartRect = myRadialAxis->chartRect;
    myRadialAxis->update();
}


/*void RadialChart::updateValues() {
    if ( this->model() == 0 ) {
        return;
    }
    myRadialAxis->chartRect = QRect( QPoint( 20, 20 ), this->size() - QSize( 40, 40 ) );
    QFontMetrics metrics( font() );
    if ( myTitle != "" ) {
        QRect r( 0, 0, myRadialAxis->centerHoleDiam(), 0 );
        myTitleRect = metrics.boundingRect( r, Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap, myTitle );
        myRadialAxis->chartRect.setHeight( myRadialAxis->chartRect.height() - myTitleRect.height() );
    }
    int w = qMin( myRadialAxis->chartRect.width(), myRadialAxis->chartRect.height() );
    myRadialAxis->valuesRect = QRect( -w/2, -w/2, w, w );
    myRadialAxis->valuesRect.translate( myRadialAxis->chartRect.center().x(), myRadialAxis->chartRect.center().y() );
    this->scan();
    this->calculateBounds();
    myX = 360.0 / qreal( this->model()->rowCount() );
    this->setAlphaBeta();
    myTitleRect.translate( myRadialAxis->valuesRect.center().x(), myRadialAxis->chartRect.height() + 30 );
}*/



/*
class RadialChart(Chart):
    def __init__(self, parent=None):
        ''' Constructor. Constructs an instance with the given [parent].
        *parent: QWidget
        '''
        super(RadialChart, self).__init__( parent )
        self._axis = RadialAxis()


    def columnStyle(self, column ):
        '''Returns the style for the column.
        If no style has been set for the column, returns the style by default
        *column: integer
        '''
        if column in self._style:
            return self._style[ column ]
        else:
            style = ChartStyle()
            c1 = Color.lightColorAt( column )
            c2 = Color.regularColorAt( column )
            c1.setAlpha( 200 )
            style.setPen( QPen( QColor(c2), 2 ) )
            style.setBrush( QBrush(c1) )
            return style


    def itemPath( self, index ):
        '''Returns the bounding path for the item pointed by the [index].
        *index: QModelIndex
        '''
        angle = float( self._axis.stepSize() / self.model().columnCount() )
        delta = 0.1 * angle
        startAngle = angle * index.column()
        startAngle += index.row() * self._axis.stepSize()
        pathCenter = QPainterPath()
        rectangle = QRect( -self._axis.centerHoleDiam/2, -self._axis.centerHoleDiam/2, self._axis.centerHoleDiam, self._axis.centerHoleDiam )
        rectangle.translate( self._axis._valuesRect.center() )
        pathCenter.addEllipse( rectangle )
        value = 0.0
        try:
            value = float(index.data())
        except:
            pass
        rectangle = self._axis.valueToRect( value )
        path = QPainterPath()
        path.moveTo( self._axis._valuesRect.center() )
        path.arcTo( rectangle, (startAngle + delta), (angle - delta * 2) )
        path.closeSubpath();
        path = path.subtracted( pathCenter )
        return path


# 0 0.0 32.20132469929538 322.0132469929538 -32.20132469929538 -257.610597594363

# 1 322.0132469929538 32.20132469929538 322.0132469929538 -354.21457169224914 -257.610597594363

# 2 644.0264939859076 32.20132469929538 322.0132469929538 -676.2278186852029 -257.610597594363

# 3 966.0397409788613 32.20132469929538 322.0132469929538 -998.2410656781567 -257.610597594363

    def paintChart(self, painter):


    def _paintColumnLegend(self, painter, c, pos, metricsH):
        r = QRect( pos.x() + 25, pos.y() - 15, 30, 30 )
        posText = pos + QPoint( 45, metricsH/2 )
        style = self.columnStyle(c)
        s = str(self.model().headerData( c, Qt.Horizontal ))
        painter.drawText( posText, s )
        painter.save()
        painter.setPen( style.pen() )
        painter.setBrush( style.brush() )
        painter.drawPie( r, 210 * 16, -60 * 16 )
        painter.restore()


*/
