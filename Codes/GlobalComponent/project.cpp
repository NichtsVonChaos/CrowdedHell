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

bool Project::__loadFromTemporaryPath()
{
    QFile projectFile(m_temporaryPath + QString("/") + m_projectName + QString(".chproj"));
    if(!projectFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        emit message(Logger::Type::Error, "Projcet Manager", tr("Open selected project file failed, at : \"%1\".").arg(m_temporaryPath + QString("/") + m_projectName + QString(".chproj")));
        return false;
    }

    QTextStream textStream(&projectFile);
    QStringList datas = textStream.readAll().split("\n");
    projectFile.close();

    if(datas.size() < 5)
    {
        emit message(Logger::Type::Error, "Project", tr("Project file format abnormal."));
        return false;
    }

    m_author = datas[2];
    m_musicFile = datas[4];

    if(!QFile(m_temporaryPath + QString("/") + m_musicFile).exists())
    {
        emit message(Logger::Type::Error, "Project", tr("Cannot find the music file of project, at : \"%1\".").arg(m_temporaryPath + QString("/") + m_musicFile));
        return false;
    }

    emit message(Logger::Type::Info, "Project", tr("Project \"%1\" opened.").arg(m_projectName));
    emit message(Logger::Type::Info, "Project", tr("Project temporary path is \"%1\".").arg(m_temporaryPath));
    QApplication::processEvents();
    emit projectOpened(m_projectPath + QString("/") + m_projectName + QString(".chproj"));
    QApplication::processEvents();
    emit musicSelected(m_temporaryPath + QString("/") + m_musicFile);
    setChanged();

    return true;
}

void Project::__clear()
{
    m_projectName.clear();
    m_projectPath.clear();
    m_temporaryPath.clear();
    m_changed = false;
    m_uuid.clear();
    m_author.clear();
    m_musicFile.clear();
    m_createDate = QDateTime();
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

bool Project::execute(const QString &operation, const QStringList &options, const QString &target)
{
    const static QString tab = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
    const static QStringList validOptions =
    {
        "d", "discard-changes",
        "f", "force-close",
        "s", "save-before",
        "x", "save-after",
    };

    for(const auto &option : options)
        if(!validOptions.contains(option))
        {
            emit message(Logger::Type::Error, "Command", tr("Unknown option \"%1\" for \"%2\", use \"%2 ?\" or \"%2 help\" to learn more.").arg(option).arg("project"));
            return false;
        }

    if(operation.isEmpty())
        ;
    else if(operation == "?" || operation == "help")
    {
        const QString help = tr("Command Line Helper") + QString("<br />") +
                tab + tr("MODULE") + QString("<br />") +
                tab + tab + QString("project") + QString("<br />") +
                tab + QString("OPERATIONS") + QString("<br />") +
                tab + tab + QString("?, help : ") + tr("List all usable commands.") + QString("<br />") +
                tab + tab + QString("close : ") + tr("Close current project. It will raise a message box for asking whether save project, use --save-before for auto save.") + QString("<br />") +
                tab + tab + QString("open <path> : ") + tr("Open a project. It will automatically call \"project close\" before operation if a project is open.") + QString("<br />") +
                tab + QString("OPTIONS") + QString("<br />") +
                tab + tab + QString("-d, --discard-changes : ") + tr("Discard all changes, cover temporary path with origin project. Note its priority is higer then --save-before and --force-close.") + QString("<br />") +
                tab + tab + QString("-f, --force-close : ") + tr("Force close current project, without either saving changes or removing teporary path. Only usable for operation \"close\".") + QString("<br />") +
                tab + tab + QString("-s, --save-before : ") + tr("Save project before operation.") + QString("<br />") +
                tab + tab + QString("-x, --save-after : ") + tr("Save project after operation. Not usable for operation \"close\".") + QString("<br />");

        emit message(Logger::Type::Tip, "Command", help);
    }
    else if(operation == "open")
    {

    }
    else
    {
        emit message(Logger::Type::Error, "Command", tr("Invalid operation \"%1\" of \"%2\". Type \"%2 help\" or \"%2 ?\" to learn more.").arg(operation).arg("project"));
        return false;
    }

    return true;
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
        __clear();
        delete wizard;
        return;
    }

    m_uuid = QUuid::createUuid().toString().remove("{").remove("}").remove("-");
    m_temporaryPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + QString("/Crowded-Hell-") + m_uuid;

    QDir tmpDir(m_temporaryPath);
    if(tmpDir.exists())
        QtShell::rm("-rf", m_temporaryPath);

    QTextStream textStream(&projectFile);
    textStream << "This is just a project file for Crowded Hell." << endl;
    textStream << m_uuid << endl;
    textStream << wizard->getAuthor() << endl;
    textStream << wizard->getDate().toString(Qt::ISODate) << endl;
    textStream << QFileInfo(musicPath).fileName() << endl;

    projectFile.flush();
    projectFile.close();

    if(!QFile::copy(musicPath, projectPath + QString("/") + QFileInfo(musicPath).fileName()))
    {
        emit message(Logger::Type::Error, "Project", tr("Failed to copy music file from \"%1\" to \"%2\".").arg(musicPath).arg(projectPath + QString("/") + QFileInfo(musicPath).fileName()));
        __clear();
        delete wizard;
        return;
    }

    m_author = wizard->getAuthor();
    m_createDate = wizard->getDate();
    m_musicFile = QFileInfo(musicPath).fileName();

    if(!QtShell::cp("-a", m_projectPath, m_temporaryPath))
    {
        emit message(Logger::Type::Error, "Project", tr("Failed to copy project to temporary path.").arg(m_temporaryPath + QString("/") + m_musicFile));
        __clear();
        return;
    }

    emit message(Logger::Type::Info, "Project", tr("Project \"%1\" created.").arg(m_projectName));
    emit message(Logger::Type::Info, "Project", tr("Project temporary path is \"%1\".").arg(m_temporaryPath));
    QApplication::processEvents();
    emit projectOpened(m_projectPath + QString("/") + m_projectName + QString(".chproj"));
    QApplication::processEvents();
    emit musicSelected(m_temporaryPath + QString("/") + QFileInfo(musicPath).fileName());

    delete wizard;
};

void Project::openProject()
{
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
        emit message(Logger::Type::Error, "Projcet Manager", tr("Failed to open selected project file, at : \"%1\".").arg(projectFilePath));
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

    m_projectName = QFileInfo(projectFilePath).fileName().remove(QString(".chproj"));
    m_projectPath = QFileInfo(projectFilePath).absolutePath();
    m_uuid = datas[1];
    m_author = datas[2];
    m_createDate = QDateTime::fromString(datas[3], Qt::ISODate);
    m_musicFile = datas[4];

    m_temporaryPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + QString("/Crowded-Hell-") + m_uuid;
    if(QDir().exists(m_temporaryPath))
    {
        if(QMessageBox::question(nullptr, tr("Temporary path"), tr("Find temporary path of project already existed. It may be caused by program's crash without saving changes. Do you want to load project from temporary path to restore unsaved changes?")) == QMessageBox::Yes)
        {
            if(__loadFromTemporaryPath())
                return;
            else if(QMessageBox::question(nullptr, tr("Failure"), tr("Failed to load project from temporary path. Would you like to remove temporary path and open project? It will discard your unsaved changes.")) == QMessageBox::No)
            {
                __clear();
                return;
            }
            else
                QDir(m_temporaryPath).removeRecursively();
            m_author = datas[2];
            m_musicFile = datas[4];
        }
        else
            QDir(m_temporaryPath).removeRecursively();
    }

    if(!QtShell::cp("-a", m_projectPath, m_temporaryPath))
    {
        emit message(Logger::Type::Error, "Project", tr("Failed to make temporary path \"%1\".").arg(m_temporaryPath));
        __clear();
        return;
    }

    if(!QFile(m_temporaryPath + QString("/") + m_musicFile).exists())
    {
        emit message(Logger::Type::Error, "Project", tr("Cannot find the music file of project, at : \"%1\".").arg(m_temporaryPath + QString("/") + m_musicFile));
        __clear();
        return;
    }

    emit message(Logger::Type::Info, "Project", tr("Project \"%1\" opened.").arg(m_projectName));
    emit message(Logger::Type::Info, "Project", tr("Project temporary path is \"%1\".").arg(m_temporaryPath));
    QApplication::processEvents();
    emit projectOpened(m_projectPath + QString("/") + m_projectName + QString(".chproj"));
    QApplication::processEvents();
    emit musicSelected(m_temporaryPath + QString("/") + m_musicFile);
}

bool Project::saveChange()
{
    QtShell::rm("-rf", m_projectPath);
    if(!QtShell::cp("-a", m_temporaryPath, m_projectPath))
    {
        emit message(Logger::Type::Error, "Project", tr("Failed to save project. Please back up your changes from temporary path \"%1\".").arg(m_temporaryPath));
        return false;
    }

    m_changed = false;
    return true;
}

void Project::saveTo()
{
    QFileDialog *projectPathDialog = new QFileDialog;
    projectPathDialog->setAcceptMode(QFileDialog::AcceptOpen);
    projectPathDialog->setOption(QFileDialog::ShowDirsOnly, true);
    projectPathDialog->setFileMode(QFileDialog::Directory);
    projectPathDialog->setDirectory(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    projectPathDialog->setWindowTitle(tr("Select a directory to save"));

    if(projectPathDialog->exec() == QFileDialog::Accepted)
    {
        QString projectPath = projectPathDialog->selectedFiles()[0];
        if(!QtShell::cp("-a", m_temporaryPath, projectPath + QString("/") + m_projectName))
            emit message(Logger::Type::Error, "Project", tr("Failed to save project to \"%1\".").arg(projectPath + QString("/") + m_projectName));
    }

    delete projectPathDialog;
}

int Project::closeProject()
{
    if(!isValid())
        return 0;

    if(m_changed)
    {
        int shouldSave = QMessageBox::No;
        if(!options()->autoSave())
            shouldSave = QMessageBox::question(nullptr, tr("Confirm"), tr("Would you like to save your project before close it?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes);

        if(shouldSave == QMessageBox::Cancel)
            return 1;

        if(options()->autoSave() || shouldSave == QMessageBox::Yes)
            if(!saveChange())
                return 2;
    }

    emit musicResetRequire();
    QDir(m_temporaryPath).removeRecursively();

    emit projectClosed();
    QApplication::processEvents();
    emit message(Logger::Type::Info, "Project", tr("Remove project temporary path at \"%1\".").arg(m_temporaryPath));
    emit message(Logger::Type::Info, "Project", tr("Project \"%1\" closed.").arg(m_projectName));

    __clear();
    return true;
}

void Project::reselectMusic()
{
    QString musicFilePath = QFileDialog::getOpenFileName(nullptr, tr("Select Music File"), qApp->applicationDirPath(), tr("Music File(*.mp3 *.wav)"));
    reselectMusic(musicFilePath);
}

void Project::reselectMusic(const QString &musicPath)
{
    emit musicResetRequire();
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
        emit message(Logger::Type::Error, "Project", tr("Failed to copy music file from \"%1\" to \"%2\".").arg(musicPath).arg(m_temporaryPath + QString("/") + QFileInfo(musicPath).fileName()));
        return;
    }

    QFile projectFileRead(m_temporaryPath + QString("/") + m_projectName + QString(".chproj"));
    if(!projectFileRead.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        emit message(Logger::Type::Error, "Projcet Manager", tr("Failed to open project file on temporary path, at : \"%1\".").arg(m_temporaryPath + QString("/") + m_projectName + QString(".chproj")));
        return;
    }

    QTextStream textStreamRead(&projectFileRead);
    QStringList datas = textStreamRead.readAll().split("\n");
    projectFileRead.close();

    if(datas.size() < 5)
    {
        emit message(Logger::Type::Error, "Project", tr("Project file format abnormal."));
        return;
    }

    datas[4] = QFileInfo(musicPath).fileName();

    QFile projectFileWrite(m_temporaryPath + QString("/") + m_projectName + QString(".chproj"));
    if(!projectFileWrite.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
        emit message(Logger::Type::Error, "Projcet Manager", tr("Failed to open project file on temporary path, at : \"%1\".").arg(m_temporaryPath + QString("/") + m_projectName + QString(".chproj")));
        return;
    }

    QTextStream textStreamWrite(&projectFileWrite);
    for(int i = 0; i < 5; ++i)
        textStreamWrite << datas[i] << endl;

    projectFileWrite.flush();
    projectFileWrite.close();

    if(!QFile(m_temporaryPath + QString("/") + m_musicFile).remove())
        emit message(Logger::Type::Error, "Warning", tr("Failed to remove old music file \"%1\".").arg(m_musicFile));

    m_musicFile = QFileInfo(musicPath).fileName();

    QApplication::processEvents();
    emit musicSelected(m_temporaryPath + QString("/") + m_musicFile);
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
