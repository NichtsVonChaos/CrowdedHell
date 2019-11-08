#include "terminal.h"
#include "ui_terminal.h"

Terminal::Terminal(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Terminal)
{
    ui->setupUi(this);
    connect(ui->commandLine, &CommandLineEdit::sendCommand, logger(), &Logger::command);
}

Terminal::~Terminal()
{
    delete ui;
}

CommandLineEdit *Terminal::commandLineEdit() const
{
    return ui->commandLine;
}
