/*
//  Buildbot Instance Manager
//  Copyright (c) 2014, Adam Rehn
//
//  ---
//
//  Interface for observers of buildbot instances.
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
#ifndef _BUILDBOT_INSTANCE_MANAGER_BUILDBOT_INSTANCE_OBSERVER
#define _BUILDBOT_INSTANCE_MANAGER_BUILDBOT_INSTANCE_OBSERVER

#include "BuildbotInstance.h"

class BuildbotInstanceObserver
{
    public:
        virtual ~BuildbotInstanceObserver();
		
		virtual void OnInstanceStateChanged(BuildbotInstance* sender, BuildbotInstanceState::BIState state) = 0;
		virtual void OnInstanceStartFailed(BuildbotInstance* sender) = 0;
		virtual void OnInstanceOutputUpdated(BuildbotInstance* sender) = 0;
};

#endif
