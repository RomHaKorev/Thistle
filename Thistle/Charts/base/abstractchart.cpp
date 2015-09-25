#include "abstractchart.h"

#include "private/abstractchart_p.h"

#include <QPainter>
#include <QHelpEvent>
#include <QPaintEngine>

namespace Thistle
{

AbstractChart::AbstractChart(QWidget* parent ) : AbstractItemView( new AbstractChartPrivate( this ), parent )
{
	this->setContentsMargins( 20, 20, 20, 20 );
	this->setEditTriggers( QAbstractItemView::NoEditTriggers );
}


AbstractChart::AbstractChart( AbstractChartPrivate* d, QWidget* parent ) : AbstractItemView( d, parent )
{
	this->setContentsMargins( 20, 20, 20, 20 );
	this->setEditTriggers( QAbstractItemView::NoEditTriggers );
}


AbstractChart::~AbstractChart()
{

}

void AbstractChart::setScrollBarValues()
{
}


QModelIndex AbstractChart::indexAt(const QPoint& point) const
{
	if ( this->model() == 0 )
	{
		return QModelIndex();
	}
	for( int row = 0; row < this->model()->rowCount(); ++row )
	{
		for( int col = 0; col < this->model()->columnCount(); ++col )
		{
			QModelIndex index = this->model()->index( row, col );
			QPainterPath r = this->itemPath( index );
			if ( r.contains( point ) )
			{
				return index;
			}
		}
	}
	return QModelIndex();
}

#if 0
QPainterPath AbstractChart::itemPath(const QModelIndex& index) const
{
	/*    QPainterPath path;
	path.addRect( QRect( -10, -10, 20, 20 ).translated( index.row() * 20, index.column() * 20 ) );
	return path;*/
}
#endif

void AbstractChart::resizeEvent(QResizeEvent* ev)
{
	AbstractItemView::resizeEvent( ev );
	this->updateValues();
}


void AbstractChart::updateValues()
{
	this->process();
}


void AbstractChart::process()
{
	if ( this->model() == 0 )
	{
		return;
	}
	this->scan();
	this->updateRects();
}

#if 0
bool AbstractChart::event( QEvent* e )
{
	QHelpEvent* he = static_cast<QHelpEvent*>( e );

	if ( this->model() && he && he->type() == QEvent::ToolTip )
	{
		QModelIndex idx = this->nearestItem( he->pos() );
		QString label = this->model()->headerData( idx.row(), Qt::Horizontal ).toString();
		if ( label == 0 )
			label = QString::number ( idx.row() + 1 );

		if ( idx.isValid() )
			this->setToolTip( QString( "Serie: %1\nX: %2\nY: %3" )
							  .arg( this->model()->headerData( idx.column(), Qt::Vertical ).toString() )
							  .arg( label )
							  .arg( idx.data().toReal() ) );
	}

	return AbstractItemView::event(e);
}
#endif

QModelIndex AbstractChart::nearestItem( const QPointF& pos ) const
{
	if ( this->model() == 0 ) return QModelIndex();

	QModelIndex nearest = QModelIndex();

	unsigned int nearestLength = -1;

	for ( int row = 0; row < this->model()->rowCount(); ++row )
	{
		for ( int col = 0; col < this->model()->columnCount(); ++col )
		{
			QModelIndex idx = this->model()->index( row, col );
			QPointF p = this->itemRect( idx ).center() - pos;

			unsigned int length = qAbs( p.manhattanLength() );

			if ( qMin( length, nearestLength  ) == length )
			{
				nearestLength = length;
				nearest = idx;
			}
		}
	}
	return nearest;
}


SerieFormat AbstractChart::serieFormat( int column ) const
{
	const Q_D( AbstractChart );
	if ( d->style.contains( column ) )
	{
		return d->style[ column ];
	}
	SerieFormat style;
	QColor c1 =  Colors::predefinedLightColor( column );
	QColor c2 = Colors::predefinedDarkColor( column );
	style.setPen( QPen( QColor(c2), 2 ) );
	style.setBrush( QBrush(c1) );
	return style;
}

void AbstractChart::setSerieFormat( int column, SerieFormat style )
{
	Q_D( AbstractChart );
	d->style[ column ] = style;
}

#if 0
void AbstractChart::addLayer( AbstractLayer* layer )
{
	Q_D( AbstractChart );
	d->addLayer( layer );
	this->update();
}
#endif
}
