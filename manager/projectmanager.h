#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QString>
#include <QFile>
#include <QDateTime>
#include <QTextStream>

class ResourceManager;
#ifndef RESOURCEMANAGER_H
#include "resourcemanager.h"
#endif

#include "createprojectwizard.h"

class ProjectManager : public QObject
{
	Q_OBJECT

public:
	ProjectManager(QTreeView *resourceView);

	/**
	 * @brief isValid
	 * Return if a project is open.
	 */
	bool isValid() const;

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
	 * @brief openProject
	 * Open a existing project.
	 * @param projectFilePath
	 * *.chproj file path.
	 */
	void openProject(QString projectFilePath);

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

	/**
	 * @brief reselectMusic
	 * Change background music.
	 */
	void reselectMusic(QString musicPath);

	/**
	 * @brief getProjectName
	 * Return project name.
	 */
	QString getProjectName() const;

	/**
	 * @brief getProjectPath
	 * Return project path with out project file name.
	 */
	QString getProjectPath() const;

	/**
	 * @brief getTemporaryPath
	 * Return temporary project path.
	 */
	QString getTemporaryPath() const;

	/**
	 * @brief getAuthor
	 * Return project author.
	 */
	QString getAuthor() const;

	/**
	 * @brief getCreateDate
	 * Return the date when project created.
	 */
	QDateTime getCreateDate() const;

	/**
	 * @brief getMusicFile
	 * Return music file name.
	 */
	QString getMusicFile() const;

	/**
	 * @brief resouceManager
	 * Return resource manager.
	 */
	ResourceManager *resouceManager() const;

signals:
	/**
	 * @see CrowdedHellGUI::sendMessage(MessageType, QString, QString);
	 */
	void sendMessage(MessageType type, QString module, QString message);

	/**
	 * @brief musicSelected
	 * Send a signal for selected music when create new project.
	 */
	void musicSelected(QString musicPath);

	/**
	 * @brief projectClosed
	 * Send a signal when project closed.
	 */
	void projectClosed();

public slots:

	/**
	 * @brief setAlwaysSave
	 * Set if always save project when close it without question.
	 */
	void setAlwaysSave(bool save);

private:
	ResourceManager *m_resouceManager;

	QString m_projectName;

	QString m_projectPath;

	QString m_temporaryPath;

	QString m_author;

	QDateTime m_createDate;

	QString m_musicFile;

	bool m_changed, m_alwaysSave;
};

#endif // PROJECTMANAGER_H
