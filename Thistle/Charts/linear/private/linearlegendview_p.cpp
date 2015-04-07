#include "linearlegendview_p.h"
#include "../linearlegendview.h"
#include "../../base/abstractlegendview.h"
#include "../../../kernel/abstractitemview.h"
#include "../linearchart.h"
#include "../../base/seriedelegates.h"

namespace Thistle
{
LinearLegendViewPrivate::LinearLegendViewPrivate( LinearLegendView* parent, LinearChart* chart )
: AbstractLegendViewPrivate( parent, chart ), dotDelegate( new DotDelegate( chart ) )
{}


LinearLegendViewPrivate::~LinearLegendViewPrivate()
{}

void LinearLegendViewPrivate::calculateSizeHint( const QSize& source )
{
    this->itemRects.clear();
    test.clear();
    if ( q_ptr->model() == 0 )
    {
        this->sizeHint = QSize( 0, 0 );
        return;
    }

    QFontMetrics metrics( associatedChart->font() );
    int h = metrics.height() + 10;
    int cols = q_ptr->model()->columnCount();
    int nbLines = 0;
    int w = 10;
    int maxWidth = source.width();
    for( int c = 0; c < cols; ++c )
    {
        QString s( q_ptr->model()->headerData( c, Qt::Horizontal ).toString() );
        int sWidth = metrics.width( s ) + 50;
        if ( ( w + sWidth ) > maxWidth )
        {
            if ( c != 0 )
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

}
