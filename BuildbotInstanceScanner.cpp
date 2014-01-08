/*
//  Buildbot Instance Manager
//  Copyright (c) 2014, Adam Rehn
//
//  ---
//
//  This class is responsible for scanning the filesystem for buildmasters
//  and buildslaves, and maintaining the list of discovered instances.
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
#include "BuildbotInstanceScanner.h"

#include <QString>
#include <QDir>
#include <QFileInfo>

#include "Utility.h"

BuildbotInstanceScanner::BuildbotInstanceScanner()
{}

BuildbotInstanceScanner::~BuildbotInstanceScanner()
{
	for (master_iterator currMaster = this->detectedBuildmasters.begin(); currMaster != this->detectedBuildmasters.end(); ++currMaster) {
		(*currMaster)->Stop();
	}
	
	for (slave_iterator currSlave = this->detectedBuildslaves.begin(); currSlave != this->detectedBuildslaves.end(); ++currSlave) {
		(*currSlave)->Stop();
	}
}

BuildbotInstanceScanner::BuildbotInstanceScanner(const string& directory)
{
	//Recursively search for "buildbot.tac" files, indicating buildmasters and buildslaves
	vector<string> foundTacFiles;
	this->RecursiveSearchDirectory(directory, foundTacFiles);
	
	//Process the identified .tac files
	for (vector<string>::iterator currTacFile = foundTacFiles.begin(); currTacFile != foundTacFiles.end(); ++currTacFile)
	{
		//Read the contents of the file and determine if we are dealing with a buildmaster or a buildslave
		QString tacFileName = QString::fromStdString(*currTacFile);
        QString tacData = Utility::GetFileContents(tacFileName);
		if (tacData.contains("application = service.Application('buildmaster')"))
		{
			this->detectedBuildmasters.push_back( unique_ptr<Buildmaster>(new Buildmaster(Utility::Dirname(tacFileName), tacData)) );
		}
		else if (tacData.contains("application = service.Application('buildslave')"))
		{
            this->detectedBuildslaves.push_back( unique_ptr<Buildslave>(new Buildslave(Utility::Dirname(tacFileName), tacData)) );
		}
		else
		{
			//Malformed .tac file, ignore
		}
	}
}

void BuildbotInstanceScanner::RecursiveSearchDirectory(const string& directory, vector<string>& foundTacFiles)
{
    QDir dir( QString::fromStdString(directory) );
	QFileInfoList files = dir.entryInfoList();
	
	for (QFileInfoList::iterator currFile = files.begin(); currFile != files.end(); ++currFile)
	{
		string fullPath = currFile->absolutePath().toStdString() + "/" + currFile->fileName().toStdString();
		
		//Ignore the "." and ".." directory entries
        if (currFile->isDir() == true && currFile->fileName() != "." && currFile->fileName() != "..")
		{
			//Recursively search all subdirectories
            this->RecursiveSearchDirectory(fullPath, foundTacFiles);
		}
		else if (currFile->fileName() == "buildbot.tac")
		{
			//Keep track of all "buildbot.tac" files we encounter
			foundTacFiles.push_back(fullPath);
		}
	}
}

unsigned int BuildbotInstanceScanner::NumMasters() const {
	return this->detectedBuildmasters.size();
}

Buildmaster& BuildbotInstanceScanner::MasterAtIndex(unsigned int index) {
    return *(this->detectedBuildmasters.at(index));
}

unsigned int BuildbotInstanceScanner::NumSlaves() const {
	return this->detectedBuildslaves.size();
}

Buildslave& BuildbotInstanceScanner::SlaveAtIndex(unsigned int index) {
    return *(this->detectedBuildslaves.at(index));
}

BuildbotInstanceScanner::master_iterator BuildbotInstanceScanner::MastersBegin() {
	return this->detectedBuildmasters.begin();
}

BuildbotInstanceScanner::master_iterator BuildbotInstanceScanner::MastersEnd() {
	return this->detectedBuildmasters.end();
}

BuildbotInstanceScanner::slave_iterator BuildbotInstanceScanner::SlavesBegin() {
	return this->detectedBuildslaves.begin();
}

BuildbotInstanceScanner::slave_iterator BuildbotInstanceScanner::SlavesEnd() {
	return this->detectedBuildslaves.end();
}
