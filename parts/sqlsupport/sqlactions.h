/***************************************************************************
 *   Copyright (C) 2003 by Harald Fernengel                                *
 *   harry@kdevelop.org                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _SQLACTION_H_
#define _SQLACTION_H_

#include <kaction.h>

//only for KDE < 3.1
#if KDE_VERSION <= 305
#include <kdevwidgetaction.h>
using namespace KDevCompat;
#endif

class SQLSupportPart;
class QComboBox;

class SqlListAction : public KWidgetAction
{
public:
    SqlListAction( SQLSupportPart *part, const QString &text, const KShortcut& cut,
                     const QObject *receiver, const char *slot,
                     KActionCollection *parent, const char *name );
    void setCurrentConnectionName(const QString &name);
    QString currentConnectionName() const;
    void refresh();
    
private:
    SQLSupportPart *m_part;
    QComboBox* m_combo;
};

#endif
