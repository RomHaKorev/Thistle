#include "graphview_p.h"
#include "graphview.h"
#include "abstractgraphalgorithm.h"
#include "../kernel/abstractitemview.h"

namespace Thistle
{

GraphViewPrivate::GraphViewPrivate(GraphView* view): AbstractItemViewPrivate( view )
{
    model = 0;
    itemOffset = QPoint( 0, 0 );
}

GraphViewPrivate::~GraphViewPrivate()
{
    algorithm->deleteLater();
}


QModelIndex GraphViewPrivate::findItemAt( const QPointF& pos ) const
{
  for (int row = 0; row < q_ptr->model()->rowCount(); ++row )
  {
      for (int col = 0; col < q_ptr->model()->columnCount(); ++col )
      {
          QModelIndex index = q_ptr->model()->index( row, col );
          QPainterPath path = q_ptr->itemPath( index );
          if ( path.contains( pos ) )
          {
              return index;
          }
      }
  }
  return QModelIndex();
}


}
