/*
* Copyright (C) 2008-2013 The Communi Project
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

#include "navigator.h"
#include "treerole.h"

Navigator::Navigator(QWidget* parent) : QObject(parent)
{
    setTreeWidget(qobject_cast<QTreeWidget*>(parent));

#ifdef Q_OS_MAC
    QString navigate(tr("Ctrl+Alt+%1"));
    QString nextActive(tr("Shift+Ctrl+Alt+%1"));
#else
    QString navigate(tr("Alt+%1"));
    QString nextActive(tr("Shift+Alt+%1"));
#endif

    d.prevShortcut = new QShortcut(parent);
    d.prevShortcut->setKey(QKeySequence(navigate.arg("Up")));
    connect(d.prevShortcut, SIGNAL(activated()), this, SLOT(moveToPrevItem()));

    d.nextShortcut = new QShortcut(parent);
    d.nextShortcut->setKey(QKeySequence(navigate.arg("Down")));
    connect(d.nextShortcut, SIGNAL(activated()), this, SLOT(moveToNextItem()));

    d.prevActiveShortcut = new QShortcut(parent);
    d.prevActiveShortcut->setKey(QKeySequence(nextActive.arg("Up")));
    connect(d.prevActiveShortcut, SIGNAL(activated()), this, SLOT(moveToPrevActiveItem()));

    d.nextActiveShortcut = new QShortcut(parent);
    d.nextActiveShortcut->setKey(QKeySequence(nextActive.arg("Down")));
    connect(d.nextActiveShortcut, SIGNAL(activated()), this, SLOT(moveToNextActiveItem()));

    d.expandShortcut = new QShortcut(parent);
    d.expandShortcut->setKey(QKeySequence(navigate.arg("Right")));
    connect(d.expandShortcut, SIGNAL(activated()), this, SLOT(expandCurrentConnection()));

    d.collapseShortcut = new QShortcut(parent);
    d.collapseShortcut->setKey(QKeySequence(navigate.arg("Left")));
    connect(d.collapseShortcut, SIGNAL(activated()), this, SLOT(collapseCurrentConnection()));

    d.mostActiveShortcut = new QShortcut(parent);
    d.mostActiveShortcut->setKey(QKeySequence(tr("Ctrl+L")));
    connect(d.mostActiveShortcut, SIGNAL(activated()), this, SLOT(moveToMostActiveItem()));
}

QTreeWidget* Navigator::treeWidget() const
{
    return d.tree;
}

void Navigator::setTreeWidget(QTreeWidget* widget)
{
    d.tree = widget;
}

QTreeWidgetItem* Navigator::currentItem() const
{
    if (d.tree)
        return d.tree->currentItem();
    return 0;
}

void Navigator::setCurrentItem(QTreeWidgetItem* item)
{
    if (d.tree)
        d.tree->setCurrentItem(item);
}

QTreeWidgetItem* Navigator::topLevelItem(int index) const
{
    if (d.tree) {
        if (index == -1)
            index = d.tree->topLevelItemCount() - 1;
        return d.tree->topLevelItem(index);
    }
    return 0;
}

void Navigator::expandItem(QTreeWidgetItem* item)
{
    if (d.tree)
        d.tree->expandItem(item);
}

void Navigator::collapseItem(QTreeWidgetItem* item)
{
    if (d.tree)
        d.tree->collapseItem(item);
}

QTreeWidgetItem* Navigator::lastItem() const
{
    QTreeWidgetItem* item = topLevelItem(-1);
    if (item->childCount() > 0)
        item = item->child(item->childCount() - 1);
    return item;
}

QTreeWidgetItem* Navigator::nextItem(QTreeWidgetItem* from) const
{
    if (!from)
        return 0;
    QTreeWidgetItemIterator it(from);
    while (*++it) {
        if (!(*it)->parent() || (*it)->parent()->isExpanded())
            break;
    }
    return *it;
}

QTreeWidgetItem* Navigator::previousItem(QTreeWidgetItem* from) const
{
    if (!from)
        return 0;
    QTreeWidgetItemIterator it(from);
    while (*--it) {
        if (!(*it)->parent() || (*it)->parent()->isExpanded())
            break;
    }
    return *it;
}

QTreeWidgetItem* Navigator::findNextItem(QTreeWidgetItem* from, int column, int role) const
{
    if (from) {
        QTreeWidgetItemIterator it(from);
        while (*++it && *it != from) {
            QTreeWidgetItem* item = *it;
            if (item->data(column, role).toBool())
                return item;
        }
    }
    return 0;
}

QTreeWidgetItem* Navigator::findPrevItem(QTreeWidgetItem* from, int column, int role) const
{
    if (from) {
        QTreeWidgetItemIterator it(from);
        while (*--it && *it != from) {
            QTreeWidgetItem* item = *it;
            if (item->data(column, role).toBool())
                return item;
        }
    }
    return 0;
}

void Navigator::moveToNextItem()
{
    QTreeWidgetItem* item = nextItem(currentItem());
    if (!item)
        item = topLevelItem(0);
    setCurrentItem(item);
}

void Navigator::moveToPrevItem()
{
    QTreeWidgetItem* item = previousItem(currentItem());
    if (!item)
        item = lastItem();
    setCurrentItem(item);
}

void Navigator::moveToNextActiveItem()
{
    QTreeWidgetItem* item = findNextItem(currentItem(), 0, TreeRole::Highlight);
    if (!item)
        item = findNextItem(currentItem(), 1, TreeRole::Badge);
    if (item)
        setCurrentItem(item);
}

void Navigator::moveToPrevActiveItem()
{
    QTreeWidgetItem* item = findPrevItem(currentItem(), 0, TreeRole::Highlight);
    if (!item)
        item = findPrevItem(currentItem(), 1, TreeRole::Badge);
    if (item)
        setCurrentItem(item);
}

void Navigator::moveToMostActiveItem()
{
    QTreeWidgetItem* mostActive = 0;
    QTreeWidgetItemIterator it(d.tree, QTreeWidgetItemIterator::Unselected);
    while (*it) {
        QTreeWidgetItem* item = *it;

        if (item->data(0, TreeRole::Highlight).toBool()) {
            // we found a channel hilight or PM to us
            setCurrentItem(item);
            return;
        }

        // as a backup, store the most active window with any sort of activity
        const int badge = item->data(0, TreeRole::Badge).toInt();
        if (badge > 0 && (!mostActive || mostActive->data(0, TreeRole::Badge).toInt() < badge))
            mostActive = item;

        it++;
    }

    if (mostActive)
        setCurrentItem(mostActive);
}

void Navigator::expandCurrentConnection()
{
    QTreeWidgetItem* item = currentItem();
    if (item && item->parent())
        item = item->parent();
    if (item)
        expandItem(item);
}

void Navigator::collapseCurrentConnection()
{
    QTreeWidgetItem* item = currentItem();
    if (item && item->parent())
        item = item->parent();
    if (item) {
        collapseItem(item);
        setCurrentItem(item);
    }
}
