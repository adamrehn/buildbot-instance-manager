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
#ifndef _BUILDBOT_INSTANCE_MANAGER_BUILDBOT_INSTANCE_SCANNER
#define _BUILDBOT_INSTANCE_MANAGER_BUILDBOT_INSTANCE_SCANNER

#include "BuildbotInstance.h"
#include "Buildmaster.h"
#include "Buildslave.h"

#include <memory>
using std::unique_ptr;

#include <string>
#include <vector>
using std::string;
using std::vector;

class BuildbotInstanceScanner
{
	public:
		typedef vector< unique_ptr<Buildmaster> >::iterator master_iterator;
		typedef vector< unique_ptr<Buildslave> >::iterator slave_iterator;
		
		BuildbotInstanceScanner();
		BuildbotInstanceScanner(const string& directory);
		~BuildbotInstanceScanner();
		
		unsigned int NumMasters() const;
		Buildmaster& MasterAtIndex(unsigned int index);
		
		master_iterator MastersBegin();
		master_iterator MastersEnd();
		
		unsigned int NumSlaves() const;
		Buildslave& SlaveAtIndex(unsigned int index);
		
		slave_iterator SlavesBegin();
		slave_iterator SlavesEnd();
		
	private:
		void RecursiveSearchDirectory(const string& directory, vector<string>& foundTacFiles);
		
		vector< unique_ptr<Buildmaster> > detectedBuildmasters;
        vector< unique_ptr<Buildslave> > detectedBuildslaves;
};

#endif
