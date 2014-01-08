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
#include "AppSettings.h"

#include <QApplication>
#include <QScopedPointer>

AppSettings AppSettings::_instance;

AppSettings::AppSettings()
{
	QCoreApplication::setOrganizationName("BuildbotInstanceManager");
	QCoreApplication::setOrganizationDomain("adamrehn.com");
	QCoreApplication::setApplicationName("BuildbotInstanceManager");
	
	#ifdef Q_OS_WIN
		QSettings::setDefaultFormat(QSettings::IniFormat);
	#endif
}

AppSettings& AppSettings::GetInstance()
{
	return _instance;
}

bool AppSettings::AreSettingsEmpty() const
{
	QSettings settings;
	return (settings.contains("searchpath") == false);
}

void AppSettings::SetSearchPath(const QString& searchPath) {
	this->SaveSetting("searchpath", searchPath);
}

QString AppSettings::GetSearchPath(const QString& defaultValue) const {
	return this->RetrieveSetting("searchpath", defaultValue);
}

void AppSettings::SaveSetting(const QString& key, const QString& value)
{
	QSettings settings;
	settings.setValue(key, value);
}

QString AppSettings::RetrieveSetting(const QString& key, const QString& defaultValue) const
{
	QSettings settings;
	return ((settings.contains(key)) ? settings.value(key).toString() : defaultValue);
}
