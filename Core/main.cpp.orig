#include <headers/mainwindow.h>
#include <headers/palette.h>

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;

    QString str = QLocale::system().name();
    if(str.startsWith("ru"))
        translator.load("translations/ru.qm");
    a.installTranslator(&translator);

    MainWindow* window = new MainWindow;
    window->show();
    int openedFiles = 0;
    for (int i = 1; i < argc; ++i)
    {
        QString filename = QString::fromLocal8Bit(argv[i]);
        qDebug() << filename;
        if(QFileInfo(filename).exists())
        {
            window->open(filename);
            openedFiles++;
        }
    }

    if(!openedFiles)
    {
        window->newFile(800, 600);
    }

    //qDebug() << QImageWriter::supportedImageFormats();
    return a.exec();
}
