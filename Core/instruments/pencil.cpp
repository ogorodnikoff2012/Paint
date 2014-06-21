#include "headers\pencil.h"
#include "headers/paintworkspace.h"
#include "headers/mainwindow.h"

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

Instrument* Pencil::instance = new Pencil;

void Pencil::mousePressed(QMouseEvent *evt, PaintWorkspace *wsp)
{
    QPoint pos = (evt->pos() - QPoint(wsp->getScale() / 2, wsp->getScale() / 2)) / wsp->getScale();
    QImage* img = wsp->newChange();
    QPainter painter(img);
    if (evt->buttons() & Qt::LeftButton)
        col = MainWindow::window->getFirstColor();
    else
        col = MainWindow::window->getSecondColor();
    painter.setPen(col);
    painter.drawPoint(pos);
    lastPos = pos;
    painter.end();
    //wsp->update();
}

void Pencil::mouseMoved(QMouseEvent *evt, PaintWorkspace *wsp)
{
    QPoint pos = (evt->pos() - QPoint(wsp->getScale() / 2, wsp->getScale() / 2)) / wsp->getScale();
    QImage* img = wsp->getChange();
    QPainter painter;
    painter.begin(img);
    painter.setPen(col);
    painter.drawLine(lastPos, pos);
    lastPos = pos;
    painter.end();
    //wsp->update();
}

void Pencil::mouseReleased(QMouseEvent *, PaintWorkspace *wsp)
{
    wsp->applyChange();
}

void Pencil::paintChange(QPainter &painter, QImage *layer, QImage *change)
{
    painter.drawImage(0, 0, *layer);
    painter.drawImage(0, 0, *change);
}

void Pencil::init()
{
    icon = new QIcon(":icons/pencil.ico");
    QPixmap pix(":cursors/pencil.png");
    cursor = new QCursor(pix, 3, 31);
}
