#include "myplugin.h"
#include <QString>

void MyPlugin::initPlugin(MainWindow *window)
{
    window->setWindowTitle("Plugin works!");
}
