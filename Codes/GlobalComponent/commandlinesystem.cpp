#include "commandlinesystem.h"

CommandLineSystem *CommandLineSystem::m_instance = nullptr;

CommandLineSystem::CommandLineSystem(QObject *parent) : QObject(parent)
{
    if(m_instance)
        throw std::logic_error(std::string("CommandLineSystem is created repeatedly. It is not your mistake, please report this bug on GitHub."));

    m_instance = this;

    enviromentVariables["PWD"] = qApp->applicationDirPath();
}

CommandLineSystem::~CommandLineSystem()
{

}

CommandLineSystem *CLsys()
{
    if(CommandLineSystem::m_instance == nullptr)
        throw std::logic_error(std::string("Logger is used before created. It is not your mistake, please report this bug on GitHub."));
    return CommandLineSystem::m_instance;
}
