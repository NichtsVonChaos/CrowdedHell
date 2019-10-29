#include "perferedwidget.h"

PerferedWidget::PerferedWidget(QWidget *parent) : QWidget(parent), size(100, 100)
{

}

QSize PerferedWidget::sizeHint() const
{
    return size;
}
