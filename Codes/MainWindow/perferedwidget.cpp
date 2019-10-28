#include "perferedwidget.h"

PerferedWidget::PerferedWidget(QWidget *parent) : QWidget(parent)
{

}

QSize PerferedWidget::sizeHint() const
{
    return QSize(100, 120);
}
