#ifndef CLINT_H
#define CLINT_H
#include <QObject>
#include <QMetaEnum>
#include <QDateTime>
#include <QColor>
#include <QDebug>

class Clint {
  Q_GADGET
public:

  enum Shape {
    None     = 0,
    Circle   = 1,
    Square   = 2,
    Triangle = 3,
    Diamond  = 4
  };

  enum PredefinedColor {
    Blue   = 0x5078BE,
    Green  = 0xAADC5A,
    Red    = 0xFA4646,
    Purple = 0x8264A0,
    Orange = 0xFA7814
  };

  Q_ENUMS( PredefinedColor )
  Q_ENUMS( Shape )

  static QColor predefinedColor( int key ) {
    QMetaObject metaObject = Clint::staticMetaObject;
    QMetaEnum metaEnum = metaObject.enumerator( metaObject.indexOfEnumerator( "PredefinedColor" ) );
    if ( key < 0 || key >= metaEnum.keyCount() ) {
      qsrand( QDateTime::currentDateTime().toTime_t() );
      //qreal v = (qrand() % ( 30 - 10) + 10);
      qreal r = qrand() % 255;
      qreal g = qrand() % 255;
      qreal b = qrand() % 255;
      qDebug() << Q_FUNC_INFO << "random color :" << r << g << b;
      return QColor( r, g, b );
    }
    //return static_cast<Clint::PredefinedColor>( metaEnum.value( key ) );
    return QColor( metaEnum.value( key ) );
  }
};

#endif // CLINT_H
