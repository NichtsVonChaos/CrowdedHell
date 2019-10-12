#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>
#include <QDateTime>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QMessageBox>
#include <QApplication>

#include "logger.h"
#include "optionsmanager.h"
#include "Codes/Wizards/createprojectwizard.h"

class Project : public QObject
{
    Q_OBJECT

public:
    ~Project() override;

    /**
     * @brief isValid
     * Return if a project is open.
     */
    bool isValid() const;

    /**
     * @brief getProjectName
     * Return project name.
     */
    const QString &projectName() const;

    /**
     * @brief getProjectPath
     * Return project path with out project file name.
     */
    const QString &projectPath() const;

    /**
     * @brief getTemporaryPath
     * Return temporary project path.
     */
    const QString &temporaryPath() const;

    /**
     * @brief getAuthor
     * Return project author.
     */
    const QString &author() const;

    /**
     * @brief getCreateDate
     * Return the date when project created.
     */
    const QDateTime &createDate() const;

    /**
     * @brief getMusicFile
     * Return music file name.
     */
    const QString &musicFile() const;

    void initialze();

signals:
    /**
     * @see CrowdedHellGUI::sendMessage(MessageType, QString, QString);
     */
    void message(Logger::Type type, const QString &module, const QString &message);

    /**
     * @brief musicSelected
     * Send a signal for selected music when create new project.
     */
    void musicSelected(const QString &musicPath);

    /**
     * @brief projectOpened
     */
    void projectOpened(const QString &projectFilePath);

    /**
     * @brief projectClosed
     * Send a signal when project closed.
     */
    void projectClosed();

public slots:
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
    void openProject(const QString &projectFilePath);

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
    void reselectMusic();
    void reselectMusic(const QString &musicPath);

private:
    explicit Project(QObject *parent = nullptr);
    friend Project *project();

    bool m_changed, m_initilized;
    QString m_projectName;
    QString m_projectPath;
    QString m_temporaryPath;
    QString m_author;
    QDateTime m_createDate;
    QString m_musicFile;
};

Project *project();

#endif // PROJECT_H
