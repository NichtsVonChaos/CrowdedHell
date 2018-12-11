#include "resourcemanager.h"

ResourceManager::ResourceManager(CrowdedHellGUI *parent, QTreeView *resourceView) :
	QObject(parent)
{
	m_model = new QStandardItemModel(resourceView);
	m_model->setHorizontalHeaderLabels(QStringList() << resourceView->tr("Resources Tree"));

	QStandardItem *item = new QStandardItem(resourceView->tr("Sprites"));
	item->setEditable(false);
	m_model->appendRow(item);
	item = new QStandardItem(resourceView->tr("Sounds"));
	item->setEditable(false);
	m_model->appendRow(item);
	item = new QStandardItem(resourceView->tr("Backgrounds"));
	item->setEditable(false);
	m_model->appendRow(item);
	item = new QStandardItem(resourceView->tr("Paths"));
	item->setEditable(false);
	m_model->appendRow(item);

	m_model->findItems(resourceView->tr("Sounds"))[0]->appendRow(new QStandardItem(resourceView->tr("Test")));
	resourceView->setModel(m_model);

	connect(this, SIGNAL(sendMessage(MessageType, QString, QString)), parent, SLOT(sendMessage(MessageType, QString, QString)));
	sendMessage(MessageType::Info, "Resource Manager", "Initialize resouce manager complete.");
}
