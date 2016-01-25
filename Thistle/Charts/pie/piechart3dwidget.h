#ifndef THISTLE_PIECHART3DWIDGET_H
#define THISTLE_PIECHART3DWIDGET_H

#include <QWidget>

class QAbstractItemModel;
class QLabel;

namespace Thistle
{

class PieChart3DWidgetPrivate;
class PieChart3D;

class PieChart3DWidget : public QWidget
{
	Q_OBJECT
    Q_DECLARE_PRIVATE( PieChart3DWidget )

protected:
    PieChart3DWidgetPrivate* d_ptr;
	void resizeEvent( QResizeEvent* ev );
public:
    PieChart3DWidget( QWidget* parent = 0 );
    ~PieChart3DWidget();

	void setModel( QAbstractItemModel* model );
	QAbstractItemModel* model() const;

	void setTitle( const QString& title );
	QString title() const;

	QFont font() const;
	void setFont( const QFont& font );

    PieChart3D& chart();
};
}

#endif // THISTLE_PIECHART3DWIDGET_H
