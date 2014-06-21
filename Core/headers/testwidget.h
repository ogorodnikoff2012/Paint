#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QEvent>
#include <QPoint>
#include <QPixmap>
#include <QResizeEvent>

class TestWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TestWidget(QWidget *parent = 0);

signals:

public slots:

protected:
    virtual void paintEvent(QPaintEvent*);
    virtual void mouseMoveEvent(QMouseEvent*);
    virtual void leaveEvent(QEvent*);
    virtual void resizeEvent(QResizeEvent*);
    QPoint pt;
    QPixmap* pixmap;
};

#endif // TESTWIDGET_H
