[![Slack Status](http://sdlslack.herokuapp.com/badge.svg)](http://slack.smartdevicelink.org)


#test line
#test line 2
#line 3
#line 4
[![Build Status](https://travis-ci.org/smartdevicelink/sdl_core.svg?branch=develop)](https://travis-ci.org/smartdevicelink/sdl_core)  **Ubuntu Linux 14.04 x64**

[![Build Status](http://109.166.244.153/app/rest/builds/buildType:WinSdlDevelop_WinX64withUt/statusIcon)](http://109.166.244.153/viewType.html?buildTypeId=WinSdlDevelop_WinX64withUt&guest=1)  **Windows 7 x64**

[![Build Status](http://109.166.244.153/app/rest/builds/buildType:QtSdlDevelop_QtX86withUt/statusIcon)](http://109.166.244.153/viewType.html?buildTypeId=QtSdlDevelop_QtX86withUt&guest=1)  **Windows 7 x86 QT**

[![codecov.io](https://codecov.io/github/smartdevicelink/sdl_core/coverage.svg?branch=develop)](https://codecov.io/github/smartdevicelink/sdl_core?branch=develop)

# SmartDeviceLink (SDL)

SmartDeviceLink (SDL) is a standard set of protocols and messages that connect applications on a smartphone to a vehicle head unit. This messaging enables a consumer to interact with their application using common in-vehicle interfaces such as a touch screen display, embedded voice recognition, steering wheel controls and various vehicle knobs and buttons. There are three main components that make up the SDL ecosystem.

  * The [Core](https://github.com/smartdevicelink/sdl_core) component is the software which Vehicle Manufacturers (OEMs)  implement in their vehicle head units. Integrating this component into their head unit and HMI based on a set of guidelines and templates enables access to various smartphone applications.
  * The optional [SDL Server](https://github.com/smartdevicelink/sdl_server) can be used by Vehicle OEMs to update application policies and gather usage information for connected applications.
  * The [iOS](https://github.com/smartdevicelink/sdl_ios) and [Android](https://github.com/smartdevicelink/sdl_android) libraries are implemented by app developers into their applications to enable command and control via the connected head unit.

Pull Requests Welcome!

## Documentation

  * [Software Architecture Document](https://app.box.com/s/v5ymu5kathzkwfx8iigxxdpr7d5a2xhh)
  * [Transport Manager Programming Guide](https://app.box.com/s/1pjquttvmhf19uujtw4x4fv4t1leqasa)
  * [Software Detailed Design](https://app.box.com/s/ohgrvemtx39f8hfea1ab676xxrzvyx1y)
  * [Integration Guidelines](https://app.box.com/s/jkj51mkaa5uganbhjxmtv06lbs8hapa9)

## SDL Core

The Core component of SDL runs on a vehicle's computing system (head unit). Core’s primary responsibility is to pass messages between connected smartphone applications and the vehicle HMI, and pass notifications from the vehicle to those applications. It can connect a smartphone to a vehicle's head unit via a variety of transport protocols such as Bluetooth, USB, Android AOA, and TCP. Once a connection is established, Core discovers compatible applications and displays them to the driver for interaction via voice or display. The core component is implemented into the vehicle HMI based on the integration guidelines above. The core component is configured to follow a set of policies defined in a policy database and updated by a [policy server](https://www.github.com/smartdevicelink/sdl_server). The messaging between a connected application and core is defined by the [Mobile API](https://github.com/smartdevicelink/sdl_core/blob/master/src/components/interfaces/MOBILE_API.xml) and the messaging between sdl core and the vehicle is defined by the [HMI API](https://github.com/smartdevicelink/sdl_core/blob/master/src/components/interfaces/HMI_API.xml).

## Project Status
We're ramping up our efforts to get SmartDeviceLink developed and maintained directly in the open. For the Mobile libraries, we're expecting better integration soon, SDL Core is slightly more complicated. We are currently working on generating documentation, creating a developer portal, an open forum, Mobile validation, and everything else that we've been asked for to renew the community's interest in this project. From a technical standpoint, SDL is stable, and the most work is being put into making it a more robust solution for app connectivity. We are, however, definitely looking for and interested in other people and company's contributions to SDL whether it be feature based, bug fixes, healthy conversation, or even just suggestions for improvement.

# Getting Started

####Linux

A quick guide to installing, configuring, and running an instance of the SDL Core on a linux OS.

  1. Clone this repository
  2. Create a folder for your build and run `cmake ../sdl_core`
  3. If there are any dependency issues, install missing dependencies
  4. Run the following commands to compile and install smartdevicelink

```
%make
%make install
%cp bin/mykey.pem src/appMain
%cp bin/mycert.pem src/appMain
```

####Windows

> Required OS: Windows 7 64-bits

##### Common steps for Windows and Qt platforms

* Download and install [Cmake](https://cmake.org/files/v3.2/cmake-3.2.3-win32-x86.exe)<br>
Note: it will ask you about adding to the `PATH` environment variable. Press `Yes`.
* Download and install [python](https://www.python.org/ftp/python/2.7.10/python-2.7.10.msi) and add it to the `PATH`
environment variable
* Download and install [perl](http://www.activestate.com/activeperl/downloads/thank-you?dl=http://downloads.activestate.com/ActivePerl/releases/5.20.2.2002/ActivePerl-5.20.2.2002-MSWin32-x86-64int-299195.msi). Add it to `PATH`
* Download and install [git](https://git-scm.com/download/win)
* Download and install [GStreamer SDK](http://docs.gstreamer.com/display/GstSDK/Installing+on+Windows). You will need both Runtime and Development files. Add path to **bin** folder to `PATH`. Create new environment variable SDL_GSTREAMER_DIR with path to SDK (e.g. `D:\gstreamer-sdk\0.10\x86_64`).

##### Steps for Windows platform
* Download [sqlite3 sources](https://www.sqlite.org/2015/sqlite-amalgamation-3090200.zip) and [sqlite3 binaries](https://www.sqlite.org/2015/sqlite-dll-win64-x64-3090200.zip). Extract all files to the same directory. Create new  environment variable `SDL_SQLITE_DIR` and assign path where sqlite sources and binaries were extracted to this variable.
* Install [Windows SDK 7.1](https://www.microsoft.com/en-us/download/details.aspx?id=8442)
* Install [Visual Studio 2013](https://www.microsoft.com/en-us/download/confirmation.aspx?id=44914)

##### Steps for Qt platform
* Install [Visual Studio 2010](http://go.microsoft.com/?linkid=9709969)
* Then install [Visual Studio SP1](https://www.microsoft.com/en-us/download/confirmation.aspx?id=23691)
* Install [Qt framework](http://download.qt.io/archive/qt/5.5/5.5.1/qt-opensource-windows-x86-msvc2010-5.5.1.exe).
**Note:** It looks like starting from Qt5.6 there is no more msvc2010 32-bit support, so check whether such option is present. If no - use Qt5.5.
 Create new environment variable called `SDL_QT_DIR` and assign your qt install dir to this variable. I.e `set SDL_QT_DIR=C:\Qt`

##### Optional steps
* Download and install [Doxygen] (http://sourceforge.net/projects/doxygen/files/latest/download) and add it to `Path` envronment variable

#### Build project

##### Common steps for Windows and Qt platforms

* Clone repository

##### Steps for Windows platform

* Go to directory with cloned repository
* Run setup_VS2013_x64.cmd to generate solution for Visual Studio 2013.
**Note:** In case Visual Studio has been installed to non-default location -- modify path in .cmd file.
**Note:** Double click on file or run from windows command prompt. DO NOT run in "git bash".
* After script execution finish, go to build directory which is located one directory upper and open smartDeviceLinkCore.sln file using Visual Studio 2013
* Set smartDeviceLinkCore project as a startup in order to be able to run or debug it from Visual Studio
* Press `ctrl+alt+F7` to run solution building process in Visual Studio
* After building done the executable file will be located in `%BUILD_DIR%\src\appMain\Debug|Release\`

##### Steps for Qt platform

* Go to directory with cloned repository.
* Run setup_Qt_x86.cmd to generate build dir for the Qt project. This will prepare the DEBUG build.
**Note:** In case Visual Studio has been installed to non-default location -- modify path in .cmd file
**Note:** Double click on file or run from windows command prompt. DO NOT run in "git bash".
* Open Qt Creator (tested on v3.6.0)
* "Open File Or Project" -> pick the CMakeLists.txt in the project root.
* In the appeared dialog choose VS 2010 x86 toolchain, leave only one "Default" path, fill it with the full path to the build directory, generated by Qt_x86.cmd.
It should be `<project root>\build\sdl_win_qt_x86`. And click next.

To change the configuration from DEBUG to RELEASE (e.g. open "Project" window and click "Run CMake" button). To the "Arguments" edit box put the release cmake arguments:
```
-DCMAKE_BUILD_TYPE=Release
```
To setup the DEBUG build use:
```
-DCMAKE_BUILD_TYPE=Debug
```


## Start SDL Core
Once SDL Core is compiled and installed you can start it from the executable in the bin folder

####Linux

```
%cd src/appMain
%./smartDeviceLinkCore
```

####Windows

##### Steps for Windows platform

* Usb driver instalation:
    * Download [Usb driver installer](http://zadig.akeo.ie/downloads/zadig_2.1.2.exe)
    * Run it and check Options -> List all devices
    * Connect the cell phone to the PС with the USB cable
    * In the list choose your cell phone
    * Pick the WinUSB (v6.1.7600.16385) driver for the cell phone. If there was some driver already (field Driver is not empty), then reinstall it. See ** usb notes ** for additional info.
* Download and install [Visual C++ 2013 Redistributable] (https://www.microsoft.com/en-us/download/details.aspx?id=40784) in case it has not been instaled during previous steps

##### Bluetooth transport setup

Install device drivers ("Windows Mobile Device Center" current version is 6.1). [Download Link](https://www.microsoft.com/en-US/download/details.aspx?id=3182)

After all drivers were installed, plug the bluetooth adapter into PC.
Windows should install all required drivers. If drivers were not installed automatically then
install them manually.

* Right click on "bluetooth system tray" icon. Click "Add Device".
* Pick the correct device
* Approve the pin code on the device.
* Install the device (mobile) drivers to the PC.
Double check: right click "bluetooth tray icon" -> "Show bluetooth devices" -> "Right click device" -> "Properties" -> "Hardware" tab. All items in the list should be "ok" (no red and yellow icons).
If there are problems with drivers then install them next way.

* Go to "Device Manager" from "Control Panel" or just right-click on Computer Icon and choose "Properties" and then click on "Device Manager".
* You'll see an "Unknown Device" named something like "Bluetooth Peripheral Device".
* Right-Click it and click on "Update Driver Software" or anyway you find it easy to bring up the Update driver window.
* Click "Browse My Computer for Driver Software" button.
* Click "Let Me Pick from a List of Device Drivers on my computer".
* Select "Bluetooth Radio" from the list if it asked you to select and then in the next window, it will show a list of drivers with Company Names in one list and Drivers in another. In the Company List choose "Microsoft Corporation", not only Microsoft.
* From the drivers list there may be one or more drivers with the name "Windows Mobile-Based Device Support" with different driver versions. Select one or the latest.
* Ignore any warnings and keep pressing next and then Finish at last. If all goes well, the last screen will show the message that device driver is successfully installed.

After all drivers have been installed allow SyncProxy service:
* On the device try to add bluetooth session in the SPT. It will not succeed. It will just wait.
* Configure access on the PC:
"Bluetooth tray icon" -> "Show bluetooth devices" -> "Right click device" -> "Properties" -> "Services" -> check "SyncProxy" -> "Apply"
* After "SyncProxy" was added there will appear new device without driver. Add driver for this device as was described above.

After all drivers were installed and "SyncProxy" service was allowed the SPT will be able to connect to the SDL.

There should be one paired device.

#####Tested and expected to work on next devices:
- Alcatel OneTouch
- Acer S500
- Nexus 4

##### Steps for Qt platform

* Download and install [Visual C++ 2010 Redistributable] (https://www.microsoft.com/en-us/download/details.aspx?id=5555) in case it has not been instaled during previous steps

Executable is available in
Windows native: <build dir>\sdl_win_x64\src\appMain\<Debug or Release>
Windows Qt: <build>\sdl_win_qt_x86\src\appMain

Notes for developer:
* to run in the IDE just click run|debug buttons.
* To run from the console do next. Go to the `%BUILD_DIR%\src\appMain` and run `smartDeviceLinkCore.exe`. Make sure that Qt dlls are in your PATH or are placed near executable. List of required dlls which expected by executable: `Qt5Core.dll`, `Qt5Network.dll`, `Qt5Core.dll`, `sqldrivers\qsqlite.dll` (this one is expected to be in sub-directory)

## Start WEB HMI
Web HMI is separated from SDL Core and located in another repository. So to make it workable please do next steps.

  1. Clone http://github.com/smartdevicelink/sdl_hmi.git
  2. Follow the instruction from readme file in sdl_hmi repository.


## A quick note about dependencies
The dependencies for SDL Core vary based on the configuration. You can change SDL Core's configuration in the top level CMakeLists.txt. We have defaulted this file to a configuration which we believe is common for people who are interested in getting up and running quickly.

### Dependencies list

####Linux

| Flag | Description | Dependencies |
|------|-------------|--------------|
|Web HMI|Use HTML5 HMI|chromium-browser|
|HMI2|Build with QT HMI|QT5, dbus-*dev|
|EXTENDED_MEDIA_MODE|Support Video and Audio Streaming|Opengl es2, gstreamer1.0*|
|Bluetooth|Enable bluetooth transport adapter|libbluetooth3, libbluetooth-dev, bluez-tools|
|Testing framework|Needed to support running unit tests|libgtest-dev|
|Cmake|Needed to configure SDL prior to compilation|cmake|

####Windows

All dependencies described in **Build project** section

### Known Dependency Issues

####Linux

  * log4cxx - We know that the version of log4cxx on a linux machine can conflict with the one used, which is why it is provided in the repository. To avoid the conflict, we recommend removing liblog4cxx*.
  * libusb - is needed for USB support. Install command: `sudo apt-get install libusb-1.0-0`
  * cmake - on some versions of linux, the included cmake package doesn't have the right version. If apt-get is your package manager, you can find the correct version using
```
sudo apt-get install cmake
sudo add-apt-repository ppa:kalakris/cmake
sudo apt-get update
sudo apt-get upgrade
```

####Windows
#####USB transport notes

For some cell phones (e.g. Nexus 4) it's not enough to install only one driver.
Check the list of devices and ensure that there are no devices without drives. It means
that some devices create two entries in the list.
E.g. in the Nexus4 case there was "Android Interface ..." without driver.

There is one more thing. To make libusb work we have to install WinUSB drivers for the device. Even if the system has it's own drivers.
This leads to the next situation with some devices. Before drivers update we can see the device in the explorer and e.g. upload files to the device.
But after the update we don't see the device. This can be unacceptable in some use cases.

#####Tested devices

Devices which work via USB:
- HTC One
- Nexus 4
- Samsung Galaxy S4 mini
- Sony Xperia C2305

Devices which don't work. Win drivers were updated, but SDL doesn't register them. There is no message transfer.
- Alcatel OneTouch

Devices which don't work. There are no win drivers
- Acer S500

#####Information regarding used library

In the posix the libusbx 1.0.16 is used in the SDL. [libusbx](http:\\libusbx.org) is the fork from the libusb(http://libusb.info/).
Currently the libusbx is completely merged into libusb. Current implementation of the usb transport requires the "hot plug" feature.
But it's missing on windows platform( [issue](https://github.com/libusb/libusb/issues/86) ).
There is a "non-official" patch in the [forked repository](https://github.com/dickens/libusbx-hp/tree/windows-hotplug-3).
This patch was applied to our version of the libusbx. The patched version is based on the more recent version, 1.0.17.

## Required RPCs
There are several RPCs that are "required" to be implemented in order for SDL to work across vehicle manufacturers and applications, listed below.  The RPC specification can be found in the [Mobile API Spec](src/components/interfaces/MOBILE_API.xml).

  * RegisterAppInterface
  * UnregisterAppInterface
  * SetGlobalProperties
  * ResetGlobalProperties
  * AddCommand
  * DeleteCommand
  * AddSubMenu
  * DeleteSubMenu
  * CreateInteractionChoiceSet
  * PerformInteraction
  * DeleteInteractionChoiceSet
  * Alert
  * Show
  * SetMediaClockTimer
  * SubscribeButton
  * UnsubscribeButton
  * ChangeRegistration
  * GenericResponse
  * SystemRequest
  * OnHMIStatus
  * OnAppInterfaceUnregistered
  * OnButtonEvent
  * OnButtonPress
  * OnCommand
  * OnDriverDistraction
  * OnPermissionsChange
  * OnLanguageChange
  * OnSystemRequest
  * Speak

## App Launching

Below are instructions for testing app launching and query with a full system set up.

### SDL Server
The app querying specification defines an endpoint within Policies where sdl_core will reach out to receive a list of applications that can be launched. The SDL Server provides the back end functionality for app launching and querying.

You can find the SDL Server on [GitHub](https://github.com/smartdevicelink/sdl_server). The README contains detailed instructions for installing and launching the server. Launch the server on your local machine, and direct your browser to http://localhost:3000.

The [App Launching Server Specification](https://github.com/smartdevicelink/sdl_server/blob/master/docs/application_launching_v1.0.md) defines an endpoint `/applications/available/:moduleId.json` which return a list of applications available for launching to the handset for filtering.

To check if there is a module already available you can go to http://localhost:3000/modules.json. If there is a module available, there will be one or more objects in the response array. Keep this response, you'll need the "_id" field for later.

If there is not a module already available, go to http://localhost:3000/cars and define a new vehicle, then check http://localhost:3000/modules.json.

Next, you'll need to define applications that can be launched. Go to http://localhost:3000/apps and define some applications. Make sure that you define a url scheme under the iOS tab of the application. This is required for an application to be launched from SDL. A URL scheme has the format `someScheme://`. Save the URL Scheme you used for later steps.

You'll also need the local ip address of your machine

At the end of the SDL Server set up you should have
  1. SDL Server running on your local machine connected to mongo db
  2. Your machine's local IP Address
  3. The module id of your vehicle
  4. The URL Scheme of the app you want to launch

### Mobile
You need at least one app installed on the test device (presumably an iPhone), which we have built for you, the [V4Tester application](https://app.box.com/s/eeloquc0fhqfmxjjubw7kousf12f3pzg). This application implements SDL 4.0 and will respond to SDL Core's QUERY_APPS system request, as well as filter the response for available applications. If you do not have any other applications on the device, you can only test QUERY_APPS functionality, in which no applications will be sent to sdl core which can be launched.

In order to support the launching of an application, you'll have to create an additional app which responds to the URL Scheme of the application that you set up on the SDL Server. To do so, go to Xcode, select File>New>Project... and under ios/application create a Single View Application. Open the application's Info.plist file (under the Supporting Files section of the project explorer by default). Highlight the Information Property List item and click the plus button to add a new entry to the Property List. From the drop down menu, select URL Types as the key. In the Item 0 dictionary add a "URL Schemes" Array, and make Item 0 in the array the prefix to the URL you previously defined (So if you defined `someScheme://` then Item 0 should be "someScheme"). Make sure the URL identifier matches your application's identifier. When you're finished you should have something that looks like the following. Install this application on your test device. **Note** - this application will only launch during this process, since it is not SDL Connected it will not register with the head unit.

![Plist Example](http://i.imgur.com/AFyJlZQ.png)

At the end of the Mobile device set up you should have
  1. The V4 Tester Application installed on your device
  2. An application for launching that matches the application submitted to SDL Server
  3. Your iPhone should be on the same network as the machine running SDL Server

### SDL Core
Take the following steps to launch applications from sdl core.

  1. Install the [correct version of SDL Core](https://github.com/smartdevicelink/sdl_core/pull/39)
  2. Add the queryAppsUrl that you saved during sdl server set up in the src/appMain/preloaded_pt.json under the "endpoints" property in the format `http://[local machine ip]:3000/applications/available[moduleId].json`. For example `http://192.168.0.150:3000/applications/available/789b739c47c7490321058200.json`.
  3. Run SDL Core
  4. Launch the V4 Tester application on the iPhone
  5. Connect the application via wifi by entering the IP address of Core into the V4 tester
  6. Both applications should show up on the head unit for launching
  7. Select the other application, and you should see it launched and brought to the foreground on the phone

## Test Coverage
### Used technologies
  * GCOV - test coverage program.
  * LCOV - graphical front-end for GCC's coverage testing tool for gcov.
  * codecov.io - service for assembling code coverage and representing it in a clear for reading form.

### Excluded folders
_We test only sources written by us and we don`t need to test external sources(open source libraries)._
  * '/usr/\*' - local libraries shouldn`t be covered by tests.
  * '\*/test/\*' - we don`t need to cover tests.
  * '\*/src/3rd\*' - open source libraries shouldn`t be covered by tests.

### Current test coverage
You can find it in [Coverage report](https://codecov.io/gh/smartdevicelink/sdl_core/branch/develop)

### How to get Test Coverage locally
 Precondition : if you dont have lcov, gcov, html2text, install it before running.
 1. Build project with flags _-DBUILD_TESTS=on -DENABLE_GCOV=on_
 2. Execute command 'make test'
 3. Execute './tools/infrastructure/collect_coverage.sh \<path_to_build_directory\>'

 Note :
   * In case of a lot of warnings probably issue is gcov ang gcc version mismatch.
   * You should install gcov-4.9 as default gcov in you system.
   * Please ensure that /usr/bin/gcov is 4.9 version.
   * And execute : _sudo ln -sf /usr/bin/gcov-4.9 /usr/bin/gcov_

## Contributions

Conversation regarding the design and development of SmartDeviceLink technology should be directed at the [GENIVI mailing list](https://lists.genivi.org/mailman/listinfo/genivi-smartdevicelink), which anyone can join. Public conference calls regarding the SmartDeviceLink technology will be announced to the GENIVI mailing list, we expect to have conversations every other week. We also encourage interested parties to write issues against our software, and submit pull requests right here in the GitHub repository.
