#include "headers\testwidget.h"
#include <QPainter>
#include <QDebug>

TestWidget::TestWidget(QWidget *parent) :
    QWidget(parent)
{
    pixmap = new QPixmap(800, 600);
    pixmap->fill();
    setMouseTracking(true);
}

void TestWidget::paintEvent(QPaintEvent *evt)
{
    QPainter painter;
    painter.begin(pixmap);
    painter.drawPoint(pt);
    painter.end();
    painter.begin(this);
    painter.drawPixmap(0, 0, *pixmap);
    painter.end();
}

void TestWidget::mouseMoveEvent(QMouseEvent *evt)
{
    pt = evt->pos();
    //qDebug() << pt;
    repaint();
}

void TestWidget::leaveEvent(QEvent *)
{
    pixmap->fill();
    repaint();
}

void TestWidget::resizeEvent(QResizeEvent *evt)
{
    delete pixmap;
    pixmap = new QPixmap(evt->size());
    pixmap->fill();
    repaint();
}
