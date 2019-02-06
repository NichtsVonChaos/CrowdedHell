#include "crowdedhellgui.h"
#include "ui_crowdedhellgui.h"

CrowdedHellGUI::CrowdedHellGUI(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::CrowdedHellGUI),
	m_currentMusicName(tr("Unknown - Unknown"))
{
	ui->setupUi(this);

	// Connect signals and slots.
	connect(ui->menuTheme, SIGNAL(hovered(QAction *)), this, SLOT(on_menuTheme_hovered(QAction *)));

	// Set up languages.
	m_translators.insert(Language::ZH_CN, new QTranslator(this));
	m_translators[Language::ZH_CN]->load(":/translations/Trans_zh_cn.qm");
	m_translators.insert(Language::ZH_TW, new QTranslator(this));
	m_translators[Language::ZH_TW]->load(":/translations/Trans_zh_tw.qm");
	m_translators.insert(Language::JP, new QTranslator(this));
	m_translators[Language::JP]->load(":/translations/Trans_jp.qm");

	// Initialize openGL widget
	m_displayWidget = new AvoidanceDisplayWidget(this);
	m_displayWidget->setFixedSize(QSize(800, 608));
	m_displayWidget->hide();

	__readSettings();

	// Initialize message box.
	ui->textEditMessageBox->document()->setMaximumBlockCount(100);
	ui->textEditMessageBox->setHtml("<font color=purple>>>></font> ");

	// Initialize project manager.
	m_projectManager = new ProjectManager(this, ui->treeViewResources);
	m_projectManager->setAlwaysSave(ui->actionAlwaysSave->isChecked());
	connect(ui->actionAlwaysSave, SIGNAL(toggled(bool)), m_projectManager, SLOT(setAlwaysSave(bool)));

	// Initialize audio player.
	m_audioPlayer = new AudioPlayer(ui->audioSlider, this);

	// Set volume bar.
	ui->volumeSliderBar->setMinimum(0);
	ui->volumeSliderBar->setMaximum(100);
	ui->volumeSliderBar->setValue(int(m_audioPlayer->getVolume() * 100));
	connect(ui->volumeSliderBar, SIGNAL(valueChanged(int)), this, SLOT(volumeBarValueChanged(int)));

	if(!ui->actionHideInfo->isChecked())
		sendMessage(MessageType::Tips, "Main Window", tr("You can hide all \"Info\" messages by selecting <font color=red>Window => Hide \"Info\" Messages</font>."));
};

CrowdedHellGUI::~CrowdedHellGUI()
{
	delete ui;
};

void CrowdedHellGUI::sendMessage(MessageType type, QString module, QString message)
{
	QString messageHtml = "<p>";
	switch(type)
	{
		case MessageType::Info:
		{
			if(ui->actionHideInfo->isChecked())
				return;
			messageHtml += QString("<font color=black><b>[ <font color=green>") + tr("Info") + QString("</font> ]</b></font> ");
		}
		break;

		case MessageType::Error:
		{
			messageHtml += QString("<font color=black><b>[ <font color=red>") + tr("Error") + QString("</font> ]</b></font> ");
		}
		break;

		case MessageType::Warning:
		{
			messageHtml += QString("<font color=black><b>[ <font color=orange>") + tr("Warning") + QString("</font> ]</b></font> ");
		}
		break;

		case MessageType::Tips:
		{
			messageHtml += QString("<font color=black><b>[ <font color=blue>") + tr("Tips") + QString("</font> ]</b></font> ");
		}
		break;
	}

	messageHtml += QString(tr("In module %1 : ")).arg(QString("<font color=purple><u>") + module + QString("</u></font>"));
	messageHtml += QString("<font color=black>") + message + QString("</font></p><font color=purple>>>> </font>");

	qDebug() << messageHtml << endl;

	ui->textEditMessageBox->moveCursor(QTextCursor::End);
	ui->textEditMessageBox->textCursor().insertHtml(messageHtml);
};

void CrowdedHellGUI::changeEvent(QEvent *event)
{

};

QString CrowdedHellGUI::currentTheme() const
{
	return m_currentTheme;
};

Language CrowdedHellGUI::currentLanguage() const
{
	return m_currentLanguage;
};

void CrowdedHellGUI::musicPositionChanged(unsigned int miliseconds)
{
	ui->lineEditFrames->setText(QString::number(miliseconds / 20));
	QTime time(0, 0);
	time = time.addMSecs(int(miliseconds));
	ui->lineEditTime->setText(QString::number(time.hour() * 60 + time.minute()) + QString(":") + QString::number(time.second()) + QString(".") + (QString::number(time.msec()).size() == 1 ? ("00" + QString::number(time.msec())) :
																																															 ((QString::number(time.msec()).size() == 2 ? ("0" + QString::number(time.msec())) :
																																																										  QString::number(time.msec())))));
};

void CrowdedHellGUI::musicPlayedOrPaused(bool play)
{
	if(play)
	{
		if(!ui->lineEditFrames->isReadOnly())
			ui->lineEditFrames->setReadOnly(true);
		if(!ui->lineEditTime->isReadOnly())
			ui->lineEditTime->setReadOnly(true);
		if(!ui->buttonPause->isChecked())
			ui->buttonPause->setChecked(true);
	}
	else
	{
		if(ui->lineEditFrames->isReadOnly())
			ui->lineEditFrames->setReadOnly(false);
		if(ui->lineEditTime->isReadOnly())
			ui->lineEditTime->setReadOnly(false);
		if(ui->buttonPause->isChecked())
			ui->buttonPause->setChecked(false);
	}
};

void CrowdedHellGUI::on_actionSimplifiedChinese_triggered()
{
	m_currentLanguage = Language::ZH_CN;
	__updateUi();
	emit languageChanged(m_currentLanguage);
};

void CrowdedHellGUI::on_actionEnglish_triggered()
{
	m_currentLanguage = Language::EN;
	__updateUi();
	emit languageChanged(m_currentLanguage);
};

void CrowdedHellGUI::on_actionTraditionalChinese_triggered()
{
	m_currentLanguage = Language::ZH_TW;
	__updateUi();
	emit languageChanged(m_currentLanguage);
};

void CrowdedHellGUI::on_actionJapanese_triggered()
{
	m_currentLanguage = Language::JP;
	__updateUi();
	emit languageChanged(m_currentLanguage);
};

void CrowdedHellGUI::__updateUi()
{
	QString mainWindowQssFile = "Themes/%1/QSS/%2 - Main Window.qss";
	if(m_themes.contains(m_currentTheme) && m_currentTheme != "Deep Blue")
		mainWindowQssFile = mainWindowQssFile.arg(m_currentTheme);
	else
	{
		mainWindowQssFile = ":/Themes/Deep Blue/QSS/%1 - Main Window.qss";
		m_currentTheme = "Deep Blue";
		__updateSettings();
	}

	switch (m_currentLanguage)
	{
		case Language::EN:
		{
			foreach(auto i, m_translators)
				qApp->removeTranslator(i);
			ui->actionEnglish->setChecked(true);
			ui->actionSimplifiedChinese->setChecked(false);
			ui->actionTraditionalChinese->setChecked(false);
			ui->actionJapanese->setChecked(false);

			mainWindowQssFile = mainWindowQssFile.arg("EN");
			QFile qssFile(mainWindowQssFile);
			if(qssFile.open(QIODevice::ReadOnly))
			{
				setStyleSheet(qssFile.readAll());
				qssFile.close();
			}
			else
			{
				sendMessage(MessageType::Error, "Main Window", tr("Cannot open the qss file \"%1\" for the theme \"%2\", therefore use default theme \"Deep Blue\".").arg(mainWindowQssFile).arg(m_currentTheme));
				__turnToDefalutTheme();
				return;
			}

			sendMessage(MessageType::Info, "Main Window", tr("Change language to English."));
		}
		break;

		case Language::ZH_CN:
		{
			qApp->installTranslator(m_translators[Language::ZH_CN]);
			ui->actionEnglish->setChecked(false);
			ui->actionSimplifiedChinese->setChecked(true);
			ui->actionTraditionalChinese->setChecked(false);
			ui->actionJapanese->setChecked(false);

			mainWindowQssFile = mainWindowQssFile.arg("ZH_CN");
			QFile qssFile(mainWindowQssFile);
			if(qssFile.open(QIODevice::ReadOnly))
			{
				setStyleSheet(qssFile.readAll());
				qssFile.close();
			}
			else
			{
				sendMessage(MessageType::Error, "Main Window", tr("Cannot open the qss file \"%1\" for the theme \"%2\", therefore use default theme \"Deep Blue\".").arg(mainWindowQssFile).arg(m_currentTheme));
				__turnToDefalutTheme();
				return;
			}
			sendMessage(MessageType::Info, "Main Window", tr("Change language to Simpified Chinese."));
		}
		break;

		case Language::ZH_TW:
		{
			qApp->installTranslator(m_translators[Language::ZH_TW]);
			ui->actionEnglish->setChecked(false);
			ui->actionSimplifiedChinese->setChecked(false);
			ui->actionTraditionalChinese->setChecked(true);
			ui->actionJapanese->setChecked(false);

			mainWindowQssFile = mainWindowQssFile.arg("ZH_TW");
			QFile qssFile(mainWindowQssFile);
			if(qssFile.open(QIODevice::ReadOnly))
			{
				setStyleSheet(qssFile.readAll());
				qssFile.close();
			}
			else
			{
				sendMessage(MessageType::Error, "Main Window", tr("Cannot open the qss file \"%1\" for the theme \"%2\", therefore use default theme \"Deep Blue\".").arg(mainWindowQssFile).arg(m_currentTheme));
				__turnToDefalutTheme();
				return;
			}
			sendMessage(MessageType::Info, "Main Window", tr("Change language to Traditional Chinese."));
		}
		break;

		case Language::JP:
		{
			qApp->installTranslator(m_translators[Language::JP]);
			ui->actionEnglish->setChecked(false);
			ui->actionSimplifiedChinese->setChecked(false);
			ui->actionTraditionalChinese->setChecked(false);
			ui->actionJapanese->setChecked(true);

			mainWindowQssFile = mainWindowQssFile.arg("JP");
			QFile qssFile(mainWindowQssFile);
			if(qssFile.open(QIODevice::ReadOnly))
			{
				setStyleSheet(qssFile.readAll());
				qssFile.close();
			}
			else
			{
				sendMessage(MessageType::Error, "Main Window", tr("Cannot open the qss file \"%1\" for the theme \"%2\", therefore use default theme \"Deep Blue\".").arg(mainWindowQssFile).arg(m_currentTheme));
				__turnToDefalutTheme();
				return;
			}
			sendMessage(MessageType::Info, "Main Window", tr("Change language to Japanese."));
		}
		break;
	}

	ui->retranslateUi(this);
	ui->labelMusicName->setText(tr("Background Music : ") + m_currentMusicName);

	__updateSettings();
};

void CrowdedHellGUI::__readSettings()
{
	QSettings iniFile("./settings.ini", QSettings::IniFormat);

	iniFile.beginGroup("Main");
	m_currentLanguage = Language(iniFile.value("Language", 0).toInt());
	ui->actionHideInfo->setChecked(iniFile.value("HideInfo", false).toBool());
	ui->actionAlwaysSave->setChecked(iniFile.value("AlwaysSave", false).toBool());

	// Read themes
	m_themes.clear();
	m_currentTheme = iniFile.value("Theme", "Deep Blue").toString();
	QFileInfo themesPath(qApp->applicationDirPath() + QString("/Themes"));
	if(themesPath.exists() && themesPath.isDir())
	{
		QDir dir(qApp->applicationDirPath() + QString("/Themes"));
		QFileInfoList fileInfo(dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs));
		foreach(auto i, fileInfo)
			if(i.isDir())
				m_themes << i.fileName();

		if(m_themes.isEmpty())
		{
			QAction *action = new QAction("Deep Blue", ui->menuTheme);
			action->setCheckable(true);
			action->setChecked(true);
			ui->menuTheme->addAction(action);
		}
		else
		{
			bool hasDeepBlue = false, hasCurrentTheme = false;
			foreach(auto i, m_themes)
			{
				if(i == "Deep Blue")
					hasDeepBlue = true;
				QAction *action = new QAction(i, ui->menuTheme);
				action->setCheckable(true);
				if(i == m_currentTheme)
				{
					action->setChecked(true);
					hasCurrentTheme = true;
				}
				else
					action->setChecked(false);
				ui->menuTheme->addAction(action);
			}

			if(!hasDeepBlue && hasCurrentTheme)
			{
				QAction *action = new QAction("Deep Blue", ui->menuTheme);
				action->setCheckable(true);
				action->setChecked(false);
				ui->menuTheme->addAction(action);
			}
			else if(hasDeepBlue && !hasCurrentTheme)
			{
				m_currentTheme = "Deep Blue";
				QList<QAction *> actions = ui->menuTheme->actions();
				foreach(auto i, actions)
					if(i->text() == "Deep Blue")
						i->setChecked(true);
			}
			else if(!hasDeepBlue && !hasCurrentTheme)
			{
				m_currentTheme = "Deep Blue";
				QAction *action = new QAction("Deep Blue", ui->menuTheme);
				action->setCheckable(true);
				action->setChecked(true);
				ui->menuTheme->addAction(action);
			}
		}
	}

	__updateUi();
	emit languageChanged(m_currentLanguage);
	iniFile.endGroup();
};

void CrowdedHellGUI::__updateSettings()
{
	QSettings iniFile("./settings.ini", QSettings::IniFormat);

	iniFile.beginGroup("Main");
	iniFile.setValue("Language", int(m_currentLanguage));
	iniFile.setValue("Theme", m_currentTheme);
	iniFile.setValue("HideInfo", ui->actionHideInfo->isChecked());
	iniFile.setValue("AlwaysSave", ui->actionAlwaysSave->isChecked());
	iniFile.endGroup();
};

void CrowdedHellGUI::__turnToDefalutTheme()
{
	QList<QAction *> themes = ui->menuTheme->actions();
	foreach(auto i, themes)
		if(i->text() == "Deep Blue")
			i->setChecked(true);
		else
			i->setChecked(false);
	m_currentTheme = "Deep Blue";
	__updateUi();
};

void CrowdedHellGUI::on_actionAddSoundEffect_triggered()
{

};

void CrowdedHellGUI::on_actionReselectMusic_triggered()
{
	if(!m_projectManager->isValid())
	{
		emit sendMessage(MessageType::Error, "Project Manager", tr("No project is open. Please create or open a project before."));
		return;
	}

	m_audioPlayer->playOrPause(false);
	QString musicFilePath = QFileDialog::getOpenFileName(this, tr("Select Music File"), qApp->applicationDirPath(), tr("Music File(*.mp3 *.wav)"));

	if(!musicFilePath.isEmpty())
		m_projectManager->reselectMusic(musicFilePath);
};

void CrowdedHellGUI::on_buttonPause_toggled(bool checked)
{
	if(!m_projectManager->isValid() && checked)
	{
		emit sendMessage(MessageType::Error, "Project Manager", tr("No project is open. Please create or open a project before."));
		ui->buttonPause->setChecked(false);
		return;
	}

	m_audioPlayer->playOrPause(checked);
};

void CrowdedHellGUI::musicInvalid()
{
	if(ui->buttonPause->isChecked())
		ui->buttonPause->setChecked(false);
};

void CrowdedHellGUI::updateMusicLength(unsigned int miliseconds)
{
	ui->labelTotalFrames->setText(QString("/") + QString::number(miliseconds / 20));
	QTime time(0, 0);
	time = time.addMSecs(int(miliseconds));
	ui->labelTotalTime->setText(QString("/") + QString::number(time.hour() * 60 + time.minute()) + QString(":") + QString::number(time.second()) + QString(".") +
								(QString::number(time.msec()).size() == 1 ? ("00" + QString::number(time.msec())) :
																			((QString::number(time.msec()).size() == 2 ? ("0" + QString::number(time.msec())) :
																														 QString::number(time.msec())))));
};

void CrowdedHellGUI::on_buttonNextFrame_clicked()
{
	if(!m_projectManager->isValid())
	{
		emit sendMessage(MessageType::Error, "Project Manager", tr("No project is open. Please create or open a project before."));
		return;
	}

	m_audioPlayer->forward(1u);
};

void CrowdedHellGUI::on_buttonPrevFrame_clicked()
{
	if(!m_projectManager->isValid())
	{
		emit sendMessage(MessageType::Error, "Project Manager", tr("No project is open. Please create or open a project before."));
		return;
	}

	m_audioPlayer->back(1u);
};

void CrowdedHellGUI::on_buttonNext10Frames_clicked()
{
	if(!m_projectManager->isValid())
	{
		emit sendMessage(MessageType::Error, "Project Manager", tr("No project is open. Please create or open a project before."));
		return;
	}

	m_audioPlayer->forward(10u);
};

void CrowdedHellGUI::on_buttonPrev10Frames_clicked()
{
	if(!m_projectManager->isValid())
	{
		emit sendMessage(MessageType::Error, "Project Manager", tr("No project is open. Please create or open a project before."));
		return;
	}

	m_audioPlayer->back(10u);
};

void CrowdedHellGUI::on_buttonNext50Frames_clicked()
{
	if(!m_projectManager->isValid())
	{
		emit sendMessage(MessageType::Error, "Project Manager", tr("No project is open. Please create or open a project before."));
		return;
	}

	m_audioPlayer->forward(50u);
};

void CrowdedHellGUI::on_buttonPrev50Frames_clicked()
{
	if(!m_projectManager->isValid())
	{
		emit sendMessage(MessageType::Error, "Project Manager", tr("No project is open. Please create or open a project before."));
		return;
	}

	m_audioPlayer->back(50u);
};

void CrowdedHellGUI::on_lineEditFrames_editingFinished()
{
	if(!m_projectManager->isValid())
	{
		emit sendMessage(MessageType::Error, "Project Manager", tr("No project is open. Please create or open a project before."));
		musicPositionChanged(0);
		return;
	}

	bool isConvertOk;
	unsigned int frames = ui->lineEditFrames->text().toUInt(&isConvertOk);
	if(!isConvertOk)
	{
		sendMessage(MessageType::Warning, "Main Window", tr("%1 is not a valid number.").arg(QString("\"") + ui->lineEditFrames->text() + QString("\"")));
		musicPositionChanged(m_audioPlayer->getPosition());
		return;
	}

	m_audioPlayer->changePosition(frames * 20);
};

void CrowdedHellGUI::on_menuTheme_hovered(QAction *action)
{
	QList<QAction *> themes = ui->menuTheme->actions();
	foreach(auto i, themes)
		if(i == action)
			i->setChecked(true);
		else
			i->setChecked(false);
	m_currentTheme = action->text();
	__updateUi();
};

void CrowdedHellGUI::on_actionNewProject_triggered()
{
	m_projectManager->newProject();
}

void CrowdedHellGUI::on_comboBoxSpeed_currentIndexChanged(int index)
{
	switch(index)
	{
		case 0:
			m_audioPlayer->changeSpeed(0.1f);
		break;

		case 1:
			m_audioPlayer->changeSpeed(0.2f);
		break;

		case 2:
			m_audioPlayer->changeSpeed(0.5f);
		break;

		case 3:
			m_audioPlayer->changeSpeed(1.0f);
		break;

		case 4:
			m_audioPlayer->changeSpeed(1.5f);
		break;

		case 5:
			m_audioPlayer->changeSpeed(2.0f);
		break;

		case 6:
			m_audioPlayer->changeSpeed(5.0f);
		break;

		default:
			m_audioPlayer->changeSpeed(1.0f);
	}
};

void CrowdedHellGUI::on_actionHideInfo_changed()
{
	__updateSettings();
};

void CrowdedHellGUI::volumeBarValueChanged(int value)
{
	m_audioPlayer->changeVolume(float(value / 100.0));
};

void CrowdedHellGUI::changeMusic(QString musicPath)
{
	m_audioPlayer->changeMusic(musicPath);
	QString fileName = QFileInfo(musicPath).fileName();
	m_currentMusicName = fileName.remove(fileName.lastIndexOf(QChar('.')), 4);
	ui->labelMusicName->setText(tr("Background Music : ") + m_currentMusicName);
};

void CrowdedHellGUI::projectClosed()
{
	m_audioPlayer->reset();
	ui->labelMusicName->setText(tr("Background Music : Unknown - Unknown"));
};

void CrowdedHellGUI::on_buttonMute_toggled(bool checked)
{
	m_audioPlayer->mute(checked);
};

void CrowdedHellGUI::on_buttonToZero_pressed()
{
	if(!m_projectManager->isValid())
	{
		emit sendMessage(MessageType::Error, "Project Manager", tr("No project is open. Please create or open a project before."));
		return;
	}

	m_audioPlayer->changePosition(0);
};

void CrowdedHellGUI::on_lineEditTime_editingFinished()
{
	if(!m_projectManager->isValid())
	{
		emit sendMessage(MessageType::Error, "Project Manager", tr("No project is open. Please create or open a project before."));
		musicPositionChanged(0);
		return;
	}

	QStringList splitForMinute = ui->lineEditTime->text().split(":");
	if(splitForMinute.size() != 2)
	{
		sendMessage(MessageType::Error, "Main Window", tr("\"%1\" is not a valid time. Plz enter valid time such as \"12:5.6\"").arg(ui->lineEditTime->text()));
		musicPositionChanged(m_audioPlayer->getPosition());
		return;
	}

	bool isConvertOk;
	int minutes = splitForMinute[0].toInt(&isConvertOk);
	if(!isConvertOk || minutes < 0)
	{
		sendMessage(MessageType::Warning, "Main Window", tr("%1 is not a valid number.").arg(QString("\"") + ui->lineEditFrames->text() + QString("\"")));
		musicPositionChanged(m_audioPlayer->getPosition());
		return;
	}

	QStringList splitForSecond = splitForMinute[1].split(".");
	if(splitForSecond.size() != 2)
	{
		sendMessage(MessageType::Error, "Main Window", tr("\"%1\" is not a valid time. Plz enter valid time such as \"12:5.600\"").arg(ui->lineEditTime->text()));
		musicPositionChanged(m_audioPlayer->getPosition());
		return;
	}

	int seconds = splitForSecond[0].toInt(&isConvertOk);
	if(!isConvertOk || seconds < 0 || seconds >= 60)
	{
		sendMessage(MessageType::Warning, "Main Window", tr("%1 is not a valid number.").arg(QString("\"") + ui->lineEditFrames->text() + QString("\"")));
		musicPositionChanged(m_audioPlayer->getPosition());
		return;
	}

	int miliseconds = splitForSecond[1].toInt(&isConvertOk);
	if(!isConvertOk || miliseconds < 0 || miliseconds >= 1000)
	{
		sendMessage(MessageType::Warning, "Main Window", tr("%1 is not a valid number.").arg(QString("\"") + ui->lineEditFrames->text() + QString("\"")));
		musicPositionChanged(m_audioPlayer->getPosition());
		return;
	}

	m_audioPlayer->changePosition(unsigned((minutes * 60 + seconds) * 1000 + miliseconds));
};

void CrowdedHellGUI::on_actionAlwaysSave_changed()
{
	__updateSettings();
};

void CrowdedHellGUI::on_actionOpenProject_triggered()
{
	m_projectManager->openProject();
};

void CrowdedHellGUI::on_actionClearMessages_triggered()
{
	ui->textEditMessageBox->clear();
	ui->textEditMessageBox->setHtml("<font color=purple>>>> </font>");
}
