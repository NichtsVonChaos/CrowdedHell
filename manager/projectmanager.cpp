#include "projectmanager.h"


ProjectManager::ProjectManager(CrowdedHellGUI *parent, QTreeView *resourceView) :
	QObject(parent)
{
	m_resouceManager = new ResourceManager(parent, resourceView);
	m_parent = parent;
	connect(this, SIGNAL(sendMessage(MessageType, QString, QString)),
			m_parent, SLOT(sendMessage(MessageType, QString, QString)));
}

bool ProjectManager::isValid()
{
	return !m_projectName.isEmpty();
}

void ProjectManager::newProject()
{
	CreateProjectWizard *wizard = new CreateProjectWizard(m_parent);
	if(wizard->exec() != QWizard::Accepted)
		return;
	QString projectPath = wizard->getPath();
	if(!QDir().exists(projectPath))
	{
		sendMessage(MessageType::Warning, "Project Manager", tr("Selected directory is not existing, try to make this directory, at : \"%1\".").arg(projectPath));
		if(!QDir().mkpath(projectPath))
		{
			sendMessage(MessageType::Error, "Project Manager", tr("Make selected directory failed, at : \"%1\".").arg(projectPath));
		}
	}

	m_projectPath = projectPath;
	m_projectName = wizard->getProjectName();
	QFile projectFile(m_projectPath + QString("/") + m_projectName + QString(".chproj"));
	if(!projectFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
	{
		sendMessage(MessageType::Error, "Project Manager", tr("Create project file failed, at : \"%1\".").arg(m_projectPath + QString("/") + m_projectName + QString(".chproj")));
		m_projectPath.clear();
		m_projectName.clear();
		return;
	}
	projectFile.flush();
	projectFile.close();

	delete wizard;
};
