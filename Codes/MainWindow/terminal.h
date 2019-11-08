#ifndef TERMINAL_H
#define TERMINAL_H

#include <QWidget>
#include "Codes/GlobalComponent/globalComponent.h"
#include "commandlineedit.h"

namespace Ui {
    class Terminal;
}

class Terminal : public QWidget
{
    Q_OBJECT

public:
    explicit Terminal(QWidget *parent = nullptr);
    ~Terminal();

    CommandLineEdit *commandLineEdit() const;

private:
    Ui::Terminal *ui;
};

#endif // TERMINAL_H
