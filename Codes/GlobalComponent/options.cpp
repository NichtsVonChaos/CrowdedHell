#include "options.h"

Options::Options(QObject *parent) : QObject(parent)
{

}

QByteArray Options::mainWindowGeometry() const
{
    return m_mainWindowGeometry;
}

void Options::setMainWindowGeometry(const QByteArray &mainWindowGeometry)
{
    m_mainWindowGeometry = mainWindowGeometry;
}

QByteArray Options::mainWindowState() const
{
    return m_mainWindowState;
}

void Options::setMainWindowState(const QByteArray &mainWindowState)
{
    m_mainWindowState = mainWindowState;
}

bool Options::maxWindowStart() const
{
    return m_maxWindowStart;
}

void Options::setMaxWindowStart(bool maxWindowStart)
{
    m_maxWindowStart = maxWindowStart;
}

const QStringList &Options::recentProject() const
{
    return m_recentProject;
}

void Options::readOptions()
{
    QSettings iniFile(QApplication::applicationDirPath() + "/config.ini", QSettings::IniFormat);

    iniFile.beginGroup("Main");
    setLanguage(Language(iniFile.value("Language", 0).toInt()));
    setTheme(iniFile.value("Theme", "Deep Blue").toString());
    setHideInfoLog(iniFile.value("HideInfoLog", false).toBool());
    setAutoSave(iniFile.value("AutoSave", false).toBool());
    iniFile.endGroup();

    iniFile.beginGroup("MainWindow");
    setMaxWindowStart(iniFile.value("maxWindowStart", true).toBool());
    setMainWindowState(iniFile.value("State", QByteArray("\0\0\0\xff\0\0\0\0\xfd\0\0\0\x3"
                                                         "\0\0\0\0\0\0\0\xda\0\0\x3\x90\xfc"
                                                         "\x2\0\0\0\x1\xfb\0\0\0&\0\x64\0o"
                                                         "\0\x63\0k\0W\0i\0\x64\0g\0\x65\0t"
                                                         "\0R\0\x65\0s\0o\0u\0r\0\x63\0\x65"
                                                         "\0s\x1\0\0\0\x18\0\0\x3\x90\0\0\0"
                                                         "\x64\0\xff\xff\xff\0\0\0\x1\0\0\0"
                                                         "\xea\0\0\x3\x90\xfc\x2\0\0\0\x2\xfb"
                                                         "\0\0\0(\0\x64\0o\0\x63\0k\0W\0i\0"
                                                         "\x64\0g\0\x65\0t\0\x41\0t\0t\0r\0i"
                                                         "\0\x62\0u\0t\0\x65\0s\x1\0\0\0\x18"
                                                         "\0\0\x1\xc5\0\0\0q\0\xff\xff\xff"
                                                         "\xfb\0\0\0\"\0\x64\0o\0\x63\0k\0W"
                                                         "\0i\0\x64\0g\0\x65\0t\0\x41\0\x63"
                                                         "\0t\0i\0o\0n\0s\x1\0\0\x1\xe3\0\0"
                                                         "\x1\xc5\0\0\0q\0\xff\xff\xff\0\0\0"
                                                         "\x3\0\0\x3\x30\0\0\0\xab\xfc\x1\0"
                                                         "\0\0\x1\xfb\0\0\0 \0\x64\0o\0\x63"
                                                         "\0k\0W\0i\0\x64\0g\0\x65\0t\0L\0o"
                                                         "\0g\0g\0\x65\0r\x1\0\0\0\xe0\0\0"
                                                         "\x3\x30\0\0\0\x64\0\a\xff\xff\0\0"
                                                         "\x3\x30\0\0\x2\xdf\0\0\0\x1\0\0\0"
                                                         "\x2\0\0\0\x1\0\0\0\x2\xfc\0\0\0\0", 328)).toByteArray());
    setMainWindowGeometry(iniFile.value("Geometry", QByteArray()).toByteArray());
    iniFile.endGroup();

    iniFile.beginGroup("Audio");
    setVolume(iniFile.value("Volume", 0.6f).toFloat());
    iniFile.endGroup();

    iniFile.beginGroup("Recent");
    QString projectPath;
    for(int i = 0; i < 10; ++i)
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

    iniFile.beginGroup("MainWindow");
    iniFile.setValue("maxWindowStart", m_maxWindowStart);
    iniFile.setValue("State", m_mainWindowState);
    iniFile.setValue("Geometry", m_mainWindowGeometry);
    iniFile.endGroup();

    iniFile.beginGroup("Audio");
    iniFile.setValue("Volume", double(m_volume));
    iniFile.endGroup();

    iniFile.beginGroup("Recent");
    for(int i = m_recentProject.size(); i < 10; ++i)
        iniFile.remove(QString("Project") + QString::number(i));
    for(int i = 0; i < m_recentProject.size(); ++i)
        iniFile.setValue(QString("Project") + QString::number(i), m_recentProject[i]);
    iniFile.endGroup();
}

float Options::volume() const
{
    return m_volume;
}

void Options::setVolume(float volume, const QObject *sender)
{
    if(sender == this)
        return;
    if(sender == nullptr)
        sender = this;
    m_volume = limit(volume, 0.f, 1.f);
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
    if(sender == this)
        return;
    if(sender == nullptr)
        sender = this;
    m_autoSave = autoSave;
    emit autoSaveChanged(autoSave, sender);
}

bool Options::hideInfoLog() const
{
    return m_hideInfoLog;
}

void Options::setHideInfoLog(bool hideInfoLog, const QObject *sender)
{
    if(sender == this)
        return;
    if(sender == nullptr)
        sender = this;
    m_hideInfoLog = hideInfoLog;
    emit hideInfoLogChanged(hideInfoLog, sender);
}

const QString &Options::theme() const
{
    return m_theme;
}

void Options::setTheme(const QString &theme, const QObject *sender)
{
    if(sender == this)
        return;
    if(sender == nullptr)
        sender = this;
    m_theme = theme;
    emit themeChanged(theme, sender);
}

Language Options::language() const
{
    return m_language;
}

void Options::setLanguage(Language language, const QObject *sender)
{
    if(sender == this)
        return;
    if(sender == nullptr)
        sender = this;
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
