#include "headers\mainwindow.h"
#include "headers/plugin.h"
#include "headers/pencil.h"
#include "headers/workspaceshell.h"
#include "headers/paintworkspace.h"

#include <QtCore>
#include <QtGui>
#include <QtWidgets>


MainWindow* MainWindow::window = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    window = this;
    initBackgroundBrush();
    createGUI();
    resize(800, 600);
    setWindowTitle(tr("Paint"));
    loadPlugins();
}

QColor MainWindow::getFirstColor()
{
    return pal->getFirstColor();
}

QColor MainWindow::getSecondColor()
{
    return pal->getSecondColor();
}

void MainWindow::newFile()
{
    WorkspaceShell* s = new WorkspaceShell;
    area->addSubWindow(s);
    s->show();
}

void MainWindow::open()
{
    QStringList list = QFileDialog::getOpenFileNames(0, tr("Open files"), QDir::homePath(), PaintWorkspace::getFilter());
    foreach(QString str, list) {
        WorkspaceShell* s = new WorkspaceShell(0, 0, 0, str);
        area->addSubWindow(s);
        s->show();
    }

    setWindowState(Qt::WindowActive | windowState());
}

void MainWindow::save()
{
    PaintWorkspace* wsp = getCurrentWorkspace();
    if (wsp)
        wsp->save();
    setWindowState(Qt::WindowActive | windowState());
}

void MainWindow::saveAs()
{
    PaintWorkspace* wsp = getCurrentWorkspace();
    if (wsp)
        wsp->save(true);
    setWindowState(Qt::WindowActive | windowState());
}

void MainWindow::zoomIn()
{
    PaintWorkspace* wsp = getCurrentWorkspace();
    if (wsp)
        wsp->zoomIn();
}

void MainWindow::zoomOut()
{
    PaintWorkspace* wsp = getCurrentWorkspace();
    if (wsp)
        wsp->zoomOut();
}

void MainWindow::restoreZoom()
{
    PaintWorkspace* wsp = getCurrentWorkspace();
    if (wsp)
        wsp->restoreZoom();
}

void MainWindow::closeEvent(QCloseEvent *evt)
{
    foreach(QMdiSubWindow * sw, area->subWindowList()) {
        if (!sw->close()) {
            evt->ignore();
            return;
        } else
            delete sw;
    }
    evt->accept();
}

void MainWindow::loadPlugins()
{
    QDir pluginsDir(qApp->applicationDirPath());
#if defined(Q_OS_WIN)
    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
        pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
        pluginsDir.cdUp();
        pluginsDir.cdUp();
        pluginsDir.cdUp();
    }
#endif
    pluginsDir.cd("plugins");
    foreach(QString fileName, pluginsDir.entryList(QDir::Files)) {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *obj = pluginLoader.instance();
        if (obj) {
            Plugin* plugin = qobject_cast<Plugin *>(obj);
            if (plugin) {
                plugin->initPlugin(this);
            }
        }
    }
}

void MainWindow::createGUI()
{
    area = new QMdiArea;
    area->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    area->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(area);

    createInstrumentsToolBar();
    createMenuBar();
    createPalette();

    statusBar()->showMessage(tr("Ready"), 2000);
}

void MainWindow::createInstrumentsToolBar()
{
    QToolBar* tBar = new QToolBar(tr("Instruments"));
    instrumentBar = tBar;
    Pencil::instance->init();
    tBar->addAction(createAction(Pencil::instance));

    setCurrentInstrument(Pencil::instance);
    addToolBar(Qt::LeftToolBarArea, tBar);
}

void MainWindow::createMenuBar()
{
    QMenu* mFile = new QMenu(tr("File"));

    QAction* aNew = new QAction(tr("New"), 0);
    aNew->setIcon(QIcon(":icons/new.png"));
    aNew->setShortcut(QKeySequence(QKeySequence::New));
    connect(aNew, SIGNAL(triggered()), SLOT(newFile()));

    QAction* aOpen = new QAction(tr("Open"), 0);
    aOpen->setIcon(QIcon(":icons/open.png"));
    aOpen->setShortcut(QKeySequence(QKeySequence::Open));
    connect(aOpen, SIGNAL(triggered()), SLOT(open()));

    QAction* aSave = new QAction(tr("Save"), 0);
    aSave->setIcon(QIcon(":icons/save.png"));
    aSave->setShortcut(QKeySequence(QKeySequence::Save));
    connect(aSave, SIGNAL(triggered()), SLOT(save()));

    QAction* aSaveAs = new QAction(tr("Save as"), 0);
    aSaveAs->setShortcut(QKeySequence(QKeySequence::SaveAs));
    connect(aSaveAs, SIGNAL(triggered()), SLOT(saveAs()));

    QAction* aExit = new QAction(tr("Exit"), 0);
    aExit->setIcon(QIcon(":icons/exit.png"));
    aExit->setShortcut(QKeySequence(QKeySequence::Quit));
    connect(aExit, SIGNAL(triggered()), SLOT(close()));

    mFile->addAction(aNew);
    mFile->addAction(aOpen);
    mFile->addAction(aSave);
    mFile->addAction(aSaveAs);
    mFile->addSeparator();
    mFile->addAction(aExit);

    QMenu* mView = new QMenu(tr("View"));

    QAction* aZoomIn = new QAction(tr("Zoom in"), 0);
    aZoomIn->setIcon(QIcon(":icons/zoom_in.png"));
    aZoomIn->setShortcut(QKeySequence(QKeySequence::ZoomIn));
    connect(aZoomIn, SIGNAL(triggered()), SLOT(zoomIn()));

    QAction* aZoomOut = new QAction(tr("Zoom out"), 0);
    aZoomOut->setIcon(QIcon(":icons/zoom_out.png"));
    aZoomOut->setShortcut(QKeySequence(QKeySequence::ZoomOut));
    connect(aZoomOut, SIGNAL(triggered()), SLOT(zoomOut()));

    QAction* aRestoreZoom = new QAction(tr("Restore zoom"), 0);
    connect(aRestoreZoom, SIGNAL(triggered()), SLOT(restoreZoom()));

    QAction* aCascade = new QAction(tr("Cascade"), 0);
    connect(aCascade, SIGNAL(triggered()), area, SLOT(cascadeSubWindows()));

    QAction* aTile = new QAction(tr("Tile"), 0);
    connect(aTile, SIGNAL(triggered()), area, SLOT(tileSubWindows()));

    mView->addAction(aZoomIn);
    mView->addAction(aZoomOut);
    mView->addAction(aRestoreZoom);
    mView->addSeparator();
    mView->addAction(aCascade);
    mView->addAction(aTile);

    menuBar()->addMenu(mFile);
    menuBar()->addMenu(mView);
}

void MainWindow::createPalette()
{
    QDockWidget* dock = new QDockWidget;
    pal = new Palette;
    dock->setWidget(pal);
    addDockWidget(Qt::BottomDockWidgetArea, dock);
}

void MainWindow::setCurrentInstrument(Instrument *i)
{
    if (!i)
        return;
    currentInstrument = i;
    area->setCursor(*(i->cursor));
    qDebug() << "Instrument selected:" << i->getName();
}

PaintWorkspace *MainWindow::getCurrentWorkspace()
{
    QMdiSubWindow* sw = area->currentSubWindow();
    if (!sw)
        return 0;
    QWidget* w = sw->widget();
    WorkspaceShell* sh = qobject_cast<WorkspaceShell *>(w);
    if (!sh)
        return 0;
    return sh->getWorkspace();
}

QAction* MainWindow::createAction(Instrument *instrument)
{
    QAction* action = new QAction(0);
    action->setText(instrument->getName());
    action->setIcon(*(instrument->icon));
    action->setData(toQVariant(instrument));
    connect(action, SIGNAL(triggered()), this, SLOT(changeInstrument()));
    return action;
}

void MainWindow::initBackgroundBrush()
{
    QImage* backgroundImage = new QImage(16, 16, QImage::Format_RGB32);
    QPainter backgroundPainter(backgroundImage);
    backgroundPainter.setBrush(QBrush(Qt::lightGray));
    backgroundPainter.setPen(Qt::NoPen);
    backgroundPainter.drawRect(0, 0, 8, 8);
    backgroundPainter.drawRect(8, 8, 8, 8);
    backgroundPainter.setBrush(QBrush(Qt::gray));
    backgroundPainter.drawRect(0, 8, 8, 8);
    backgroundPainter.drawRect(8, 0, 8, 8);
    backgroundPainter.end();

    background = QBrush(*backgroundImage);
}
