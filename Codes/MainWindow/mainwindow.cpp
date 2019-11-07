#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), m_sender(this)
{
    ui->setupUi(this);
    setCorner(Qt::Corner::TopLeftCorner, Qt::DockWidgetArea::LeftDockWidgetArea);
    setCorner(Qt::Corner::TopRightCorner, Qt::DockWidgetArea::RightDockWidgetArea);
    setCorner(Qt::Corner::BottomLeftCorner, Qt::DockWidgetArea::LeftDockWidgetArea);
    setCorner(Qt::Corner::BottomRightCorner, Qt::DockWidgetArea::RightDockWidgetArea);

    ui->actionReselectMusic->setEnabled(false);
    ui->lineEditTime->setEnabled(false);
    ui->lineEditFrame->setEnabled(false);

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
    connect(options(), &Options::volumeChanged, ui->sliderVolume, &VolumeSlider::setVolume, Qt::UniqueConnection);
    connect(options(), &Options::volumeChanged, this, &MainWindow::updateVolumeLable, Qt::UniqueConnection);
    connect(this, &MainWindow::message, logger(), &Logger::message, Qt::UniqueConnection);

    options()->readOptions();
    m_musicPlayer = new MusicPlayer(this);
    ui->musicSlider->initialze(m_musicPlayer);

    connect(this, &MainWindow::musicPaused, m_musicPlayer, &MusicPlayer::setPaused, Qt::UniqueConnection);
    connect(this, &MainWindow::muted, m_musicPlayer, &MusicPlayer::setMuted, Qt::UniqueConnection);
    connect(m_musicPlayer, &MusicPlayer::paused, this, &MainWindow::pauseMusic, Qt::UniqueConnection);
    connect(m_musicPlayer, &MusicPlayer::muted, this, &MainWindow::setMuted, Qt::UniqueConnection);
    connect(m_musicPlayer, &MusicPlayer::positionChanged, this, &MainWindow::setMusicPosition, Qt::UniqueConnection);
    connect(m_actionClearRecent, &QAction::triggered, options(), &Options::clearRecentProject, Qt::UniqueConnection);
    connect(m_actionClearRecent, &QAction::triggered, this, &MainWindow::refreshRecentProject, Qt::UniqueConnection);
    connect(ui->actionNewProject, &QAction::triggered, m_musicPlayer, &MusicPlayer::pause, Qt::UniqueConnection);
    connect(ui->actionNewProject, &QAction::triggered, project(), &Project::newProject, Qt::UniqueConnection);
    connect(ui->actionOpenProject, &QAction::triggered, m_musicPlayer, &MusicPlayer::pause, Qt::UniqueConnection);
    connect(ui->actionOpenProject, &QAction::triggered, project(), static_cast<void(Project::*)()>(&Project::openProject), Qt::UniqueConnection);
    connect(ui->actionSave, &QAction::triggered, project(), &Project::saveChange, Qt::UniqueConnection);
    connect(ui->actionSaveTo, &QAction::triggered, project(), &Project::saveTo, Qt::UniqueConnection);
    connect(ui->actionSaveAndClose, &QAction::triggered, project(), &Project::saveChange, Qt::UniqueConnection);
    connect(ui->actionSaveAndClose, &QAction::triggered, project(), &Project::closeProject, Qt::UniqueConnection);
    connect(ui->actionCloseProject, &QAction::triggered, project(), &Project::closeProject, Qt::UniqueConnection);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->menuRecentProject, &QMenu::triggered, this, &MainWindow::openRecentProject, Qt::UniqueConnection);
    connect(ui->actionReselectMusic, &QAction::triggered, m_musicPlayer, &MusicPlayer::pause, Qt::UniqueConnection);
    connect(ui->actionReselectMusic, &QAction::triggered, project(), static_cast<void(Project::*)()>(&Project::reselectMusic), Qt::UniqueConnection);
    connect(ui->actionClearLogger, &QAction::triggered, logger(), &Logger::clear);
    connect(ui->menuLanguage, &QMenu::triggered, this, &MainWindow::languageButtonClicked, Qt::UniqueConnection);
    connect(ui->actionShowLoggerWidget, &QAction::triggered, ui->dockWidgetLogger, &QDockWidget::setVisible, Qt::UniqueConnection);
    connect(ui->dockWidgetLogger, &QDockWidget::visibilityChanged, ui->actionShowLoggerWidget, &QAction::setChecked, Qt::UniqueConnection);
    connect(ui->actionShowResourcesWidget, &QAction::triggered, ui->dockWidgetResources, &QDockWidget::setVisible, Qt::UniqueConnection);
    connect(ui->dockWidgetResources, &QDockWidget::visibilityChanged, ui->actionShowResourcesWidget, &QAction::setChecked, Qt::UniqueConnection);
    connect(ui->actionShowActionsWidget, &QAction::triggered, ui->dockWidgetActions, &QDockWidget::setVisible, Qt::UniqueConnection);
    connect(ui->dockWidgetActions, &QDockWidget::visibilityChanged, ui->actionShowActionsWidget, &QAction::setChecked, Qt::UniqueConnection);
    connect(ui->actionShowAttributesWidget, &QAction::triggered, ui->dockWidgetAttributes, &QDockWidget::setVisible, Qt::UniqueConnection);
    connect(ui->dockWidgetAttributes, &QDockWidget::visibilityChanged, ui->actionShowAttributesWidget, &QAction::setChecked, Qt::UniqueConnection);
    connect(project(), &Project::projectOpened, options(), &Options::addRecentProject, Qt::UniqueConnection);
    connect(project(), &Project::projectOpened, this, &MainWindow::projectOpened, Qt::UniqueConnection);
    connect(project(), &Project::projectClosed, this, &MainWindow::projectClosed, Qt::UniqueConnection);
    connect(project(), &Project::musicSelected, m_musicPlayer, &MusicPlayer::setMusicFile, Qt::UniqueConnection);
    connect(project(), &Project::musicSelected, this, &MainWindow::changeMusic, Qt::UniqueConnection);
    connect(project(), &Project::musicResetRequire, m_musicPlayer, &MusicPlayer::reset, Qt::UniqueConnection);

    refreshRecentProject();
    if(!options()->mainWindowState().isEmpty())
        restoreState(options()->mainWindowState());

    if(options()->maxWindowStart())
        setWindowState(Qt::WindowMaximized);
    else if(!options()->mainWindowGeometry().isEmpty())
        restoreGeometry(options()->mainWindowGeometry());
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_musicPlayer;
}

void MainWindow::showEvent(QShowEvent *ev)
{
    QMainWindow::showEvent(ev);
}

void MainWindow::closeEvent(QCloseEvent *ev)
{
    switch(project()->closeProject())
    {
        case 0:
            options()->setMainWindowState(saveState());
            options()->setMainWindowGeometry(saveGeometry());
            options()->setMaxWindowStart(isMaximized());
            options()->writeOptions();
            QMainWindow::closeEvent(ev);
            ev->accept();
        break;

        case 1:
            ev->ignore();
        break;

        case 2:
            if(QMessageBox::question(nullptr, tr("Failure"), tr("Error occurred when save project. Force exit?"), QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::No), QMessageBox::No) == QMessageBox::No)
                ev->ignore();
            else
                ev->accept();
        break;
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
            updateVolumeLable(options()->volume());

            if(project()->projectName().isEmpty())
            {
                setWindowTitle("Crowded Hell");
                ui->labelMusic->setText(tr("No Music"));
            }
            else
            {
                setWindowTitle(project()->projectName() + QString(" -- Crowded Hell"));
                ui->labelMusic->setText(tr("Music : ") + QFileInfo(project()->musicFile()).completeBaseName());
            }
        }
        break;

        default:
        break;
    }

    ev->accept();
}

void MainWindow::pauseMusic(bool paused, const QObject *sender)
{
    if(sender == this)
        return;

    if(sender == nullptr)
        sender = this;

    if(sender != ui->pushButtonPause)
        ui->pushButtonPause->setChecked(paused);
    ui->lineEditTime->setEnabled(paused);
    ui->lineEditFrame->setEnabled(paused);
    emit musicPaused(paused, sender);
}

void MainWindow::setMusicPosition(unsigned int milliseconds, const QObject *sender)
{
    ui->lineEditTime->setText(__generateStringFromTime(milliseconds));
    ui->lineEditFrame->setText(QString::number(m_musicPlayer->position() / 50));
}

void MainWindow::setMuted(bool muted, const QObject *sender)
{
    if(sender == this)
        return;

    if(sender == nullptr)
        sender = this;

    ui->pushButtonMute->setChecked(muted);
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
    ui->labelMusic->setText(tr("Music : ") + QFileInfo(project()->musicFile()).completeBaseName());
}

void MainWindow::projectClosed()
{
    ui->actionReselectMusic->setEnabled(false);
    setWindowTitle("Crowded Hell");
    ui->labelMusic->setText(tr("No Music"));
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

void MainWindow::updateVolumeLable(float volume)
{
    ui->labelVolume->setText(QString::number(int(volume * 100.f)));
}

void MainWindow::changeMusic(const QString &musicFile)
{
    ui->labelMusic->setText(tr("Music : ") + QFileInfo(musicFile).completeBaseName());
    ui->labelTotalTime->setText(QString("/") + __generateStringFromTime(m_musicPlayer->length()));
    ui->labelTotalFrame->setText(QString("/") + QString::number(m_musicPlayer->length() / 50));
    ui->lineEditTime->setText(__generateStringFromTime(0));
    ui->lineEditFrame->setText(QString::number(0));
    ui->lineEditTime->setEnabled(true);
    ui->lineEditFrame->setEnabled(true);
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

void MainWindow::on_pushButtonPause_clicked(bool checked)
{
    emit musicPaused(checked, ui->pushButtonPause);
}

void MainWindow::on_pushButtonMute_clicked(bool checked)
{
    emit muted(checked, ui->pushButtonMute);
}

void MainWindow::on_pushButtonNext1_released()
{
    m_musicPlayer->forward();
}

void MainWindow::on_pushButtonNext5_released()
{
    m_musicPlayer->forward(5);
}

void MainWindow::on_pushButtonNext50_released()
{
    m_musicPlayer->forward(50);
}

void MainWindow::on_pushButtonPrev1_released()
{
    m_musicPlayer->back();
}

void MainWindow::on_pushButtonPrev5_released()
{
    m_musicPlayer->back(5);
}

void MainWindow::on_pushButtonPrev50_released()
{
    m_musicPlayer->back(50);
}

bool MainWindow::__readTimeFromString(const QString &time, unsigned int &milliseconds)
{
    bool ok = false;

    QStringList timeSplitted = time.split(QRegExp("[:]"), QString::SkipEmptyParts);
    if(timeSplitted.size() != 2)
    {
        emit message(Logger::Type::Error, "Main Window", tr("Failed to parse \"%1\". Use \"m:s[.ms]\".").arg(time));
        return false;
    }

    milliseconds = timeSplitted[0].toUInt(&ok) * 60000;
    if(!ok)
    {
        emit message(Logger::Type::Error, "Main Window", tr("Value of minutes \"%1\" on \"%2\" is invalid. Use \"m:s[.ms]\".").arg(timeSplitted[0]).arg(time));
        return false;
    }

    double seconds = timeSplitted[1].toDouble(&ok);
    if(!ok)
    {
        emit message(Logger::Type::Error, "Main Window", tr("Value of seconds \"%1\" on \"%2\" is invalid. Use \"m:s[.ms]\".").arg(timeSplitted[1]).arg(time));
        return false;
    }
    if(seconds >= 60)
    {
        emit message(Logger::Type::Error, "Main Window", tr("Value of seconds \"%1\" on \"%2\" is bigger then 60.").arg(timeSplitted[1]).arg(time));
        return false;
    }
    milliseconds += unsigned(seconds * 1000);

    return true;
}

QString MainWindow::__generateStringFromTime(unsigned int milliseconds)
{
    QString time;
    time = QString::number(milliseconds / 60000);
    time += ":";
    time += QString::number(double(milliseconds % 60000) / 1000.0);
    return time;
}

void MainWindow::on_lineEditTime_editingFinished()
{
    unsigned int milliseconds = 0;
    if(!__readTimeFromString(ui->lineEditTime->text(), milliseconds))
    {
        ui->lineEditTime->setText(__generateStringFromTime(m_musicPlayer->position()));
        return;
    }
    m_musicPlayer->setPosition(milliseconds);
}

void MainWindow::on_lineEditFrame_editingFinished()
{
    bool ok = false;
    unsigned int millisecond = ui->lineEditFrame->text().toUInt(&ok) * 50;
    if(!ok)
    {
        ui->lineEditFrame->setText(QString::number(m_musicPlayer->position() / 50));
        emit message(Logger::Type::Error, "Main Window", tr("Value of frame \"%1\" is invalid.").arg(ui->lineEditFrame->text()));
        return;
    }
    m_musicPlayer->setPosition(millisecond);
}

void MainWindow::on_comboBoxSpeed_currentIndexChanged(int index)
{
    const float speed[] = {0.2f, 0.5f, 1.f, 1.5f, 2.0f, 5.0f};
    m_musicPlayer->setSpeed(speed[index], ui->comboBoxSpeed);
}

void MainWindow::on_pushButtonFmod_clicked()
{
    if(QDesktopServices::openUrl(QUrl("https://www.fmod.com/")))
        message(Logger::Type::Info, "Main Window", tr("Open fmod official website."));
    else
        message(Logger::Type::Warning, "Main Window", tr("Cannot open fmod official website \"https://www.fmod.com/\"."));
}

void MainWindow::on_actionOpenProjectPath_triggered()
{
    if(!project()->projectPath().isEmpty())
        if(QDesktopServices::openUrl(QUrl(QString("file://") + project()->projectPath())))
            message(Logger::Type::Info, "Main Window", tr("Open project path."));
        else
            message(Logger::Type::Warning, "Main Window", tr("Cannot open project path \"%1\".").arg(project()->projectPath()));
    else
        message(Logger::Type::Warning, "Main Window", tr("No project is open. Please create or open a project before."));
}

void MainWindow::on_actionOpenTemporaryPath_triggered()
{
    if(!project()->projectPath().isEmpty())
        if(QDesktopServices::openUrl(QUrl(QString("file://") + project()->temporaryPath())))
            message(Logger::Type::Info, "Main Window", tr("Open project temporary path."));
        else
            message(Logger::Type::Warning, "Main Window", tr("Cannot open project temporary path \"%1\".").arg(project()->temporaryPath()));
    else
        message(Logger::Type::Warning, "Main Window", tr("No project is open. Please create or open a project before."));
}
