#include "projectmanager.h"


ProjectManager::ProjectManager(CrowdedHellGUI *parent, QTreeView *resourceView) :
	QObject(parent)
{
	m_resouceManager = new ResourceManager(parent, resourceView);
	m_parent = parent;
	connect(this, SIGNAL(sendMessage(MessageType, QString, QString)),
			m_parent, SLOT(sendMessage(MessageType, QString, QString)));
	connect(this, SIGNAL(musicSelected(QString)),
			m_parent, SLOT(changeMusic(QString)));
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
	QString projectPath = wizard->getPath() + QString("/") + wizard->getProjectName();

	QString musicPath = wizard->getMusicPath();
	if(!QFile(musicPath).exists())
	{
		sendMessage(MessageType::Error, "Project Manager", tr("Select music file doesn't exist, at : \"%1\"").arg(musicPath));
		delete wizard;
		return;
	}

	if(!QDir().exists(projectPath))
	{
		if(!QDir().mkpath(projectPath))
		{
			sendMessage(MessageType::Error, "Project Manager", tr("Make selected directory failed, at : \"%1\".").arg(projectPath));
			delete wizard;
			return;
		}
	}
	else
	{
		sendMessage(MessageType::Error, "Project Manager", tr("Project is already exists, at : \"%1\".").arg(projectPath));
		delete wizard;
		return;
	}

	if(!QDir().mkpath(projectPath + QString("/sprites")))
	{
		sendMessage(MessageType::Error, "Project Manager", tr("Make selected directory failed, at : \"%1\".").arg(projectPath) + QString("/sprites"));
		delete wizard;
		return;
	}
	if(!QDir().mkpath(projectPath + QString("/sounds")))
	{
		sendMessage(MessageType::Error, "Project Manager", tr("Make selected directory failed, at : \"%1\".").arg(projectPath) + QString("/sounds"));
		delete wizard;
		return;
	}
	if(!QDir().mkpath(projectPath + QString("/backgrounds")))
	{
		sendMessage(MessageType::Error, "Project Manager", tr("Make selected directory failed, at : \"%1\".").arg(projectPath) + QString("/backgrounds"));
		delete wizard;
		return;
	}
	if(!QDir().mkpath(projectPath + QString("/paths")))
	{
		sendMessage(MessageType::Error, "Project Manager", tr("Make selected directory failed, at : \"%1\".").arg(projectPath) + QString("/paths"));
		delete wizard;
		return;
	}

	m_projectPath = projectPath;
	m_projectName = wizard->getProjectName();
	QFile projectFile(m_projectPath + QString("/") + m_projectName + QString(".chproj"));
	if(!projectFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
	{
		sendMessage(MessageType::Error, "Project Manager", tr("Create project file failed, at : \"%1\".").arg(m_projectPath + QString("/") + m_projectName + QString(".chproj")));
		m_projectPath.clear();
		m_projectName.clear();
		delete wizard;
		return;
	}

	QTextStream textStream(&projectFile);
	textStream << "This is just a project file for Crowded Hell." << endl;
	textStream << wizard->getAuthor() << endl;
	textStream << wizard->getDate().toString() << endl;
	textStream << QFileInfo(musicPath).fileName();

	projectFile.flush();
	projectFile.close();

	if(!QFile::copy(musicPath, projectPath + QString("/") + QFileInfo(musicPath).fileName()))
	{
		sendMessage(MessageType::Error, "Project Manager", tr("Cannot copy music file from \"%1\" to \"%2\".").arg(musicPath).arg(projectPath + QString("/") + QFileInfo(musicPath).fileName()));
		m_projectPath.clear();
		m_projectName.clear();
		delete wizard;
		return;
	}

	musicSelected(projectPath + QString("/") + QFileInfo(musicPath).fileName());

	delete wizard;
}

void ProjectManager::reselectMusic(QString musicPath)
{
	if(!isValid())
	{
		sendMessage(MessageType::Error, "Project Manager", tr("No project is open. Please create or open a project before."));
		return;
	}

	if(!QFile(musicPath).exists())
	{
		sendMessage(MessageType::Error, "Project Manager", tr("Select music file doesn't exist, at : \"%1\"").arg(musicPath));
		return;
	}
	QFile projectFile(m_projectPath + QString("/") + m_projectName + QString(".chproj"));
	if(!projectFile.open(QIODevice::ReadWrite | QIODevice::Text))
	{
		sendMessage(MessageType::Error, "Project Manager", tr("Project file doesn't exist, at : \"%1\".").arg(m_projectPath + QString("/") + m_projectName + QString(".chproj")));
		return;
	}

};
