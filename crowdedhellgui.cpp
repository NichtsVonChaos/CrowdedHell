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

	// Initialize message box.
	ui->textEditMessageBox->document()->setMaximumBlockCount(200);
	ui->textEditMessageBox->setHtml("<font color=purple>>>></font> ");

	__readSettings();

	// Initialize audio player.
	m_audioPlayer = new AudioPlayer(ui->audioSlider, this);

	// Initialize project manager.
	m_projectManager = new ProjectManager(this, ui->treeViewResources);
};

CrowdedHellGUI::~CrowdedHellGUI()
{
	delete ui;
};

void CrowdedHellGUI::sendMessage(MessageType type, QString module, QString message)
{
	switch(type)
	{
		case MessageType::Info:
		{
			if(ui->menuHideInfo->isChecked())
				return;
			ui->textEditMessageBox->moveCursor(QTextCursor::End);
			ui->textEditMessageBox->insertHtml(QString("<font color=black><b>[ <font color=green>") + tr("Info") + QString("</font> ]</b></font> "));
		}
		break;

		case MessageType::Error:
		{
			ui->textEditMessageBox->moveCursor(QTextCursor::End);
			ui->textEditMessageBox->insertHtml(QString("<font color=black><b>[ <font color=red>") + tr("Error") + QString("</font> ]</b></font> "));
		}
		break;

		case MessageType::Warning:
		{
			ui->textEditMessageBox->moveCursor(QTextCursor::End);
			ui->textEditMessageBox->insertHtml(QString("<font color=black><b>[ <font color=orange>") + tr("Warning") + QString("</font> ]</b></font> "));
		}
		break;

		case MessageType::Tips:
		{
			ui->textEditMessageBox->moveCursor(QTextCursor::End);
			ui->textEditMessageBox->insertHtml(QString("<font color=black><b>[ <font color=blue>") + tr("Tips") + QString("</font> ]</b></font> "));
		}
		break;
	}

	ui->textEditMessageBox->moveCursor(QTextCursor::End);
	ui->textEditMessageBox->insertHtml(QString(tr("In module %1 : ")).arg(QString("<font color=purple><u>") + module + QString("</font></u>")));
	ui->textEditMessageBox->setFontUnderline(false);
	ui->textEditMessageBox->moveCursor(QTextCursor::End);
	ui->textEditMessageBox->insertHtml(QString("<font color=black>") + message + QString("</font><br />"));
	ui->textEditMessageBox->moveCursor(QTextCursor::End);
	ui->textEditMessageBox->insertHtml("<font color=purple>>>></font> ");
};

void CrowdedHellGUI::changeEvent(QEvent *event)
{

};

void CrowdedHellGUI::musicPositionChanged(unsigned int miliseconds)
{
	ui->lineEditFrames->setText(QString::number(miliseconds / 20));
	QTime time(0, 0);
	time = time.addMSecs(int(miliseconds));
	ui->lineEditTime->setText(QString::number(time.hour() * 60 + time.minute()) + QString(":") + QString::number(time.second()) + QString(".") + QString::number(time.msec() / 10));
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
				sendMessage(MessageType::Error, "MainWindow", tr("Cannot open the qss file \"%1\" for the theme \"%2\", therefore use default theme \"Deep Blue\".").arg(mainWindowQssFile).arg(m_currentTheme));
				__turnToDefalutTheme();
				return;
			}

			sendMessage(MessageType::Info, "MainWindow", tr("Change language to English."));
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
				sendMessage(MessageType::Error, "MainWindow", tr("Cannot open the qss file \"%1\" for the theme \"%2\", therefore use default theme \"Deep Blue\".").arg(mainWindowQssFile).arg(m_currentTheme));
				__turnToDefalutTheme();
				return;
			}
			sendMessage(MessageType::Info, "MainWindow", tr("Change language to Simpified Chinese."));
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
				sendMessage(MessageType::Error, "MainWindow", tr("Cannot open the qss file \"%1\" for the theme \"%2\", therefore use default theme \"Deep Blue\".").arg(mainWindowQssFile).arg(m_currentTheme));
				__turnToDefalutTheme();
				return;
			}
			sendMessage(MessageType::Info, "MainWindow", tr("Change language to Traditional Chinese."));
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
				sendMessage(MessageType::Error, "MainWindow", tr("Cannot open the qss file \"%1\" for the theme \"%2\", therefore use default theme \"Deep Blue\".").arg(mainWindowQssFile).arg(m_currentTheme));
				__turnToDefalutTheme();
				return;
			}
			sendMessage(MessageType::Info, "MainWindow", tr("Change language to Japanese."));
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
	iniFile.endGroup();
}

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
	QString musicFilePath = QFileDialog::getOpenFileName(this, tr("Select Music File"), qApp->applicationDirPath(), tr("Music File(*.mp3 *.wav)"));
	m_audioPlayer->reselectMusic(musicFilePath);
	QString fileName = QFileInfo(musicFilePath).fileName();
	m_currentMusicName = fileName.remove(fileName.lastIndexOf(QChar('.')), 4);
	ui->labelMusicName->setText(tr("Background Music : ") + m_currentMusicName);
};

void CrowdedHellGUI::on_buttonPause_toggled(bool checked)
{
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
	ui->labelTotalTime->setText(QString("/") + QString::number(time.hour() * 60 + time.minute()) + QString(":") + QString::number(time.second()) + QString(".") + QString::number(time.msec() / 10));
};

void CrowdedHellGUI::on_buttonNextFrame_clicked()
{
	m_audioPlayer->forward(1u);
};

void CrowdedHellGUI::on_buttonPrevFrame_clicked()
{
	m_audioPlayer->back(1u);
};

void CrowdedHellGUI::on_buttonNext10Frames_clicked()
{
	m_audioPlayer->forward(10u);
};

void CrowdedHellGUI::on_buttonPrev10Frames_clicked()
{
	m_audioPlayer->back(10u);
};

void CrowdedHellGUI::on_buttonNext50Frames_clicked()
{
	m_audioPlayer->forward(50u);
};

void CrowdedHellGUI::on_buttonPrev50Frames_clicked()
{
	m_audioPlayer->back(50u);
};

void CrowdedHellGUI::on_lineEditFrames_editingFinished()
{
	bool isConvertOk;
	unsigned int frames = ui->lineEditFrames->text().toUInt(&isConvertOk);
	if(!isConvertOk)
	{
		sendMessage(MessageType::Warning, "MainWindow", tr("%1 is not a valid number.").arg(QString("\"") + ui->lineEditFrames->text() + QString("\"")));
		musicPositionChanged(m_audioPlayer->getPosition());
	}
	else
		m_audioPlayer->changePosition(frames * 20);
}

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
