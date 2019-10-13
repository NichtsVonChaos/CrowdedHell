#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->actionReselectMusic->setEnabled(false);

    options()->readOptions();
    project()->initialze();

    ui->actionAlwaysSaveProjectBeforeClose->setChecked(options()->autoSave());
    ui->actionHideAllInfoTypeMessage->setChecked(options()->hideInfoLog());

    m_musicPlayer = new MusicPlayer(this);
    ui->musicSlider->initialze(m_musicPlayer);

    m_actionClearRecent = new QAction("Clear all records");
    m_actionNoRecord = new QAction("(No record)");
    m_actionNoRecord->setEnabled(false);

    connect(project(), &Project::projectOpened, options(), &Options::addRecentProject, Qt::UniqueConnection);
    connect(project(), &Project::projectOpened, this, &MainWindow::projectOpened, Qt::UniqueConnection);
    connect(project(), &Project::projectClosed, this, &MainWindow::projectClosed, Qt::UniqueConnection);
    connect(project(), &Project::projectClosed, m_musicPlayer, &MusicPlayer::reset, Qt::UniqueConnection);
    connect(project(), &Project::musicSelected, m_musicPlayer, &MusicPlayer::setMusicFile, Qt::UniqueConnection);

    connect(options(), &Options::hideInfoLogChanged, ui->actionHideAllInfoTypeMessage, &QAction::setChecked, Qt::UniqueConnection);
    connect(options(), &Options::autoSaveChanged, ui->actionAlwaysSaveProjectBeforeClose, &QAction::setChecked, Qt::UniqueConnection);

    connect(this, &MainWindow::message, logger(), &Logger::message);
    connect(this, &MainWindow::musicPaused, m_musicPlayer, &MusicPlayer::setPaused, Qt::UniqueConnection);

    connect(m_musicPlayer, &MusicPlayer::paused, this, &MainWindow::pauseMusic, Qt::UniqueConnection);

    // debug usage
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::playButtonChecked, Qt::UniqueConnection);

    connect(m_actionClearRecent, &QAction::triggered, options(), &Options::clearRecentProject, Qt::UniqueConnection);
    connect(m_actionClearRecent, &QAction::triggered, this, &MainWindow::refreshRecentProject, Qt::UniqueConnection);
    connect(ui->actionNewProject, &QAction::triggered, m_musicPlayer, &MusicPlayer::pause, Qt::UniqueConnection);
    connect(ui->actionNewProject, &QAction::triggered, project(), &Project::newProject, Qt::UniqueConnection);
    connect(ui->actionOpenProject, &QAction::triggered, m_musicPlayer, &MusicPlayer::pause, Qt::UniqueConnection);
    connect(ui->actionOpenProject, &QAction::triggered, project(), static_cast<void(Project::*)()>(&Project::openProject), Qt::UniqueConnection);
    connect(ui->actionCloseProject, &QAction::triggered, project(), &Project::closeProject, Qt::UniqueConnection);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->menuRecentProject, &QMenu::triggered, this, &MainWindow::openRecentProject, Qt::UniqueConnection);
    connect(ui->actionReselectMusic, &QAction::triggered, m_musicPlayer, &MusicPlayer::pause, Qt::UniqueConnection);
    connect(ui->actionReselectMusic, &QAction::triggered, project(), static_cast<void(Project::*)()>(&Project::reselectMusic), Qt::UniqueConnection);
    connect(ui->actionClearLogger, &QAction::triggered, logger(), &Logger::clear);

    refreshRecentProject();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_musicPlayer;
}

void MainWindow::closeEvent(QCloseEvent *ev)
{
    if(!project()->closeProject())
        ev->ignore();
    else
    {
        options()->writeOptions();
        ev->accept();
    }
}

void MainWindow::pauseMusic(bool paused, const QObject *sender)
{
    if(sender == ui->pushButton)
        return;

    if(sender == nullptr)
        sender = ui->pushButton;

    ui->pushButton->setChecked(paused);
}

void MainWindow::refreshRecentProject()
{
    ui->menuRecentProject->clear();
    if(options()->recentProject().size() == 0)
        ui->menuRecentProject->addAction(m_actionNoRecord);
    else for(int i = 0; i < options()->recentProject().size(); i++)
        ui->menuRecentProject->addAction(options()->recentProject()[i]);
    ui->menuRecentProject->addSeparator();
    ui->menuRecentProject->addAction(m_actionClearRecent);
}

void MainWindow::openRecentProject(QAction *action)
{
    if(!QFile(action->text()).exists())
    {
        emit message(Logger::Type::Warning, "Main Window", tr("Project file \"%1\" does not exists, remove it from recent project list.").arg(action->text()));
        options()->removeRecentProject(action->text());
        refreshRecentProject();
    }
    else
        project()->openProject(action->text());
}

void MainWindow::projectOpened(const QString &projectFilePath)
{
    ui->actionReselectMusic->setEnabled(true);
    refreshRecentProject();
}

void MainWindow::projectClosed()
{
    ui->actionReselectMusic->setEnabled(false);
}

void MainWindow::playButtonChecked(bool checked)
{
    emit musicPaused(checked);
}

void MainWindow::on_actionHideAllInfoTypeMessage_triggered(bool checked)
{
    options()->setHideInfoLog(checked, ui->actionHideAllInfoTypeMessage);
}

void MainWindow::on_actionAlwaysSaveProjectBeforeClose_triggered(bool checked)
{
    options()->setAutoSave(checked, ui->actionAlwaysSaveProjectBeforeClose);
}

void MainWindow::on_actionExportLogToFile_triggered()
{
    m_musicPlayer->pause();
    QString file = QFileDialog::getSaveFileName(nullptr, "Export Log", ".");
    if(!file.isEmpty())
        logger()->logToFile(file);
}
