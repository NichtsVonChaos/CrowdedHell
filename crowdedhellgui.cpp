#include "crowdedhellgui.h"
#include "ui_crowdedhellgui.h"

CrowdedHellGUI::CrowdedHellGUI(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::CrowdedHellGUI)
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
	ui->textEditMessageBox->setTextColor(QColor(255,0,255));
	ui->textEditMessageBox->setText(">>> ");

	// Initialize audio player.
	m_player = new AudioPlayer(ui->audioSlider);
	connect(m_player, SIGNAL(sendMessage(CrowdedHellGUI::MessageType, QString, QString)),
			this, SLOT(sendMessage(CrowdedHellGUI::MessageType, QString, QString)));
	connect(ui->buttonPause, SIGNAL(toggled(bool)), m_player, SLOT(playerOrPause(bool)));
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
			ui->textEditMessageBox->setTextColor(QColor(0,255,0));
			ui->textEditMessageBox->append(tr("[ Info ] "));
		}
		break;

		case MessageType::Error:
		{
			ui->textEditMessageBox->setTextColor(QColor(255,0,0));
			ui->textEditMessageBox->append(tr("[ Error ] "));
		}
		break;

		case MessageType::Warning:
		{
			ui->textEditMessageBox->setTextColor(QColor(255,128,0));
			ui->textEditMessageBox->append(tr("[ Warning ] "));
		}
		break;

		case MessageType::Tips:
		{
			ui->textEditMessageBox->setTextColor(QColor(0,0,255));
			ui->textEditMessageBox->append(tr("[ Tips ] "));
		}
		break;
	}

	ui->textEditMessageBox->setTextColor(QColor(0,0,0));
	ui->textEditMessageBox->insertHtml(QString(tr("In module \"<b>%1</b>\" : ")).arg(module));
	ui->textEditMessageBox->append(message + QString("\n"));
	ui->textEditMessageBox->setTextColor(QColor(255,0,255));
	ui->textEditMessageBox->setText(">>> ");
}

void CrowdedHellGUI::changeEvent(QEvent *event)
{
	if(event -> type() == QEvent::LanguageChange)
	{

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
		}
		break;
	}

	ui->retranslateUi(this);

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
}
