//!!

import QtQuick 2.0
import "../hmi_api/Common.js" as Common

QtObject {
    property real speed: 80.0 // 0-700
    property int rpm: 5000 //0-20000
    property real fuelLevel: 0.2
    property int fuelLevel_State: Common.ComponentVolumeStatus.CVS_UNKNOWN
    property real instantFuelConsumption: 2.2
    property int driverBraking: Common.VehicleDataEventStatus.VDES_NO_EVENT
    property int wiperStatus: Common.WiperStatus.OFF
    property real engineTorque: 2.5 // -1000 - 2000
    property real accPedalPosition: 0.5 // 0-100
    property real steeringWheelAngle: 1.2 // -2000 - 2000
    property real externalTemperature: 40.0
    property string vin: '52-452-52-752' // maxLenght 17
    property int odometer: 0 //0-17000000
    property int prndl: Common.PRNDL.PARK

    property var myKey: {
        "e911Override": Common.VehicleDataStatus.VDS_NO_DATA_EXISTS
    }
    property var headLampStatus: {
        "lowBeamsOn": false,
        "highBeamsOn": false
    }
    property var deviceStatus: {
        'voiceRecOn': false,
        'btIconOn': false,
        'callActive': false,
        'phoneRoaming': false,
        'textMsgAvailable': false,
        'battLevelStatus': Common.DeviceLevelStatus.ZERO_LEVEL_BARS,
        'stereoAudioOutputMuted': false,
        'monoAudioOutputMuted': false,
        'signalLevelStatus': Common.DeviceLevelStatus.ZERO_LEVEL_BARS,
        'primaryAudioSource': Common.PrimaryAudioSource.NO_SOURCE_SELECTED,
        'eCallEventActive': false
    }
    property var bodyInformation: {
        'parkBrakeActive': false,
        'ignitionStableStatus': Common.IgnitionStableStatus.IGNITION_SWITCH_NOT_STABLE,
        'ignitionStatus': Common.IgnitionStatus.IS_UNKNOWN
    }
    property var beltStatus: {
        'driverBeltDeployed': Common.VehicleDataEventStatus.VDES_NO_EVENT,
        'passengerBeltDeployed': Common.VehicleDataEventStatus.VDES_NO_EVENT,
        'passengerBuckleBelted': Common.VehicleDataEventStatus.VDES_NO_EVENT,
        'driverBuckleBelted': Common.VehicleDataEventStatus.VDES_NO_EVENT,
        'leftRow2BuckleBelted': Common.VehicleDataEventStatus.VDES_NO_EVENT,
        'passengerChildDetected': Common.VehicleDataEventStatus.VDES_NO_EVENT,
        'rightRow2BuckleBelted': Common.VehicleDataEventStatus.VDES_NO_EVENT,
        'middleRow2BuckleBelted': Common.VehicleDataEventStatus.VDES_NO_EVENT,
        'middleRow3BuckleBelted': Common.VehicleDataEventStatus.VDES_NO_EVENT,
        'leftRow3BuckleBelted': Common.VehicleDataEventStatus.VDES_NO_EVENT,
        'rightRow3BuckleBelted': Common.VehicleDataEventStatus.VDES_NO_EVENT,
        'leftRearInflatableBelted': Common.VehicleDataEventStatus.VDES_NO_EVENT,
        'rightRearInflatableBelted': Common.VehicleDataEventStatus.VDES_NO_EVENT,
        'middleRow1BeltDeployed': Common.VehicleDataEventStatus.VDES_NO_EVENT,
        'middleRow1BuckleBelted': Common.VehicleDataEventStatus.VDES_NO_EVENT,
    }
    property var tirePressure: {
        'pressureTelltale': Common.WarningLightStatus.WLS_OFF,
        'leftFront': Common.ComponentVolumeStatus.CVS_UNKNOWN,
        'rightFront': Common.ComponentVolumeStatus.CVS_UNKNOWN,
        'leftRear': Common.ComponentVolumeStatus.CVS_UNKNOWN,
        'rightRear': Common.ComponentVolumeStatus.CVS_UNKNOWN,
        'innerLeftRear': Common.ComponentVolumeStatus.CVS_UNKNOWN,
        'innerRightRear': Common.ComponentVolumeStatus.CVS_UNKNOWN
    }
    property var gps: {
        'longitudeDegrees': 423293,
        'latitudeDegrees': -830464,
        'utcYear': 2013,
        'utcMonth': 2,
        'utcDay': 14,
        'utcHours': 13,
        'utcMinutes': 16,
        'utcSeconds': 54,
        'compassDirection': Common.CompassDirection.SOUTHWEST,
        'pdop': 15,
        'hdop': 5,
        'vdop': 30,
        'actual': false,
        'satellites': 8,
        'dimension': Common.Dimension.Dimension_2D,
        'altitude': 7,
        'heading': 173,
        'speed': 2
    }
    ///    'genericbinary': '165165650',
    ///    'satRadioESN': "165165650",
    ///    'rainSensor': 165165650,
    ///    'batteryPackVoltage': 12.5,
    ///    'batteryPackCurrent': 7.0,
    ///    'batteryPackTemperature': 30,
    ///    'engineTorque': 650,
}
