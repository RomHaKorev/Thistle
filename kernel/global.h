/*
This file is part of Thistle.

Thistle is free software: you can redistribute it and/or modify
it under the terms of the Lesser GNU General Public License as published by
the Free Software Foundation, either version 3 of the License.

Thistle is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
Lesser GNU General Public License for more details.

You should have received a copy of the Lesser GNU General Public License
along with Thistle.    If not, see <http://www.gnu.org/licenses/>.

Thistle    Copyright (C) 2013    Dimitry Ernot & Romha Korev
*/

#ifndef GLOBAL_H
#define GLOBAL_H
#include <QObject>
#include <QMetaEnum>
#include <QDateTime>
#include <QColor>
#include <QDebug>
#include <QtCore/qnamespace.h>
#include <QList>

namespace Thistle {

    /*!
    \class Colors
    \brief Contains the predefined colors and static methods to get default colors for series.
    */
    class Colors {
        Q_GADGET
    public:
        /*!
        \enum PredefinedColor
        \brief Thistle predefined colors used by default.
        \htmlinclude colors.html
        */
        enum PredefinedColor {
            Blue		= 0x6090E4,
            Green		= 0x9AEC7A,
            Purple		= 0x8600C8,
            Red			= 0xDB0000,
            Orange		= 0xFF8000,
            Yellow      = 0xFDEF46,
            Gray		= 0x868686,

            LightBlue	= 0x68B9FF,
            LightGreen	= 0xC9FF71,
            LightPurple = 0x8274B0,
            LightRed	= 0xFF5656,
            LightOrange = 0xFFB366,
            LightYellow = 0xFDF285,
            LightGray	= 0xBEBEBE,

            DarkBlue     = 0x0839A1,
            DarkGreen    = 0x8CD01E,
            DarkPurple   = 0x7300AB,
            DarkRed	     = 0xBA0000,
            DarkOrange   = 0xff5100,
            DarkYellow   = 0xFFF219,
            DarkGray     = 0x505050
        };

        Q_ENUMS( PredefinedColor )

        /*!
        Returns the \a QColor in the enumeration \s PredefinedColors at the index \a key.
        This function is used by \s ItemStyle to get a color by default.
        */
        static QColor predefinedColor( int key ) {
            QMetaObject metaObject = Colors::staticMetaObject;

            QMetaEnum metaEnum = metaObject.enumerator( metaObject.indexOfEnumerator( "PredefinedColor" ) );

            if ( key < 0 || key >= metaEnum.keyCount() ) return QColor( 90, 90, 90 );

            return QColor( metaEnum.value( key ) );
        }

        /*!
        Returns the *light* \a QColor in the enumeration \s PredefinedColors at the index \a key.
        This function is used by \s ItemStyle to get a color by default.
        */
        static QColor predefinedLightColor( int key ) {
            QMetaObject metaObject = Colors::staticMetaObject;

            QMetaEnum metaEnum = metaObject.enumerator( metaObject.indexOfEnumerator( "PredefinedColor" ) );
            QList<int> colors;

            for( int i = 0; i < metaEnum.keyCount(); ++i ) {
                QString key( metaEnum.key( i ) );
                if ( key.startsWith("Light") == true ) colors << metaEnum.value( i );
            }

            if ( key < 0 || key >= colors.count() ) return QColor( 90, 90, 90 );

            return QColor( colors.at( key ) );
        }

        /*!
        Returns the *regular* \a QColor in the enumeration \s PredefinedColors at the index \a key.
        This function is used by \s ItemStyle to get a color by default.
        */
        static QColor predefinedRegularColor( int key ) {
            QMetaObject metaObject = Colors::staticMetaObject;
            QMetaEnum metaEnum = metaObject.enumerator( metaObject.indexOfEnumerator( "PredefinedColor" ) );
            QList<int> colors;
            for( int i = 0; i < metaEnum.keyCount(); ++i ) {
                QString key( metaEnum.key( i ) );
                if ( (key.startsWith("Light") || key.startsWith("Dark") ) == false ) colors << metaEnum.value( i );
            }
            if ( key < 0 || key >= colors.count() )  return QColor( 20, 20, 20 );

            return QColor( colors.at( key ) );
        }

        /*!
        Returns the *dark* \a QColor in the enumeration \s PredefinedColors at the index \a key.
        This function is used by \s ItemStyle to get a color by default.
        */
        static QColor predefinedDarkColor( int key ) {
            QMetaObject metaObject = Colors::staticMetaObject;
            QMetaEnum metaEnum = metaObject.enumerator( metaObject.indexOfEnumerator( "PredefinedColor" ) );
            QList<int> colors;
            for( int i = 0; i < metaEnum.keyCount(); ++i ) {
                QString key( metaEnum.key( i ) );
                if ( key.startsWith("Dark") == true ) colors << metaEnum.value( i );
            }
            if ( key < 0 || key >= colors.count() ) return QColor( 20, 20, 20 );

            return QColor( colors.at( key ) );
        }
    };

    /*!
    \enum Shape
    \brief The predefined shape for item delegates.
    */
    enum Shape {
        Rectangle = 0,
        Ellipse     = 1,
        RoundedRect = 2,
        Diamond = 3,
        Triangle = 4,
        ReversedTriangle = 5
    };

    /*!
    \enum Type
    \brief This enum contains the type of serie that can be used in \s LinearChart class.
    \s Dot and \s Line can be used together. \s Bar should be used alone.
    */
    enum Type {
        Dot     = 1,
        Line    = 2,
        Bar     = 4,
        Spline  = 8, 
        Area    = 16
    };

    Q_DECLARE_FLAGS( Types, Type )
    Q_DECLARE_OPERATORS_FOR_FLAGS( Types )

}

#endif // MARB_H
