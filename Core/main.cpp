#include <headers/mainwindow.h>
#include <headers/palette.h>

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    for (int i = 0; i < argc; ++i)
        qDebug() << argv[i];
    QTranslator translator;

    QString str = QLocale::system().name();
    if(str.startsWith("ru"))
        translator.load("translations/ru.qm");
    a.installTranslator(&translator);

    MainWindow* window = new MainWindow;
    window->show();

    //qDebug() << QImageWriter::supportedImageFormats();
    return a.exec();
}
