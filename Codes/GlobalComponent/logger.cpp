#include "logger.h"

Logger *Logger::m_instance = nullptr;

Logger::Logger(QWidget *parent): QTextEdit(parent)
{
    if(m_instance)
        throw std::logic_error(std::string("Logger is created repeatedly. It is not your mistake, please report this bug on GitHub."));

    m_instance = this;
    document()->setMaximumBlockCount(100);
    setHtml("<font color=purple>>>></font> ");
}

void Logger::message(Logger::Type type, const QString &module, const QString &message)
{
    QString messageHtml = "<p>";
    switch(type)
    {
        case Type::Info:
        {
            if(options()->hideInfoLog())
                return;
            messageHtml += QString("<font color=black><b>[ <font color=green>") + tr("Info") + QString("</font> ]</b></font> ");
        }
        break;

        case Type::Error:
        {
            messageHtml += QString("<font color=black><b>[ <font color=red>") + tr("Error") + QString("</font> ]</b></font> ");
        }
        break;

        case Type::Warning:
        {
            messageHtml += QString("<font color=black><b>[ <font color=orange>") + tr("Warning") + QString("</font> ]</b></font> ");
        }
        break;

        case Type::Tip:
        {
            messageHtml += QString("<font color=black><b>[ <font color=blue>") + tr("Tip") + QString("</font> ]</b></font> ");
        }
        break;
    }

    messageHtml += QString(tr(" %1 : ")).arg(QString("<font color=purple><u>") + module + QString("</u></font>"));
    messageHtml += QString("<font color=black>") + message + QString("</font></p><font color=purple>>>> </font>");

    moveCursor(QTextCursor::End);
    textCursor().insertHtml(messageHtml);
}

Logger *logger()
{
    if(Logger::m_instance == nullptr)
        throw std::logic_error(std::string("Logger is used before created. It is not your mistake, please report this bug on GitHub."));

    return Logger::m_instance;
}
