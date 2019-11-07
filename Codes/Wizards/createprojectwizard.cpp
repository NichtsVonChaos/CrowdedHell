#include "createprojectwizard.h"
#include "ui_createprojectwizard.h"

CreateProjectWizard::CreateProjectWizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::CreateProjectWizard)
{
    ui->setupUi(this);
    setWindowTitle(tr("Create Project Wizard"));

    m_createDate = QDateTime::currentDateTime();
    ui->labelRealDate->setText(m_createDate.toString());
    ui->lineEditPath->setText(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
};

CreateProjectWizard::~CreateProjectWizard()
{
    delete ui;
};

QString CreateProjectWizard::getProjectName()
{
    return ui->lineEditProjectName->text();
};

QString CreateProjectWizard::getPath()
{
    return ui->lineEditPath->text();
};

QString CreateProjectWizard::getAuthor()
{
    return ui->lineEditAuthor->text();
};

QDateTime CreateProjectWizard::getDate()
{
    return m_createDate;
};

QString CreateProjectWizard::getMusicPath()
{
    return ui->lineEditBackgroundMusic->text();
};

void CreateProjectWizard::on_toolButtonSelectPath_clicked()
{
    QFileDialog *projectPathDialog = new QFileDialog(this);
    projectPathDialog->setAcceptMode(QFileDialog::AcceptOpen);
    projectPathDialog->setOption(QFileDialog::ShowDirsOnly, true);
    projectPathDialog->setFileMode(QFileDialog::Directory);
    projectPathDialog->setDirectory(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    projectPathDialog->setWindowTitle(tr("Select a directory to save your project"));

    if(projectPathDialog->exec() == QFileDialog::Accepted)
    {
        QString projectPath = projectPathDialog->selectedFiles()[0];
        ui->lineEditPath->setText(projectPath);
    }

    delete projectPathDialog;
};

void CreateProjectWizard::on_toolButtonBackgroundMusic_pressed()
{
    QString musicFilePath = QFileDialog::getOpenFileName(this, tr("Select Music File"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation), tr("Music File(*.mp3 *.wav)"));
    ui->lineEditBackgroundMusic->setText(musicFilePath);
};
