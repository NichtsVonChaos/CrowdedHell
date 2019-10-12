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

    m_musicPlayer = new MusicPlayer(this);
    ui->musicSlider->initialze(m_musicPlayer);

    m_actionClearRecent = new QAction("Clear all records");
    m_actionNoRecord = new QAction("(No record)");
    m_actionNoRecord->setEnabled(false);

    connect(project(), &Project::projectOpened, options(), &OptionsManager::addRecentProject, Qt::UniqueConnection);
    connect(project(), &Project::projectOpened, this, &MainWindow::projectOpened, Qt::UniqueConnection);
    connect(project(), &Project::projectClosed, this, &MainWindow::projectClosed, Qt::UniqueConnection);
    connect(project(), &Project::projectClosed, m_musicPlayer, &MusicPlayer::reset, Qt::UniqueConnection);
    connect(project(), &Project::musicSelected, m_musicPlayer, &MusicPlayer::setMusicFile, Qt::UniqueConnection);

    connect(this, &MainWindow::musicPaused, m_musicPlayer, &MusicPlayer::setPaused, Qt::UniqueConnection);

    connect(m_musicPlayer, &MusicPlayer::paused, this, &MainWindow::pauseMusic, Qt::UniqueConnection);

    // debug usage
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::playButtonChecked, Qt::UniqueConnection);

    connect(m_actionClearRecent, &QAction::triggered, options(), &OptionsManager::clearRecentProject, Qt::UniqueConnection);
    connect(m_actionClearRecent, &QAction::triggered, this, &MainWindow::refreshRecentProject, Qt::UniqueConnection);
    connect(ui->actionNewProject, &QAction::triggered, m_musicPlayer, &MusicPlayer::pause, Qt::UniqueConnection);
    connect(ui->actionNewProject, &QAction::triggered, project(), &Project::newProject, Qt::UniqueConnection);
    connect(ui->actionOpenProject, &QAction::triggered, m_musicPlayer, &MusicPlayer::pause, Qt::UniqueConnection);
    connect(ui->actionOpenProject, &QAction::triggered, project(), static_cast<void(Project::*)()>(&Project::openProject), Qt::UniqueConnection);
    connect(ui->actionCloseProject, &QAction::triggered, project(), &Project::closeProject, Qt::UniqueConnection);
    connect(ui->menuRecentProject, &QMenu::triggered, this, &MainWindow::openRecentProject, Qt::UniqueConnection);
    connect(ui->actionReselectMusic, &QAction::triggered, m_musicPlayer, &MusicPlayer::pause, Qt::UniqueConnection);
    connect(ui->actionReselectMusic, &QAction::triggered, project(), static_cast<void(Project::*)()>(&Project::reselectMusic), Qt::UniqueConnection);


    refreshRecentProject();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_musicPlayer;
}

void MainWindow::closeEvent(QCloseEvent *ev)
{
    options()->writeOptions();
    ev->accept();
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
