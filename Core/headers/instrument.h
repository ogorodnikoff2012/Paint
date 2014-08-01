#ifndef INSTRUMENT_H
#define INSTRUMENT_H

class QMouseEvent;
class QIcon;
class QCursor;
class PaintWorkspace;
class QPainter;
class QImage;
class QDockWidget;

class Instrument
{
public:
    virtual void mousePressed(QMouseEvent *evt, PaintWorkspace *wsp) = 0;
    virtual void mouseMoved(QMouseEvent *evt, PaintWorkspace *wsp) = 0;
    virtual void mouseReleased(QMouseEvent *evt, PaintWorkspace *wsp) = 0;
    virtual void paintChange(QPainter &painter, QImage *layer, QImage *change) = 0;
    virtual void initSettingsDockWidget() = 0;
    virtual const QString getName() = 0;
    virtual void init() = 0;
    static Instrument *instance;
    QIcon *icon;
    QCursor *cursor;
    QDockWidget *settings;
};

#endif // INSTRUMENT_H
