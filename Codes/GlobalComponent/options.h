#ifndef OPTIONS_H
#define OPTIONS_H

#include <QObject>
#include <QSettings>
#include <QtMath>
#include <QApplication>
#include <QStringList>
#include <QDockWidget>
#include "customfunctions.h"

/**
 * @brief The Language enum
 * EN: English
 * ZH_CN: Simplified Chinese
 * ZH_TW: Traditional Chinese
 * JP: Japanese
 */
enum class Language
{
    EN, ZH_CN, ZH_TW, JP
};

/**
 * @brief The Options class
 * Options manager.
 */
class Options : public QObject
{
    Q_OBJECT

public:
    ~Options() override;

    /**
     * @brief language
     * Get current language.
     */
    Language language() const;

    /**
     * @brief theme
     * @Get current theme.
     */
    const QString &theme() const;

    /**
     * @brief hideInfoLog
     * Get whether user checked "Hide all 'Info' massage type" option.
     */
    bool hideInfoLog() const;

    /**
     * @brief autoSave
     * Get whether user checked "Always save project before close" option.
     */
    bool autoSave() const;

    /**
     * @brief volume
     * Get current volume.
     */
    float volume() const;

    /**
     * @brief maxWindowStart
     * Get whether start with max window.
     */
    bool maxWindowStart() const;

    /**
     * @brief recentProject
     * Get recent opened project path list.
     */
    const QStringList &recentProject() const;

    /**
     * @brief readOptions
     * Read options from config.ini.
     */
    void readOptions();

    /**
     * @brief writeOptions
     * Save options to config.ini.
     */
    void writeOptions();

    QByteArray mainWindowState() const;
    void setMainWindowState(const QByteArray &mainWindowState);

    QByteArray mainWindowGeometry() const;
    void setMainWindowGeometry(const QByteArray &mainWindowGeometry);

signals:
    /**
     * @brief languageChanged
     * Emit a signal when change language.
     */
    void languageChanged(Language language, const QObject *sender);

    /**
     * @brief themeChanged
     * Emit a signal when change theme.
     */
    void themeChanged(const QString &theme, const QObject *sender);

    /**
     * @brief hideInfoLogChanged
     * Emit a signal when user check or uncheck "Hide all 'Info' massage type" option.
     */
    void hideInfoLogChanged(bool hideInfoLog, const QObject *sender);

    /**
     * @brief autoSaveChanged
     * Emit a signal when user check or uncheck "Always save project before close" option.
     */
    void autoSaveChanged(bool autoSave, const QObject *sender);

    /**
     * @brief volumeChanged
     * Emit a signal when change volume.
     */
    void volumeChanged(float volume, const QObject *sender);

public slots:
    /**
     * @brief setLanguage
     * Set current language.
     */
    void setLanguage(Language language, const QObject *sender = nullptr);

    /**
     * @brief setTheme
     * Set current theme.
     */
    void setTheme(const QString &theme, const QObject *sender = nullptr);

    /**
     * @brief setHideInfoLog
     * Set whether user checked "Hide all 'Info' massage type" option.
     */
    void setHideInfoLog(bool hideInfoLog, const QObject *sender = nullptr);

    /**
     * @brief setAutoSave
     * Set whether user checked "Always save project before close" option.
     */
    void setAutoSave(bool autoSave, const QObject *sender = nullptr);

    /**
     * @brief setVolume
     * Set current volume.
     */
    void setVolume(float volume, const QObject *sender = nullptr);

    /**
     * @brief setMaxWindowStart
     * Set if start with max window.
     */
    void setMaxWindowStart(bool maxWindowStart);

    /**
     * @brief addRecentProject
     * Add project path to recent opened project path list.
     */
    void addRecentProject(const QString &projectFilePath);

    /**
     * @brief removeRecentProject
     * Remove project path from recent opened project path list.
     */
    void removeRecentProject(const QString &projectFilePath);

    /**
     * @brief clearRecentProject
     * Clear recent opened project path list.
     */
    void clearRecentProject();

private:
    explicit Options(QObject *parent = nullptr);
    friend Options *options();

    Language m_language;
    QString m_theme;
    bool m_hideInfoLog;
    bool m_autoSave;
    bool m_maxWindowStart;

    float m_volume;

    QStringList m_recentProject;
    QByteArray m_mainWindowState, m_mainWindowGeometry;
};

/**
 * @brief options
 * Get the unique Options instance.
 */
Options *options();

#endif // OPTIONS_H
