#include "headers\brush.h"
#include "headers/paintworkspace.h"
#include "headers/mainwindow.h"

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

Instrument* Brush::instance = new Brush;



void Brush::mousePressed(QMouseEvent *evt, PaintWorkspace *wsp)
{
    QPoint pos = wsp->getCoords();
    QImage* img = wsp->newChange();
    QPainter painter(img);
    if (evt->buttons() & Qt::LeftButton)
        col = MainWindow::window->getFirstColor();
    else
        col = MainWindow::window->getSecondColor();
    QPen pen(col);
    pen.setWidth(size->value());
    pen.setCapStyle((Qt::PenCapStyle)(endStyle->itemData(endStyle->currentIndex()).toInt()));
    painter.setPen(pen);
    painter.drawPoint(pos);
    polygon.clear();
    polygon.append(pos);
    painter.end();
}

void Brush::mouseMoved(QMouseEvent *, PaintWorkspace *wsp)
{
    QPoint pos = wsp->getCoords();
    QImage* img = wsp->getChange();
    img->fill(Qt::transparent);
    QPainter painter(img);
    polygon.append(pos);
    QPen pen(col);
    pen.setWidth(size->value());
    pen.setCapStyle((Qt::PenCapStyle)(endStyle->itemData(endStyle->currentIndex()).toInt()));
    painter.setPen(pen);
    painter.drawPolyline(QPolygon(polygon));
    painter.end();
}

void Brush::mouseReleased(QMouseEvent *, PaintWorkspace *wsp)
{
    wsp->applyChange();
    polygon.clear();
}

void Brush::paintChange(QPainter &painter, QImage *layer, QImage *change)
{
    painter.drawImage(0, 0, *layer);
    painter.drawImage(0, 0, *change);
}

void Brush::initSettingsDockWidget()
{
    settings = new QDockWidget;
    QWidget* w = new QWidget;
    QVBoxLayout* l = new QVBoxLayout;
    size = new QSpinBox;
    size->setMinimum(1);
    size->setMaximum(1000);
    size->setSuffix("px");

    endStyle = new QComboBox;
    endStyle->addItem(QObject::tr("Flat"), QVariant(Qt::FlatCap));
    endStyle->addItem(QObject::tr("Round"), QVariant(Qt::RoundCap));
    endStyle->addItem(QObject::tr("Square"), QVariant(Qt::SquareCap));

    l->addWidget(size);
    l->addWidget(endStyle);
    l->addStretch();
    w->setLayout(l);
    settings->setWidget(w);
}

void Brush::init()
{
    icon = new QIcon(":icons/brush.png");
    QPixmap pix(":cursors/brush.png");
    cursor = new QCursor(pix, 3, 30);
    initSettingsDockWidget();
}
