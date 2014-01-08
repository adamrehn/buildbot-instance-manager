Buildbot Instance Manager
=========================

Buildbot Instance Manager is a GUI control panel for managing buildmasters and buildslaves located on the local filesystem. It is designed to assist in testing buildmaster configuration changes on the local machine prior to pushing them to a production server, which often involves stopping and starting the local buildmaster repeatedly.

Features
--------

<ul>
	<li>Recursively scans a user-specified directory and locates all buildmasters and buildslaves.</li>
	<li>Start and stop buildmasters and buildslaves with a simple double-click.</li>
	<li>Run the <code style="display:inline;">checkconfig</code> command to check buildmaster configuration files for errors.</li>
	<li>Output window displays the stdout and stderr of the selected instance, including <code style="display:inline;">statuslog</code> output for buildmasters.</li>
</ul>

Requirements
------------

<ul>
	<li>Buildbot version 0.8.8 or newer.*</li>
	<li>The commands <code style="display:inline;">buildbot</code> and <code style="display:inline;">buildslave</code> need to be in the system PATH.</li>
</ul>

\* Under previous versions of buildbot, the <code style="display:inline;">buildslave</code> command does not respect the <code style="display:inline;">--nodaemon</code> option. As a result, buildslaves will <code style="display:inline;">fork()</code> when started, preventing them from being controlled as child processes. This issue does not occur under Windows (where <code style="display:inline;">fork()</code> is not supported anyway), but a version that recognises the <code style="display:inline;">--nodaemon</code> option is still required.

Build Requirements
------------------

<ul>
	<li>Qt version 5 or newer.</li>
	<li>A C++ compiler that supports C++11 mode.</li>
</ul>
