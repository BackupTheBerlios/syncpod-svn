 ____                   ____           _ 
/ ___| _   _ _ __   ___|  _ \ ___   __| |
\___ \| | | | '_ \ / __| |_) / _ \ / _` |
 ___) | |_| | | | | (__|  __/ (_) | (_| |
|____/ \__, |_| |_|\___|_|   \___/ \__,_|
       |___/                             

Introduction:
=============

SyncPod is a small utility that will synchronize a directory on your
PC hard drive with a directory on a USB Mass Storage device (say an
MP3 player).  This program is released under the MIT license, see the
Copyright.txt file.

What does it do?
================

Reading a configuration file that specifies a destination and source directories, the program will
enumerate the content of the source and destination directories and detect whether files in the
source directory are newer than the ones in the destination one (check is based on the dates).
If the source files are newer, they will be copied across to the destination directory.
If a source file is not present in the destination directory, it will be copied.
The synchronisation process will take place at program startup and everytime a USB Mass
Storage device is inserted.

What can I use it for?
======================

I personally use this program to update my MP3 player with my PodCasts subscriptions everytime
I plug my player to my computer ;)

Usage:
======

prompt>syncPod.exe

Author and contact:
===================
Jean-Baptiste Lab, jean-baptiste dot lab at laposte dot net