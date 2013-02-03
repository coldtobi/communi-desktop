/*
* Copyright (C) 2008-2013 Communi authors
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

#ifndef GENERALWIZARDPAGE_H
#define GENERALWIZARDPAGE_H

#include "ui_generalwizardpage.h"
#include "settings.h"

class GeneralWizardPage : public QWizardPage
{
    Q_OBJECT

public:
    GeneralWizardPage(QWidget* parent = 0);

    QFont font() const;
    void setFont(const QFont& font);

    QString language() const;
    void setLanguage(const QString& language);

    int maxBlockCount() const;
    void setMaxBlockCount(int count);

    bool timeStamp() const;
    void setTimeStamp(bool timeStamp);

    QString timeStampFormat() const;
    void setTimeStampFormat(const QString& format);

    bool stripNicks() const;
    void setStripNicks(bool strip);

private:
    Ui::GeneralWizardPage ui;
};

#endif // GENERALWIZARDPAGE_H
