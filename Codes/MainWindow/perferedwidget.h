#ifndef PERFEREDWIDGET_H
#define PERFEREDWIDGET_H

#include <QWidget>

class PerferedWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PerferedWidget(QWidget *parent = nullptr);
    QSize sizeHint() const;

signals:

public slots:
};

#endif // PERFEREDWIDGET_H
