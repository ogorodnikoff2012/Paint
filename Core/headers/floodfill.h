#ifndef FLOODFILL_H
#define FLOODFILL_H

#include "instrument.h"

class FloodFill : Instrument
{
public:
    void mousePressed(QMouseEvent *evt, PaintWorkspace *wsp);
    void mouseMoved(QMouseEvent *evt, PaintWorkspace *wsp);
    void mouseReleased(QMouseEvent *evt, PaintWorkspace *wsp);
    void paintChange(QPainter &painter, QImage *layer, QImage *change);
    void initSettingsDockWidget();
    const QString getName();
    void init();
    static Instrument *instance;
};

#endif // FLOODFILL_H
