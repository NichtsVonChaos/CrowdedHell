#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QAction>

#include "Codes/GlobalComponent/globalComponent.h"
#include "Codes/ResourceManager/Music/musicplayer.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void closeEvent(QCloseEvent *ev) override;

signals:
    void message(Logger::Type type, const QString &module, const QString &message);
    void musicPaused(bool paused, const QObject *sender = nullptr);

public slots:
    void pauseMusic(bool paused, const QObject *sender);
    void refreshRecentProject();
    void openRecentProject(QAction *action);
    void projectOpened(const QString &projectFilePath);
    void projectClosed();

private slots:
    void playButtonChecked(bool checked);

    void on_actionHideAllInfoTypeMessage_triggered(bool checked);

    void on_actionAlwaysSaveProjectBeforeClose_triggered(bool checked);

    void on_actionExportLogToFile_triggered();

private:
	Ui::MainWindow *ui;

    MusicPlayer *m_musicPlayer;
    QAction *m_actionClearRecent, *m_actionNoRecord;
};

#endif // MAINWINDOW_H
