#ifndef PLUGIN_H
#define PLUGIN_H

#include "mainwindow.h"

class Plugin
{
public:
    virtual ~Plugin() { }
    virtual void initPlugin(MainWindow* window) = 0;
};

QT_BEGIN_NAMESPACE

#define Plugin_iid "xenon.Paint.Plugin"

Q_DECLARE_INTERFACE(Plugin, Plugin_iid)
QT_END_NAMESPACE

#endif // PLUGIN_H
