/*
//  Buildbot Instance Manager
//  Copyright (c) 2014, Adam Rehn
//
//  ---
//
//  This class represents an instance of a buildmaster.
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
#ifndef _BUILDBOT_INSTANCE_MANAGER_BUILDMASTER
#define _BUILDBOT_INSTANCE_MANAGER_BUILDMASTER

#include <QString>
#include <QProcess>

#include "BuildbotInstance.h"

class Buildmaster : public BuildbotInstance
{
	Q_OBJECT
	
    public:
        Buildmaster(const QString& directory, const QString& tacData);
		~Buildmaster();
		
		void Start();
		void Stop();
		
		QString GetProjectTitle() const;
		QString GetDirectory() const;
		
		BuildbotInstanceType::BIType GetType() const;
		
		//Executes the "checkconfig" command to verify the buildmaster's configuration file
		QString CheckConfig();
		
	protected slots:
		virtual void ProcessStarted();
		void StatusLogProcessStarted();
		
		void StatusLogReadStdOut();
		void StatusLogReadStdErr();
		
	private:
		QString directory;
		QString masterName;
		
		QProcess statusLogProcess;
		
		void ParseTacData(const QString& tacData);
};

#endif
