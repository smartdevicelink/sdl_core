[![Slack Status](http://sdlslack.herokuapp.com/badge.svg)](http://slack.smartdevicelink.com)
[![Build Status](https://opensdl-jenkins.prjdmz.luxoft.com/job/develop_weekly_coverage/badge/icon?subject=UT%20coverage%20build)](https://opensdl-jenkins.prjdmz.luxoft.com/job/develop_weekly_coverage/)
[![Build Status](https://opensdl-jenkins.prjdmz.luxoft.com/view/Smokes/job/Develop_TCP_ATF_Smoke_P/badge/icon?subject=Smoke%20tests)](https://opensdl-jenkins.prjdmz.luxoft.com/view/Smokes/job/Develop_TCP_ATF_Smoke_P/)
[![Build Status](https://opensdl-jenkins.prjdmz.luxoft.com/job/Develop_SDL_Checkstyle/badge/icon?subject=Coding%20style)](https://opensdl-jenkins.prjdmz.luxoft.com/job/Develop_SDL_Checkstyle/)

# SmartDeviceLink (SDL)

SmartDeviceLink (SDL) is a standard set of protocols and messages that connect applications on a smartphone to a vehicle head unit. This messaging enables a consumer to interact with their application using common in-vehicle interfaces such as a touch screen display, embedded voice recognition, steering wheel controls and various vehicle knobs and buttons. There are three main components that make up the SDL ecosystem.

  * The [Core](https://github.com/smartdevicelink/sdl_core) component is the software which Vehicle Manufacturers (OEMs) include in their vehicle head units. Integrating this component into their head unit and HMI based on a set of guidelines and templates enables access to various smartphone applications.
  * The optional [SDL Server](https://github.com/smartdevicelink/sdl_server) can be used by Vehicle OEMs to update application policies and gather usage information for connected applications.
  * The [iOS](https://github.com/smartdevicelink/sdl_ios) and [Android](https://github.com/smartdevicelink/sdl_android) libraries are implemented by app developers into their applications to enable command and control via the connected head unit.

## SDL Core

The Core component of SDL runs on a vehicle's computing system (head unit). Core’s primary responsibility is to route messages between connected smartphone applications and the vehicle HMI. It can connect a smartphone to a vehicle's head unit via a variety of transport protocols such as Bluetooth, USB, Android AOA, and TCP. Once a connection is established, Core discovers compatible applications and displays them to the driver for interaction via voice or display. The Core component is configured to follow a set of policies defined in its local policy database and updated by a [policy server](https://www.github.com/smartdevicelink/sdl_server). The messaging between a connected application and SDL Core is defined by the [Mobile API](https://github.com/smartdevicelink/rpc_spec/blob/master/MOBILE_API.xml) and the messaging between SDL Core and the vehicle is defined by the [HMI API](https://github.com/smartdevicelink/sdl_core/blob/master/src/components/interfaces/HMI_API.xml).

Pull Requests Welcome!

## Environment
Currently supported:
* Ubuntu Linux 16.04 with GCC 5.4.x
* Ubuntu Linux 18.04 with GCC 7.5.x
* Ubuntu Linux 20.04 with GCC 9.3.x
* [C++11 standard](https://github.com/smartdevicelink/sdl_evolution/issues/132)

## Getting Started
See the [Getting Started](https://smartdevicelink.com/en/guides/core/getting-started/install-and-run/) guide in the developer portal for instructions on how to build and start SDL Core.

## Connect a Web HMI
After setting up SDL Core, you will need to connect it to an external HMI. The HMI component of SDL needs to be developed separately from this project, and should be tailored to your specific head unit. Several sample browser-based HMIs are available for testing purposes:

  1. The [Generic HMI](https://github.com/smartdevicelink/generic_hmi) (preferred) is a reference implementation for an SDL-connected HMI, including examples of many of the UI elements and templates available to SDL applications.
  2. The [SDL HMI](https://github.com/smartdevicelink/sdl_hmi) emulates a full infotainment system which includes a simplified SDL component.

See the [HMI Development Guide](https://smartdevicelink.com/en/guides/core/integrating-your-hmi/sdl-core-and-hmi-communication/) for more information on building your own HMI.

## Manticore
If you are just developing an SDL-connected application and simply want an environment to test it in, it is not necessary to build SDL Core from scratch. You can use our [Manticore](https://smartdevicelink.com/resources/manticore/) project first, which creates a testing environment in the cloud using SDL Core and the Generic HMI.

## Frequently Asked Questions

If you have any questions regarding the setup of SDL Core or regarding the integration of SDL into a head unit, first visit our [SDL Core FAQ](https://smartdevicelink.com/en/guides/core/faq/). If you still have questions, you can post a question in the `core-help` channel of our Slack group (sign up [here](http://slack.smartdevicelink.com/)).

## Additional References

  * [Transport Manager Programming Guide](https://smartdevicelink.com/en/guides/core/transport-manager-programming/)
  * [HMI API Documentation](https://smartdevicelink.com/en/docs/hmi/master/overview/)
  * [Evolution Proposals versus Issues](https://github.com/smartdevicelink/sdl_evolution/blob/master/proposals_versus_issues.md)

## Contributions

Conversation regarding the design and development of SmartDeviceLink technology is managed via the [SDL Evolution Process](https://github.com/smartdevicelink/sdl_evolution/blob/master/process.md). We also encourage interested parties to write issues and submit pull requests right here in the GitHub repository. If you choose to do so, please read our [Contributing Guide](https://github.com/smartdevicelink/sdl_core/blob/master/.github/CONTRIBUTING.md) beforehand.
