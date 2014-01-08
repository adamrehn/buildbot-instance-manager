/*
//  Buildbot Instance Manager
//  Copyright (c) 2014, Adam Rehn
//
//  ---
//
//  List adapter for the list of buildmaster instances.
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
#include "BuildmasterListAdapter.h"

BuildmasterListAdapter::BuildmasterListAdapter(BuildbotInstanceScanner* instances, QObject *parent) : QAbstractTableModel(parent)
{
	this->instances = instances;
}

QVariant BuildmasterListAdapter::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		switch (section)
		{
			case 0:
				return QVariant( QString("Name") );
			
			case 1:
				return QVariant( QString("Status") );
			
			case 2:
				return QVariant( QString("Directory") );
			
			default:
				return QVariant();
		}
	}
	
	return QVariant();
}

int BuildmasterListAdapter::rowCount(const QModelIndex&) const {
	return this->instances->NumMasters();
}

int BuildmasterListAdapter::columnCount(const QModelIndex&) const {
	return 3;
}

QVariant BuildmasterListAdapter::data(const QModelIndex& index, int role) const
{
	//We support only the "display" role
	if (role != Qt::DisplayRole) {
		return QVariant();
	}
	
	Buildmaster& instance = this->instances->MasterAtIndex(index.row());
	
	switch (index.column())
	{
		case 0:
			return QVariant( instance.GetProjectTitle() );
			
		case 1:
			return QVariant( BuildbotInstanceState::toString(instance.GetState()) );
		
		case 2:
			return QVariant( instance.GetDirectory() );
		
		default:
			return QVariant();
	}
}
