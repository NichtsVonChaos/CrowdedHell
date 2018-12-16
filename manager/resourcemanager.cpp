#include "resourcemanager.h"

ResourceManager::ResourceManager(CrowdedHellGUI *parent, QTreeView *resourceView) :
	QObject(parent)
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

	connect(this, SIGNAL(sendMessage(MessageType, QString, QString)), parent, SLOT(sendMessage(MessageType, QString, QString)));
	connect(parent, SIGNAL(languageChanged(Language)), this, SLOT(changeLanguage(Language)));

	emit sendMessage(MessageType::Info, "Resource Manager", tr("Initialize resouce manager complete."));
};

void ResourceManager::changeLanguage(Language language)
{
	m_model->setHorizontalHeaderLabels(QStringList() << tr("Resources Tree"));
	m_model->item(0)->setText(tr("Sprites"));
	m_model->item(1)->setText(tr("Sounds"));
	m_model->item(2)->setText(tr("Backgrounds"));
	m_model->item(3)->setText(tr("Paths"));
};
