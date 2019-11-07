#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QAction>
#include <QMap>
#include <QTranslator>
#include <QDesktopServices>
#include <QRegExp>

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

    void showEvent(QShowEvent *ev) override;
    void closeEvent(QCloseEvent *ev) override;
    void changeEvent(QEvent *ev) override;

signals:
    void message(Logger::Type type, const QString &module, const QString &message);
    void musicPaused(bool paused, const QObject *sender = nullptr);
    void muted(bool muted, const QObject *sender = nullptr);

public slots:
    void pauseMusic(bool paused, const QObject *sender);
    void setMusicPosition(unsigned int milliseconds, const QObject *sender);
    void setMuted(bool muted, const QObject *sender);
    void refreshRecentProject();
    void openRecentProject(QAction *action);
    void projectOpened(const QString &projectFilePath);
    void projectClosed();
    void setLanguage(Language language, const QObject *sender = nullptr);
    void updateVolumeLable(float volume);
    void changeMusic(const QString &musicFile);

private slots:
    void languageButtonClicked(QAction *action);

    void on_actionHideAllInfoTypeMessage_triggered(bool checked);

    void on_actionAlwaysSaveProjectBeforeClose_triggered(bool checked);

    void on_actionExportLogToFile_triggered();

    void on_pushButtonPause_clicked(bool checked);

    void on_pushButtonMute_clicked(bool checked);

    void on_pushButtonNext1_released();

    void on_pushButtonNext5_released();

    void on_pushButtonNext50_released();

    void on_pushButtonPrev1_released();

    void on_pushButtonPrev5_released();

    void on_pushButtonPrev50_released();

    void on_lineEditTime_editingFinished();

    void on_lineEditFrame_editingFinished();

    void on_comboBoxSpeed_currentIndexChanged(int index);

    void on_pushButtonFmod_clicked();

    void on_actionOpenProjectPath_triggered();

    void on_actionOpenTemporaryPath_triggered();

private:
    Ui::MainWindow *ui;

    MusicPlayer *m_musicPlayer;
    QAction *m_actionClearRecent, *m_actionNoRecord;
    QMap<Language, QAction *> m_buttonLanguageOptions;
    QMap<Language, QString> m_translationFiles;
    QTranslator m_translator;
    const QObject *m_sender;

    bool __readTimeFromString(const QString &time, unsigned int &milliseconds);
    QString __generateStringFromTime(unsigned int milliseconds);
};

#endif // MAINWINDOW_H
