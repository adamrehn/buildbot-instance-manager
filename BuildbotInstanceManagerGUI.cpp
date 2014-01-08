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
#include "BuildbotInstanceManagerGUI.h"
#include "ui_BuildbotInstanceManagerGUI.h"

#include <QMessageBox>
#include <QStringList>
#include <QItemSelectionModel>
#include <QScrollBar>

#include "AppSettings.h"
#include "SettingsDialog.h"
#include "Utility.h"

#include <iostream>
using std::clog;
using std::endl;

BuildbotInstanceManagerGUI::BuildbotInstanceManagerGUI(QWidget *parent) : QWidget(parent), ui(new Ui::BuildbotInstanceManagerGUI)
{
    ui->setupUi(this);
	
	this->buildmasterRunning = false;
	this->buildslavesRunning = 0;
	this->inSelectionHandler = false;
}

BuildbotInstanceManagerGUI::~BuildbotInstanceManagerGUI()
{
    delete ui;
}

void BuildbotInstanceManagerGUI::showEvent(QShowEvent*)
{
    //Determine that buildmasters and buildslaves can be run
    if (this->CanRunBuildmasters() == false || this->CanRunBuildslaves() == false) {
        QMessageBox::critical(this, "Error", "Buildbot does not appear to be in the system PATH.\nStarting buildmasters and buildslaves may not succeed.");
    }
	
	#ifndef Q_OS_WIN
	
	//Check the buildbot version
	if (BuildbotInstance::DoesBuildbotVersionSatisfyMinimumRequirements() == false)
	{
		QMessageBox::critical(this, "Old Buildbot Version", "Buildbot version " + BuildbotInstance::GetMinimumRequiredBuildbotVersion() +
			" or greater is required, detected version " + BuildbotInstance::GetBuildbotVersion() + ".\nBuildslave control may not function correctly.\n\n" +
			"(Under non-Windows platforms, buildslaves will not respect the --nodaemon option and will fork, preventing them from being controlled as child processes.)");
	}
	
	#endif
	
	//On the first run of the application, prompt the user for the search path
	AppSettings& settings = AppSettings::GetInstance();
	if (settings.AreSettingsEmpty())
	{
		QMessageBox::information(this, "First Run", "Please set the search path for finding buildmasters and buildslaves.");
		this->ShowSettingsDialog();
	}
    else
    {
        this->RescanBuildbotInstances();
    }
}

void BuildbotInstanceManagerGUI::closeEvent(QCloseEvent*)
{
	for (BuildbotInstanceScanner::master_iterator currMaster = this->scanner->MastersBegin(); currMaster != this->scanner->MastersEnd(); ++currMaster)
	{
		(*currMaster)->UnRegisterObserver(this);
		(*currMaster)->Stop();
	}

	for (BuildbotInstanceScanner::slave_iterator currSlave = this->scanner->SlavesBegin(); currSlave != this->scanner->SlavesEnd(); ++currSlave)
	{
		(*currSlave)->UnRegisterObserver(this);
		(*currSlave)->Stop();
	}
}

void BuildbotInstanceManagerGUI::OnInstanceStateChanged(BuildbotInstance* sender, BuildbotInstanceState::BIState state)
{
	switch (sender->GetType())
	{
		case BuildbotInstanceType::Buildmaster:
		{
			if (state == BuildbotInstanceState::Running) {
				this->buildmasterRunning = true;
			}
			else if (state == BuildbotInstanceState::Stopped) {
				this->buildmasterRunning = false;
			}
			
			break;
		}
		
		case BuildbotInstanceType::Buildslave:
		{
			if (state == BuildbotInstanceState::Running) {
				this->buildslavesRunning++;
			}
			else if (state == BuildbotInstanceState::Stopped) {
				this->buildslavesRunning--;
			}
			
			break;
		}
	}
	
	this->RefreshInstanceLists();
	this->ResetButtonAvailability();
}

void BuildbotInstanceManagerGUI::OnInstanceStartFailed(BuildbotInstance* sender)
{
	QString type = "";
	switch (sender->GetType())
	{
		case BuildbotInstanceType::Buildmaster:
			type = "buildmaster";
			break;
		
		case BuildbotInstanceType::Buildslave:
			type = "buildslave";
			break;
	}
	
	QMessageBox::critical(this, "Error", "Failed to start " + type + "!");
	this->RefreshInstanceLists();
	this->ResetButtonAvailability();
}

void BuildbotInstanceManagerGUI::OnInstanceOutputUpdated(BuildbotInstance* sender)
{
	Buildmaster* selectedMaster = this->GetSelectedMaster();
	Buildslave* selectedSlave = this->GetSelectedSlave();
	
	//Only refresh the output window if the sender is the selected instance
	if (sender == selectedMaster || sender == selectedSlave) {
		this->RefreshOutputWindow();
	}
}

void BuildbotInstanceManagerGUI::MasterSelectionChanged(const QModelIndex&, const QModelIndex&)
{
	if (this->inSelectionHandler == false)
	{
		this->inSelectionHandler = true;
		this->ui->buildslavesTable->selectionModel()->clearSelection();
		this->ui->buildslavesTable->selectionModel()->clearCurrentIndex();
		
		this->RefreshOutputWindow();
		this->inSelectionHandler = false;
	}
}

void BuildbotInstanceManagerGUI::SlaveSelectionChanged(const QModelIndex&, const QModelIndex&)
{
	if (this->inSelectionHandler == false)
	{
		this->inSelectionHandler = true;
		this->ui->buildmastersTable->selectionModel()->clearSelection();
		this->ui->buildmastersTable->selectionModel()->clearCurrentIndex();
		
		this->RefreshOutputWindow();
		this->inSelectionHandler = false;
	}
}

void BuildbotInstanceManagerGUI::ShowSettingsDialog()
{
	SettingsDialog settingsDialog(this);
    settingsDialog.exec();
    this->RescanBuildbotInstances();
}

void BuildbotInstanceManagerGUI::RescanBuildbotInstances()
{
    AppSettings& settings = AppSettings::GetInstance();
    this->scanner.reset( new BuildbotInstanceScanner(settings.GetSearchPath().toStdString()) );
	
	for (BuildbotInstanceScanner::master_iterator currMaster = this->scanner->MastersBegin(); currMaster != this->scanner->MastersEnd(); ++currMaster) {
		(*currMaster)->RegisterObserver(this);
	}
	
	for (BuildbotInstanceScanner::slave_iterator currSlave = this->scanner->SlavesBegin(); currSlave != this->scanner->SlavesEnd(); ++currSlave) {
		(*currSlave)->RegisterObserver(this);
	}
	
	this->RefreshInstanceLists();
	this->ResetButtonAvailability();
}

void BuildbotInstanceManagerGUI::RefreshInstanceLists()
{
	this->buildmasterListAdapter.reset( new BuildmasterListAdapter(this->scanner.data()) );
	this->buildslaveListAdapter.reset( new BuildslaveListAdapter(this->scanner.data()) );
	
	this->ui->buildmastersTable->setModel(this->buildmasterListAdapter.data());
	this->ui->buildslavesTable->setModel(this->buildslaveListAdapter.data());
	
	//Connect the selection event signals of the new selection models to our handlers
	QObject::connect(this->ui->buildmastersTable->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(MasterSelectionChanged(const QModelIndex&, const QModelIndex&)));
	QObject::connect(this->ui->buildslavesTable->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(SlaveSelectionChanged(const QModelIndex&, const QModelIndex&)));
}

void BuildbotInstanceManagerGUI::RefreshOutputWindow()
{
	//Determine if we have selected a buildmaster or a buildslave
	Buildmaster* selectedMaster = this->GetSelectedMaster();
	Buildslave* selectedSlave = this->GetSelectedSlave();
	if (selectedMaster != NULL)
	{
		//A buildmaster is selected
        this->ui->selectedInstanceOutput->setPlainText( selectedMaster->GetOutput() );
	}
	else if (selectedSlave != NULL)
	{
		//A buildslave is selected
        this->ui->selectedInstanceOutput->setPlainText( selectedSlave->GetOutput() );
	}
	else
	{
		//Clear the output window
		this->ui->selectedInstanceOutput->setPlainText("");
	}
	
	//Scroll the output window to the bottom
	this->ui->selectedInstanceOutput->verticalScrollBar()->triggerAction(QScrollBar::SliderToMaximum);
}

void BuildbotInstanceManagerGUI::ResetButtonAvailability()
{
	//Disable the "Rescan" and "Settings" buttons when buildbot instances are running
	bool buttonsEnabled = (this->buildmasterRunning == false && this->buildslavesRunning == 0);
    this->ui->rescanButton->setEnabled(buttonsEnabled);
	this->ui->settingsButton->setEnabled(buttonsEnabled);
}

bool BuildbotInstanceManagerGUI::CanRunBuildmasters() const
{
	return Utility::IsCommandAvailable(QString("buildbot"), QStringList() << "--version");
}

bool BuildbotInstanceManagerGUI::CanRunBuildslaves() const
{
	return Utility::IsCommandAvailable(QString("buildslave"), QStringList() << "--version");
}

Buildmaster* BuildbotInstanceManagerGUI::GetSelectedMaster() const
{
	QModelIndex selectedIndex = this->ui->buildmastersTable->selectionModel()->currentIndex();
	if (selectedIndex.isValid())
	{
		Buildmaster& selectedMaster = this->scanner->MasterAtIndex(selectedIndex.row());
		return &selectedMaster;
	}
	
	return NULL;
}

Buildslave* BuildbotInstanceManagerGUI::GetSelectedSlave() const
{
	QModelIndex selectedIndex = this->ui->buildslavesTable->selectionModel()->currentIndex();
    if (selectedIndex.isValid())
    {
        Buildslave& selectedSlave = this->scanner->SlaveAtIndex(selectedIndex.row());
		return &selectedSlave;
    }
	
	return NULL;
}

void BuildbotInstanceManagerGUI::on_settingsButton_clicked()
{
    this->ShowSettingsDialog();
}

void BuildbotInstanceManagerGUI::on_buttonStartBuildmaster_clicked()
{
	//Start selected buildmaster
	Buildmaster* selectedMaster = this->GetSelectedMaster();
	if (selectedMaster != NULL && selectedMaster->GetState() == BuildbotInstanceState::Stopped)
	{
		//Prevent the user from attempting to run multiple buildmasters simultaneously
		if (this->buildmasterRunning == true) {
			QMessageBox::warning(this, "Error", "Only one buildmaster can run at a time.");
		}
		else {
			selectedMaster->Start();
		}
	}
}

void BuildbotInstanceManagerGUI::on_buttonStopBuildmaster_clicked()
{
	//Stop selected buildmaster
	Buildmaster* selectedMaster = this->GetSelectedMaster();
	if (selectedMaster != NULL && selectedMaster->GetState() == BuildbotInstanceState::Running) {
		selectedMaster->Stop();
	}
}

void BuildbotInstanceManagerGUI::on_buttonStartBuildslave_clicked()
{
	//Start selected buildslave
	Buildslave* selectedSlave = this->GetSelectedSlave();
	if (selectedSlave != NULL && selectedSlave->GetState() == BuildbotInstanceState::Stopped) {
		selectedSlave->Start();
	}
}

void BuildbotInstanceManagerGUI::on_buttonStopBuildslave_clicked()
{
	//Stop selected buildslave
	Buildslave* selectedSlave = this->GetSelectedSlave();
	if (selectedSlave != NULL && selectedSlave->GetState() == BuildbotInstanceState::Running) {
		selectedSlave->Stop();
	}
}

void BuildbotInstanceManagerGUI::on_buttonCheckConfig_clicked()
{
	//Check selected buildmaster config
	Buildmaster* selectedMaster = this->GetSelectedMaster();
	if (selectedMaster != NULL) {
		QMessageBox::information(this, "Check Config", selectedMaster->CheckConfig());
	}
}

void BuildbotInstanceManagerGUI::on_rescanButton_clicked()
{
	this->RescanBuildbotInstances();
}

void BuildbotInstanceManagerGUI::on_buildmastersTable_doubleClicked(const QModelIndex &index)
{
    if (index.isValid())
    {
        //Toggle the running state of the double-clicked buildmaster
        Buildmaster& selectedMaster = this->scanner->MasterAtIndex(index.row());
        if (selectedMaster.GetState() == BuildbotInstanceState::Stopped) {
            selectedMaster.Start();
        }
        else if (selectedMaster.GetState() == BuildbotInstanceState::Running) {
            selectedMaster.Stop();
        }
    }
}

void BuildbotInstanceManagerGUI::on_buildslavesTable_doubleClicked(const QModelIndex &index)
{
    if (index.isValid())
    {
        //Toggle the running state of the double-clicked buildslave
        Buildslave& selectedSlave = this->scanner->SlaveAtIndex(index.row());
        if (selectedSlave.GetState() == BuildbotInstanceState::Stopped) {
            selectedSlave.Start();
        }
        else if (selectedSlave.GetState() == BuildbotInstanceState::Running) {
            selectedSlave.Stop();
        }
    }
}
