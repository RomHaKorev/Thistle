#ifndef THISTLE_KIVIATCHART_H
#define THISTLE_KIVIATCHART_H

//#include "../base/seriechart.h"
#include "../linear/linearchart.h"
#include "private/kiviatchart_p.h"

namespace Thistle {

class KiviatChart: public LinearChart
{
	Q_DECLARE_PRIVATE( KiviatChart );
public:
	KiviatChart( QWidget* parent = 0 );
	~KiviatChart();

	QPainterPath itemPath( const QModelIndex& index ) const;
};

}

#endif // THISTLE_KIVIATCHART_H
