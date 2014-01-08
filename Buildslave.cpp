/*
//  Buildbot Instance Manager
//  Copyright (c) 2014, Adam Rehn
//
//  ---
//
//  This class represents an instance of a buildslave.
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
#include "Buildslave.h"

#include "Utility.h"

Buildslave::Buildslave(const QString& directory, const QString& tacData)
{
	this->directory = directory;
	this->ParseTacData(tacData);
}

Buildslave::~Buildslave()
{
	if (this->GetState() == BuildbotInstanceState::Running) {
		this->Stop();
	}
}

void Buildslave::Start()
{
	if (this->GetState() == BuildbotInstanceState::Running) {
		this->Stop();
	}
	
	this->currentState = BuildbotInstanceState::Starting;
	this->NotifyObserverInstanceStateChanged();
	BuildbotInstance::ExecuteBuildbotComponentUsingPython(this->process, "buildslave", QStringList() << "start" << "--nodaemon" << this->directory);
}

void Buildslave::Stop()
{
	if (this->GetState() == BuildbotInstanceState::Running)
	{
		this->currentState = BuildbotInstanceState::Stopping;
		this->NotifyObserverInstanceStateChanged();
		this->process.kill();
	}
}

QString Buildslave::GetDirectory() const {
	return this->directory;
}

QString Buildslave::GetBuildslaveName() const {
	return this->slaveName;
}

QString Buildslave::GetBuildmasterHost() const {
	return this->masterHost;
}

BuildbotInstanceType::BIType Buildslave::GetType() const {
	return BuildbotInstanceType::Buildslave;
}

void Buildslave::ParseTacData(const QString& tacData)
{
	this->slaveName = Utility::RegexRetrieveFirstSubMatch("slavename = '(.*)'", tacData);
	this->masterHost = Utility::RegexRetrieveFirstSubMatch("buildmaster_host = '(.*)'", tacData);
}
