Buildbot Instance Manager
=========================

Buildbot Instance Manager is a GUI control panel for managing buildmasters and buildslaves located on the local filesystem. It is designed to assist in testing buildmaster configuration changes on the local machine prior to pushing them to a production server, which often involves stopping and starting the local buildmaster repeatedly.

Binaries for Windows, Mac OS X, and Linux are available from [the releases page](https://github.com/adamrehn/buildbot-instance-manager/releases).

Features
--------

- Recursively scans a user-specified directory and locates all buildmasters and buildslaves.
- Start and stop buildmasters and buildslaves with a simple double-click.
- Run the `checkconfig` command to check buildmaster configuration files for errors.
- Output window displays the stdout and stderr of the selected instance, including `statuslog` output for buildmasters.

Requirements
------------

- Buildbot version 0.8.8 or newer.\*
- The commands `buildbot` and `buildslave` need to be in the system PATH.

\* Under previous versions of buildbot, the `buildslave` command does not respect the `--nodaemon` option. As a result, buildslaves will `fork()` when started, preventing them from being controlled as child processes. This issue does not occur under Windows (where `fork()` is not supported anyway), but a version that recognises the `--nodaemon` option is still required.

Build Requirements
------------------

- Qt version 5 or newer.
- A C++ compiler that supports C++11 mode.
