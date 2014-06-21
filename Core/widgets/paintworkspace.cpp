#include "headers\paintworkspace.h"
#include "headers/mainwindow.h"
#include "headers/workspaceshell.h"

#include <QtCore>
#include <QtGui>
#include <QtWidgets>


const double PaintWorkspace::minScale = 0.03125; // 2^(-5)
const double PaintWorkspace::maxScale = 32; // 2^(5)

PaintWorkspace::PaintWorkspace(WorkspaceShell* shell, QWidget *parent, int width, int height) :
    QWidget(parent)
{
    this->shell = shell;
    initWorkspace(width, height);
    addNewLayer(White);
}

PaintWorkspace::PaintWorkspace(WorkspaceShell* shell, const QString &filename)
{
    this->shell = shell;
    open(filename);
    initWorkspace(layers[0]->width(), layers[0]->height());
    hasFile = true;
}

PaintWorkspace::~PaintWorkspace()
{
    foreach(QImage * img, layers) {
        delete img;
    }
    delete change;
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
}

QImage *PaintWorkspace::newChange()
{
    delete change;
    change = new QImage(layerWidth, layerHeight, QImage::Format_ARGB32_Premultiplied);
    change->fill(Qt::transparent);
    return change;
}

void PaintWorkspace::applyChange()
{
    if (!change)
        return;
    QPainter painter;
    painter.begin(layers[curLayer]);
    painter.drawImage(0, 0, *change);
    painter.end();
    delete change;
    change = 0;
    if (!changed)
        emit nameChanged("*" + QFileInfo(name).fileName());
    changed = true;
}

void PaintWorkspace::setName(const QString &name)
{
    this->name = name;
    emit nameChanged(QFileInfo(name).fileName());
}

void PaintWorkspace::addNewLayer(const NewLayerType type, const QColor &color)
{
    QImage* img = new QImage(layerWidth, layerHeight, QImage::Format_ARGB32_Premultiplied);
    switch (type) {
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
    layers << img;
    layerNames << tr("Layer #") + QString().setNum(++layerCounter);
    repaint();
}

bool PaintWorkspace::save(bool forced)
{
    QString str;
    if (!hasFile)
        str = QFileDialog::getSaveFileName(0, tr("Save File"), QDir::homePath() + QDir::separator() + name + ".png", getFilter());
    else if (forced)
        str = QFileDialog::getSaveFileName(0, tr("Save File"), name, getFilter());
    else
        str = name;
    if (!str.isEmpty()) {
        if (QFileInfo(str).suffix() == "qtp")
            exportQtp(str);
        else {
            QImage* img = new QImage(layerWidth, layerHeight, QImage::Format_ARGB32_Premultiplied);
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

void PaintWorkspace::open(const QString& str)
{
    if (!str.isEmpty()) {
        if (QFileInfo(str).suffix() == "qtp")
            importQtp(str);
        else {
            QImage img(str);
            QImage* layer = new QImage(img.width(), img.height(), QImage::Format_ARGB32_Premultiplied);
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
        return;
    MainWindow::window->getInstrument()->mousePressed(event, this);
    update();
    shell->updateRulers();
}

void PaintWorkspace::mouseReleaseEvent(QMouseEvent *event)
{
    --mouseBlocker;
    if (mouseBlocker)
        return;
    MainWindow::window->getInstrument()->mouseReleased(event, this);
    update();
    shell->updateRulers();

}

void PaintWorkspace::mouseMoveEvent(QMouseEvent *event)
{
    MainWindow::window->getInstrument()->mouseMoved(event, this);
    update();
    shell->updateRulers();
}

void PaintWorkspace::wheelEvent(QWheelEvent *event)
{
    int delta = event->delta();
    int modifiers = event->modifiers();
    if (modifiers & Qt::ControlModifier)
        changeScale(delta);
    QWidget::wheelEvent(event);
}

void PaintWorkspace::paint(QPaintDevice *dev, QPaintEvent* event)
{
    QPainter painter;
    painter.begin(dev);
    if (event)
        painter.fillRect(event->rect(), MainWindow::window->background);
    if (event)
        painter.scale(scale, scale);
    for (int i = 0; i < layers.size(); ++i) {
        QImage *img = layers[i];
        if (i == curLayer && change)
            MainWindow::window->getInstrument()->paintChange(painter, img, change);
        else
            painter.drawImage(0, 0, *img);
    }

    painter.end();
}

void PaintWorkspace::exportQtp(const QString& filename)
{
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly)) {
        QDataStream stream(&file);
        stream.setVersion(QDataStream::Qt_5_0);
        stream << layers.size();
        foreach(QImage * img, layers)
        stream << *img;

        foreach(QString name, layerNames)
        stream << name;

        if (stream.status() != QDataStream::Ok)
            QMessageBox::critical(0, tr("Error"), tr("Saving failed!"));
    } else {
        QMessageBox::critical(0, tr("Error"), tr("Saving failed!"));
    }
    file.close();
}

void PaintWorkspace::importQtp(const QString &filename)
{
    layers.clear();
    layerNames.clear();
    layerCounter = 0;
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly)) {
        QDataStream stream(&file);
        stream.setVersion(QDataStream::Qt_5_0);
        int size = 0;
        stream >> size;
        for (int i = 0; i < size; ++i) {
            QImage img;
            stream >> img;
            QImage* layer = new QImage(img.width(), img.height(), QImage::Format_ARGB32_Premultiplied);
            QPainter painter(layer);
            painter.drawImage(0, 0, img);
            painter.end();
            layers << layer;
        }

        for (int i = 0; i < size; ++i) {
            QString str;
            stream >> str;
            layerNames << str;
        }

        if (stream.status() != QDataStream::Ok)
            QMessageBox::critical(0, tr("Error"), tr("Saving failed!"));
    } else {
        QMessageBox::critical(0, tr("Error"), tr("Saving failed!"));
    }
    file.close();
}

QString PaintWorkspace::getFilter()
{
    QStringList list;
    QList<QByteArray> formatList = QImageWriter::supportedImageFormats();

    for (int i = 0; i < formatList.size(); ++i)
        list << formatList[i];

    list << "qtp";

    QString filter;
    filter.append("Images (*.");
    filter.append(list.join(" *."));
    filter.append(")");

    return filter;
}

void PaintWorkspace::zoomIn()
{
    double newScale = minScale;
    while (newScale <= getScale())
        newScale *= 2.0;
    setScale(newScale);
    update();
}

void PaintWorkspace::zoomOut()
{
    double newScale = maxScale;
    while (newScale >= getScale())
        newScale /= 2.0;
    setScale(newScale);
    update();
}

void PaintWorkspace::restoreZoom()
{
    setScale(1);
    update();
}

void PaintWorkspace::changeScale(int direction)
{
    if (direction < 0)
        zoomOut();
    else if (direction > 0)
        zoomIn();
}
