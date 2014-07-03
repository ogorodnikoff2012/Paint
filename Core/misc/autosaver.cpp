#include "headers\autosaver.h"
#include <QtCore>
#include <QtGui>

AutoSaver::AutoSaver(int layerIndex, QImage *img, QObject *parent) :
    QThread(parent)
{
    this->layerIndex = layerIndex;
    this->img = img;
}

void AutoSaver::run()
{
    QTemporaryFile* file = new QTemporaryFile;
    //qDebug() << QThread::currentThread();
    emit copyingFinished();

    if (file->open()) {
        QDataStream stream(file);
        stream.setVersion(QDataStream::Qt_5_0);
        stream << layerIndex;
        stream << *img;

        if (stream.status() != QDataStream::Ok)
            qDebug() << "Auto-saving failed!";
    } else {
        qDebug() << "Auto-saving failed!";
    }
    file->close();
    //img->save("preview.png");

    //qDebug() << file->fileName();

    emit savingFinished(file);
    delete img;
}
