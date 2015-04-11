#include "headers/line.h"
#include "headers/paintworkspace.h"
#include "headers/mainwindow.h"

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

Instrument *Line::instance = new Line;

void Line::mousePressed(QMouseEvent *evt, PaintWorkspace *wsp)
{
    start = end = wsp->getCoords();
    wsp->newChange();

    if (evt->buttons() & Qt::LeftButton)
    {
        col = MainWindow::window->getFirstColor();
    }
    else
    {
        col = MainWindow::window->getSecondColor();
    }

}

void Line::mouseMoved(QMouseEvent *, PaintWorkspace *wsp)
{
    end = wsp->getCoords();
}

void Line::mouseReleased(QMouseEvent *, PaintWorkspace *wsp)
{
    QImage *img = wsp->getChange();
    QPainter painter(img);
    painter.translate(0.5, 0.5);
    QPen pen(col);
    pen.setWidth(size->value());
    pen.setCapStyle((Qt::PenCapStyle)(endStyle->itemData(
                                          endStyle->currentIndex()).toInt()));
    painter.setPen(pen);
    painter.drawLine(start, end);
    painter.end();
    wsp->applyChange();
}

void Line::paintChange(QPainter &painter, QImage *layer, QImage *)
{
    painter.drawImage(0, 0, *layer);
    painter.translate(0.5, 0.5);
    QPen pen(col);
    pen.setWidth(size->value());
    pen.setCapStyle((Qt::PenCapStyle)(endStyle->itemData(
                                          endStyle->currentIndex()).toInt()));
    painter.setPen(pen);
    painter.drawLine(start, end);
    painter.translate(-0.5, -0.5);
}

void Line::initSettingsDockWidget()
{
    settings = new QDockWidget;
    QWidget *w = new QWidget;
    QVBoxLayout *l = new QVBoxLayout;
    size = new QSpinBox;
    size->setMinimum(1);
    size->setMaximum(1000);
    size->setSuffix("px");

    endStyle = new QComboBox;
    endStyle->addItem(QObject::tr("Flat"), QVariant(Qt::FlatCap));
    endStyle->addItem(QObject::tr("Round"), QVariant(Qt::RoundCap));
    endStyle->addItem(QObject::tr("Square"), QVariant(Qt::SquareCap));
    endStyle->setCurrentIndex(1);

    l->addWidget(size);
    l->addWidget(endStyle);
    l->addStretch();
    w->setLayout(l);
    settings->setWidget(w);
}

void Line::init()
{
    icon = new QIcon(":icons/line.png");
    QPixmap pix(":cursors/line.png");
    cursor = new QCursor(pix, 15, 15);
    initSettingsDockWidget();
}
