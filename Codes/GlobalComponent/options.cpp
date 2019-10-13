#include "options.h"

Options::Options(QObject *parent) : QObject(parent)
{

}

const QStringList &Options::recentProject() const
{
    return m_recentProject;
}

void Options::readOptions()
{
    QSettings iniFile(QApplication::applicationDirPath() + "/config.ini", QSettings::IniFormat);

    iniFile.beginGroup("Main");
    m_language = Language(iniFile.value("Language", 0).toInt());
    m_theme = iniFile.value("Theme", "Deep Blue").toString();
    m_hideInfoLog = iniFile.value("HideInfoLog", false).toBool();
    m_autoSave = iniFile.value("AutoSave", false).toBool();
    iniFile.endGroup();

    iniFile.beginGroup("Audio");
    m_volume = iniFile.value("Volume", 0.6f).toFloat();
    iniFile.endGroup();

    iniFile.beginGroup("Recent");
    QString projectPath;
    for(int i = 0; i < 10; i++)
    {
        projectPath = iniFile.value(QString("Project") + QString::number(i), QString()).toString();
        if(projectPath.isEmpty())
            break;
        m_recentProject << projectPath;
    }
    iniFile.endGroup();
}

void Options::writeOptions()
{
    QSettings iniFile(QApplication::applicationDirPath() + "/config.ini", QSettings::IniFormat);

    iniFile.beginGroup("Main");
    iniFile.setValue("Language", int(m_language));
    iniFile.setValue("Theme", m_theme);
    iniFile.setValue("HideInfoLog", m_hideInfoLog);
    iniFile.setValue("AutoSave", m_autoSave);
    iniFile.endGroup();

    iniFile.beginGroup("Audio");
    iniFile.setValue("Volume", double(m_volume));
    iniFile.endGroup();

    iniFile.beginGroup("Recent");
    for(int i = m_recentProject.size(); i < 10; i++)
        iniFile.remove(QString("Project") + QString::number(i));
    for(int i = 0; i < m_recentProject.size(); i++)
        iniFile.setValue(QString("Project") + QString::number(i), m_recentProject[i]);
    iniFile.endGroup();
}

float Options::volume() const
{
    return m_volume;
}

void Options::setVolume(float volume, const QObject *sender)
{
    m_volume = qMin(qMax(volume, 0.0f), 1.0f);
    emit volumeChanged(volume, sender);
}

void Options::addRecentProject(const QString &projectFilePath)
{
    int index = m_recentProject.indexOf(projectFilePath);
    if(index != -1)
        m_recentProject.removeAt(index);

    if(m_recentProject.size() == 10)
        m_recentProject.removeAt(9);

    m_recentProject.push_front(projectFilePath);
}

void Options::removeRecentProject(const QString &projectFilePath)
{
    int index = m_recentProject.indexOf(projectFilePath);
    if(index != -1)
        m_recentProject.removeAt(index);
}

void Options::clearRecentProject()
{
    m_recentProject.clear();
}

bool Options::autoSave() const
{
    return m_autoSave;
}

void Options::setAutoSave(bool autoSave, const QObject *sender)
{
    m_autoSave = autoSave;
    emit autoSaveChanged(autoSave, sender);
}

bool Options::hideInfoLog() const
{
    return m_hideInfoLog;
}

void Options::setHideInfoLog(bool hideInfoLog, const QObject *sender)
{
    m_hideInfoLog = hideInfoLog;
    emit hideInfoLogChanged(hideInfoLog, sender);
}

const QString &Options::theme() const
{
    return m_theme;
}

void Options::setTheme(const QString &theme, const QObject *sender)
{
    m_theme = theme;
    emit themeChanged(theme, sender);
}

Language Options::language() const
{
    return m_language;
}

void Options::setLanguage(Language language, const QObject *sender)
{
    m_language = language;
    emit languageChanged(language, sender);
}

Options::~Options()
{

}

Options *options()
{
    static Options *uniqueOptionsManager = new Options;
    return uniqueOptionsManager;
}
