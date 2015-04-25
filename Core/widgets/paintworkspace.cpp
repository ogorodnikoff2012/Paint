#include "headers/paintworkspace.h"
#include "headers/mainwindow.h"
#include "headers/workspaceshell.h"
#include "headers/autosaver.h"
#include "headers/action.h"

#include <QtCore>
#include <QtGui>
#include <QtWidgets>


const double PaintWorkspace::minScale = 0.03125; // 2^(-5)
const double PaintWorkspace::maxScale = 32; // 2^(5)

PaintWorkspace::PaintWorkspace(WorkspaceShell *shell, QWidget *parent,
                               int width, int height, bool transparent) :
    QWidget(parent)
{
    this->shell = shell;
    initWorkspace(width, height);
    addNewLayer(0, transparent ? Transparent : White, QColor(Qt::white), false);
}

PaintWorkspace::PaintWorkspace(WorkspaceShell *shell, const QString &filename)
{
    this->shell = shell;
    open(filename);
    initWorkspace(layers[0]->width(), layers[0]->height());
    hasFile = true;
    autoSave();
}

PaintWorkspace::~PaintWorkspace()
{
    foreach(QImage * img, layers)
    {
        delete img;
    }
    delete change;
    foreach (Action * action, undoStack)
    {
        delete action;
    }
}

void PaintWorkspace::initWorkspace(int width, int height)
{
    curLayer = 0;
    scale = 1;
    setMinimumSize(width, height);
    layerWidth = width;
    layerHeight = height;
    change = 0;
    layerCounter = 0;
    mouseBlocker = 0;
    hasFile = false;
    changed = false;
    undoPointer = -1;
    setMouseTracking(true);
}

QImage *PaintWorkspace::newChange(bool get_layer)
{
    delete change;
    change = new QImage(layerWidth, layerHeight,
                        QImage::Format_ARGB32_Premultiplied);
    change->fill(Qt::transparent);
    return get_layer ? layers[curLayer] : change;
}

void PaintWorkspace::applyChange()
{
    if (!change)
    {
        return;
    }

    mergeLayerAndChange();
    autoSave();
}

void PaintWorkspace::setName(const QString &name)
{
    this->name = name;
    emit nameChanged(QFileInfo(name).fileName());
}

void PaintWorkspace::addNewLayer(int layerIdx, const NewLayerType type,
                                 const QColor &color, bool saveToUndoStack)
{
    QImage *img = new QImage(layerWidth, layerHeight,
                             QImage::Format_ARGB32_Premultiplied);

    switch (type)
    {
        case Transparent:
            img->fill(Qt::transparent);
            break;

        case White:
            img->fill(Qt::white);
            break;

        default:
            img->fill(color);
            break;
    }

    if(layerIdx >= 0 && layerIdx < layers.size())
    {
        layers.insert(layerIdx, img);
        layerNames.insert(layerIdx, tr("Layer #") + QString().setNum(++layerCounter));
    }
    else
    {
        layers << img;
        layerNames << tr("Layer #") + QString().setNum(++layerCounter);
        layerIdx = layers.size() - 1;
    }

    if (saveToUndoStack)
        updateUndoStack(new Action(QVariant(QString().setNum(
                                                layerIdx).append("~").append(layerNames[layerIdx])), Action::LAYER_CREATED));

    curLayer = layerIdx;
    autoSave();
    update();
}

void PaintWorkspace::removeLayer(const int layerIdx, bool saveToUndoStack)
{
    if (layerIdx >= 0 && layerIdx < layers.size())
    {
        if (saveToUndoStack)
            updateUndoStack(new Action(QVariant(QString().setNum(
                                                    layerIdx).append("~").append(layerNames[layerIdx])), Action::LAYER_DELETED));

        layerNames.removeAt(layerIdx);
        layers.remove(layerIdx);
        if(curLayer == layers.size())
            curLayer--;
    }

    update();
}

void PaintWorkspace::renameLayer(const int layerIdx, const QString &newName,
                                 bool saveToUndoStack)
{
    if (layerIdx >= 0 && layerIdx < layers.size())
    {
        if (saveToUndoStack)
            updateUndoStack(new Action(QVariant(QString().setNum(
                                                    layerIdx).append("~").append(QString().setNum(
                                                            layerNames[layerIdx].length())).append("~").append(layerNames[layerIdx]).append(
                                                    newName)), Action::LAYER_RENAMED));

        layerNames[layerIdx] = newName;
    }
}

bool PaintWorkspace::save(bool forced)
{
    QString str;

    if (!hasFile)
    {
        str = QFileDialog::getSaveFileName(0, tr("Save File"),
                                           QDir::homePath() + QDir::separator() + name + ".png", getFilenameFilter());
    }
    else if (forced)
    {
        str = QFileDialog::getSaveFileName(0, tr("Save File"), name, getFilenameFilter());
    }
    else
    {
        str = name;
    }

    if (!str.isEmpty())
    {
        if (QFileInfo(str).suffix() == "qtp")
        {
            exportQtp(str);
        }
        else
        {
            QImage *img = new QImage(layerWidth, layerHeight,
                                     QImage::Format_ARGB32_Premultiplied);
            paint(img);
            img->save(str);
            delete img;
        }

        setName(str);
        changed = false;
        hasFile = true;
    }

    return !str.isEmpty();
}

void PaintWorkspace::open(const QString &str)
{
    if (!str.isEmpty())
    {
        if (QFileInfo(str).suffix() == "qtp")
        {
            importQtp(str);
        }
        else
        {
            QImage img(str);
            QImage *layer = new QImage(img.width(), img.height(),
                                       QImage::Format_ARGB32_Premultiplied);
            QPainter painter(layer);
            painter.drawImage(0, 0, img);
            painter.end();
            layers << layer;
            layerNames << layerNames << tr("Layer #") + QString().setNum(++layerCounter);
        }

        setName(str);
        changed = false;
        hasFile = true;
    }
}

void PaintWorkspace::paintEvent(QPaintEvent *event)
{
    QSize size((int)(scale * layerWidth), (int)(scale * layerHeight));
    setMinimumSize(size);
    parentWidget()->resize(size + QSize(2, 2));
    paint(this, event);
    shell->updateRulers();
    //qDebug() << parentWidget()->pos() + pos();
}

void PaintWorkspace::mousePressEvent(QMouseEvent *event)
{
    ++mouseBlocker;

    if (mouseBlocker - 1)
    {
        return;
    }

    coords = (event->pos() - QPoint(getScale() / 2, getScale() / 2)) / getScale();
    MainWindow::window->getInstrument()->mousePressed(event, this);
    update();
    shell->updateRulers();
}

void PaintWorkspace::mouseReleaseEvent(QMouseEvent *event)
{
    --mouseBlocker;

    if (mouseBlocker)
    {
        return;
    }

    coords = (event->pos() - QPoint(getScale() / 2, getScale() / 2)) / getScale();
    MainWindow::window->getInstrument()->mouseReleased(event, this);
    update();
    shell->updateRulers();

}

void PaintWorkspace::mouseMoveEvent(QMouseEvent *event)
{
    coords = (event->pos() - QPoint(getScale() / 2, getScale() / 2)) / getScale();

    if (mouseBlocker == 1)
    {
        MainWindow::window->getInstrument()->mouseMoved(event, this);
    }

    update();

    shell->updateRulers();
    MainWindow::window->updateIndicators();
}

void PaintWorkspace::wheelEvent(QWheelEvent *event)
{
    int delta = event->delta();
    int modifiers = event->modifiers();

    if (modifiers & Qt::ControlModifier)
    {
        changeScale(delta);
    }

    QWidget::wheelEvent(event);
}

void PaintWorkspace::paint(QPaintDevice *dev, QPaintEvent *event)
{
    QPainter painter;
    painter.begin(dev);

    if (event)
    {
        painter.fillRect(event->rect(), MainWindow::window->background);
    }

    if (event)
    {
        painter.scale(scale, scale);
    }

    for (int i = 0; i < layers.size(); ++i)
    {
        QImage *img = layers[i];

        if (i == curLayer && change)
        {
            MainWindow::window->getInstrument()->paintChange(painter, img, change);
        }
        else
        {
            painter.drawImage(0, 0, *img);
        }
    }

    painter.end();
}

void PaintWorkspace::exportQtp(const QString &filename)
{
    QTime before = QTime::currentTime();
    QFile file(filename);

    if (file.open(QIODevice::WriteOnly))
    {
        QDataStream stream(&file);
        stream.setVersion(QDataStream::Qt_5_0);
        stream << layers.size();
        foreach(QImage * img, layers)
        stream << *img;

        foreach(QString name, layerNames)
        stream << name;

        if (stream.status() != QDataStream::Ok)
        {
            QMessageBox::critical(0, tr("Error"), tr("Saving failed!"));
        }
    }
    else
    {
        QMessageBox::critical(0, tr("Error"), tr("Saving failed!"));
    }

    file.close();
    QTime after = QTime::currentTime();
    qDebug() << before.msecsTo(after);
}

void PaintWorkspace::importQtp(const QString &filename)
{
    layers.clear();
    layerNames.clear();
    layerCounter = 0;
    QFile file(filename);

    if (file.open(QIODevice::ReadOnly))
    {
        QDataStream stream(&file);
        stream.setVersion(QDataStream::Qt_5_0);
        int size = 0;
        stream >> size;

        for (int i = 0; i < size; ++i)
        {
            QImage img;
            stream >> img;
            QImage *layer = new QImage(img.width(), img.height(),
                                       QImage::Format_ARGB32_Premultiplied);
            QPainter painter(layer);
            painter.drawImage(0, 0, img);
            painter.end();
            layers << layer;
        }

        for (int i = 0; i < size; ++i)
        {
            QString str;
            stream >> str;
            layerNames << str;
        }

        if (stream.status() != QDataStream::Ok)
        {
            QMessageBox::critical(0, tr("Error"), tr("Saving failed!"));
        }
    }
    else
    {
        QMessageBox::critical(0, tr("Error"), tr("Saving failed!"));
    }

    file.close();
}

void PaintWorkspace::autoSave()
{
    AutoSaver *saver = new AutoSaver(curLayer, new QImage(*layers[curLayer]));
    saver->file = new QTemporaryFile;
    connect(saver, SIGNAL(finished()), saver, SLOT(deleteLater()));
    //connect(saver, SIGNAL(savingFinished(QTemporaryFile *)),
    //        SLOT(updateUndoStack(QTemporaryFile *)));
    updateUndoStack(saver->file);
    saver->start();
}

QString PaintWorkspace::getFilenameFilter()
{
    QStringList list;
    QList<QByteArray> formatList = QImageWriter::supportedImageFormats();

    for (int i = 0; i < formatList.size(); ++i)
    {
        list << formatList[i];
    }

    list << "qtp";

    QString filter;

    filter.append("Images (*.");
    filter.append(list.join(" *."));
    filter.append(")");

    foreach(QByteArray format, formatList)
    {
        QString str(format);
        filter.append(";;" + str.toUpper() + " image (*." + str + ")");
    }

    return filter;
}

void PaintWorkspace::zoomIn()
{
    double newScale = minScale;

    while (newScale <= getScale())
    {
        newScale *= 2.0;
    }

    setScale(newScale);
    update();
}

void PaintWorkspace::zoomOut()
{
    double newScale = maxScale;

    while (newScale >= getScale())
    {
        newScale /= 2.0;
    }

    setScale(newScale);
    update();
}

void PaintWorkspace::restoreZoom()
{
    setScale(1);
    update();
}

void PaintWorkspace::undo()
{
    if (!undoAvailable())
    {
        return;
    }

    undoPointer--;

    Action *a = undoStack[undoPointer];

    if (a->type == Action::LAYER_CHANGED)
    {
        QTemporaryFile *f = (QTemporaryFile *)(a->data.toULongLong());

        if (f->open())
        {
            QDataStream stream(f);
            stream.setVersion(QDataStream::Qt_5_0);
            int layerIndex = 0;
            stream >> layerIndex;

            QImage img;
            stream >> img;
            QImage *layer = new QImage(img);
            QImage *oldLayer = layers[layerIndex];
            layers[layerIndex] = layer;
            delete oldLayer;
        }
        else
        {
            QMessageBox::critical(0, tr("Error"), tr("Saving failed!"));
        }

        f->close();
    }
    else if (a->type == Action::LAYER_CREATED)
    {
        QString str = a->data.toString();
        int layerIdx = str.left(str.indexOf('~')).toInt();
        removeLayer(layerIdx, false);
    }
    else if (a->type == Action::LAYER_DELETED)
    {
        QString str = a->data.toString();
        int layerIdx = str.left(str.indexOf('~')).toInt();
        addNewLayer(layerIdx, Transparent, QColor(Qt::white), false);
    }
    else if (a->type == Action::LAYER_RENAMED)
    {
        QString str = a->data.toString();
        int firstTilda = str.indexOf('~');
        int secondTilda = str.indexOf('~', firstTilda);
        int layerIdx = str.left(firstTilda).toInt();
        int oldNameLength = str.mid(firstTilda, secondTilda - firstTilda).toInt();
        QString oldName = str.mid(secondTilda, oldNameLength);
        renameLayer(layerIdx, oldName, false);
    }

    update();
    shell->updateRulers();
}

void PaintWorkspace::redo()
{
    if (!redoAvailable())
    {
        return;
    }

    undoPointer++;

    Action *a = undoStack[undoPointer];

    if (a->type == Action::LAYER_CHANGED)
    {
        QTemporaryFile *f = (QTemporaryFile *)(a->data.toULongLong());

        if (f->open())
        {
            QDataStream stream(f);
            stream.setVersion(QDataStream::Qt_5_0);
            int layerIndex = 0;
            stream >> layerIndex;

            QImage img;
            stream >> img;
            QImage *layer = new QImage(img);
            QImage *oldLayer = layers[layerIndex];
            layers[layerIndex] = layer;
            delete oldLayer;
        }
        else
        {
            QMessageBox::critical(0, tr("Error"), tr("Saving failed!"));
        }

        f->close();
    }
    else if (a->type == Action::LAYER_DELETED)
    {
        QString str = a->data.toString();
        int layerIdx = str.left(str.indexOf('~')).toInt();
        removeLayer(layerIdx, false);
    }
    else if (a->type == Action::LAYER_CREATED)
    {
        QString str = a->data.toString();
        int layerIdx = str.left(str.indexOf('~')).toInt();
        addNewLayer(layerIdx, Transparent, QColor(Qt::white), false);
    }
    else if (a->type == Action::LAYER_RENAMED)
    {
        QString str = a->data.toString();
        int firstTilda = str.indexOf('~');
        int secondTilda = str.indexOf('~', firstTilda);
        int layerIdx = str.left(firstTilda).toInt();
        int oldNameLength = str.mid(firstTilda, secondTilda - firstTilda).toInt();
        QString newName = str.mid(secondTilda + oldNameLength);
        renameLayer(layerIdx, newName, false);
    }

    update();
    shell->updateRulers();
}

void PaintWorkspace::mergeLayerAndChange()
{
    QPainter painter;
    painter.begin(layers[curLayer]);
    painter.drawImage(0, 0, *change);
    painter.end();
    delete change;
    change = 0;

    if (!changed)
    {
        emit nameChanged("*" + QFileInfo(name).fileName());
    }

    changed = true;
}

void PaintWorkspace::updateUndoStack(QTemporaryFile *f)
{
    updateUndoStack(new Action(QVariant((qulonglong)f), Action::LAYER_CHANGED));
}

void PaintWorkspace::updateUndoStack(Action *a)
{
    undoPointer++;
    int newSize = undoPointer + 1;

    for (int i = newSize - 1; i < undoStack.size(); i++)
    {
        delete undoStack[i];
    }

    undoStack.resize(newSize - 1);
    undoStack.append(a);
}

void PaintWorkspace::changeScale(int direction)
{
    if (direction < 0)
    {
        zoomOut();
    }
    else if (direction > 0)
    {
        zoomIn();
    }
}

void PaintWorkspace::resetUndoStack()
{
    while(!undoStack.empty())
    {
        delete undoStack.back();
        undoStack.pop_back();
    }
    undoPointer = -1;
    autoSave();
}
