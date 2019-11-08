#include "commandlineedit.h"

CommandLineEdit::CommandLineEdit(QWidget *parent) :
    QLineEdit(parent), m_historyPos(0), m_history()
{

}

void CommandLineEdit::keyPressEvent(QKeyEvent *ev)
{
    if(ev->key() == Qt::Key_Enter || ev->key() == Qt::Key_Return)
    {
        if(!text().remove(QChar(' ')).isEmpty())
            emit sendCommand(text());
        m_history << text();
        m_historyPos = 0;
        clear();
        ev->accept();
    }
    else if(ev->key() == Qt::Key_Up)
    {
        if(m_history.isEmpty())
        {
            ev->ignore();
            return;
        }
        ++m_historyPos;
        if(m_historyPos >= m_history.size())
        {
            setText(m_history[0]);
            m_historyPos = m_history.size();
        }
        else
            setText(m_history.at(m_history.size() - m_historyPos));
        ev->accept();
    }
    else if(ev->key() == Qt::Key_Down)
    {
        if(m_history.isEmpty())
            clear();
        if(m_historyPos > 1)
        {
            --m_historyPos;
            setText(m_history.at(m_history.size() - m_historyPos));
        }
        else
        {
            m_historyPos = 0;
            clear();
        }
        ev->accept();
    }
    else
        QLineEdit::keyPressEvent(ev);
}
