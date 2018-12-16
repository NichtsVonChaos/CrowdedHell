#include "projectmanager.h"


ProjectManager::ProjectManager(CrowdedHellGUI *parent, QTreeView *resourceView) :
	QObject(parent)
{
	m_resouceManager = new ResourceManager(parent, resourceView);
};
