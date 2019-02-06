#include "projectmanager.h"

ProjectManager::ProjectManager(CrowdedHellGUI *parent, QTreeView *resourceView) :
	QObject(parent), m_changed(false), m_alwaysSave(false)
{
	m_resouceManager = new ResourceManager(parent, resourceView);
	m_parent = parent;
	connect(this, SIGNAL(sendMessage(MessageType, QString, QString)),
			m_parent, SLOT(sendMessage(MessageType, QString, QString)));
	connect(this, SIGNAL(musicSelected(QString)),
			m_parent, SLOT(changeMusic(QString)));
	connect(this, SIGNAL(projectClosed()),
			m_parent, SLOT(projectClosed()));
};

bool ProjectManager::isValid()
{
	return !m_projectName.isEmpty();
};

void ProjectManager::newProject()
{
	CreateProjectWizard *wizard = new CreateProjectWizard(m_parent);
	if(wizard->exec() != QWizard::Accepted)
	{
		delete wizard;
		return;
	}

	QString projectPath = wizard->getPath() + QString("/") + wizard->getProjectName();

	QString musicPath = wizard->getMusicPath();
	if(!QFile(musicPath).exists())
	{
		emit sendMessage(MessageType::Error, "Project Manager", tr("Select music file doesn't exist, at : \"%1\"").arg(musicPath));
		delete wizard;
		return;
	}

	if(!QDir().exists(projectPath))
	{
		if(!QDir().mkpath(projectPath))
		{
			emit sendMessage(MessageType::Error, "Project Manager", tr("Make selected directory failed, at : \"%1\".").arg(projectPath));
			delete wizard;
			return;
		}
	}
	else
	{
		emit sendMessage(MessageType::Error, "Project Manager", tr("Project is already exists, at : \"%1\".").arg(projectPath));
		delete wizard;
		return;
	}

	if(!QDir().mkpath(projectPath + QString("/sprites")))
	{
		emit sendMessage(MessageType::Error, "Project Manager", tr("Make selected directory failed, at : \"%1\".").arg(projectPath) + QString("/sprites"));
		delete wizard;
		return;
	}
	if(!QDir().mkpath(projectPath + QString("/sounds")))
	{
		emit sendMessage(MessageType::Error, "Project Manager", tr("Make selected directory failed, at : \"%1\".").arg(projectPath) + QString("/sounds"));
		delete wizard;
		return;
	}
	if(!QDir().mkpath(projectPath + QString("/backgrounds")))
	{
		emit sendMessage(MessageType::Error, "Project Manager", tr("Make selected directory failed, at : \"%1\".").arg(projectPath) + QString("/backgrounds"));
		delete wizard;
		return;
	}
	if(!QDir().mkpath(projectPath + QString("/paths")))
	{
		emit sendMessage(MessageType::Error, "Project Manager", tr("Make selected directory failed, at : \"%1\".").arg(projectPath) + QString("/paths"));
		delete wizard;
		return;
	}

	m_projectPath = projectPath;
	m_projectName = wizard->getProjectName();
	QFile projectFile(m_projectPath + QString("/") + m_projectName + QString(".chproj"));
	if(!projectFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
	{
		emit sendMessage(MessageType::Error, "Project Manager", tr("Create project file failed, at : \"%1\".").arg(m_projectPath + QString("/") + m_projectName + QString(".chproj")));
		m_projectPath.clear();
		m_projectName.clear();
		delete wizard;
		return;
	}

	QTextStream textStream(&projectFile);
	textStream << "This is just a project file for Crowded Hell." << endl;
	textStream << wizard->getAuthor() << endl;
	textStream << wizard->getDate().toString() << endl;
	textStream << QFileInfo(musicPath).fileName() << endl;

	projectFile.flush();
	projectFile.close();

	if(!QFile::copy(musicPath, projectPath + QString("/") + QFileInfo(musicPath).fileName()))
	{
		emit sendMessage(MessageType::Error, "Project Manager", tr("Cannot copy music file from \"%1\" to \"%2\".").arg(musicPath).arg(projectPath + QString("/") + QFileInfo(musicPath).fileName()));
		m_projectPath.clear();
		m_projectName.clear();
		delete wizard;
		return;
	}
	else
		emit sendMessage(MessageType::Info, "Project Manager", tr("Successfully copy music file from \"%1\" to \"%2\".").arg(musicPath).arg(projectPath + QString("/") + QFileInfo(musicPath).fileName()));

	m_author = wizard->getAuthor();
	m_date = wizard->getDate();
	m_music = QFileInfo(musicPath).fileName();

	emit musicSelected(projectPath + QString("/") + QFileInfo(musicPath).fileName());

	emit sendMessage(MessageType::Info, "Project Manager", tr("Create project finished."));

	delete wizard;
};

void ProjectManager::openProject()
{
	QString projectFilePath = QFileDialog::getOpenFileName(nullptr, tr("Select a project"), ".", "Crowded Hell Project(*.chproj)");
	if(projectFilePath.isEmpty())
		return;

	QFile projectFile(projectFilePath);
	if(!projectFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		emit sendMessage(MessageType::Error, "Projcet Manager", tr("Open selected project file failed, at : \"%1\".").arg(projectFilePath));
		return;
	}

	QTextStream textStream(&projectFile);
	QStringList datas = textStream.readAll().split("\n");
	projectFile.close();

	if(datas.size() < 4)
	{
		emit sendMessage(MessageType::Error, "Project Manager", tr("Project file format abnormal."));
		return;
	}

	QString projectPath = QFileInfo(projectFilePath).absolutePath();

	if(!QFile(projectPath + QString("/") + m_music).exists())
	{
		emit sendMessage(MessageType::Error, "Project Manager", tr("Cannot find the music file of project, at : \"%1\"").arg(m_projectPath + QString("/") + m_music));
		return;
	}

	closeProject();

	m_projectName = QFileInfo(projectFilePath).fileName().remove(QString(".chproj"));
	m_projectPath = QFileInfo(projectFilePath).absolutePath();
	m_author = datas[1];
	m_date = QDateTime::fromString(datas[2]);
	m_music = datas[3];
	emit musicSelected(projectPath + QString("/") + m_music);
};

void ProjectManager::closeProject()
{
	if(!isValid())
		return;

	if(m_changed)
	{
		int shouldSave = QMessageBox::No;
		if(!m_alwaysSave)
			shouldSave = QMessageBox::question(nullptr, tr("Confirm"), tr("Would you like to save your project before close it?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes);

		if(shouldSave == QMessageBox::Cancel)
			return;

		if(m_alwaysSave || shouldSave == QMessageBox::Yes)
		{

		}
	}

	emit sendMessage(MessageType::Info, "Project Manager", tr("Project \"%1\" closed.").arg(m_projectName));
	emit projectClosed();

	m_projectName.clear();
	m_projectPath.clear();
	m_temporaryPath.clear();
	m_author.clear();
	m_music.clear();

	m_changed = false;
};

void ProjectManager::reselectMusic(QString musicPath)
{
	if(!isValid())
	{
		emit sendMessage(MessageType::Error, "Project Manager", tr("No project is open. Please create or open a project before."));
		return;
	}

	if(!QFile(musicPath).exists())
	{
		emit sendMessage(MessageType::Error, "Project Manager", tr("Select music file doesn't exist, at : \"%1\"").arg(musicPath));
		return;
	}

	if(!QFile::copy(musicPath, m_projectPath + QString("/") + QFileInfo(musicPath).fileName()))
	{
		emit sendMessage(MessageType::Error, "Project Manager", tr("Cannot copy music file from \"%1\" to \"%2\".").arg(musicPath).arg(m_projectPath + QString("/") + QFileInfo(musicPath).fileName()));
		return;
	}

	QFile projectFile(m_projectPath + QString("/") + m_projectName + QString(".chproj"));
	if(!projectFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		emit sendMessage(MessageType::Error, "Project Manager", tr("Project file doesn't exist, at : \"%1\".").arg(m_projectPath + QString("/") + m_projectName + QString(".chproj")));
		return;
	}

	QTextStream textStream(&projectFile);
	QStringList datas = textStream.readAll().split("\n");
	projectFile.close();

	if(datas.size() < 4)
	{
		emit sendMessage(MessageType::Error, "Project Manager", tr("Project file format abnormal."));
		return;
	}

	if(QFile(m_projectPath + "/" + datas[3]).exists())
		if(!QFile(m_projectPath + "/" + datas[3]).remove())
			emit sendMessage(MessageType::Warning, "Project Manager", tr("Failed remove music file \"%1\".").arg(m_projectPath + "/" + datas[3]));

	datas[3] = QFileInfo(musicPath).fileName();

	if(!projectFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
	{
		emit sendMessage(MessageType::Error, "Project Manager", tr("Project file doesn't exist, at : \"%1\".").arg(m_projectPath + QString("/") + m_projectName + QString(".chproj")));
		return;
	}

	foreach(auto i, datas)
		textStream << i << endl;
	projectFile.flush();

	emit musicSelected(m_projectPath + QString("/") + QFileInfo(musicPath).fileName());

	return;
};

void ProjectManager::setAlwaysSave(bool save)
{
	m_alwaysSave = save;
};
