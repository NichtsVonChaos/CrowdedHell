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

class AddResourceWizard;
#ifndef ADDRESOUCEWIZARD_H
#include "addresourcewizard.h"
#endif

class ResourceItem : public QObject, public QStandardItem
{
	Q_OBJECT

public:
	ResourceItem(ResourceItem *parent = Q_NULLPTR);
	explicit ResourceItem(const QString &text, ResourceItem *parent = Q_NULLPTR);
	ResourceItem(const QIcon &icon, const QString &text, ResourceItem *parent = Q_NULLPTR);
	explicit ResourceItem(int rows, int columns = 1, ResourceItem *parent = Q_NULLPTR);
	~ResourceItem();

	enum class Type : bool
	{
		GROUP, RESOURCE
	};

	Type getType() const;
	void setType(const Type &type);

	QString getPath() const;

	void setText(const QString &text);
	void setFileName(const QString &fileName);

private:
	QString m_name;
	QString m_fileExtension;
	ResourceItem *m_parent;
	Type m_type;
};

class ResourceManager : public QObject
{
	Q_OBJECT

public:
	ResourceManager (QTreeView *resourceView);

	QStringList getSpriteNames();
	QString getSpriteFile(QString name);
	QString getSpriteFile(int index);
	QString getSpriteIndex(QString name);
	bool addSprite();
	bool removeSprite(QString name);
	bool renameSprite(QString name);

	QStringList getSoundNames();
	QString getSoundFile(QString name);
	bool addSound();
	bool removeSound(QString name);
	bool renameSound(QString name);

	QStringList getBackgroundNames();
	QString getBackgroundFile(QString name);
	bool addBackground();
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
