/*
//  Buildbot Instance Manager
//  Copyright (c) 2014, Adam Rehn
//
//  ---
//
//  Settings dialog view controller.
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
#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"

#include <QFileDialog>
#include <QLayout>

#include "AppSettings.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

	AppSettings& settings = AppSettings::GetInstance();
	if (settings.AreSettingsEmpty() == false)
	{
		ui->searchPath->setText( settings.GetSearchPath() );
	}
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_SettingsDialog_accepted()
{
	AppSettings& settings = AppSettings::GetInstance();
	settings.SetSearchPath(ui->searchPath->text());
}

void SettingsDialog::on_chooseDirectoryButton_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, "", "", QFileDialog::ShowDirsOnly);
    ui->searchPath->setText(path);
}
