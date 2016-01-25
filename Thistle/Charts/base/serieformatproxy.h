#ifndef THISTLE_SERIEFORMATPROXY_H
#define THISTLE_SERIEFORMATPROXY_H

#include "serieformat.h"

#include <QtCore/QObject>

namespace Thistle
{

class SerieFormatProxy: public QObject
{
private:
	QMap<int, SerieFormat> formats;

public:
	explicit SerieFormatProxy( QObject* parent = 0 );
	virtual ~SerieFormatProxy();

	virtual Thistle::SerieFormat serieFormat( int column ) const;
	void setSerieFormat( int column, const Thistle::SerieFormat& style );
	bool contains( int column ) const;
};

}

#endif //THISTLE_SERIEFORMATPROXY_H
