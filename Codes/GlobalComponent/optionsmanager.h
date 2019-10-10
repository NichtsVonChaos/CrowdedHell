#ifndef OPTIONSMANAGER_H
#define OPTIONSMANAGER_H

#include <QObject>
#include <QSettings>
#include <QtMath>

enum class Language
{
    EN, ZH_CN, ZH_TW, JP
};

class OptionsManager : public QObject
{
    Q_OBJECT

public:
    ~OptionsManager();
    Language language() const;
    QString theme() const;
    bool hideInfoLog() const;
    bool autoSave() const;
    float volume() const;

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

private:
    explicit OptionsManager(QObject *parent = nullptr);
    friend OptionsManager *options();

    void readOptions();
    void writeOptions();

    Language m_language;
    QString m_theme;
    bool m_hideInfoLog;
    bool m_autoSave;

    float m_volume;
};

OptionsManager *options();

#endif // OPTIONSMANAGER_H
