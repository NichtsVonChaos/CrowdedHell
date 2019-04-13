#include "addresourcewizard.h"
#include "ui_addresourcewizard.h"

AddResourceWizard::AddResourceWizard(AddResourceWizard::Type resourceType, QWidget *parent, const QString &group) :
	QWizard(parent),
	ui(new Ui::AddResourceWizard),
	m_type(resourceType)
{
	ui->setupUi(this);

	setWindowTitle(tr("Create Resource Wizard"));

	QString QSSfile = "Theme/%1/QSS/%2 - Main Window.qss";

	if(g_mainWindow->currentTheme() == "Deep Blue")
		QSSfile = ":/Theme/Deep Blue/QSS/%1 - Main Window.qss";
	else
		QSSfile = QSSfile.arg(g_mainWindow->currentTheme());

	switch (g_mainWindow->currentLanguage())
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

	if(group.isEmpty())
		switch(resourceType)
		{
			case Type::SPRITE:
				ui->lineEditGroup->setText("Sprites/");
			break;

			case Type::SOUND:
				ui->lineEditGroup->setText("Sounds/");
			break;

			case Type::BACKGROUND:
				ui->lineEditGroup->setText("Backgrounds/");
			break;
		}
	else
		ui->lineEditGroup->setText(group);
}

AddResourceWizard::~AddResourceWizard()
{
	delete ui;
}

void AddResourceWizard::on_toolButtonFile_clicked()
{
	QFileDialog fileDialog(this, tr("Select a sprite file"), ".");
	fileDialog.setAcceptMode(QFileDialog::AcceptMode::AcceptOpen);
	fileDialog.setFileMode(QFileDialog::FileMode::ExistingFile);
	fileDialog.setNameFilter(tr("Sprite File(*.png *.jpeg *.jpg *.bmp)"));
	if(fileDialog.exec() == QFileDialog::Accepted)
		ui->lineEditFile->setText(fileDialog.selectedFiles()[0]);
}

AddResourceWizard::Type AddResourceWizard::getType() const
{
	return m_type;
}

QString AddResourceWizard::getFilePath()
{
	return ui->lineEditFile->text();
}

QString AddResourceWizard::getName()
{
	return ui->lineEditName->text();
}

QString AddResourceWizard::getGroup()
{
	return ui->lineEditGroup->text();
}

void AddResourceWizard::on_toolButtonGroup_released()
{
	QFileDialog fileDialog(this, tr("Select a sprite file"), ".");
	fileDialog.setAcceptMode(QFileDialog::AcceptMode::AcceptOpen);
	fileDialog.setFileMode(QFileDialog::FileMode::ExistingFile);
	fileDialog.setNameFilter(tr("Sprite File(*.png *.jpeg *.jpg *.bmp)"));
	if(fileDialog.exec() == QFileDialog::Accepted)
		ui->lineEditFile->setText(fileDialog.selectedFiles()[0]);
}
