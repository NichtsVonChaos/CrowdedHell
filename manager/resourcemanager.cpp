#include "resourcemanager.h"

ResourceManager::ResourceManager(QTreeView *resourceView) :
	QObject(g_mainWindow)
{
	m_model = new QStandardItemModel(resourceView);
	m_model->setHorizontalHeaderLabels(QStringList() << tr("Resources Tree"));

	ResourceItem *item = new ResourceItem(tr("Sprites"));
	item->setType(ResourceItem::Type::GROUP);
	m_model->appendRow(item);
	item = new ResourceItem(tr("Sounds"));
	item->setType(ResourceItem::Type::GROUP);
	m_model->appendRow(item);
	item = new ResourceItem(tr("Backgrounds"));
	item->setType(ResourceItem::Type::GROUP);
	m_model->appendRow(item);
	item = new ResourceItem(tr("Paths"));
	item->setType(ResourceItem::Type::GROUP);
	m_model->appendRow(item);

	resourceView->setModel(m_model);

	connect(this, &ResourceManager::sendMessage, g_mainWindow, &CrowdedHellGUI::sendMessage);
	connect(g_mainWindow, &CrowdedHellGUI::languageChanged, this, &ResourceManager::changeLanguage);

	emit sendMessage(MessageType::Info, "Resource Manager", tr("Initialize resouce manager complete."));
}

bool ResourceManager::addSprite()
{
	AddResourceWizard *wizard = new AddResourceWizard(AddResourceWizard::Type::SPRITE, g_mainWindow);
	wizard->exec();
	delete wizard;
	/*
	QString spriteFilePathOnProject = g_mainWindow->projectManager()->getProjectPath() + "/Sprites/";
	QFileInfo spriteFileInfo(filePath);
	if(!spriteFileInfo.exists())
	{
		sendMessage(MessageType::Warning, "Resource Manager", tr("Sprite file \"%1\" does not exist, so it will not be added.").arg(filePath));
		return false;
	}

	spriteFilePathOnProject += name + "." + spriteFileInfo.completeSuffix();
	if(!QFile::copy(filePath, spriteFilePathOnProject))
	{
		sendMessage(MessageType::Error, "Resource Manager", tr("Cannot copy sprite file from \"%1\" to \"%2\", failed to add it.").arg(filePath).arg(spriteFilePathOnProject));
		return false;
	}

	m_sprites.insert(name, spriteFilePathOnProject);

	ResourceItem *item = new ResourceItem(name);
	item->setType(ResourceItem::Type::RESOURCE);
	item->setFileName(spriteFilePathOnProject);
	item->setIcon(QIcon(spriteFilePathOnProject));
	m_model->item(0)->appendRow(item);

	return true;*/
	return true;
};

void ResourceManager::changeLanguage()
{
	m_model->setHorizontalHeaderLabels(QStringList() << tr("Resources Tree"));
	dynamic_cast<ResourceItem *>(m_model->item(0))->setText(tr("Sprites"));
	dynamic_cast<ResourceItem *>(m_model->item(1))->setText(tr("Sounds"));
	dynamic_cast<ResourceItem *>(m_model->item(2))->setText(tr("Backgrounds"));
	dynamic_cast<ResourceItem *>(m_model->item(3))->setText(tr("Paths"));
};

ResourceItem::ResourceItem(ResourceItem *parent) :
	QObject(parent), m_parent(parent)
{
	setEditable(false);
}

ResourceItem::ResourceItem(const QString &text, ResourceItem *parent) :
	QObject(parent), QStandardItem(text), m_parent(parent)
{
	m_name = text;
	setEditable(false);
}

ResourceItem::ResourceItem(const QIcon &icon, const QString &text, ResourceItem *parent) :
	QObject(parent), QStandardItem(icon, text), m_parent(parent)
{
	m_name = text;
	setEditable(false);
}

ResourceItem::ResourceItem(int rows, int columns, ResourceItem *parent) :
	QObject(parent), QStandardItem(rows, columns), m_parent(parent)
{
	setEditable(false);
}

ResourceItem::~ResourceItem()
{

}

ResourceItem::Type ResourceItem::getType() const
{
	return m_type;
}

void ResourceItem::setType(const ResourceItem::Type &type)
{
	m_type = type;
	switch(type)
	{
		case Type::GROUP:
			setProperty("itemType", "group");
		break;

		case Type::RESOURCE:
			setProperty("itemType", "resource");
		break;
	}
}

QString ResourceItem::getPath() const
{
	if(m_parent == Q_NULLPTR)
		return g_mainWindow->projectManager()->getProjectPath() + "/" + m_name;
	else
	{
		QString name;
		if(m_name == tr("Sprites"))
			name = "Sprites";
		else if(m_name == tr("Sounds"))
			name = "Sounds";
		else if(m_name == tr("Backgrounds"))
			name = "Backgrounds";
		else if(m_name == tr("Path"))
			name = "Path";
		return m_parent->getPath() + "/" + name + m_fileExtension;
	}
}

void ResourceItem::setText(const QString &text)
{
	if(m_type == Type::GROUP)
	{
		m_name = text;
		m_fileExtension.clear();
		QStandardItem::setText(text);
	}
	else
	{
		QFile fileInfo(getPath());
		fileInfo.rename(m_parent->getPath() + "/" + text + m_fileExtension);
		m_name = text;
		QStandardItem::setText(m_name);
	}
}

void ResourceItem::setFileName(const QString &fileName)
{
	if(m_type != Type::RESOURCE)
		return;

	QFileInfo fileInfo((m_parent ? m_parent->getPath() : g_mainWindow->projectManager()->getProjectPath()) + "/" + fileName);
	m_name = fileInfo.baseName();
	m_fileExtension = fileInfo.completeSuffix();
	QStandardItem::setText(m_name);
}
