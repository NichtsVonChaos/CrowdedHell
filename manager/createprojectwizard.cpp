#include "createprojectwizard.h"
#include "ui_createprojectwizard.h"

CreateProjectWizard::CreateProjectWizard(CrowdedHellGUI *parent) :
	QWizard(parent),
	ui(new Ui::CreateProjectWizard)
{
	ui->setupUi(this);
	setWindowTitle(tr("Create Project Wizard"));

	QString QSSfile = "Theme/%1/QSS/%2 - Main Window.qss";


	if(parent->currentTheme() == "Deep Blue")
		QSSfile = ":/Theme/Deep Blue/QSS/%1 - Main Window.qss";
	else
		QSSfile = QSSfile.arg(parent->currentTheme());

	switch (parent->currentLanguage())
	{
		case Language::EN:
			QSSfile = QSSfile.arg("EN");
		break;

		case Language::JP:
			QSSfile = QSSfile.arg("JP");
		break;

		case Language::ZH_CN:
			QSSfile = QSSfile.arg("ZH_CN");
		break;

		case Language::ZH_TW:
			QSSfile = QSSfile.arg("ZH_TW");
		break;
	}

	QFile qssFile(QSSfile);
	if(qssFile.open(QIODevice::ReadOnly))
	{
		setStyleSheet(qssFile.readAll());
		qssFile.close();
	}

	m_createDate = QDateTime::currentDateTime();
	ui->labelRealDate->setText(m_createDate.toString());
}

CreateProjectWizard::~CreateProjectWizard()
{
	delete ui;
}

QString CreateProjectWizard::getProjectName()
{
	return ui->lineEditProjectName->text();
}

QString CreateProjectWizard::getPath()
{
	return ui->lineEditPath->text();
}

QString CreateProjectWizard::getAuthor()
{
	return ui->lineEditAuthor->text();
}

QDateTime CreateProjectWizard::getDate()
{
	return m_createDate;
}

QString CreateProjectWizard::getMusicPath()
{
	return ui->lineEditBackgroundMusic->text();
}

void CreateProjectWizard::on_toolButtonSelectPath_clicked()
{
	QFileDialog *projectPathDialog = new QFileDialog(this);
	projectPathDialog->setAcceptMode(QFileDialog::AcceptOpen);
	projectPathDialog->setFileMode(QFileDialog::DirectoryOnly);
	projectPathDialog->setDirectory(".");
	projectPathDialog->setWindowTitle(tr("Select a directory to save your project"));

	if(projectPathDialog->exec() == QFileDialog::Accepted)
	{
		QString projectPath = projectPathDialog->selectedFiles()[0];
		qDebug() << projectPath;
		ui->lineEditPath->setText(projectPath);
	}

	delete projectPathDialog;
}

void CreateProjectWizard::on_toolButtonBackgroundMusic_pressed()
{
	QString musicFilePath = QFileDialog::getOpenFileName(this, tr("Select Music File"), qApp->applicationDirPath(), tr("Music File(*.mp3 *.wav)"));
	ui->lineEditBackgroundMusic->setText(musicFilePath);
}
