#ifndef COMMANDLINEEDIT_H
#define COMMANDLINEEDIT_H

#include <QLineEdit>
#include <QKeyEvent>

class CommandLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit CommandLineEdit(QWidget *parent = nullptr);
    void keyPressEvent(QKeyEvent *ev) override;

signals:
    void sendCommand(const QString &command);

public slots:

private:
    int m_historyPos;
    QStringList m_history;
};

#endif // COMMANDLINEEDIT_H
