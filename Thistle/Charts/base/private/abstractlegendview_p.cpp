#include "abstractlegendview_p.h"
#include "../abstractlegendview.h"
#include "../../../Core/abstractitemview.h"
#include "../abstractchart.h"

namespace Thistle
{
AbstractLegendViewPrivate::AbstractLegendViewPrivate( AbstractLegendView* parent )
	: AbstractItemViewPrivate(parent), q_ptr(parent), formatProxy( new Thistle::SerieFormatProxy(this) )
{}


AbstractLegendViewPrivate::~AbstractLegendViewPrivate()
{}



QRect AbstractLegendViewPrivate::serieRect( unsigned int serieIndex ) const
{
	if ( q_ptr->model() == 0 )
		return QRect();

	QFontMetrics metrics( q_ptr->font() );
	int h = metrics.height() + 8;
	int nbLines = 0;
	int w = 10;
	int maxWidth = q_ptr->width();
	for( unsigned int c = 0; c < q_ptr->serieCount(); ++c )
	{
		QString s( q_ptr->serieName( c ) );
		int sWidth = metrics.width( s ) + 50;
		if ( ( w + sWidth ) > maxWidth )
		{
			if ( c != 0 )
				++nbLines;
			w = 10 + sWidth;
			if ( c == serieIndex )
				return QRect( 10, nbLines * h, sWidth, h );
		}
		else
		{
			if ( c == serieIndex )
				return QRect( w, nbLines * h, sWidth, h );
			w += sWidth;
		}
		maxWidth = qMax( maxWidth, 20 + sWidth );
	}
	return QRect();
}


void AbstractLegendViewPrivate::calculateSizeHint( const QSize& source )
{
	if ( q_ptr->model() == 0 )
	{
		this->sizeHint = QSize( 0, 0 );
		return;
	}
	int x = 0;
	int y = 0;
	for( unsigned int c = 0; c < q_ptr->serieCount(); ++c )
	{
		x = qMax( x, this->serieRect( c ).right() );
		y = qMax( y, this->serieRect( c ).bottom() );
	}
	this->sizeHint = QSize( x + 1, y + 4 );
}


}
