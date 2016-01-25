#include "abstractlegendview.h"
#include "private/abstractlegendview_p.h"
#include "abstractchart.h"
#include "serieformat.h"

#include <QPainter>
#include <QWidget>
#include <QScrollBar>

namespace Thistle
{
AbstractLegendView::AbstractLegendView( AbstractLegendViewPrivate* d, QWidget* parent )
	: AbstractItemView( d, parent )
{
	this->setFrameShape( QFrame::Box );
	this->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	this->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
}


AbstractLegendView::~AbstractLegendView()
{}


QModelIndex AbstractLegendView::indexAt( const QPoint& point ) const
{
	const Q_D( AbstractLegendView );
	QPoint p = point + QPoint( horizontalOffset(), verticalOffset() );

	if ( this->model() == 0 )
		return QModelIndex();

	for ( unsigned int c = 0; c < this->serieCount(); ++c )
	{
		QRect r = d->serieRect( c );
		if ( r.contains( p ) )
			return this->model()->index( 0, c );
	}
	return QModelIndex();
}


QPainterPath AbstractLegendView::itemPath( const QModelIndex& index ) const
{
	const Q_D( AbstractLegendView );
	QPainterPath path;
	path.addRect( d->serieRect( index.column() ) );
	return path;
}

void AbstractLegendView::setScrollBarValues()
{
	Q_D( AbstractLegendView );

	if ( d->sizeHint.height() > this->size().height() )
		this->verticalScrollBar()->setMaximum( d->sizeHint.height() - this->size().height() );
	else
		this->verticalScrollBar()->setMaximum( 0 );

	if ( d->sizeHint.width() > this->size().width() )
		this->horizontalScrollBar()->setMaximum( d->sizeHint.width() - this->size().width() );
	else
		this->horizontalScrollBar()->setMaximum( 0 );
}


QSize AbstractLegendView::sizeHint() const
{
	const Q_D( AbstractLegendView );
	return d->sizeHint;
}


void AbstractLegendView::updateSizeHint( const QSize& source )
{
	Q_D( AbstractLegendView );
	d->calculateSizeHint( source );
}

void AbstractLegendView::paintEvent( QPaintEvent* ev )
{
	Q_UNUSED( ev )
	if ( this->model() == 0 )
		return;

	QPainter painter( viewport() );
	painter.translate( -this->horizontalOffset(), -this->verticalOffset() );
	painter.setRenderHint( QPainter::Antialiasing );

	painter.save();

	for ( unsigned int c = 0; c != this->serieCount(); ++c )
	{
		this->paintSerie( painter, c );
	}
	painter.restore();
}


void AbstractLegendView::paintSerie( QPainter& painter, int serie ) const
{
	const Q_D( AbstractLegendView );

	QRect rect = d->serieRect( serie );

	QPoint p1( rect.topLeft() + QPoint( 4, 2 ) );
	QPoint p2( rect.topLeft() + QPoint( 34, rect.height() - 4 ) );

	QString s( this->serieName( serie ) );

	bool hasSelection = false;
	bool isActive = this->isActiveSerie( serie, hasSelection );

	if ( !isActive )
	{
		QPen p = painter.pen();
		QColor c = p.color();
		c.setAlpha( c.alpha() * 0.5 );
		QPen pen( p );
		pen.setColor( c );
		painter.setPen( pen );
		painter.drawText( rect.translated( 38, -3 ), Qt::AlignVCenter, s );
		painter.setPen( p );
	}
	else
	{
		if ( hasSelection )
		{
			painter.save();
			painter.setRenderHint( QPainter::Antialiasing, false );
			painter.setPen( QColor(42, 97, 243) );
			painter.drawRect( rect );
			painter.restore();
		}
		painter.drawText( rect.translated( 38, -3 ), Qt::AlignVCenter, s );
	}

	this->paintSeriePicto( painter, QRect( p1, p2 ), serie );
}


void AbstractLegendView::setSelection( const QRect& rect, QItemSelectionModel::SelectionFlags command )
{
	Q_D( AbstractLegendView );
	QRect contentsRect = rect.translated(
				this->horizontalScrollBar()->value(),
				this->verticalScrollBar()->value()).normalized();

	int count = 0;

	for( unsigned int c = 0; c < this->serieCount(); ++c )
	{
		QRect r = d->serieRect( c );
		if ( !r.intersected( contentsRect ).isEmpty() )
		{
			++count;
			this->selectionModel()->select( this->model()->index( 0, c ), command );
		}
	}
	if ( count == 0 )
	{
		this->selectionModel()->clear();
	}
	this->viewport()->update();
}


bool AbstractLegendView::isActiveSerie( int serieIdx, bool& hasSelection ) const
{
	hasSelection = false;
	bool isActive = true;
	if ( this->selectionModel() != 0 )
	{
		QModelIndexList selectedIndexes = this->selectionModel()->selectedIndexes();
		if ( !selectedIndexes.isEmpty() )
		{
			hasSelection = true;
			isActive = false;
			Q_FOREACH( QModelIndex idx, selectedIndexes )
			{
				if ( idx.column() == serieIdx )
				{
					return true;
				}
			}
		}
	}
	return isActive;
}

QPointer<Thistle::SerieFormatProxy> AbstractLegendView::serieFormatProxy() const
{
	const Q_D( AbstractLegendView );
	return d->formatProxy;
}

void AbstractLegendView::setSerieFormatProxy( QPointer<Thistle::SerieFormatProxy> proxy )
{
	Q_D( AbstractLegendView );
	d->formatProxy = proxy;
}

}
