#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QTreeView>
#include <QHeaderView>
#include <QStandardItemModel>

class CrowdedHellGUI;
enum class MessageType;
enum class Language;

#ifndef CROWDEDHELLGUI_H
#include "crowdedhellgui.h"
#endif

class ResourceManager : public QObject
{
	Q_OBJECT

public:
	ResourceManager (QTreeView *resourceView);

	QStringList getSpriteNames();
	QString getSpriteFile(QString name);
	QString getSpriteFile(int index);
	QString getSpriteIndex(QString name);
	bool addSprite(QString name, QString filePath);
	bool removeSprite(QString name);
	bool renameSprite(QString name);

	QStringList getSoundNames();
	QString getSoundFile(QString name);
	bool addSound(QString name, QString filePath);
	bool removeSound(QString name);
	bool renameSound(QString name);

	QStringList getBackgroundNames();
	QString getBackgroundFile(QString name);
	bool addBackground(QString name, QString filePath);
	bool removeBackground(QString name);
	bool renameBackground(QString name);

	QStringList getPathNames();
	QString getPathFile(QString name);
	bool addPath(QString name, QString filePath);
	bool removePath(QString name);
	bool renamePath(QString name);

	void clear();

signals:
	void sendMessage(MessageType type, QString module, QString message);

public slots:
	void changeLanguage();

private:
	QStandardItemModel *m_model;

	QMap<QString, QString> m_sprites;
	QMap<QString, QString> m_sounds;
	QMap<QString, QString> m_backgrounds;
	QMap<QString, QString> m_paths;
};

#endif // RESOURCEMANAGER_H
