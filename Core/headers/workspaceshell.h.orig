#ifndef WORKSPACESHELL_H
#define WORKSPACESHELL_H

#include <QWidget>
#include "paintworkspace.h"
#include <QString>
#include "verticalruler.h"
#include "horizontalruler.h"
#include <QVector>

class QCloseEvent;

class WorkspaceShell : public QWidget
{
    Q_OBJECT
public:
    explicit WorkspaceShell(QWidget *parent = 0, const int width = 800, const int height = 600, const QString &str = "");
    PaintWorkspace* getWorkspace()
    {
        return wsp;
    }
    void updateRulers()
    {
        vRuler->update();
        hRuler->update();
    }

signals:

public slots:

protected:
    void closeEvent(QCloseEvent *evt);
    //void wheelEvent(QWheelEvent *evt);
    VerticalRuler* vRuler;
    HorizontalRuler* hRuler;
    PaintWorkspace* wsp;
    void initWorkspace(const int width, const int height, const QString& str);
    static int untitledCounter;
    static int nextUntitled()
    {
        return ++untitledCounter;
    }
};

#endif // WORKSPACESHELL_H
