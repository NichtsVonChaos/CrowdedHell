#ifndef LOGGER_H
#define LOGGER_H

#include <QTextEdit>
#include <QString>
#include <QTime>
#include <QFile>
#include <QTextStream>
#include <QtGlobal>
#include "options.h"

/**
 * @brief The Logger class
 */
class Logger : public QTextEdit
{
    Q_OBJECT
public:
    explicit Logger(QWidget *parent = nullptr);
    ~Logger() override;

    /**
     * @brief The Type enum
     * Log type.
     */
    enum class Type
    {
        Info, Tip, Warning, Error
    };

    /**
     * @brief QT_DEFAULT_MESSAGE_HANDLER
     * The default message handler of Qt.
     * Before Logger destroyed, set Qt message handler to default.
     */
    static const QtMessageHandler QT_DEFAULT_MESSAGE_HANDLER;

    /**
     * @brief redirectQtMessage
     * Custom Qt message handler, redirect to Logger.
     */
    static void redirectQtMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);

signals:

public slots:
    /**
     * @brief message
     * Receive a message.
     */
    void message(Type type, const QString &module, const QString &message);

    /**
     * @brief logToFile
     * Save log to file.
     * Note that the log saved to file is not similar as you see on main window.
     * @param filePath
     * Log file path to save.
     */
    void logToFile(const QString &filePath);

    /**
     * @brief clear
     * Clear all log.
     */
    void clear();

private:
    static Logger *m_instance;
    friend Logger *logger();

    size_t count;
    QStringList backup;
};

/**
 * @brief logger
 * Get the unique Logger instance.
 * Note it will be created by MainWindow.
 */
Logger *logger();

#endif // LOGGER_H
