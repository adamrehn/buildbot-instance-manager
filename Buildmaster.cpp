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
#include "Buildmaster.h"

#include "Utility.h"

Buildmaster::Buildmaster(const QString& directory, const QString& tacData)
{
	this->directory = directory;
	this->ParseTacData(tacData);
	
	//Connect the signals of the status log QProcess objects to our handlers
	QObject::connect(&this->statusLogProcess, SIGNAL(started()), this, SLOT(StatusLogProcessStarted()));
	QObject::connect(&this->statusLogProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(StatusLogReadStdOut()));
	QObject::connect(&this->statusLogProcess, SIGNAL(readyReadStandardError()), this, SLOT(StatusLogReadStdErr()));
}

Buildmaster::~Buildmaster()
{
	if (this->GetState() == BuildbotInstanceState::Running) {
		this->Stop();
	}
}

void Buildmaster::Start()
{
	if (this->GetState() == BuildbotInstanceState::Running) {
		this->Stop();
	}
	
	this->currentState = BuildbotInstanceState::Starting;
	this->NotifyObserverInstanceStateChanged();
	BuildbotInstance::ExecuteBuildbotComponentUsingPython(this->process, "buildbot", QStringList() << "start" << "--nodaemon" << this->directory);
}

void Buildmaster::Stop()
{
	if (this->GetState() == BuildbotInstanceState::Running)
	{
		this->currentState = BuildbotInstanceState::Stopping;
		this->NotifyObserverInstanceStateChanged();
		this->process.kill();
		this->statusLogProcess.kill();
	}
}

void Buildmaster::ProcessStarted()
{
	//Attempt to start the status log process
	BuildbotInstance::ExecuteBuildbotComponentUsingPython(this->statusLogProcess, "buildbot", QStringList() << "statuslog" << "--master" << "localhost:9898");
	
	//Call the superclass implementation
	BuildbotInstance::ProcessStarted();
}

void Buildmaster::StatusLogProcessStarted()
{
	this->statusLogProcess.closeWriteChannel();
}

void Buildmaster::StatusLogReadStdOut()
{
	QString newData( this->statusLogProcess.readAllStandardOutput() );
	this->AppendOutput(newData);
}

void Buildmaster::StatusLogReadStdErr()
{
	QString newData( this->statusLogProcess.readAllStandardError() );
	this->AppendOutput(newData);
}

QString Buildmaster::GetProjectTitle() const {
	return this->masterName;
}

QString Buildmaster::GetDirectory() const {
	return this->directory;
}

BuildbotInstanceType::BIType Buildmaster::GetType() const {
	return BuildbotInstanceType::Buildmaster;
}

QString Buildmaster::CheckConfig()
{
	return BuildbotInstance::EBCUPSynchronousAndRetrieveOutput("buildbot", QStringList() << "checkconfig" << this->directory);
}

void Buildmaster::ParseTacData(const QString& tacData)
{
	//Set the default "<Unknown>" string values for our buildmaster info
	this->masterName = "<Unknown>";
	
	//Retrieve the contents of the buildmaster configuration file
	QString configFile = Utility::RegexRetrieveFirstSubMatch("configfile = '(.*)'", tacData);
	if (configFile != "")
	{
		QString configData = Utility::GetFileContents(this->directory + "/" + configFile);
		if (configData.length() > 0)
		{
			QString detectedMasterName = Utility::RegexRetrieveFirstSubMatch("\\[['|\"]title['|\"]\\] = ['|\"](.*)['|\"]", configData);
			if (detectedMasterName != "") {
				this->masterName = detectedMasterName;
			}
		}
	}
}
