#include "headers\verticalruler.h"
#include "headers/workspaceshell.h"

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

VerticalRuler::VerticalRuler(WorkspaceShell* shell, QWidget *parent) :
    QWidget(parent)
{
    this->shell = shell;
    setMinimumSize(RULER_SIZE, RULER_SIZE);
}

void VerticalRuler::paintEvent(QPaintEvent *evt)
{
    QPainter painter(this);

    //background
    painter.setBrush(palette().background());
    QFont f = font();
    f.setPointSize(f.pointSize() - 1);
    painter.setFont(f);
    painter.drawRect(evt->rect());

    int anchorY = shell->getWorkspace()->parentWidget()->pos().y() + 1;
    double pixelSize = shell->getWorkspace()->getScale();
    double step = 1;
    int mul = 2;
    while (pixelSize * step < 10) {
        step *= mul;
        mul = 7 - mul;
    }

    //minor ticks
    double start = anchorY;
    while (start >= evt->rect().y())
        start -= pixelSize * step;

    int end = evt->rect().y() + evt->rect().height();
    for (double pos = start; pos <= end; pos += step * pixelSize)
        painter.drawLine(2 * RULER_SIZE / 3, (int)pos, RULER_SIZE, pos);

    //major ticks
    mul = 7 - mul;
    start = anchorY;
    while (start >= evt->rect().y())
        start -= step * mul * pixelSize;

    end = evt->rect().y() + evt->rect().height();
    for (double pos = start; pos <= end; pos += step * pixelSize * mul) {
        painter.drawLine(RULER_SIZE / 3, (int)pos, RULER_SIZE, pos);
        QString label;
        label.setNum((int)((pos - anchorY) / pixelSize));
        painter.drawText(QRect(0, (int)pos, RULER_SIZE, 0), Qt::AlignTop | Qt::AlignLeft | Qt::TextDontClip, label);
    }
    painter.end();
}
