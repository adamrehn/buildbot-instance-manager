/*
//  Buildbot Instance Manager
//  Copyright (c) 2014, Adam Rehn
//
//  ---
//
//  This class manages the application's persistent settings.
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
#ifndef _BUILDBOT_INSTANCE_MANAGER_APP_SETTINGS
#define _BUILDBOT_INSTANCE_MANAGER_APP_SETTINGS

#include <QString>
#include <QSettings>

class AppSettings
{
	public:
		static AppSettings& GetInstance();
		
		bool AreSettingsEmpty() const;
		
		void SetSearchPath(const QString& searchPath);
		QString GetSearchPath(const QString& defaultValue = "") const;
		
	private:
		AppSettings();
		static AppSettings _instance;
		
		void SaveSetting(const QString& key, const QString& value);
		QString RetrieveSetting(const QString& key, const QString& defaultValue = "") const;
};

#endif
