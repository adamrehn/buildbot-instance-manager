/*
//  Buildbot Instance Manager
//  Copyright (c) 2014, Adam Rehn
//
//  ---
//
//  Base class for classes representing buildbot instances.
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
#include "BuildbotInstance.h"

#include "BuildbotInstanceObserver.h"
#include "Utility.h"

namespace BuildbotInstanceState
{
	QString toString(BIState s)
	{
		switch (s)
		{
			case BuildbotInstanceState::Stopped:
				return "Stopped";
			
			case BuildbotInstanceState::Starting:
				return "Starting";
			
			case BuildbotInstanceState::Running:
				return "Running";
			
			case BuildbotInstanceState::Stopping:
				return "Stopping";
			
			default:
				return "";
		}
	}
}

BuildbotInstance::BuildbotInstance()
{
	this->currentState = BuildbotInstanceState::Stopped;
	this->observer = NULL;
	
	//Connect the signals of the QProcess object to our handlers
	QObject::connect(&this->process, SIGNAL(started()), this, SLOT(ProcessStarted()));
	QObject::connect(&this->process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(ProcessFinished(int, QProcess::ExitStatus)));
	QObject::connect(&this->process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(ProcessError(QProcess::ProcessError)));
	QObject::connect(&this->process, SIGNAL(readyReadStandardOutput()), this, SLOT(ReadStdOut()));
	QObject::connect(&this->process, SIGNAL(readyReadStandardError()), this, SLOT(ReadStdErr()));
}

BuildbotInstance::~BuildbotInstance()
{
	this->observer = NULL;
}

void BuildbotInstance::RegisterObserver(BuildbotInstanceObserver* observer) {
	this->observer = observer;
}

void BuildbotInstance::UnRegisterObserver(BuildbotInstanceObserver* observer)
{
	if (this->observer == observer) {
		this->observer = NULL;
	}
}

BuildbotInstanceState::BIState BuildbotInstance::GetState() const {
	return this->currentState;
}

void BuildbotInstance::ProcessStarted()
{
	this->process.closeWriteChannel();
	this->currentState = BuildbotInstanceState::Running;
	this->NotifyObserverInstanceStateChanged();
}

void BuildbotInstance::ProcessFinished(int, QProcess::ExitStatus)
{
	this->currentState = BuildbotInstanceState::Stopped;
	this->NotifyObserverInstanceStateChanged();
}

void BuildbotInstance::ProcessError(QProcess::ProcessError error)
{
	if (error == QProcess::FailedToStart)
	{
		this->currentState = BuildbotInstanceState::Stopped;
		this->NotifyObserverInstanceStartFailed();
	}
}

void BuildbotInstance::ReadStdOut()
{
	QString newData( this->process.readAllStandardOutput() );
	this->AppendOutput(newData);
}

void BuildbotInstance::ReadStdErr()
{
	QString newData( this->process.readAllStandardError() );
	this->AppendOutput(newData);
}

QString BuildbotInstance::GetOutput() const {
	return this->output;
}

QString BuildbotInstance::GetBuildbotVersion()
{
	QString buildmasterVersion = Utility::RegexRetrieveFirstSubMatch("Buildbot version: (.*)\n", BuildbotInstance::EBCUPSynchronousAndRetrieveOutput("buildbot", QStringList() << "--version"));
	QString buildslaveVersion  = Utility::RegexRetrieveFirstSubMatch("Buildslave version: (.*)\n", BuildbotInstance::EBCUPSynchronousAndRetrieveOutput("buildslave", QStringList() << "--version"));
    return ((buildslaveVersion != "") ? buildslaveVersion : buildmasterVersion);
}

QString BuildbotInstance::GetMinimumRequiredBuildbotVersion() {
	return "0.8.8";
}

bool BuildbotInstance::DoesBuildbotVersionSatisfyMinimumRequirements()
{
	QString detectedVersion = BuildbotInstance::GetBuildbotVersion();
	QString requiredVersion = BuildbotInstance::GetMinimumRequiredBuildbotVersion();
	
    uint64_t detectedVerInt = Utility::ConvertVersionStringToInteger(detectedVersion);
    uint64_t requiredVerInt = Utility::ConvertVersionStringToInteger(requiredVersion);

    return (detectedVerInt >= requiredVerInt);
}

void BuildbotInstance::ClearOutput()
{
	this->output.clear();
	this->NotifyObserverOutputUpdated();
}

void BuildbotInstance::AppendOutput(const QString& newData)
{
	this->output.append(newData);
	this->NotifyObserverOutputUpdated();
}

void BuildbotInstance::NotifyObserverInstanceStateChanged()
{
	if (this->observer != NULL) {
		this->observer->OnInstanceStateChanged(this, this->currentState);
	}
}

void BuildbotInstance::NotifyObserverInstanceStartFailed()
{
	if (this->observer != NULL) {
		this->observer->OnInstanceStartFailed(this);
	}
}

void BuildbotInstance::NotifyObserverOutputUpdated()
{
	if (this->observer != NULL) {
		this->observer->OnInstanceOutputUpdated(this);
	}
}

#ifdef Q_OS_WIN

void BuildbotInstance::ExecuteBuildbotComponentUsingPython(QProcess& proc, const QString& buildbotComponent, const QStringList& args)
{
	//Locate the buildbot component batch script
	QString buildbotBatchLoc = Utility::LocateFileInSystemPathDirectories(buildbotComponent + ".bat");
	if (buildbotBatchLoc != "")
	{
		//Use the location of the batch script to locate python itself
		QString pythonLoc = QString(buildbotBatchLoc).replace("/Scripts/" + buildbotComponent + ".bat", "/python.exe", Qt::CaseInsensitive);
		QString pythonScriptLoc = QString(buildbotBatchLoc).replace(".bat", "");
		
		//Execute the buildbot component directly, using python
		proc.start(pythonLoc, QStringList() << "-u" << pythonScriptLoc << args);
	}
	
	//Failed to locate the buildbot component batch script
}

#else

void BuildbotInstance::ExecuteBuildbotComponentUsingPython(QProcess& proc, const QString& buildbotComponent, const QStringList& args)
{
	//Locate the buildbot component python script
	QString buildbotScriptLoc = Utility::LocateFileInSystemPathDirectories(buildbotComponent);
	if (buildbotScriptLoc != "")
	{
		//Read the contents of the script file and use the hash-bang to locate python itself
		QString scriptData = Utility::GetFileContents(buildbotScriptLoc);
		QString pythonLoc = Utility::RegexRetrieveFirstSubMatch("#!(.*)\n", scriptData);
		
		//Execute the buildbot component directly, using python
		proc.start(pythonLoc, QStringList() << "-u" << buildbotScriptLoc << args);
	}
	
	//Failed to locate the buildbot component python script
}

#endif

QString BuildbotInstance::EBCUPSynchronousAndRetrieveOutput(const QString& buildbotComponent, const QStringList& args)
{
	QProcess process;
	process.setProcessChannelMode(QProcess::MergedChannels);
	BuildbotInstance::ExecuteBuildbotComponentUsingPython(process, buildbotComponent, args);
	
    if (!process.waitForStarted()) {
        return "";
	}
	
    process.closeWriteChannel();
	
    if (!process.waitForFinished()) {
        return "";
	}
	
    return QString( process.readAll() );
}
