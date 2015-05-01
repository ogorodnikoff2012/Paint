#include "headers/floodfill.h"
#include "headers/paintworkspace.h"
#include "headers/mainwindow.h"

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

Instrument *FloodFill::instance = new FloodFill;

const int x_deltas[] = {-1, 0, 1, 0};
const int y_deltas[] = {0, 1, 0, -1};
const int deltas = 4;

void FloodFill::mousePressed(QMouseEvent *evt, PaintWorkspace *wsp)
// Do NOT use QImage::pixel() and QImage::setPixel(): they're extremely slow.
{
    QPoint pos = wsp->getCoords();
    QRgb col;
    QImage *img = wsp->newChange(true);
    int minX = 0, minY = 0, maxX = img->width() - 1, maxY = img->height() - 1;

    QVector<QRgb *> lines(maxY + 1);
    for(int i = minY; i <= maxY; i++)
    {
        lines[i] = (QRgb *)img->scanLine(i);
    }

    if (evt->buttons() & Qt::LeftButton)
    {
        col = MainWindow::window->getFirstColor().rgba();
    }
    else
    {
        col = MainWindow::window->getSecondColor().rgba();
    }

    QQueue<QPoint> points;
    points.enqueue(pos);
    QRgb background = img->pixel(pos);
    if(col == background)
    {
        wsp->applyChange();
        return;
    }

    lines[pos.y()][pos.x()] = col;

    while(!points.empty())
    {
        QPoint pt = points.dequeue();
        for(int i = 0; i < deltas; i++)
        {
            QPoint next(pt.x() + x_deltas[i], pt.y() + y_deltas[i]);
            if(minX <= next.x() && next.x() <= maxX && minY <= next.y() && next.y() <= maxY &&
                    lines[next.y()][next.x()] == background)
            {
                points.enqueue(next);
                lines[next.y()][next.x()] = col;
                //qDebug() << next;
            }
        }
    }

    wsp->applyChange();
}

void FloodFill::mouseMoved(QMouseEvent *, PaintWorkspace *)
{
}

void FloodFill::mouseReleased(QMouseEvent *, PaintWorkspace *)
{
}

void FloodFill::paintChange(QPainter &, QImage *, QImage *)
{
}

void FloodFill::initSettingsDockWidget()
{
    settings = 0;
}

const QString FloodFill::getName()
{
    return QObject::tr("FloodFill");
}

void FloodFill::init()
{
    icon = new QIcon(":icons/flood_fill.png");
    QPixmap pix(":cursors/line.png");
    cursor = new QCursor(pix, 15, 15);
    initSettingsDockWidget();
}
