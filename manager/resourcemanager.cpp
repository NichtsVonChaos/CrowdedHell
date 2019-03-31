#include "resourcemanager.h"

ResourceManager::ResourceManager(QTreeView *resourceView) :
	QObject(g_mainWindow)
{
	m_model = new QStandardItemModel(resourceView);
	m_model->setHorizontalHeaderLabels(QStringList() << tr("Resources Tree"));

	QStandardItem *item = new QStandardItem(tr("Sprites"));
	item->setEditable(false);
	m_model->appendRow(item);
	item = new QStandardItem(tr("Sounds"));
	item->setEditable(false);
	m_model->appendRow(item);
	item = new QStandardItem(tr("Backgrounds"));
	item->setEditable(false);
	m_model->appendRow(item);
	item = new QStandardItem(tr("Paths"));
	item->setEditable(false);
	m_model->appendRow(item);

	resourceView->setModel(m_model);

	connect(this, &ResourceManager::sendMessage, g_mainWindow, &CrowdedHellGUI::sendMessage);
	connect(g_mainWindow, &CrowdedHellGUI::languageChanged, this, &ResourceManager::changeLanguage);

	emit sendMessage(MessageType::Info, "Resource Manager", tr("Initialize resouce manager complete."));
};

void ResourceManager::changeLanguage()
{
	m_model->setHorizontalHeaderLabels(QStringList() << tr("Resources Tree"));
	m_model->item(0)->setText(tr("Sprites"));
	m_model->item(1)->setText(tr("Sounds"));
	m_model->item(2)->setText(tr("Backgrounds"));
	m_model->item(3)->setText(tr("Paths"));
};
