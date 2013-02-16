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

    Blue       = 0x5078BE,
    Green      = 0xAADC5A,
    Orange     = 0xFA7814,
    Purple     = 0x8264A0,
    Red        = 0xFA4646,
    Yellow     = 0xFFF43A,
    Gray       = 0x707070,

    LightBlue   = 0x4889F1,
    LightGreen  = 0x9AEC7A,
    LightOrange = 0xFC9834,
    LightPurple = 0x8274B0,
    LightRed    = 0xFC5656,
    LightYellow = 0xFFF46A,
    LightGray   = 0xA0A0A0,

    DarkBlue   = 0x0839A1,
    DarkGray   = 0x505050,
    DarkGreen  = 0x8CD01E,
    DarkOrange = 0xE46B0F,
    DarkPurple = 0x6D3AA0,
    DarkYellow = 0xFFF219

    /*Green  = 0xAADC5A,
    Red    = 0xFA4646,
    Purple = 0x8264A0,
    Orange = 0xFA7814*/
  };

  Q_ENUMS( PredefinedColor )
  Q_ENUMS( Shape )

  static QColor predefinedColor( int key ) {
    QMetaObject metaObject = Clint::staticMetaObject;
    QMetaEnum metaEnum = metaObject.enumerator( metaObject.indexOfEnumerator( "PredefinedColor" ) );
    if ( key < 0 || key >= metaEnum.keyCount() ) {
      return QColor( 20, 20, 20 );
    }
    //return static_cast<Clint::PredefinedColor>( metaEnum.value( key ) );
    return QColor( metaEnum.value( key ) );
  }
};

#endif // CLINT_H
