#include "logger.h"

Logger *Logger::m_instance = nullptr;

const QtMessageHandler Logger::QT_DEFAULT_MESSAGE_HANDLER = qInstallMessageHandler(nullptr);

Logger::Logger(QWidget *parent):
    QTextEdit(parent), count(0)
{
    if(m_instance)
        throw std::logic_error(std::string("Logger is created repeatedly. It is not your mistake, please report this bug on GitHub."));

    m_instance = this;
    document()->setMaximumBlockCount(500);

    qInstallMessageHandler(Logger::redirectQtMessage);
}

Logger::~Logger()
{
    qInstallMessageHandler(nullptr);
}

void Logger::redirectQtMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type)
    {
        case QtDebugMsg:
            QT_DEFAULT_MESSAGE_HANDLER(type, context, msg);
        break;

        case QtInfoMsg:
            logger()->message(Logger::Type::Info, "Qt Message",
                              QString("%1 (%2:%3, %4)").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function));
        break;

        case QtWarningMsg:
            logger()->message(Logger::Type::Warning, "Qt Message",
                              QString("%1 (%2:%3, %4)").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function));
        break;

        case QtCriticalMsg:
            logger()->message(Logger::Type::Error, "Qt Message",
                              QString("%1 (%2:%3, %4)").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function));
        break;

        case QtFatalMsg:
            QT_DEFAULT_MESSAGE_HANDLER(type, context, msg);
    }
}

void Logger::message(Logger::Type type, const QString &module, const QString &message)
{
    count++;
    QString time = QTime::currentTime().toString();
    QString messageHtml = QString("<font color=purple>&lt;== </font>(") + time + QString(") ");
    switch(type)
    {
        case Type::Info:
        {
            if(options()->hideInfoLog())
                return;
            messageHtml += QString("<font color=black><b>[ <font color=green>") + tr("Info") + QString("</font> ]</b></font> ");
        }
        break;

        case Type::Tip:
        {
            messageHtml += QString("<font color=black><b>[ <font color=blue>") + tr("Tip") + QString("</font> ]</b></font> ");
        }
        break;

        case Type::Warning:
        {
            messageHtml += QString("<font color=black><b>[ <font color=orange>") + tr("Warning") + QString("</font> ]</b></font> ");
        }
        break;

        case Type::Error:
        {
            messageHtml += QString("<font color=black><b>[ <font color=red>") + tr("Error") + QString("</font> ]</b></font> ");
        }
        break;
    }

    messageHtml += QString(" <font color=purple><u>") + module + QString("</u></font> :");
    messageHtml += QString("<font color=black>") + message + QString(" </font><font color=gray>--log(") + QString::number(count) + QString(")</font><br />");

    moveCursor(QTextCursor::End);
    textCursor().insertHtml(messageHtml);

    backup << QString::number(count) + QString(" (") + time + QString(") [") + QString::number(int(type)) + QString("-") + module + QString("] :") + message;
}

void Logger::command(const QString &command)
{
    moveCursor(QTextCursor::End);
    textCursor().insertHtml(QString("<font color=purple>==&gt;</font>") + command + QString("<br />"));
}

void Logger::logToFile(const QString &filePath)
{
    QFile file(filePath);
    if(!file.open(QIODevice::Text | QIODevice::WriteOnly | QIODevice::Truncate))
        message(Logger::Type::Error, "Logger", tr("Failed to export log because cannot open file \"%1\" to write.").arg(filePath));

    QTextStream qts(&file);
    foreach(QString log, backup)
        qts << log << endl;
    file.flush();
    file.close();

    message(Logger::Type::Info, "Logger", tr("Log exported to file \"%1\".").arg(filePath));
}

void Logger::clear()
{
    count = 0;
    setHtml("<font color=purple>>>></font> ");
    backup.clear();
}

Logger *logger()
{
    if(Logger::m_instance == nullptr)
        throw std::logic_error(std::string("Logger is used before created. It is not your mistake, please report this bug on GitHub."));

    return Logger::m_instance;
}
