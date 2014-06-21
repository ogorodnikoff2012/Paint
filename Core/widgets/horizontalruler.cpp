#include "headers\horizontalruler.h"
#include "headers/workspaceshell.h"

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

HorizontalRuler::HorizontalRuler(WorkspaceShell* shell, QWidget *parent) :
    QWidget(parent)
{
    this->shell = shell;
    setMinimumSize(RULER_SIZE, RULER_SIZE);
}

void HorizontalRuler::paintEvent(QPaintEvent *evt)
{
    QPainter painter(this);

    //background
    painter.setBrush(palette().background());
    QFont f = font();
    f.setPointSize(f.pointSize() - 1);
    painter.setFont(f);
    painter.drawRect(evt->rect());

    int anchorX = shell->getWorkspace()->parentWidget()->pos().x() + 1;
    double pixelSize = shell->getWorkspace()->getScale();
    double step = 1;
    int mul = 2;
    while (pixelSize * step < 10) {
        step *= mul;
        mul = 7 - mul;
    }

    //minor ticks
    double start = anchorX;
    while (start >= evt->rect().x())
        start -= pixelSize * step;

    int end = evt->rect().x() + evt->rect().width();
    for (double pos = start; pos <= end; pos += step * pixelSize)
        painter.drawLine(pos, 2 * RULER_SIZE / 3, pos, RULER_SIZE);

    //major ticks
    mul = 7 - mul;
    start = anchorX;
    while (start >= evt->rect().x())
        start -= step * mul * pixelSize;

    end = evt->rect().x() + evt->rect().width();
    for (double pos = start; pos <= end; pos += step * pixelSize * mul) {
        painter.drawLine(pos, RULER_SIZE / 3, pos, RULER_SIZE);
        QString label;
        label.setNum((int)((pos - anchorX) / pixelSize));
        painter.drawText(QRect(pos, 0, 0, RULER_SIZE), Qt::AlignTop | Qt::AlignLeft | Qt::TextDontClip, label);
    }
    painter.end();
}
