#include "pielegendview_p.h"

#include "../pielegendview.h"
#include "../../base/abstractlegendview.h"
#include "../../../kernel/abstractitemview.h"
#include "../piechart.h"


namespace Thistle
{

PieLegendViewPrivate::PieLegendViewPrivate( PieLegendView* parent, PieChart* associatedChart )
	: AbstractLegendViewPrivate( parent, associatedChart )
{}


PieLegendViewPrivate::~PieLegendViewPrivate()
{}


void PieLegendViewPrivate::calculateSizeHint( const QSize& source )
{
	this->itemRects.clear();
	if ( q_ptr->model() == 0 )
	{
		this->sizeHint = QSize( 0, 0 );
		return;
	}

	QFontMetrics metrics( associatedChart->font() );
	int h = metrics.height() + 10;
	int rows = q_ptr->model()->rowCount();
	int nbLines = 0;
	int w = 10;
	int maxWidth = source.width();
	for ( int r = 0; r < rows; ++r )
	{
		QString s( q_ptr->model()->headerData( r, Qt::Vertical ).toString() );
		int sWidth = metrics.width( s ) + 50;
		if ( ( w + sWidth ) > maxWidth )
		{
			if ( r != 0 )
				++nbLines;

			w = 10 + sWidth;
			this->itemRects << QRect( 10, nbLines * h, sWidth, h );
		}
		else
		{
			this->itemRects << QRect( w, nbLines * h, sWidth, h );
			w += sWidth;
		}
		maxWidth = qMax( maxWidth, 20 + sWidth );
	}

	this->sizeHint = QSize( maxWidth, ( nbLines + 1 ) * h + 10 );
}

} // namespace Thistle
