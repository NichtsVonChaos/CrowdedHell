#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QString>

class ResourceManager;
#ifndef RESOURCEMANAGER_H
#include "resourcemanager.h"
#endif

#include "createprojectwizard.h"

class ProjectManager : public QObject
{
	Q_OBJECT

public:
	ProjectManager(CrowdedHellGUI *parent, QTreeView *resourceView);

	/**
	 * @brief isValid
	 * Return if a project is open.
	 */
	bool isValid();

	/**
	 * @brief newProject
	 * Create a new project.
	 */
	void newProject();

	/**
	 * @brief openProject
	 * Open a existing project.
	 */
	void openProject();

	/**
	 * @brief saveChange
	 * Save all changes of project.
	 */
	void saveChange();

	/**
	 * @brief saveTo
	 * Save project to another path.
	 * @param path
	 * The another path to save the project.
	 */
	void saveTo(const QString &path);

	/**
	 * @brief closeProject
	 * Close current project
	 */
	void closeProject();

signals:
	/**
	 * @see CrowdedHellGUI::sendMessage(MessageType, QString, QString);
	 */
	void sendMessage(MessageType type, QString module, QString message);

private:
	CrowdedHellGUI *m_parent;

	ResourceManager *m_resouceManager;

	QString m_projectName;

	QString m_projectPath;

	QString m_temporaryPath;

	bool m_changed;
};

#endif // PROJECTMANAGER_H
