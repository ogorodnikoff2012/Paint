#ifndef AUTOSAVER_H
#define AUTOSAVER_H

#include <QObject>
#include <QThread>

class QImage;
class QTemporaryFile;

class AutoSaver : public QThread
{
    Q_OBJECT
public:
    AutoSaver(int layerIndex, QImage* img, QObject *parent = 0);
    void run();

signals:
    void copyingFinished();
    void savingFinished(QTemporaryFile* f);

public slots:

protected:
    int layerIndex;
    QImage* img;
};

#endif // AUTOSAVER_H
