#include "crowdedhellgui.h"
#include "ui_crowdedhellgui.h"

CrowdedHellGUI::CrowdedHellGUI(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::CrowdedHellGUI),
	m_currentMusicName(tr("Unknown - Unknown"))
{
	ui->setupUi(this);

	// Set up languages.
	m_translators.insert(Language::ZH_CN, new QTranslator(this));
	m_translators[Language::ZH_CN]->load(":/translations/Trans_zh_cn.qm");
	m_translators.insert(Language::ZH_TW, new QTranslator(this));
	m_translators[Language::ZH_TW]->load(":/translations/Trans_zh_tw.qm");
	m_translators.insert(Language::JP, new QTranslator(this));
	m_translators[Language::JP]->load(":/translations/Trans_jp.qm");

	__updateLanguage(Language::EN);

	// Initialize openGL widget
	m_displayWidget = new AvoidanceDisplayWidget(this);
	m_displayWidget->setFixedSize(QSize(800, 608));
	m_displayWidget->hide();

	// Initialize message box.
	ui->textEditMessageBox->document()->setMaximumBlockCount(200);
	ui->textEditMessageBox->setHtml("<font color=purple>>>></font> ");

	// Initialize audio player.
	m_player = new AudioPlayer(ui->audioSlider, this);
}

CrowdedHellGUI::~CrowdedHellGUI()
{
	delete ui;
}

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
}

void CrowdedHellGUI::changeEvent(QEvent *event)
{
	if(event -> type() == QEvent::LanguageChange)
	{

	}
}

void CrowdedHellGUI::musicPositionChanged(unsigned int miliseconds)
{
	ui->lineEditFrames->setText(QString::number(miliseconds / 20));
	QTime time(0, 0);
	time = time.addMSecs(int(miliseconds));
	ui->lineEditTime->setText(QString::number(time.hour() * 60 + time.minute()) + QString(":") + QString::number(time.second()) + QString(".") + QString::number(time.msec() / 10));
}

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
}

void CrowdedHellGUI::on_actionSimplifiedChinese_triggered()
{
	__updateLanguage(Language::ZH_CN);
}

void CrowdedHellGUI::on_actionEnglish_triggered()
{
	__updateLanguage(Language::EN);
}

void CrowdedHellGUI::on_actionTraditionalChinese_triggered()
{
	__updateLanguage(Language::ZH_TW);
}

void CrowdedHellGUI::on_actionJapanese_triggered()
{
	__updateLanguage(Language::JP);
}

void CrowdedHellGUI::__updateLanguage(Language language)
{
	switch (language)
	{
		case Language::EN:
		{
			foreach(auto i, m_translators)
				qApp->removeTranslator(i);
			ui->actionEnglish->setChecked(true);
			ui->actionSimplifiedChinese->setChecked(false);
			ui->actionTraditionalChinese->setChecked(false);
			ui->actionJapanese->setChecked(false);

			QFile qssFile(":/Themes/Deep Blue/QSS/EN - Main Window.qss");
			if(qssFile.open(QIODevice::ReadOnly))
			{
				setStyleSheet(qssFile.readAll());
				qssFile.close();
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

			QFile qssFile(":/Themes/Deep Blue/QSS/ZH_CN - Main Window.qss");
			if(qssFile.open(QIODevice::ReadOnly))
			{
				setStyleSheet(qssFile.readAll());
				qssFile.close();
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

			QFile qssFile(":/Themes/Deep Blue/QSS/ZH_TW - Main Window.qss");
			if(qssFile.open(QIODevice::ReadOnly))
			{
				setStyleSheet(qssFile.readAll());
				qssFile.close();
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

			QFile qssFile(":/Themes/Deep Blue/QSS/JP - Main Window.qss");
			if(qssFile.open(QIODevice::ReadOnly))
			{
				setStyleSheet(qssFile.readAll());
				qssFile.close();
			}
			sendMessage(MessageType::Info, "MainWindow", tr("Change language to Japanese."));
		}
		break;
	}

	ui->retranslateUi(this);
	ui->labelMusicName->setText(tr("Background Music : ") + m_currentMusicName);

	// Sent signal
	languageChanged(language);
}

void CrowdedHellGUI::__readSettings()
{
	QSettings iniFile("settings.ini", QSettings::IniFormat);
}

void CrowdedHellGUI::on_actionAddSoundEffect_triggered()
{

}

void CrowdedHellGUI::on_actionReselectMusic_triggered()
{
	QString musicFilePath = QFileDialog::getOpenFileName(this, tr("Select Music File"), qApp->applicationDirPath(), tr("Music File(*.mp3 *.wav)"));
	m_player->reselectMusic(musicFilePath);
	QString fileName = QFileInfo(musicFilePath).fileName();
	m_currentMusicName = fileName.remove(fileName.lastIndexOf(QChar('.')), 4);
	ui->labelMusicName->setText(tr("Background Music : ") + m_currentMusicName);
}

void CrowdedHellGUI::on_buttonPause_toggled(bool checked)
{
	m_player->playOrPause(checked);
}

void CrowdedHellGUI::musicInvalid()
{
	if(ui->buttonPause->isChecked())
		ui->buttonPause->setChecked(false);
}

void CrowdedHellGUI::updateMusicLength(unsigned int miliseconds)
{
	ui->labelTotalFrames->setText(QString("/") + QString::number(miliseconds / 20));
	QTime time(0, 0);
	time = time.addMSecs(int(miliseconds));
	ui->labelTotalTime->setText(QString("/") + QString::number(time.hour() * 60 + time.minute()) + QString(":") + QString::number(time.second()) + QString(".") + QString::number(time.msec() / 10));
}

void CrowdedHellGUI::on_buttonNextFrame_clicked()
{
	m_player->forward(1u);
}

void CrowdedHellGUI::on_buttonPrevFrame_clicked()
{
	m_player->back(1u);
}

void CrowdedHellGUI::on_buttonNext10Frames_clicked()
{
	m_player->forward(10u);
}

void CrowdedHellGUI::on_buttonPrev10Frames_clicked()
{
	m_player->back(10u);
}

void CrowdedHellGUI::on_buttonNext50Frames_clicked()
{
	m_player->forward(50u);
}

void CrowdedHellGUI::on_buttonPrev50Frames_clicked()
{
	m_player->back(50u);
}

void CrowdedHellGUI::on_lineEditFrames_editingFinished()
{
	bool isConvertOk;
	unsigned int frames = ui->lineEditFrames->text().toUInt(&isConvertOk);
	if(!isConvertOk)
	{
		sendMessage(MessageType::Warning, "MainWindow", tr("%1 is not a valid number.").arg(QString("\"") + ui->lineEditFrames->text() + QString("\"")));
		musicPositionChanged(m_player->getPosition());
	}
	else
	{
		m_player->changePosition(frames * 20);
		qDebug() << m_player->getPosition();
	}
}
