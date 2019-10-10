#include "optionsmanager.h"

OptionsManager::OptionsManager(QObject *parent) : QObject(parent)
{
    readOptions();
}

void OptionsManager::readOptions()
{
    QSettings iniFile("./config.ini", QSettings::IniFormat);

    iniFile.beginGroup("Main");
    setLanguage(Language(iniFile.value("Language", 0).toInt()), this);
    setTheme(iniFile.value("Theme", "Deep Blue").toString(), this);
    setHideInfoLog(iniFile.value("HideInfoLog", false).toBool(), this);
    setAutoSave(iniFile.value("AutoSave", false).toBool(), this);
    iniFile.endGroup();

    iniFile.beginGroup("Audio");
    setVolume(iniFile.value("Volume", 0.6f).toFloat(), this);
    iniFile.endGroup();
}

void OptionsManager::writeOptions()
{
    QSettings iniFile("./config.ini", QSettings::IniFormat);

    iniFile.beginGroup("Main");
    iniFile.setValue("Language", int(m_language));
    iniFile.setValue("Theme", m_theme);
    iniFile.setValue("HideInfoLog", m_hideInfoLog);
    iniFile.setValue("AutoSave", m_autoSave);
    iniFile.endGroup();

    iniFile.beginGroup("Audio");
    iniFile.setValue("Volume", m_volume);
    iniFile.endGroup();
}

float OptionsManager::volume() const
{
    return m_volume;
}

void OptionsManager::setVolume(float volume, const QObject *sender)
{
    m_volume = qMin(qMax(volume, 0.0f), 1.0f);
    emit volumeChanged(volume, sender);
}

bool OptionsManager::autoSave() const
{
    return m_autoSave;
}

void OptionsManager::setAutoSave(bool autoSave, const QObject *sender)
{
    m_autoSave = autoSave;
    emit autoSaveChanged(autoSave, sender);
}

bool OptionsManager::hideInfoLog() const
{
    return m_hideInfoLog;
}

void OptionsManager::setHideInfoLog(bool hideInfoLog, const QObject *sender)
{
    m_hideInfoLog = hideInfoLog;
    emit hideInfoLogChanged(hideInfoLog, sender);
}

QString OptionsManager::theme() const
{
    return m_theme;
}

void OptionsManager::setTheme(const QString &theme, const QObject *sender)
{
    m_theme = theme;
    emit themeChanged(theme, sender);
}

Language OptionsManager::language() const
{
    return m_language;
}

void OptionsManager::setLanguage(Language language, const QObject *sender)
{
    m_language = language;
    emit languageChanged(language, sender);
}

OptionsManager::~OptionsManager()
{
    writeOptions();
}

OptionsManager *options()
{
    static OptionsManager *uniqueOptionsManager = new OptionsManager;
    return uniqueOptionsManager;
}
