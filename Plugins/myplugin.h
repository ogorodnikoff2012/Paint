#ifndef MYPLUGIN_H
#define MYPLUGIN_H

#include <QObject>
#include <QtPlugin>
#include "plugin.h"

//! [0]
class MyPlugin : public QObject, Plugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID Plugin_iid FILE "myplugin.json")
    Q_INTERFACES(Plugin)

public:
    void initPlugin(MainWindow *window);
};
//! [0]

#endif // MYPLUGIN_H
