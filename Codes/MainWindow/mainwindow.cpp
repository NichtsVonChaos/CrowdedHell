#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), m_sender(this)
{
    ui->setupUi(this);

    ui->actionReselectMusic->setEnabled(false);

    m_actionClearRecent = new QAction(tr("Clear all records"));
    m_actionNoRecord = new QAction(tr("(No record)"));
    m_actionNoRecord->setEnabled(false);

    project()->initialze();

    m_translationFiles[Language::EN] = ":/trans/en.qm";
    m_translationFiles[Language::ZH_CN] = ":/trans/zh_cn.qm";
    m_translationFiles[Language::ZH_TW] = ":/trans/zh_tw.qm";
    m_translationFiles[Language::JP] = ":/trans/jp.qm";
    m_buttonLanguageOptions[Language::EN] = new QAction(tr("English"), ui->menuLanguage);
    m_buttonLanguageOptions[Language::ZH_CN] = new QAction(tr("Simplified Chinese"), ui->menuLanguage);
    m_buttonLanguageOptions[Language::ZH_TW] = new QAction(tr("Traditional Chinese"), ui->menuLanguage);
    m_buttonLanguageOptions[Language::JP] = new QAction(tr("Japanese"), ui->menuLanguage);
    for(auto i : m_buttonLanguageOptions.values())
        i->setCheckable(true);
    if(m_translator.load(m_translationFiles[Language::EN]))
        qApp->installTranslator(&m_translator);

    ui->menuLanguage->addActions(m_buttonLanguageOptions.values());

    connect(options(), &Options::hideInfoLogChanged, ui->actionHideAllInfoTypeMessage, &QAction::setChecked, Qt::UniqueConnection);
    connect(options(), &Options::autoSaveChanged, ui->actionAlwaysSaveProjectBeforeClose, &QAction::setChecked, Qt::UniqueConnection);
    connect(options(), &Options::languageChanged, this, &MainWindow::setLanguage, Qt::UniqueConnection);
    connect(this, &MainWindow::message, logger(), &Logger::message);

    // debug usage
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::playButtonChecked, Qt::UniqueConnection);

    options()->readOptions();
    m_musicPlayer = new MusicPlayer(this);
    ui->musicSlider->initialze(m_musicPlayer);

    connect(this, &MainWindow::musicPaused, m_musicPlayer, &MusicPlayer::setPaused, Qt::UniqueConnection);
    connect(m_musicPlayer, &MusicPlayer::paused, this, &MainWindow::pauseMusic, Qt::UniqueConnection);
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
    connect(ui->menuLanguage, &QMenu::triggered, this, &MainWindow::languageButtonClicked, Qt::UniqueConnection);
    connect(project(), &Project::projectOpened, options(), &Options::addRecentProject, Qt::UniqueConnection);
    connect(project(), &Project::projectOpened, this, &MainWindow::projectOpened, Qt::UniqueConnection);
    connect(project(), &Project::projectClosed, this, &MainWindow::projectClosed, Qt::UniqueConnection);
    connect(project(), &Project::projectClosed, m_musicPlayer, &MusicPlayer::reset, Qt::UniqueConnection);
    connect(project(), &Project::musicSelected, m_musicPlayer, &MusicPlayer::setMusicFile, Qt::UniqueConnection);

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

void MainWindow::changeEvent(QEvent *ev)
{

    QMainWindow::changeEvent(ev);
    switch (ev->type())
    {
        case QEvent::LanguageChange:
        {
            if(ui)
                ui->retranslateUi(this);
            m_buttonLanguageOptions[Language::EN]->setText(tr("English"));
            m_buttonLanguageOptions[Language::ZH_CN]->setText(tr("Simplified Chinese"));
            m_buttonLanguageOptions[Language::ZH_TW]->setText(tr("Traditional Chinese"));
            m_buttonLanguageOptions[Language::JP]->setText(tr("Japanese"));
            m_actionClearRecent->setText(tr("Clear all records"));
            m_actionNoRecord->setText(tr("(No record)"));

            if(project()->projectName().isEmpty())
                setWindowTitle("Crowded Hell");
            else
                setWindowTitle(project()->projectName() + QString(" -- Crowded Hell"));
        }
        break;

        default:
        break;
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
    else for(int i = 0; i < options()->recentProject().size(); ++i)
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
    setWindowTitle(project()->projectName() + QString(" -- Crowded Hell"));
}

void MainWindow::projectClosed()
{
    ui->actionReselectMusic->setEnabled(false);
    setWindowTitle("Crowded Hell");
}

void MainWindow::playButtonChecked(bool checked)
{
    emit musicPaused(checked);
}

void MainWindow::languageButtonClicked(QAction *action)
{
    Language language;
    for(auto i = m_buttonLanguageOptions.constBegin(); i != m_buttonLanguageOptions.constEnd(); ++i)
    {
        if(i.value() == action)
        {
            language = i.key();
            i.value()->setChecked(true);
            setLanguage(language);
        }
        else
            i.value()->setChecked(false);
    }
}

void MainWindow::setLanguage(Language language, const QObject *sender)
{
    if(sender == this)
        return;
    if(sender == nullptr)
        sender = this;

    for(auto i = m_buttonLanguageOptions.constBegin(); i != m_buttonLanguageOptions.constEnd(); ++i)
    {
        if(i.key() == language)
            i.value()->setChecked(true);
        else
            i.value()->setChecked(false);
    }

    if(m_translator.load(m_translationFiles[language]))
    {
        qApp->removeTranslator(&m_translator);
        qApp->installTranslator(&m_translator);
    }
    else
    {
        message(Logger::Type::Error, "Main Window", tr("Failed to load translation file \"%1\".").arg(m_translationFiles[language]));
        return;
    }

    options()->setLanguage(language, sender);
}

void MainWindow::on_actionHideAllInfoTypeMessage_triggered(bool checked)
{
    options()->setHideInfoLog(checked, this);
}

void MainWindow::on_actionAlwaysSaveProjectBeforeClose_triggered(bool checked)
{
    options()->setAutoSave(checked, this);
}

void MainWindow::on_actionExportLogToFile_triggered()
{
    m_musicPlayer->pause();
    QString file = QFileDialog::getSaveFileName(nullptr, "Export Log", ".");
    if(!file.isEmpty())
        logger()->logToFile(file);
}
