## Important

The current stable SDL codebase can be found [here](http://git.projects.genivi.org/?p=smartdevicelink.git;a=shortlog;h=refs/heads/dev/merge_3.5), an updated 3.x version will follow on github and be mirrored to GENIVI ASAP. The instructions below refer to the 3.x version that is not yet available. :( I hope to have this resolved before you read this readme.

# SmartDeviceLink (SDL)

SmartDeviceLink is a way to connect applications on a smart phone to a vehicle so they can be controlled by a vehicle's head unit (radio interface).  There are three main pieces to SDL - [core](https://github.com/smartdevicelink/sdl_core) (head unit), [mobile libraries](https://github.com/smartdevicelink/sdl_android), and the [server](https://github.com/smartdevicelink/sdl_server).

Pull Requests Welcome!

## SDL Core

The Core component of SDL is meant to run on a vehicle's computing system. It connects to a consumer's smartphone over a variety of transports such as Bluetooth, USB, Android AOA, and TCP. Transport adapters can be written to extend the capabilities of Core's transport manager. Once a smartphone is connected over a preferred transport applications on the mobile phone that implement the mobile libraries can begin to communicate with the vehicle. First, the Core component queries the phone to understand which applications are compatible. The user interface shows the list of compatible applications to the driver. When the user selects an application, it is sent an HMI_STATUS level of FULL (all applications start with a BACKGROUND status). Once an application has the FULL status, it is allowed to populate a template, defined by the vehicle manufacturer, with content such as metadata, artwork, buttons and even supported grammars for voice commands (voice recognition engine not included).

The communication between the Core component and the mobile phone is based on JSON RPC. The communication between the Core component and the user interface is also via JSON RPC. The current code base includes an HTML5 user interface that was released by Ford Motor Company for the My Ford Touch system. The core component can also be configured to support QT5 and DBus, and includes a QT interface as well.

## Project Status
We're ramping up our efforts to get SmartDeviceLink developed and maintained directly in the open. For the Mobile libraries, we're expecting better integration soon, SDL Core is slightly more complicated. We are currently working on generating documentation, creating a developer portal, an open forum, interface requirements, HMI guidelines, Mobile validation, and everything else that we've been asked for to renew the community's interest in this project. From a technical standpoint, SDL is stable, and the most work is being put into making it a more robust solution for app connectivity. We are, however, definitely looking for and interesting in other people and company's contributions to SDL whether it be feature based, bug fixes, healthy conversation, or even just suggestions for improvement.

# Getting Started
A quick guide to installing, configuring, and running an instance of the SDL Core on a linux OS.

  1. Clone this repository
  2. Create a folder for your build and run `cmake ../sdl_core`
  3. If there are any dependency issues, install missing dependencies
  4. Run the following commands to compile and install smartdevicelink



```
%make
%sudo make install
%cp bin/mykey.pem src/appMain
%cp bin/mycert.pem src/appMain
%sudo updatedb
%sudo ldconfig
```

## Start SDL Core
Once SDL Core is compiled and installed you can start it from the executable in the bin folder

```
%cd src/appMain
%./smartDeviceLinkCore
```


## A quick note about dependencies
The dependencies for SDL Core vary based on the configuration. You can change SDL Core's configuration in the top level CMakeLists.txt. We have defaulted this file to a configuration which we believe is common for people who are interested in getting up and running quickly, generally on a Linux VM.

### Dependencies list

| Flag | Description | Dependencies |
|------|-------------|--------------|
|Web HMI|Use HTML5 HMI|chromium-browser|
|HMI2|Build with QT HMI|QT5, dbus-*dev|
|EXTENDED_MEDIA_MODE|Support Video and Audio Streaming|Opengl es2, gstreamer1.0*|
|Bluetooth|Enable bluetooth transport adapter|libbluetooth3, libbluetooth-dev, bluez-tools|
|Testing framework|Needed to support running unit tests|libgtest-dev|
|Cmake|Needed to configure SDL prior to compilation|cmake|

### Known Dependency Issues
  * log4cxx - We know that the version of log4cxx on a linux machine can conflict with the one used, which is why it is provided in the repository. To avoid the conflict, we recommend removing liblog4cxx*.
  * cmake - on some versions of linux, the included cmake package doesn't have the right version. If apt-get is your package manager, you can find the correct version using
```
sudo apt-get install cmake
sudo add-apt-repository ppa:kalakris/cmake
sudo apt-get update
sudo apt-get upgrade
```

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

## Contributions

Conversation regarding the design and development of SmartDeviceLink technology should be directed at the [GENIVI mailing list](https://lists.genivi.org/mailman/listinfo/genivi-smartdevicelink), which anyone can join. Public conference calls regarding the SmartDeviceLink technology will be announced to the GENIVI mailing list, we expect to have conversations every other week. We also encourage interested parties to write issues against our software, and submit pull requests right here in the GitHub repository.
