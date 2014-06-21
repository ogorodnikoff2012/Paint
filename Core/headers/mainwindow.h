#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVariant>
#include <QAction>
#include "instrument.h"
#include "palette.h"

class QToolBar;
class QMdiArea;
class QColor;
class QCloseEvent;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    static QVariant toQVariant(const Instrument* instr)
    {
        return QVariant((uint) instr);
    }
    static Instrument* toInstrument(const QVariant& var)
    {
        Instrument* i = (Instrument*)var.toUInt();
        return i;
    }
    static MainWindow* window;
    Instrument* getInstrument()
    {
        return currentInstrument;
    }
    QColor getFirstColor();
    QColor getSecondColor();

    void initBackgroundBrush();
    QBrush background;

signals:

public slots:
    void changeInstrument()
    {
        QAction* action = qobject_cast<QAction *>(sender());
        if (!action)
            return;
        Instrument* i = toInstrument(action->data());
        setCurrentInstrument(i);
    }
    void newFile();
    void open();
    void save();
    void saveAs();
    void zoomIn();
    void zoomOut();
    void restoreZoom();

protected:
    void closeEvent(QCloseEvent* evt);

private:
    void loadPlugins();
    void createGUI();
    void createInstrumentsToolBar();
    void createMenuBar();
    void createPalette();
    void setCurrentInstrument(Instrument* i);
    PaintWorkspace* getCurrentWorkspace();
    QToolBar* instrumentBar;
    Instrument* currentInstrument;
    QAction* createAction(Instrument* instrument);
    QMdiArea* area;
    Palette* pal;
};

#endif // MAINWINDOW_H
