/*
//  Buildbot Instance Manager
//  Copyright (c) 2014, Adam Rehn
//
//  ---
//
//  Utility functions.
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
#include "Utility.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QVector>
#include <QRegularExpression>
#include <QProcessEnvironment>

#include <cstdlib>
#include <vector>

QString Utility::GetFileContents(const QString& filename)
{
	QFile file(filename);
	if (file.open(QIODevice::ReadOnly))
	{
		QString data(file.readAll());
		file.close();
		return data;
	}
	
	return "";
}

QString Utility::Dirname(const QString& path)
{
	QFileInfo info(path);
	return info.absolutePath();
}

QString Utility::RegexRetrieveFirstSubMatch(const QString& pattern, const QString& subject)
{
	QRegularExpression re(pattern);
	QRegularExpressionMatch match = re.match(subject);
	if (match.hasMatch() && match.capturedLength(0) > 0)
	{
		return match.captured(1);
	}
	
	return "";
}

bool Utility::IsCommandAvailable(const QString& command, const QStringList& arguments)
{
	return (Utility::IsCommandAvailableRaw(command, arguments) || Utility::IsCommandAvailableShell(command, arguments));
}

bool Utility::IsCommandAvailableRaw(const QString& command, const QStringList& arguments)
{
	QProcess process;
    process.start(command, arguments);
	
    if (process.waitForStarted() == false) {
        return false;
	}

    if (process.waitForFinished() == false) {
        return false;
	}
	
	return true;
}

bool Utility::IsCommandAvailableShell(const QString& command, const QStringList& arguments)
{
	QString shell = "sh";
	QStringList shellArgs;
	
	#ifdef Q_OS_WIN
		shell = "cmd";
		shellArgs << "/c";
	#endif
	
	return Utility::IsCommandAvailableRaw(shell, shellArgs << (command + " " + arguments.join(" ")));
}

void Utility::StartCommandDirectOrShell(QProcess& proc, const QString& command, const QStringList& arguments)
{
	if (Utility::IsCommandAvailableRaw(command, arguments)) {
		proc.start(command, arguments);
	}
	else
	{
		#ifdef Q_OS_WIN
			proc.start("cmd", QStringList() << "/c" << (command + " " + arguments.join(" ")));
		#else
			proc.start("sh", QStringList() << command << arguments);
		#endif
	}
}

QString Utility::LocateFileInSystemPathDirectories(const QString& filename)
{
	#ifdef Q_OS_WIN
		QString separatorChar = ";";
	#else
		QString separatorChar = ":";
	#endif
	
	QString path( getenv("PATH") );
	if (path != "")
	{
		#ifndef Q_OS_WIN
		
		//Under Unix platforms, read the user's shell profile and incorporate any extra path components identified
		QString profileData = Utility::GetFileContents(QDir::homePath() + "/.profile") + Utility::GetFileContents(QDir::homePath() + "/.bashrc");
		QString pathExport = Utility::RegexRetrieveFirstSubMatch("export PATH=(.*)\n", profileData);
		pathExport = pathExport.replace("$PATH:", "");
		pathExport = pathExport.replace(":$PATH", "");
		path.append(":" + pathExport);
		
		#endif
		
		#ifdef Q_OS_DARWIN
		
		//Under Darwin, some components of the PATH may not be visible to GUI applications
		//As such, we manually ensure that the standard locations are included
		path.append(":/bin:/usr/bin:/usr/local/bin");
		
		#endif
		
		QStringList pathDirectories = path.split(separatorChar);
		for (auto currDir : pathDirectories)
		{
			QString filePath = QString(currDir).replace("\\", "/");
			filePath = filePath + (filePath.endsWith("/") ? "" : "/") + filename;
			QFileInfo file(filePath);
			if (file.exists()) {
				return filePath;
			}
		}
	}
	
	return "";
}

QString Utility::ExecuteSynchronousAndGetStdOut(const QString& command, const QStringList& arguments, const QString stdInput)
{
	QProcess process;
    process.start(command, arguments);
	
    if (!process.waitForStarted()) {
        return "";
	}
	
	process.write(stdInput.toStdString().c_str());
    process.closeWriteChannel();

    if (!process.waitForFinished()) {
        return "";
	}
	
    return QString( process.readAll() );
}

uint64_t Utility::ConvertVersionStringToInteger(const QString& versionString)
{
	uint64_t versionInt = 0;
	QStringList components = QString(versionString).replace(QRegularExpression("p.*"), "").split(".");
	
	//Iterate over the version components in right-to-left order
	uint64_t placeValue = 1;
	std::vector<QString> componentsVec = components.toVector().toStdVector();
	for (std::vector<QString>::reverse_iterator currComponent = componentsVec.rbegin(); currComponent != componentsVec.rend(); ++currComponent)
	{
		//Convert the string to an integer
		bool conversionSucceeded = false;
		uint parsedNum = currComponent->toUInt(&conversionSucceeded);
		
		//Add the value to the version, multiplied by the place value
		if (conversionSucceeded) {
			versionInt += (parsedNum * placeValue);
		}
		
		//Advance using base-10 place value rules
		placeValue *= 10;
	}
	
	return versionInt;
}
