#include "project.h"

Project *project()
{
    static Project *uniqueProject = new Project;
    return uniqueProject;
}

Project::~Project()
{

}

Project::Project(QObject *parent) :
    QObject(parent),
    m_changed(false)
{

}

QString Project::uuid() const
{
    return m_uuid;
}

void Project::initialze()
{
    connect(this, &Project::message, logger(), &Logger::message, Qt::UniqueConnection);
    m_initilized = true;
}

bool Project::isValid() const
{
    return !m_projectName.isEmpty() && m_initilized;
};

void Project::newProject()
{
    closeProject();

    CreateProjectWizard *wizard = new CreateProjectWizard;
    if(wizard->exec() != QWizard::Accepted)
    {
        delete wizard;
        return;
    }

    QString projectPath = wizard->getPath() + QString("/") + wizard->getProjectName();

    QString musicPath = wizard->getMusicPath();
    if(!QFile(musicPath).exists())
    {
        emit message(Logger::Type::Error, "Project", tr("Selected music file doesn't exist, at : \"%1\".").arg(musicPath));
        delete wizard;
        return;
    }

    if(!QDir().exists(projectPath))
    {
        if(!QDir().mkpath(projectPath))
        {
            emit message(Logger::Type::Error, "Project", tr("Make selected directory failed, at : \"%1\".").arg(projectPath));
            delete wizard;
            return;
        }
    }
    else
    {
        emit message(Logger::Type::Error, "Project", tr("Project is already exists, at : \"%1\".").arg(projectPath));
        delete wizard;
        return;
    }

    if(!QDir().mkpath(projectPath + QString("/Sprites")))
    {
        emit message(Logger::Type::Error, "Project", tr("Make selected directory failed, at : \"%1\".").arg(projectPath) + QString("/sprites"));
        delete wizard;
        return;
    }
    if(!QDir().mkpath(projectPath + QString("/Sounds")))
    {
        emit message(Logger::Type::Error, "Project", tr("Make selected directory failed, at : \"%1\".").arg(projectPath) + QString("/sounds"));
        delete wizard;
        return;
    }
    if(!QDir().mkpath(projectPath + QString("/Backgrounds")))
    {
        emit message(Logger::Type::Error, "Project", tr("Make selected directory failed, at : \"%1\".").arg(projectPath) + QString("/backgrounds"));
        delete wizard;
        return;
    }
    if(!QDir().mkpath(projectPath + QString("/Paths")))
    {
        emit message(Logger::Type::Error, "Project", tr("Make selected directory failed, at : \"%1\".").arg(projectPath) + QString("/paths"));
        delete wizard;
        return;
    }

    m_projectPath = projectPath;
    m_projectName = wizard->getProjectName();
    QFile projectFile(m_projectPath + QString("/") + m_projectName + QString(".chproj"));
    if(!projectFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
        emit message(Logger::Type::Error, "Project", tr("Create project file failed, at : \"%1\".").arg(m_projectPath + QString("/") + m_projectName + QString(".chproj")));
        m_projectPath.clear();
        m_projectName.clear();
        delete wizard;
        return;
    }

    m_uuid = Utility::generateUUID();
    m_temporaryPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + QString("/Crowded-Hell-") + m_uuid;
    QDir().mkdir(m_temporaryPath);

    QTextStream textStream(&projectFile);
    textStream << "This is just a project file for Crowded Hell." << endl;
    textStream << m_uuid << endl;
    textStream << wizard->getAuthor() << endl;
    textStream << wizard->getDate().toString() << endl;
    textStream << QFileInfo(musicPath).fileName() << endl;

    projectFile.flush();
    projectFile.close();

    if(!QFile::copy(musicPath, projectPath + QString("/") + QFileInfo(musicPath).fileName()))
    {
        emit message(Logger::Type::Error, "Project", tr("Cannot copy music file from \"%1\" to \"%2\".").arg(musicPath).arg(projectPath + QString("/") + QFileInfo(musicPath).fileName()));
        m_projectPath.clear();
        m_projectName.clear();
        delete wizard;
        return;
    }

    m_author = wizard->getAuthor();
    m_createDate = wizard->getDate();
    m_musicFile = QFileInfo(musicPath).fileName();

    emit message(Logger::Type::Info, "Project", tr("Project \"%1\" created.").arg(m_projectName));
    emit message(Logger::Type::Info, "Project", tr("Project temporary path is \"%1\".").arg(m_temporaryPath));
    QApplication::processEvents();
    emit projectOpened(m_projectPath + QString("/") + m_projectName + QString(".chproj"));
    QApplication::processEvents();
    emit musicSelected(projectPath + QString("/") + QFileInfo(musicPath).fileName());

    delete wizard;
};

void Project::openProject()
{
    closeProject();

    QString projectFilePath = QFileDialog::getOpenFileName(nullptr, tr("Select a project"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation), "Crowded Hell Project(*.chproj)");
    if(projectFilePath.isEmpty())
        return;

    openProject(projectFilePath);
}

void Project::openProject(const QString &projectFilePath)
{
    closeProject();

    QFile projectFile(projectFilePath);
    if(!projectFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        emit message(Logger::Type::Error, "Projcet Manager", tr("Open selected project file failed, at : \"%1\".").arg(projectFilePath));
        return;
    }

    QTextStream textStream(&projectFile);
    QStringList datas = textStream.readAll().split("\n");
    projectFile.close();

    if(datas.size() < 5)
    {
        emit message(Logger::Type::Error, "Project", tr("Project file format abnormal."));
        return;
    }

    QString projectPath = QFileInfo(projectFilePath).absolutePath();

    closeProject();

    m_projectName = QFileInfo(projectFilePath).fileName().remove(QString(".chproj"));
    m_projectPath = QFileInfo(projectFilePath).absolutePath();
    m_uuid = datas[1];
    m_author = datas[2];
    m_createDate = QDateTime::fromString(datas[3]);
    m_musicFile = datas[4];

    m_temporaryPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + QString("/Crowded-Hell-") + m_uuid;
    QDir().mkdir(m_temporaryPath);

    if(!QFile(projectPath + QString("/") + m_musicFile).exists())
    {
        emit message(Logger::Type::Error, "Project", tr("Cannot find the music file of project, at : \"%1\".").arg(m_projectPath + QString("/") + m_musicFile));
        return;
    }

    emit message(Logger::Type::Info, "Project", tr("Project \"%1\" opened.").arg(m_projectName));
    emit message(Logger::Type::Info, "Project", tr("Project temporary path is \"%1\".").arg(m_temporaryPath));
    QApplication::processEvents();
    emit projectOpened(m_projectPath + QString("/") + m_projectName + QString(".chproj"));
    QApplication::processEvents();
    emit musicSelected(projectPath + QString("/") + m_musicFile);
}

void Project::saveChange()
{
    if(!QFile::copy(m_musicFile, m_projectPath + QString("/") + QFileInfo(m_musicFile).fileName()))
    {
        emit message(Logger::Type::Error, "Project", tr("Cannot copy music file from \"%1\" to \"%2\".").arg(m_musicFile).arg(m_projectPath + QString("/") + QFileInfo(m_musicFile).fileName()));
        return;
    }

    QFile projectFile(m_projectPath + QString("/") + m_projectName + QString(".chproj"));
    if(!projectFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        emit message(Logger::Type::Error, "Project", tr("Project file doesn't exist, at : \"%1\".").arg(m_projectPath + QString("/") + m_projectName + QString(".chproj")));
        return;
    }

    QTextStream textStream(&projectFile);
    QStringList datas = textStream.readAll().split("\n");
    projectFile.close();

    if(datas.size() < 5)
    {
        emit message(Logger::Type::Error, "Project", tr("Project file format is abnormal."));
        return;
    }

    if(QFile(m_projectPath + "/" + datas[4]).exists())
        if(!QFile(m_projectPath + "/" + datas[4]).remove())
            emit message(Logger::Type::Warning, "Project", tr("Failed remove music file \"%1\".").arg(m_projectPath + "/" + datas[3]));

    datas[4] = QFileInfo(m_musicFile).fileName();

    if(!projectFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        emit message(Logger::Type::Error, "Project", tr("Project file doesn't exist, at : \"%1\".").arg(m_projectPath + QString("/") + m_projectName + QString(".chproj")));
        return;
    }

    foreach(auto i, datas)
        textStream << i << endl;
    projectFile.flush();

}

void Project::saveTo(const QString &path)
{

};

bool Project::closeProject()
{
    if(!isValid())
        return true;

    if(m_changed)
    {
        int shouldSave = QMessageBox::No;
        if(!options()->autoSave())
            shouldSave = QMessageBox::question(nullptr, tr("Confirm"), tr("Would you like to save your project before close it?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes);

        if(shouldSave == QMessageBox::Cancel)
            return false;

        if(options()->autoSave() || shouldSave == QMessageBox::Yes)
            saveChange();
    }

    QDir(m_temporaryPath).removeRecursively();

    emit projectClosed();
    QApplication::processEvents();
    emit message(Logger::Type::Info, "Project", tr("Remove project temporary path at \"%1\".").arg(m_temporaryPath));
    emit message(Logger::Type::Info, "Project", tr("Project \"%1\" closed.").arg(m_projectName));

    m_projectName.clear();
    m_projectPath.clear();
    m_temporaryPath.clear();
    m_author.clear();
    m_musicFile.clear();

    m_changed = false;
    return true;
}

void Project::reselectMusic()
{
    QString musicFilePath = QFileDialog::getOpenFileName(nullptr, tr("Select Music File"), qApp->applicationDirPath(), tr("Music File(*.mp3 *.wav)"));
    reselectMusic(musicFilePath);
}

void Project::reselectMusic(const QString &musicPath)
{
    if(!isValid())
    {
        emit message(Logger::Type::Error, "Project", tr("No project is open. Please create or open a project before."));
        return;
    }

    if(!QFile(musicPath).exists())
    {
        emit message(Logger::Type::Error, "Project", tr("Selected music file doesn't exist, at : \"%1\".").arg(musicPath));
        return;
    }

    if(!QFile::copy(musicPath, m_temporaryPath + QString("/") + QFileInfo(musicPath).fileName()))
    {
        emit message(Logger::Type::Error, "Project", tr("Cannot copy music file from \"%1\" to \"%2\".").arg(musicPath).arg(m_projectPath + QString("/") + QFileInfo(musicPath).fileName()));
        return;
    }

    m_musicFile = m_temporaryPath + QString("/") + QFileInfo(musicPath).fileName();

    QApplication::processEvents();
    emit musicSelected(m_musicFile);

    setChanged();

    return;
}

void Project::setChanged()
{
    m_changed = true;
};

const QString &Project::temporaryPath() const
{
    return m_temporaryPath;
}

const QString &Project::musicFile() const
{
    return m_musicFile;
}

const QDateTime &Project::createDate() const
{
    return m_createDate;
}

const QString &Project::author() const
{
    return m_author;
}

const QString &Project::projectPath() const
{
    return m_projectPath;
}

const QString &Project::projectName() const
{
    return m_projectName;
};
