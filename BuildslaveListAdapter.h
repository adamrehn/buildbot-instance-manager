/*
//  Buildbot Instance Manager
//  Copyright (c) 2014, Adam Rehn
//
//  ---
//
//  List adapter for the list of buildslave instances.
//
//  ---
//
//  This file is part of Buildbot Instance Manager.
//
//  Buildbot Instance Manager is free software: you can redistribute it and/or
//  modify it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along with
//  Buildbot Instance Manager. If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef _BUILDBOT_INSTANCE_MANAGER_BUILDSLAVE_LIST_ADAPTER
#define _BUILDBOT_INSTANCE_MANAGER_BUILDSLAVE_LIST_ADAPTER

#include <QAbstractTableModel>

#include "BuildbotInstanceScanner.h"

class BuildslaveListAdapter : public QAbstractTableModel
{
        Q_OBJECT

    public:
        explicit BuildslaveListAdapter(BuildbotInstanceScanner* instances, QObject *parent = 0);
		
		QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
        int rowCount(const QModelIndex& parent = QModelIndex()) const;
        int columnCount(const QModelIndex& parent = QModelIndex()) const;
        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
		
	private:
		BuildbotInstanceScanner* instances;
};

#endif
