#ifndef CHARTTEST_H
#define CHARTTEST_H

#include <QObject>
#include <qpixmap.h>
#include <qwidget.h>
#include <QDebug>
#include <qtest.h>

#include <Thistle/Charts/line/linechartwidget.h>

class ChartTest : public QObject
{
	Q_OBJECT
protected:
	Thistle::LineChartWidget* chart;
private slots:
	void init();
    void test1();
    void test2();
	void test3();
	void test4();
	void test5();
	void test6();
	void test7();
	void test8();
	void test9();
	void test10();
    void cleanup();
public:
	static bool compare( const QImage& img1, const QImage& img2 );
	static QPixmap snapshot( QWidget& w );
	static void save( QWidget& w, const QString& name );
	
};


//#include "moc_charttest.cpp"

#endif // CHARTTEST_H
