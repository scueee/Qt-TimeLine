#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QPainter>
#include <QPaintEvent>

class MyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyWidget(QWidget *parent = nullptr);
    int row = 1;
    double scale = 1.0;
    int size = 80;

protected:
    void paintEvent(QPaintEvent *);
};

#endif // MYWIDGET_H
