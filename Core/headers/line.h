#ifndef LINE_H
#define LINE_H

#include <QString>
#include "instrument.h"
#include <QPoint>
#include <QColor>
#include <QObject>

class QSpinBox;
class QComboBox;

class Line : Instrument
{
public:
    void mousePressed(QMouseEvent *evt, PaintWorkspace *wsp);
    void mouseMoved(QMouseEvent *evt, PaintWorkspace *wsp);
    void mouseReleased(QMouseEvent *evt, PaintWorkspace *wsp);
    void paintChange(QPainter &painter, QImage *layer, QImage *change);
    void initSettingsDockWidget();
    static Instrument *instance;
    const QString getName()
    {
        return QObject::tr("Line");
    }
    void init();
protected:
    QPoint start, end;
    QColor col;
    QSpinBox *size;
    QComboBox *endStyle;
};

#endif // LINE_H
