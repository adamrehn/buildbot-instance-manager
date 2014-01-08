/*
//  Buildbot Instance Manager
//  Copyright (c) 2014, Adam Rehn
//
//  ---
//
//  Main application entry point.
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
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    BuildbotInstanceManagerGUI w;
    w.show();

    return a.exec();
}
