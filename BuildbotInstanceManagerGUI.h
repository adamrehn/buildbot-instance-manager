/*
//  Buildbot Instance Manager
//  Copyright (c) 2014, Adam Rehn
//
//  ---
//
//  Main window view controller.
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
#ifndef _BUILDBOT_INSTANCE_MANAGER_BUILDBOT_INSTANCE_MANAGER_GUI
#define _BUILDBOT_INSTANCE_MANAGER_BUILDBOT_INSTANCE_MANAGER_GUI

#include <QWidget>
#include <QScopedPointer>

#include "Buildmaster.h"
#include "Buildslave.h"
#include "BuildbotInstanceObserver.h"
#include "BuildbotInstanceScanner.h"
#include "BuildmasterListAdapter.h"
#include "BuildslaveListAdapter.h"

namespace Ui {
	class BuildbotInstanceManagerGUI;
}

class BuildbotInstanceManagerGUI : public QWidget, public BuildbotInstanceObserver
{
    Q_OBJECT

	public:
		explicit BuildbotInstanceManagerGUI(QWidget *parent = 0);
		~BuildbotInstanceManagerGUI();
		
		void OnInstanceStateChanged(BuildbotInstance* sender, BuildbotInstanceState::BIState state);
		void OnInstanceStartFailed(BuildbotInstance* sender);
		void OnInstanceOutputUpdated(BuildbotInstance* sender);
		
    private slots:
		void MasterSelectionChanged(const QModelIndex&, const QModelIndex&);
		void SlaveSelectionChanged(const QModelIndex&, const QModelIndex&);
		
        void on_settingsButton_clicked();
        void on_buttonStartBuildmaster_clicked();
        void on_buttonStopBuildmaster_clicked();
        void on_buttonStartBuildslave_clicked();
        void on_buttonStopBuildslave_clicked();
        void on_buttonCheckConfig_clicked();
        void on_rescanButton_clicked();
        void on_buildmastersTable_doubleClicked(const QModelIndex &index);
        void on_buildslavesTable_doubleClicked(const QModelIndex &index);

    private:
        void showEvent(QShowEvent*);
		void closeEvent(QCloseEvent*);
        void ShowSettingsDialog();
        void RescanBuildbotInstances();
		void RefreshInstanceLists();
		void RefreshOutputWindow();
		void ResetButtonAvailability();
		
		bool CanRunBuildmasters() const;
		bool CanRunBuildslaves() const;
		
        Buildmaster* GetSelectedMaster() const;
		Buildslave* GetSelectedSlave() const;
		
		//This is used to prevent recursion in our selection handlers
		bool inSelectionHandler;
		
		//Keep track of how many instances are running
		bool buildmasterRunning;
		unsigned int buildslavesRunning;
		
		//The buildbot instance scanner and the list adapters that utilise it as their data source
		QScopedPointer<BuildbotInstanceScanner> scanner;
		QScopedPointer<BuildmasterListAdapter> buildmasterListAdapter;
		QScopedPointer<BuildslaveListAdapter> buildslaveListAdapter;
		
		//The UI object
		Ui::BuildbotInstanceManagerGUI *ui;
};

#endif
