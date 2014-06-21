#ifndef HORIZONTALRULER_H
#define HORIZONTALRULER_H

#include "ruler.h"
#include <QWidget>

class QPaintEvent;
class WorkspaceShell;

class HorizontalRuler : public QWidget
{
    Q_OBJECT
public:
    explicit HorizontalRuler(WorkspaceShell* shell, QWidget *parent = 0);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent* evt);
    WorkspaceShell* shell;
};

#endif // HORIZONTALRULER_H
