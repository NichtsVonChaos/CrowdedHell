#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QString>

class ResourceManager;
#ifndef RESOURCEMANAGER_H
#include "resourcemanager.h"
#endif

class ProjectManager : public QObject
{
	Q_OBJECT

public:
	ProjectManager(CrowdedHellGUI *parent, QTreeView *resourceView);

	bool isValid();

private:
	QString m_name;
	QString m_tempPath;
	ResourceManager *m_resouceManager;
};

#endif // PROJECTMANAGER_H
