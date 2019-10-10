#ifndef LOGGER_H
#define LOGGER_H

#include <QTextEdit>
#include <QString>
#include "optionsmanager.h"

class Logger : public QTextEdit
{
    Q_OBJECT
public:
    Logger();

    enum class Type
    {
        Info, Tip, Warning, Error
    };

signals:

public slots:
    void message(const QString &module, Type type, const QString &message);

private:
    static Logger *m_logger;
    friend Logger *logger();
};

Logger *logger();

#endif // LOGGER_H
