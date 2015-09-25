#include "treeview_p.h"

namespace Thistle
{

TreeViewPrivate::TreeViewPrivate() : AbstractItemViewPrivate()
{
	layout = 0;
	connectionPen = QPen( QColor( Thistle::Colors::LightGray ), 2 );
	delegate = new ItemDelegate();
}

TreeViewPrivate::~TreeViewPrivate()
{}

}
