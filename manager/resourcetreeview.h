#ifndef RESOURCETREEVIEW_H
#define RESOURCETREEVIEW_H

#include <QTreeView>

class ResourceTreeView :  public QTreeView
{
	Q_OBJECT

public:
	ResourceTreeView(QWidget *parent = Q_NULLPTR);
};

#endif // RESOURCETREEVIEW_H
