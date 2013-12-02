#!/bin/bash

echo "Check Python installation ..."
command -v python >/dev/null 2>&1 || { echo >&2 "Python is not installed. Installing Python now ...";
sudo apt-get install python; }
echo "   Python is installed"

echo "Check Python Pip installation ..."
command -v python-pip >/dev/null 2>&1 || { echo >&2 "Python Pip is not installed. Installing Python Pip now ...";
sudo apt-get install python-pip; }
echo "   Python Pip is installed"

echo "Check OpenXC installation ..."
command -v openxc-control version >/dev/null 2>&1 || { echo >&2 "   OpenXC is not installed. Installing OpenXC now ...";
sudo pip install -U openxc; }
echo "   OpenXC is installed"

echo "Installing Tornado ..."
sudo pip install tornado --upgrade
echo "   Tornado is installed"
