#include "kiviatcoordinatesystem_p.h"

#include <stdexcept>

namespace Thistle
{
KiviatCoordinateSystemPrivate::KiviatCoordinateSystemPrivate()
{}


KiviatCoordinateSystemPrivate::~KiviatCoordinateSystemPrivate()
{}


AbstractAxis& KiviatCoordinateSystemPrivate::axis( unsigned int index )
{
  if ( int(index) >= this->axisList.count() ) // Count could be be equal to -1
    throw std::out_of_range( "Axis index out of range" );

    return *( this->axisList[ index ].data() );
}


unsigned int KiviatCoordinateSystemPrivate::axisCount() const
{
  return this->axisList.count();
}


void KiviatCoordinateSystemPrivate::update( QAbstractItemModel* model, const QRectF& chartRect )
{
  Q_UNUSED( chartRect ) // Could be used later or by another view
  if ( model == 0 )
    return;

  this->axisList.clear();


  double minimum = 0;
  double maximum = 0;

  for ( int row = 0; row < model->rowCount(); ++row )
  {
    for ( int column = 0; column < model->columnCount(); ++column )
    {
      double value = model->index( row, column ).data().toDouble();
      minimum = qMin( minimum, value );
      maximum = qMax( maximum, value );
    }
  }

  QLineF line( rect.center(), QPointF( rect.right(), rect.center().y() ) );
  qreal angle = 360.0 / qreal( model->rowCount() );
  for ( int row = 0; row < model->rowCount(); ++row )
  {
	QSharedPointer<LinearAxis> axis( new LinearAxis() );
    axis->setLine( line );
    axis->setBounds( minimum, maximum );
    line.setAngle( line.angle() + angle );
    this->axisList.append( axis );
  }
}

}
