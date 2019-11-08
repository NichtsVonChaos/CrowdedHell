#ifndef COMMANDLINESYSTEM_H
#define COMMANDLINESYSTEM_H

#include <QObject>
#include <QApplication>
#include <QMap>

class CommandLineSystem : public QObject
{
    Q_OBJECT
public:
    explicit CommandLineSystem(QObject *parent = nullptr);
    ~CommandLineSystem();

signals:

public slots:

private:
    static CommandLineSystem *m_instance;
    friend CommandLineSystem *CLsys();

    QMap<QString, QString> enviromentVariables;
};

extern CommandLineSystem *CLsys();

#endif // COMMANDLINESYSTEM_H
