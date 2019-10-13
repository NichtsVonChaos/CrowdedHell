#ifndef OPTIONS_H
#define OPTIONS_H

#include <QObject>
#include <QSettings>
#include <QtMath>
#include <QApplication>
#include <QStringList>

enum class Language
{
    EN, ZH_CN, ZH_TW, JP
};

class Options : public QObject
{
    Q_OBJECT

public:
    ~Options() override;
    Language language() const;
    const QString &theme() const;
    bool hideInfoLog() const;
    bool autoSave() const;
    float volume() const;

    const QStringList &recentProject() const;

    void readOptions();
    void writeOptions();

signals:
    void languageChanged(Language language, const QObject *sender);
    void themeChanged(const QString &theme, const QObject *sender);
    void hideInfoLogChanged(bool hideInfoLog, const QObject *sender);
    void autoSaveChanged(bool autoSave, const QObject *sender);

    void volumeChanged(float volume, const QObject *sender);

public slots:
    void setLanguage(Language language, const QObject *sender = nullptr);
    void setTheme(const QString &theme, const QObject *sender = nullptr);
    void setHideInfoLog(bool hideInfoLog, const QObject *sender = nullptr);
    void setAutoSave(bool autoSave, const QObject *sender = nullptr);

    void setVolume(float volume, const QObject *sender = nullptr);

    void addRecentProject(const QString &projectFilePath);
    void removeRecentProject(const QString &projectFilePath);
    void clearRecentProject();

private:
    explicit Options(QObject *parent = nullptr);
    friend Options *options();

    Language m_language;
    QString m_theme;
    bool m_hideInfoLog;
    bool m_autoSave;

    float m_volume;

    QStringList m_recentProject;
};

Options *options();

#endif // OPTIONS_H
