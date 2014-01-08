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
#ifndef _BUILDBOT_INSTANCE_MANAGER_UTILITY
#define _BUILDBOT_INSTANCE_MANAGER_UTILITY

#include <QString>
#include <QStringList>
#include <QProcess>

#include <stdint.h>

class Utility
{
    public:
		
		//Retrieves the contents of a file
        static QString GetFileContents(const QString& filename);
		
		//Retrieves the directory component of a path
		static QString Dirname(const QString& path);
		
		//Performs a regular expression match and returns the first parenthesised subpattern
		static QString RegexRetrieveFirstSubMatch(const QString& pattern, const QString& subject);
		
		//Determines if a command can be executed, either directly or via the system shell
        static bool IsCommandAvailable(const QString& command, const QStringList& arguments);
		
		//Determines if a command can be executed directly
		static bool IsCommandAvailableRaw(const QString& command, const QStringList& arguments);
		
		//Determines if a command can be executed via the system shell
		static bool IsCommandAvailableShell(const QString& command, const QStringList& arguments);
		
		//Instructs a QProcess to execute a command, either directly or via the system shell, as appropriate
		static void StartCommandDirectOrShell(QProcess& proc, const QString& command, const QStringList& arguments);
		
		//Searches the directories in the system's PATH environment variable attempting to locate a given file
		static QString LocateFileInSystemPathDirectories(const QString& filename);
		
		//Executes a command synchronously and returns its standard output
		static QString ExecuteSynchronousAndGetStdOut(const QString& command, const QStringList& arguments, const QString stdInput = "");
		
		//Converts a version string into a integer value
		static uint64_t ConvertVersionStringToInteger(const QString& versionString);
};

#endif
