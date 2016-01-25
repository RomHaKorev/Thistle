#include "linelegendview_p.h"
#include "../linelegendview.h"
#include "../../base/abstractlegendview.h"
#include "../../../Core/abstractitemview.h"
#include "../../base/seriedelegates.h"

#include <QPointer>

namespace Thistle
{

LineLegendViewPrivate::LineLegendViewPrivate( LineLegendView* parent )
	: AbstractLegendViewPrivate( parent ), dotDelegate( new DotDelegate( QPointer<Thistle::SerieFormatProxy>(0), parent ) )
{}


LineLegendViewPrivate::~LineLegendViewPrivate()
{
	dotDelegate->deleteLater();
}

}
