[![Slack Status](http://sdlslack.herokuapp.com/badge.svg)](http://slack.smartdevicelink.org)
 
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

## Start SDL Core
Once SDL Core is compiled and installed you can start it from the executable in the bin folder

```
%cd src/appMain
%./smartDeviceLinkCore
```

## Start WEB HMI
Web HMI is separated from SDL Core and located in another repository. So to make it workable please do next steps.

  1. Clone http://github.com/smartdevicelink/sdl_hmi.git
  2. Follow the instruction from readme file in sdl_hmi repository.


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

## App Launching

Below are instructions for testing app launching and query with a full system set up.

### SDL Server
The app querying specification defines an endpoint within Policies where sdl_core will reach out to receive a list of applications that can be launched. The SDL Server provides the back end functionality for app launching and querying.

You can find the SDL Server on [GitHub](https://github.com/smartdevicelink/sdl_server). The README contains detailed instructions for installing and launching the server. Launch the server on your local machine, and direct your browser to http://localhost:3000. Note that you need `mongod` running on your machine before launching the server.

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

## Contributions

Conversation regarding the design and development of SmartDeviceLink technology should be directed at the [GENIVI mailing list](https://lists.genivi.org/mailman/listinfo/genivi-smartdevicelink), which anyone can join. Public conference calls regarding the SmartDeviceLink technology will be announced to the GENIVI mailing list, we expect to have conversations every other week. We also encourage interested parties to write issues against our software, and submit pull requests right here in the GitHub repository.
