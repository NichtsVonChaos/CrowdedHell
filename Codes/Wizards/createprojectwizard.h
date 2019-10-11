#ifndef CREATEPROJECTWIZARD_H
#define CREATEPROJECTWIZARD_H

#include <QWizard>
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QDateTime>
#include <QDebug>

namespace Ui {
    class CreateProjectWizard;
}

class CreateProjectWizard : public QWizard
{
    Q_OBJECT

public:
    explicit CreateProjectWizard(QWidget *parent = nullptr);
    ~CreateProjectWizard() override;

    QString getProjectName();
    QString getPath();
    QString getAuthor();
    QDateTime getDate();
    QString getMusicPath();

private slots:

    void on_toolButtonSelectPath_clicked();

    void on_toolButtonBackgroundMusic_pressed();

private:
    Ui::CreateProjectWizard *ui;

    QDateTime m_createDate;
};

#endif // CREATEPROJECTWIZARD_H
