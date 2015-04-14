#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVariant>
#include <QAction>
#include <QString>
#include <QVector>
#include "instrument.h"
#include "palette.h"

class QToolBar;
class QMdiArea;
class QColor;
class QCloseEvent;
class QLabel;
class QDialog;
class QSpinBox;
class QCheckBox;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    static QVariant toQVariant(const Instrument *instr)
    {
        return QVariant(instr->index);
    }
    static Instrument *toInstrument(const QVariant &var)
    {
        Instrument *i = window->instruments[var.toInt()];
        return i;
    }
    static MainWindow *window;
    Instrument *getInstrument()
    {
        return currentInstrument;
    }
    QColor getFirstColor();
    QColor getSecondColor();

    void initBackgroundBrush();
    void updateIndicators();
    QBrush background;
    void open(const QString &filename);
    PaintWorkspace *newFile(int width, int height, bool transparent = false);
    void loadInstrument(Instrument *);

signals:

public slots:
    void changeInstrument()
    {
        QAction *action = qobject_cast<QAction *>(sender());

        if (!action)
        {
            return;
        }

        Instrument *i = toInstrument(action->data());
        setCurrentInstrument(i);
    }
    void newFile();
    void open();
    void save();
    void saveAs();
    void zoomIn();
    void zoomOut();
    void restoreZoom();
    void undo();
    void redo();

protected:
    void closeEvent(QCloseEvent *evt);

protected slots:
    void updateUndoButtons();
    void enableUndoButtons();

private:
    void loadPlugins();
    void createGUI();
    void createInstrumentsToolBar();
    void createMenuBar();
    void createPalette();
    void createStatusBar();
    void setCurrentInstrument(Instrument *i);
    QDialog *createNewFileDialog(QSpinBox *width, QSpinBox *height,
                                 QCheckBox *transparentBackground);
    PaintWorkspace *getCurrentWorkspace();
    QToolBar *instrumentBar;
    Instrument *currentInstrument;
    QAction *createAction(Instrument *instrument);
    QMdiArea *area;
    Palette *pal;
    QAction *aUndo;
    QAction *aRedo;
    QLabel *mouseCoords, *statusInformation;
    QVector<Instrument *> instruments;
};

#endif // MAINWINDOW_H
