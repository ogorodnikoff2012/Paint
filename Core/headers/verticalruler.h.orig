#ifndef VERTICALRULER_H
#define VERTICALRULER_H

#include "ruler.h"
#include <QWidget>

class QPaintEvent;
class WorkspaceShell;

class VerticalRuler : public QWidget
{
    Q_OBJECT
public:
    explicit VerticalRuler(WorkspaceShell* shell, QWidget *parent = 0);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent* evt);
    WorkspaceShell* shell;
};

#endif // VERTICALRULER_H
