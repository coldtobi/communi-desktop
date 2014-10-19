/*
* Copyright (C) 2008-2014 The Communi Project
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*/

#ifndef WINDOWSPLUGIN_H
#define WINDOWSPLUGIN_H

#include <QObject>
#include <QtPlugin>
#include "dockplugin.h"

class WindowsPlugin : public QObject, public DockPlugin
{
    Q_OBJECT
    Q_INTERFACES(DockPlugin)
    Q_PLUGIN_METADATA(IID "Communi.DockPlugin")

public:
    WindowsPlugin(QObject* parent = 0);

    void setupTrayIcon(QSystemTrayIcon* tray);
    void dockAlert(IrcMessage* message);

private:
    struct Private {
        QSystemTrayIcon* tray;
    } d;
};

#endif // WINDOWSPLUGIN_H
