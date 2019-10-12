#ifndef LOGGER_H
#define LOGGER_H

#include <QTextEdit>
#include <QString>
#include <QTime>
#include <QFile>
#include <QTextStream>
#include "optionsmanager.h"

class Logger : public QTextEdit
{
    Q_OBJECT
public:
    explicit Logger(QWidget *parent = nullptr);
    ~Logger() override;

    enum class Type
    {
        Info, Tip, Warning, Error
    };

signals:

public slots:
    void message(Type type, const QString &module, const QString &message);
    void logToFile(const QString &filePath);
    void clear();

private:
    static Logger *m_instance;
    friend Logger *logger();

    size_t count;
    QStringList backup;
};

Logger *logger();

#endif // LOGGER_H
