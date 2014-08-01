#ifndef BRUSH_H
#define BRUSH_H

#include <QString>
#include "instrument.h"
#include <QPoint>
#include <QColor>
#include <QObject>
#include <QVector>

class QSpinBox;
class QComboBox;

class Brush : Instrument
{
public:
    void mousePressed(QMouseEvent* evt, PaintWorkspace* wsp);
    void mouseMoved(QMouseEvent* evt, PaintWorkspace* wsp);
    void mouseReleased(QMouseEvent* evt, PaintWorkspace* wsp);
    void paintChange(QPainter& painter, QImage* layer, QImage* change);
    void initSettingsDockWidget();
    static Instrument* instance;
    const QString getName()
    {
        return QObject::tr("Brush");
    }
    void init();
protected:
    QVector<QPoint> polygon;
    QColor col;
    QSpinBox* size;
    QComboBox* endStyle;
};

#endif // BRUSH_H
