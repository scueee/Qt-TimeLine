#include "mywidget.h"

MyWidget::MyWidget(QWidget *parent) : QWidget(parent)
{

}

void MyWidget::paintEvent(QPaintEvent *){
    QPainter painter(this);
    QPen pen(Qt::red,3);
    painter.setPen(pen);
    for(int i=0;i<=row;i++){
        int length = int(size*(i+1.5)*scale) + 15*(i+1);
        painter.drawLine(0,length,width(),length);
    }
}
