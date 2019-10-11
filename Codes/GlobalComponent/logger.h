#ifndef LOGGER_H
#define LOGGER_H

#include <QTextEdit>
#include <QString>
#include "optionsmanager.h"

class Logger : public QTextEdit
{
    Q_OBJECT
public:
    explicit Logger(QWidget *parent = nullptr);

    enum class Type
    {
        Info, Tip, Warning, Error
    };

signals:

public slots:
    void message(Type type, const QString &module, const QString &message);

private:
    static Logger *m_instance;
    friend Logger *logger();
};

Logger *logger();

#endif // LOGGER_H
