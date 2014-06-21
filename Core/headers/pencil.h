#ifndef PENCIL_H
#define PENCIL_H

#include <QString>
#include "instrument.h"
#include <QPoint>
#include <QColor>

class Pencil : Instrument
{
public:
    void mousePressed(QMouseEvent* evt, PaintWorkspace* wsp);
    void mouseMoved(QMouseEvent* evt, PaintWorkspace* wsp);
    void mouseReleased(QMouseEvent* evt, PaintWorkspace* wsp);
    void paintChange(QPainter& painter, QImage* layer, QImage* change);
    static Instrument* instance;
    const QString getName()
    {
        return QObject::tr("Pencil");
    }
    void init();
protected:
    QPoint lastPos;
    QColor col;
};

#endif // PENCIL_H
