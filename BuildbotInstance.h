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
#ifndef _BUILDBOT_INSTANCE_MANAGER_BUILDBOT_INSTANCE
#define _BUILDBOT_INSTANCE_MANAGER_BUILDBOT_INSTANCE

#include <QString>
#include <QProcess>

#ifdef Q_OS_WIN
#include <QString>
#include <QStringList>
#endif

class BuildbotInstanceObserver;

namespace BuildbotInstanceType
{
	enum BIType
	{
		Buildmaster,
		Buildslave
	};
}

namespace BuildbotInstanceState
{
	enum BIState
	{
		Stopped,
		Starting,
		Running,
		Stopping
	};
	
	QString toString(BIState s);
}

class BuildbotInstance : public QObject
{
	Q_OBJECT
	
	public:
		BuildbotInstance();
		virtual ~BuildbotInstance();
		
		//Interface methods to be implemented by concrete subclasses
		virtual void Start() = 0;
		virtual void Stop() = 0;
		virtual QString GetDirectory() const = 0;
		virtual BuildbotInstanceType::BIType GetType() const = 0;
		
		//Observer registraton
		void RegisterObserver(BuildbotInstanceObserver* observer);
		void UnRegisterObserver(BuildbotInstanceObserver* observer);
		
		//Process output retrieval
		QString GetOutput() const;
		
		//State information
		BuildbotInstanceState::BIState GetState() const;
		
		//Detects and verifies the version of buildbot
		static QString GetBuildbotVersion();
		static QString GetMinimumRequiredBuildbotVersion();
		static bool DoesBuildbotVersionSatisfyMinimumRequirements();
		
	protected slots:
		virtual void ProcessStarted();
		virtual void ProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
		virtual void ProcessError(QProcess::ProcessError error);
		virtual void ReadStdOut();
		virtual void ReadStdErr();
		
	protected:
		BuildbotInstanceState::BIState currentState;
		
		QProcess process;
		QString output;
		
		void ClearOutput();
		void AppendOutput(const QString& newData);
		
		void NotifyObserverInstanceStateChanged();
		void NotifyObserverInstanceStartFailed();
		void NotifyObserverOutputUpdated();
		
		BuildbotInstanceObserver* observer;
		
		//Executes a buildbot component directly, using python
		static void ExecuteBuildbotComponentUsingPython(QProcess& proc, const QString& buildbotComponent, const QStringList& args);
		
		//Executes a buildbot component directly, using python, and synchronously retrieves the contents of stdout and stderr
		static QString EBCUPSynchronousAndRetrieveOutput(const QString& buildbotComponent, const QStringList& args);
};

#endif
