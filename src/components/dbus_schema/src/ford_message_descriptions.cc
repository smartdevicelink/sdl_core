#include "dbus_schema/ford_message_descriptions.h"

namespace {

struct Structs {
  static const ford_message_descriptions::ParameterDescription* Common__SingleTireStatus__parameters[];
  static const ford_message_descriptions::ParameterDescription* Common__TTSChunk__parameters[];
  static const ford_message_descriptions::ParameterDescription* Common__BodyInformation__parameters[];
  static const ford_message_descriptions::ParameterDescription* Common__Image__parameters[];
  static const ford_message_descriptions::ParameterDescription* Common__VrHelpItem__parameters[];
  static const ford_message_descriptions::ParameterDescription* Common__HeadLampStatus__parameters[];
  static const ford_message_descriptions::ParameterDescription* Common__TimeFormat__parameters[];
  static const ford_message_descriptions::ParameterDescription* Common__GPSData__parameters[];
  static const ford_message_descriptions::ParameterDescription* Common__SoftButton__parameters[];
  static const ford_message_descriptions::ParameterDescription* Common__DIDResult__parameters[];
  static const ford_message_descriptions::ParameterDescription* Common__PresetBankCapabilities__parameters[];
  static const ford_message_descriptions::ParameterDescription* Common__HMIApplication__parameters[];
  static const ford_message_descriptions::ParameterDescription* Common__Choice__parameters[];
  static const ford_message_descriptions::ParameterDescription* Common__TireStatus__parameters[];
  static const ford_message_descriptions::ParameterDescription* Common__ButtonCapabilities__parameters[];
  static const ford_message_descriptions::ParameterDescription* Common__BeltStatus__parameters[];
  static const ford_message_descriptions::ParameterDescription* Common__DeviceInfo__parameters[];
  static const ford_message_descriptions::ParameterDescription* Common__MenuParams__parameters[];
  static const ford_message_descriptions::ParameterDescription* Common__SoftButtonCapabilities__parameters[];
  static const ford_message_descriptions::ParameterDescription* Common__DisplayCapabilities__parameters[];
  static const ford_message_descriptions::ParameterDescription* Common__VehicleType__parameters[];
  static const ford_message_descriptions::ParameterDescription* Common__MyKey__parameters[];
  static const ford_message_descriptions::ParameterDescription* Common__DeviceStatus__parameters[];
  static const ford_message_descriptions::ParameterDescription* Common__TextFieldStruct__parameters[];
  static const ford_message_descriptions::ParameterDescription* Common__Turn__parameters[];
};

struct Enums {
  static const ford_message_descriptions::EnumDescription::Entry* Common__ButtonEventMode__entries[];
  static const ford_message_descriptions::EnumDescription::Entry* Common__IgnitionStatus__entries[];
  static const ford_message_descriptions::EnumDescription::Entry* Common__ApplicationsCloseReason__entries[];
  static const ford_message_descriptions::EnumDescription::Entry* Common__PRNDL__entries[];
  static const ford_message_descriptions::EnumDescription::Entry* Common__ButtonName__entries[];
  static const ford_message_descriptions::EnumDescription::Entry* Common__TextFieldName__entries[];
  static const ford_message_descriptions::EnumDescription::Entry* Common__Language__entries[];
  static const ford_message_descriptions::EnumDescription::Entry* Common__TBTState__entries[];
  static const ford_message_descriptions::EnumDescription::Entry* Common__Result__entries[];
  static const ford_message_descriptions::EnumDescription::Entry* Common__VehicleDataResultCode__entries[];
  static const ford_message_descriptions::EnumDescription::Entry* Common__ClockUpdateMode__entries[];
  static const ford_message_descriptions::EnumDescription::Entry* Common__IgnitionStableStatus__entries[];
  static const ford_message_descriptions::EnumDescription::Entry* Common__TextAlignment__entries[];
  static const ford_message_descriptions::EnumDescription::Entry* Common__ButtonPressMode__entries[];
  static const ford_message_descriptions::EnumDescription::Entry* Common__SystemContext__entries[];
  static const ford_message_descriptions::EnumDescription::Entry* Common__MediaClockFormat__entries[];
  static const ford_message_descriptions::EnumDescription::Entry* Common__AppHMIType__entries[];
  static const ford_message_descriptions::EnumDescription::Entry* Common__PrimaryAudioSource__entries[];
  static const ford_message_descriptions::EnumDescription::Entry* Common__CompassDirection__entries[];
  static const ford_message_descriptions::EnumDescription::Entry* Common__SystemAction__entries[];
  static const ford_message_descriptions::EnumDescription::Entry* Common__DriverDistractionState__entries[];
  static const ford_message_descriptions::EnumDescription::Entry* Common__ImageType__entries[];
  static const ford_message_descriptions::EnumDescription::Entry* Common__WiperStatus__entries[];
  static const ford_message_descriptions::EnumDescription::Entry* Common__VehicleDataStatus__entries[];
  static const ford_message_descriptions::EnumDescription::Entry* Common__VehicleDataEventStatus__entries[];
  static const ford_message_descriptions::EnumDescription::Entry* Common__ComponentVolumeStatus__entries[];
  static const ford_message_descriptions::EnumDescription::Entry* Common__SoftButtonType__entries[];
  static const ford_message_descriptions::EnumDescription::Entry* Common__Dimension__entries[];
  static const ford_message_descriptions::EnumDescription::Entry* Common__WarningLightStatus__entries[];
  static const ford_message_descriptions::EnumDescription::Entry* Common__DeviceLevelStatus__entries[];
  static const ford_message_descriptions::EnumDescription::Entry* Common__DeactivateReason__entries[];
  static const ford_message_descriptions::EnumDescription::Entry* Common__DisplayType__entries[];
  static const ford_message_descriptions::EnumDescription::Entry* Common__HmiZoneCapabilities__entries[];
};

const ford_message_descriptions::EnumDescription Common__SingleTireStatus__parameter1 = {
  {
    "status",
    ford_message_descriptions::Enum,
    false,
    true
  },
  Enums::Common__ComponentVolumeStatus__entries
};
const ford_message_descriptions::ParameterDescription* Structs::Common__SingleTireStatus__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&Common__SingleTireStatus__parameter1,
  NULL };

const ford_message_descriptions::ParameterDescription Common__TTSChunk__parameter1 = {
  "text",
  ford_message_descriptions::String,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* Structs::Common__TTSChunk__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&Common__TTSChunk__parameter1,
  NULL };

const ford_message_descriptions::ParameterDescription Common__BodyInformation__parameter1 = {
  "parkBrakeActive",
  ford_message_descriptions::Boolean,
  false,
  true
};
const ford_message_descriptions::EnumDescription Common__BodyInformation__parameter2 = {
  {
    "ignitionStableStatus",
    ford_message_descriptions::Enum,
    false,
    true
  },
  Enums::Common__IgnitionStableStatus__entries
};
const ford_message_descriptions::EnumDescription Common__BodyInformation__parameter3 = {
  {
    "ignitionStatus",
    ford_message_descriptions::Enum,
    false,
    true
  },
  Enums::Common__IgnitionStatus__entries
};
const ford_message_descriptions::ParameterDescription* Structs::Common__BodyInformation__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&Common__BodyInformation__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&Common__BodyInformation__parameter2,
  (const ford_message_descriptions::ParameterDescription*)&Common__BodyInformation__parameter3,
  NULL };

const ford_message_descriptions::ParameterDescription Common__Image__parameter1 = {
  "value",
  ford_message_descriptions::String,
  false,
  true
};
const ford_message_descriptions::EnumDescription Common__Image__parameter2 = {
  {
    "imageType",
    ford_message_descriptions::Enum,
    false,
    true
  },
  Enums::Common__ImageType__entries
};
const ford_message_descriptions::ParameterDescription* Structs::Common__Image__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&Common__Image__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&Common__Image__parameter2,
  NULL };

const ford_message_descriptions::ParameterDescription Common__VrHelpItem__parameter1 = {
  "text",
  ford_message_descriptions::String,
  false,
  true
};
const ford_message_descriptions::StructDescription Common__VrHelpItem__parameter2 = {
  {
    "image",
    ford_message_descriptions::Struct,
    false,
    false
  },
  Structs::Common__Image__parameters
};
const ford_message_descriptions::ParameterDescription Common__VrHelpItem__parameter3 = {
  "position",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* Structs::Common__VrHelpItem__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&Common__VrHelpItem__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&Common__VrHelpItem__parameter2,
  (const ford_message_descriptions::ParameterDescription*)&Common__VrHelpItem__parameter3,
  NULL };

const ford_message_descriptions::ParameterDescription Common__HeadLampStatus__parameter1 = {
  "lowBeamsOn",
  ford_message_descriptions::Boolean,
  false,
  true
};
const ford_message_descriptions::ParameterDescription Common__HeadLampStatus__parameter2 = {
  "highBeamsOn",
  ford_message_descriptions::Boolean,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* Structs::Common__HeadLampStatus__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&Common__HeadLampStatus__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&Common__HeadLampStatus__parameter2,
  NULL };

const ford_message_descriptions::ParameterDescription Common__TimeFormat__parameter1 = {
  "hours",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription Common__TimeFormat__parameter2 = {
  "minutes",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription Common__TimeFormat__parameter3 = {
  "seconds",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* Structs::Common__TimeFormat__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&Common__TimeFormat__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&Common__TimeFormat__parameter2,
  (const ford_message_descriptions::ParameterDescription*)&Common__TimeFormat__parameter3,
  NULL };

const ford_message_descriptions::ParameterDescription Common__GPSData__parameter1 = {
  "longitudeDegrees",
  ford_message_descriptions::Float,
  false,
  true
};
const ford_message_descriptions::ParameterDescription Common__GPSData__parameter2 = {
  "latitudeDegrees",
  ford_message_descriptions::Float,
  false,
  true
};
const ford_message_descriptions::ParameterDescription Common__GPSData__parameter3 = {
  "utcYear",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription Common__GPSData__parameter4 = {
  "utcMonth",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription Common__GPSData__parameter5 = {
  "utcDay",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription Common__GPSData__parameter6 = {
  "utcHours",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription Common__GPSData__parameter7 = {
  "utcMinutes",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription Common__GPSData__parameter8 = {
  "utcSeconds",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::EnumDescription Common__GPSData__parameter9 = {
  {
    "compassDirection",
    ford_message_descriptions::Enum,
    false,
    true
  },
  Enums::Common__CompassDirection__entries
};
const ford_message_descriptions::ParameterDescription Common__GPSData__parameter10 = {
  "pdop",
  ford_message_descriptions::Float,
  false,
  true
};
const ford_message_descriptions::ParameterDescription Common__GPSData__parameter11 = {
  "hdop",
  ford_message_descriptions::Float,
  false,
  true
};
const ford_message_descriptions::ParameterDescription Common__GPSData__parameter12 = {
  "vdop",
  ford_message_descriptions::Float,
  false,
  true
};
const ford_message_descriptions::ParameterDescription Common__GPSData__parameter13 = {
  "actual",
  ford_message_descriptions::Boolean,
  false,
  true
};
const ford_message_descriptions::ParameterDescription Common__GPSData__parameter14 = {
  "satellites",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::EnumDescription Common__GPSData__parameter15 = {
  {
    "dimension",
    ford_message_descriptions::Enum,
    false,
    true
  },
  Enums::Common__Dimension__entries
};
const ford_message_descriptions::ParameterDescription Common__GPSData__parameter16 = {
  "altitude",
  ford_message_descriptions::Float,
  false,
  true
};
const ford_message_descriptions::ParameterDescription Common__GPSData__parameter17 = {
  "heading",
  ford_message_descriptions::Float,
  false,
  true
};
const ford_message_descriptions::ParameterDescription Common__GPSData__parameter18 = {
  "speed",
  ford_message_descriptions::Float,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* Structs::Common__GPSData__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&Common__GPSData__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&Common__GPSData__parameter2,
  (const ford_message_descriptions::ParameterDescription*)&Common__GPSData__parameter3,
  (const ford_message_descriptions::ParameterDescription*)&Common__GPSData__parameter4,
  (const ford_message_descriptions::ParameterDescription*)&Common__GPSData__parameter5,
  (const ford_message_descriptions::ParameterDescription*)&Common__GPSData__parameter6,
  (const ford_message_descriptions::ParameterDescription*)&Common__GPSData__parameter7,
  (const ford_message_descriptions::ParameterDescription*)&Common__GPSData__parameter8,
  (const ford_message_descriptions::ParameterDescription*)&Common__GPSData__parameter9,
  (const ford_message_descriptions::ParameterDescription*)&Common__GPSData__parameter10,
  (const ford_message_descriptions::ParameterDescription*)&Common__GPSData__parameter11,
  (const ford_message_descriptions::ParameterDescription*)&Common__GPSData__parameter12,
  (const ford_message_descriptions::ParameterDescription*)&Common__GPSData__parameter13,
  (const ford_message_descriptions::ParameterDescription*)&Common__GPSData__parameter14,
  (const ford_message_descriptions::ParameterDescription*)&Common__GPSData__parameter15,
  (const ford_message_descriptions::ParameterDescription*)&Common__GPSData__parameter16,
  (const ford_message_descriptions::ParameterDescription*)&Common__GPSData__parameter17,
  (const ford_message_descriptions::ParameterDescription*)&Common__GPSData__parameter18,
  NULL };

const ford_message_descriptions::EnumDescription Common__SoftButton__parameter1 = {
  {
    "type",
    ford_message_descriptions::Enum,
    false,
    true
  },
  Enums::Common__SoftButtonType__entries
};
const ford_message_descriptions::ParameterDescription Common__SoftButton__parameter2 = {
  "text",
  ford_message_descriptions::String,
  false,
  false
};
const ford_message_descriptions::StructDescription Common__SoftButton__parameter3 = {
  {
    "image",
    ford_message_descriptions::Struct,
    false,
    false
  },
  Structs::Common__Image__parameters
};
const ford_message_descriptions::ParameterDescription Common__SoftButton__parameter4 = {
  "isHighlighted",
  ford_message_descriptions::Boolean,
  false,
  false
};
const ford_message_descriptions::ParameterDescription Common__SoftButton__parameter5 = {
  "softButtonID",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::EnumDescription Common__SoftButton__parameter6 = {
  {
    "systemAction",
    ford_message_descriptions::Enum,
    false,
    false
  },
  Enums::Common__SystemAction__entries
};
const ford_message_descriptions::ParameterDescription* Structs::Common__SoftButton__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&Common__SoftButton__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&Common__SoftButton__parameter2,
  (const ford_message_descriptions::ParameterDescription*)&Common__SoftButton__parameter3,
  (const ford_message_descriptions::ParameterDescription*)&Common__SoftButton__parameter4,
  (const ford_message_descriptions::ParameterDescription*)&Common__SoftButton__parameter5,
  (const ford_message_descriptions::ParameterDescription*)&Common__SoftButton__parameter6,
  NULL };

const ford_message_descriptions::EnumDescription Common__DIDResult__parameter1 = {
  {
    "resultCode",
    ford_message_descriptions::Enum,
    false,
    true
  },
  Enums::Common__VehicleDataResultCode__entries
};
const ford_message_descriptions::ParameterDescription Common__DIDResult__parameter2 = {
  "didLocation",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription Common__DIDResult__parameter3 = {
  "data",
  ford_message_descriptions::String,
  false,
  false
};
const ford_message_descriptions::ParameterDescription* Structs::Common__DIDResult__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&Common__DIDResult__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&Common__DIDResult__parameter2,
  (const ford_message_descriptions::ParameterDescription*)&Common__DIDResult__parameter3,
  NULL };

const ford_message_descriptions::ParameterDescription Common__PresetBankCapabilities__parameter1 = {
  "onScreenPresetsAvailable",
  ford_message_descriptions::Boolean,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* Structs::Common__PresetBankCapabilities__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&Common__PresetBankCapabilities__parameter1,
  NULL };

const ford_message_descriptions::ParameterDescription Common__HMIApplication__parameter1 = {
  "appName",
  ford_message_descriptions::String,
  false,
  true
};
const ford_message_descriptions::ParameterDescription Common__HMIApplication__parameter2 = {
  "ngnMediaScreenAppName",
  ford_message_descriptions::String,
  false,
  false
};
const ford_message_descriptions::ParameterDescription Common__HMIApplication__parameter3 = {
  "icon",
  ford_message_descriptions::String,
  false,
  false
};
const ford_message_descriptions::ParameterDescription Common__HMIApplication__parameter4 = {
  "deviceName",
  ford_message_descriptions::String,
  false,
  true
};
const ford_message_descriptions::ParameterDescription Common__HMIApplication__parameter5 = {
  "appID",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::EnumDescription Common__HMIApplication__parameter6 = {
  {
    "hmiDisplayLanguageDesired",
    ford_message_descriptions::Enum,
    false,
    true
  },
  Enums::Common__Language__entries
};
const ford_message_descriptions::ParameterDescription Common__HMIApplication__parameter7 = {
  "isMediaApplication",
  ford_message_descriptions::Boolean,
  false,
  true
};
const ford_message_descriptions::EnumDescription Common__HMIApplication__parameter8 = {
  {
    "appType",
    ford_message_descriptions::Enum,
    true,
    false
  },
  Enums::Common__AppHMIType__entries
};
const ford_message_descriptions::ParameterDescription* Structs::Common__HMIApplication__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&Common__HMIApplication__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&Common__HMIApplication__parameter2,
  (const ford_message_descriptions::ParameterDescription*)&Common__HMIApplication__parameter3,
  (const ford_message_descriptions::ParameterDescription*)&Common__HMIApplication__parameter4,
  (const ford_message_descriptions::ParameterDescription*)&Common__HMIApplication__parameter5,
  (const ford_message_descriptions::ParameterDescription*)&Common__HMIApplication__parameter6,
  (const ford_message_descriptions::ParameterDescription*)&Common__HMIApplication__parameter7,
  (const ford_message_descriptions::ParameterDescription*)&Common__HMIApplication__parameter8,
  NULL };

const ford_message_descriptions::ParameterDescription Common__Choice__parameter1 = {
  "choiceID",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription Common__Choice__parameter2 = {
  "menuName",
  ford_message_descriptions::String,
  false,
  false
};
const ford_message_descriptions::StructDescription Common__Choice__parameter3 = {
  {
    "image",
    ford_message_descriptions::Struct,
    false,
    false
  },
  Structs::Common__Image__parameters
};
const ford_message_descriptions::ParameterDescription* Structs::Common__Choice__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&Common__Choice__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&Common__Choice__parameter2,
  (const ford_message_descriptions::ParameterDescription*)&Common__Choice__parameter3,
  NULL };

const ford_message_descriptions::EnumDescription Common__TireStatus__parameter1 = {
  {
    "pressureTelltale",
    ford_message_descriptions::Enum,
    false,
    true
  },
  Enums::Common__WarningLightStatus__entries
};
const ford_message_descriptions::StructDescription Common__TireStatus__parameter2 = {
  {
    "leftFront",
    ford_message_descriptions::Struct,
    false,
    true
  },
  Structs::Common__SingleTireStatus__parameters
};
const ford_message_descriptions::StructDescription Common__TireStatus__parameter3 = {
  {
    "rightFront",
    ford_message_descriptions::Struct,
    false,
    true
  },
  Structs::Common__SingleTireStatus__parameters
};
const ford_message_descriptions::StructDescription Common__TireStatus__parameter4 = {
  {
    "leftRear",
    ford_message_descriptions::Struct,
    false,
    true
  },
  Structs::Common__SingleTireStatus__parameters
};
const ford_message_descriptions::StructDescription Common__TireStatus__parameter5 = {
  {
    "rightRear",
    ford_message_descriptions::Struct,
    false,
    true
  },
  Structs::Common__SingleTireStatus__parameters
};
const ford_message_descriptions::StructDescription Common__TireStatus__parameter6 = {
  {
    "innerLeftRear",
    ford_message_descriptions::Struct,
    false,
    true
  },
  Structs::Common__SingleTireStatus__parameters
};
const ford_message_descriptions::StructDescription Common__TireStatus__parameter7 = {
  {
    "innerRightRear",
    ford_message_descriptions::Struct,
    false,
    true
  },
  Structs::Common__SingleTireStatus__parameters
};
const ford_message_descriptions::ParameterDescription* Structs::Common__TireStatus__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&Common__TireStatus__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&Common__TireStatus__parameter2,
  (const ford_message_descriptions::ParameterDescription*)&Common__TireStatus__parameter3,
  (const ford_message_descriptions::ParameterDescription*)&Common__TireStatus__parameter4,
  (const ford_message_descriptions::ParameterDescription*)&Common__TireStatus__parameter5,
  (const ford_message_descriptions::ParameterDescription*)&Common__TireStatus__parameter6,
  (const ford_message_descriptions::ParameterDescription*)&Common__TireStatus__parameter7,
  NULL };

const ford_message_descriptions::EnumDescription Common__ButtonCapabilities__parameter1 = {
  {
    "name",
    ford_message_descriptions::Enum,
    false,
    true
  },
  Enums::Common__ButtonName__entries
};
const ford_message_descriptions::ParameterDescription Common__ButtonCapabilities__parameter2 = {
  "shortPressAvailable",
  ford_message_descriptions::Boolean,
  false,
  true
};
const ford_message_descriptions::ParameterDescription Common__ButtonCapabilities__parameter3 = {
  "longPressAvailable",
  ford_message_descriptions::Boolean,
  false,
  true
};
const ford_message_descriptions::ParameterDescription Common__ButtonCapabilities__parameter4 = {
  "upDownAvailable",
  ford_message_descriptions::Boolean,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* Structs::Common__ButtonCapabilities__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&Common__ButtonCapabilities__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&Common__ButtonCapabilities__parameter2,
  (const ford_message_descriptions::ParameterDescription*)&Common__ButtonCapabilities__parameter3,
  (const ford_message_descriptions::ParameterDescription*)&Common__ButtonCapabilities__parameter4,
  NULL };

const ford_message_descriptions::EnumDescription Common__BeltStatus__parameter1 = {
  {
    "driverBeltDeployed",
    ford_message_descriptions::Enum,
    false,
    false
  },
  Enums::Common__VehicleDataEventStatus__entries
};
const ford_message_descriptions::EnumDescription Common__BeltStatus__parameter2 = {
  {
    "passengerBeltDeployed",
    ford_message_descriptions::Enum,
    false,
    false
  },
  Enums::Common__VehicleDataEventStatus__entries
};
const ford_message_descriptions::EnumDescription Common__BeltStatus__parameter3 = {
  {
    "passengerBuckleBelted",
    ford_message_descriptions::Enum,
    false,
    false
  },
  Enums::Common__VehicleDataEventStatus__entries
};
const ford_message_descriptions::EnumDescription Common__BeltStatus__parameter4 = {
  {
    "driverBuckleBelted",
    ford_message_descriptions::Enum,
    false,
    false
  },
  Enums::Common__VehicleDataEventStatus__entries
};
const ford_message_descriptions::EnumDescription Common__BeltStatus__parameter5 = {
  {
    "leftRow2BuckleBelted",
    ford_message_descriptions::Enum,
    false,
    false
  },
  Enums::Common__VehicleDataEventStatus__entries
};
const ford_message_descriptions::EnumDescription Common__BeltStatus__parameter6 = {
  {
    "passengerChildDetected",
    ford_message_descriptions::Enum,
    false,
    false
  },
  Enums::Common__VehicleDataEventStatus__entries
};
const ford_message_descriptions::EnumDescription Common__BeltStatus__parameter7 = {
  {
    "rightRow2BuckleBelted",
    ford_message_descriptions::Enum,
    false,
    false
  },
  Enums::Common__VehicleDataEventStatus__entries
};
const ford_message_descriptions::EnumDescription Common__BeltStatus__parameter8 = {
  {
    "middleRow2BuckleBelted",
    ford_message_descriptions::Enum,
    false,
    false
  },
  Enums::Common__VehicleDataEventStatus__entries
};
const ford_message_descriptions::EnumDescription Common__BeltStatus__parameter9 = {
  {
    "middleRow3BuckleBelted",
    ford_message_descriptions::Enum,
    false,
    false
  },
  Enums::Common__VehicleDataEventStatus__entries
};
const ford_message_descriptions::EnumDescription Common__BeltStatus__parameter10 = {
  {
    "leftRow3BuckleBelted",
    ford_message_descriptions::Enum,
    false,
    false
  },
  Enums::Common__VehicleDataEventStatus__entries
};
const ford_message_descriptions::EnumDescription Common__BeltStatus__parameter11 = {
  {
    "rightRow3BuckleBelted",
    ford_message_descriptions::Enum,
    false,
    false
  },
  Enums::Common__VehicleDataEventStatus__entries
};
const ford_message_descriptions::EnumDescription Common__BeltStatus__parameter12 = {
  {
    "leftRearInflatableBelted",
    ford_message_descriptions::Enum,
    false,
    false
  },
  Enums::Common__VehicleDataEventStatus__entries
};
const ford_message_descriptions::EnumDescription Common__BeltStatus__parameter13 = {
  {
    "rightRearInflatableBelted",
    ford_message_descriptions::Enum,
    false,
    false
  },
  Enums::Common__VehicleDataEventStatus__entries
};
const ford_message_descriptions::EnumDescription Common__BeltStatus__parameter14 = {
  {
    "middleRow1BeltDeployed",
    ford_message_descriptions::Enum,
    false,
    false
  },
  Enums::Common__VehicleDataEventStatus__entries
};
const ford_message_descriptions::EnumDescription Common__BeltStatus__parameter15 = {
  {
    "middleRow1BuckleBelted",
    ford_message_descriptions::Enum,
    false,
    false
  },
  Enums::Common__VehicleDataEventStatus__entries
};
const ford_message_descriptions::ParameterDescription* Structs::Common__BeltStatus__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&Common__BeltStatus__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&Common__BeltStatus__parameter2,
  (const ford_message_descriptions::ParameterDescription*)&Common__BeltStatus__parameter3,
  (const ford_message_descriptions::ParameterDescription*)&Common__BeltStatus__parameter4,
  (const ford_message_descriptions::ParameterDescription*)&Common__BeltStatus__parameter5,
  (const ford_message_descriptions::ParameterDescription*)&Common__BeltStatus__parameter6,
  (const ford_message_descriptions::ParameterDescription*)&Common__BeltStatus__parameter7,
  (const ford_message_descriptions::ParameterDescription*)&Common__BeltStatus__parameter8,
  (const ford_message_descriptions::ParameterDescription*)&Common__BeltStatus__parameter9,
  (const ford_message_descriptions::ParameterDescription*)&Common__BeltStatus__parameter10,
  (const ford_message_descriptions::ParameterDescription*)&Common__BeltStatus__parameter11,
  (const ford_message_descriptions::ParameterDescription*)&Common__BeltStatus__parameter12,
  (const ford_message_descriptions::ParameterDescription*)&Common__BeltStatus__parameter13,
  (const ford_message_descriptions::ParameterDescription*)&Common__BeltStatus__parameter14,
  (const ford_message_descriptions::ParameterDescription*)&Common__BeltStatus__parameter15,
  NULL };

const ford_message_descriptions::ParameterDescription Common__DeviceInfo__parameter1 = {
  "name",
  ford_message_descriptions::String,
  false,
  true
};
const ford_message_descriptions::ParameterDescription Common__DeviceInfo__parameter2 = {
  "id",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* Structs::Common__DeviceInfo__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&Common__DeviceInfo__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&Common__DeviceInfo__parameter2,
  NULL };

const ford_message_descriptions::ParameterDescription Common__MenuParams__parameter1 = {
  "parentID",
  ford_message_descriptions::Integer,
  false,
  false
};
const ford_message_descriptions::ParameterDescription Common__MenuParams__parameter2 = {
  "position",
  ford_message_descriptions::Integer,
  false,
  false
};
const ford_message_descriptions::ParameterDescription Common__MenuParams__parameter3 = {
  "menuName",
  ford_message_descriptions::String,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* Structs::Common__MenuParams__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&Common__MenuParams__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&Common__MenuParams__parameter2,
  (const ford_message_descriptions::ParameterDescription*)&Common__MenuParams__parameter3,
  NULL };

const ford_message_descriptions::ParameterDescription Common__SoftButtonCapabilities__parameter1 = {
  "shortPressAvailable",
  ford_message_descriptions::Boolean,
  false,
  true
};
const ford_message_descriptions::ParameterDescription Common__SoftButtonCapabilities__parameter2 = {
  "longPressAvailable",
  ford_message_descriptions::Boolean,
  false,
  true
};
const ford_message_descriptions::ParameterDescription Common__SoftButtonCapabilities__parameter3 = {
  "upDownAvailable",
  ford_message_descriptions::Boolean,
  false,
  true
};
const ford_message_descriptions::ParameterDescription Common__SoftButtonCapabilities__parameter4 = {
  "imageSupported",
  ford_message_descriptions::Boolean,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* Structs::Common__SoftButtonCapabilities__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&Common__SoftButtonCapabilities__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&Common__SoftButtonCapabilities__parameter2,
  (const ford_message_descriptions::ParameterDescription*)&Common__SoftButtonCapabilities__parameter3,
  (const ford_message_descriptions::ParameterDescription*)&Common__SoftButtonCapabilities__parameter4,
  NULL };

const ford_message_descriptions::EnumDescription Common__DisplayCapabilities__parameter1 = {
  {
    "displayType",
    ford_message_descriptions::Enum,
    false,
    true
  },
  Enums::Common__DisplayType__entries
};
const ford_message_descriptions::EnumDescription Common__DisplayCapabilities__parameter2 = {
  {
    "textFields",
    ford_message_descriptions::Enum,
    true,
    true
  },
  Enums::Common__TextFieldName__entries
};
const ford_message_descriptions::EnumDescription Common__DisplayCapabilities__parameter3 = {
  {
    "mediaClockFormats",
    ford_message_descriptions::Enum,
    true,
    true
  },
  Enums::Common__MediaClockFormat__entries
};
const ford_message_descriptions::EnumDescription Common__DisplayCapabilities__parameter4 = {
  {
    "imageCapabilities",
    ford_message_descriptions::Enum,
    true,
    true
  },
  Enums::Common__ImageType__entries
};
const ford_message_descriptions::ParameterDescription* Structs::Common__DisplayCapabilities__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&Common__DisplayCapabilities__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&Common__DisplayCapabilities__parameter2,
  (const ford_message_descriptions::ParameterDescription*)&Common__DisplayCapabilities__parameter3,
  (const ford_message_descriptions::ParameterDescription*)&Common__DisplayCapabilities__parameter4,
  NULL };

const ford_message_descriptions::ParameterDescription Common__VehicleType__parameter1 = {
  "make",
  ford_message_descriptions::String,
  false,
  false
};
const ford_message_descriptions::ParameterDescription Common__VehicleType__parameter2 = {
  "model",
  ford_message_descriptions::String,
  false,
  false
};
const ford_message_descriptions::ParameterDescription Common__VehicleType__parameter3 = {
  "modelYear",
  ford_message_descriptions::String,
  false,
  false
};
const ford_message_descriptions::ParameterDescription Common__VehicleType__parameter4 = {
  "trim",
  ford_message_descriptions::String,
  false,
  false
};
const ford_message_descriptions::ParameterDescription* Structs::Common__VehicleType__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&Common__VehicleType__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&Common__VehicleType__parameter2,
  (const ford_message_descriptions::ParameterDescription*)&Common__VehicleType__parameter3,
  (const ford_message_descriptions::ParameterDescription*)&Common__VehicleType__parameter4,
  NULL };

const ford_message_descriptions::EnumDescription Common__MyKey__parameter1 = {
  {
    "e911Override",
    ford_message_descriptions::Enum,
    false,
    true
  },
  Enums::Common__VehicleDataStatus__entries
};
const ford_message_descriptions::ParameterDescription* Structs::Common__MyKey__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&Common__MyKey__parameter1,
  NULL };

const ford_message_descriptions::ParameterDescription Common__DeviceStatus__parameter1 = {
  "voiceRecOn",
  ford_message_descriptions::Boolean,
  false,
  false
};
const ford_message_descriptions::ParameterDescription Common__DeviceStatus__parameter2 = {
  "btIconOn",
  ford_message_descriptions::Boolean,
  false,
  false
};
const ford_message_descriptions::ParameterDescription Common__DeviceStatus__parameter3 = {
  "callActive",
  ford_message_descriptions::Boolean,
  false,
  false
};
const ford_message_descriptions::ParameterDescription Common__DeviceStatus__parameter4 = {
  "phoneRoaming",
  ford_message_descriptions::Boolean,
  false,
  false
};
const ford_message_descriptions::ParameterDescription Common__DeviceStatus__parameter5 = {
  "textMsgAvailable",
  ford_message_descriptions::Boolean,
  false,
  false
};
const ford_message_descriptions::EnumDescription Common__DeviceStatus__parameter6 = {
  {
    "battLevelStatus",
    ford_message_descriptions::Enum,
    false,
    false
  },
  Enums::Common__DeviceLevelStatus__entries
};
const ford_message_descriptions::ParameterDescription Common__DeviceStatus__parameter7 = {
  "stereoAudioOutputMuted",
  ford_message_descriptions::Boolean,
  false,
  false
};
const ford_message_descriptions::ParameterDescription Common__DeviceStatus__parameter8 = {
  "monoAudioOutputMuted",
  ford_message_descriptions::Boolean,
  false,
  false
};
const ford_message_descriptions::EnumDescription Common__DeviceStatus__parameter9 = {
  {
    "signalLevelStatus",
    ford_message_descriptions::Enum,
    false,
    false
  },
  Enums::Common__DeviceLevelStatus__entries
};
const ford_message_descriptions::EnumDescription Common__DeviceStatus__parameter10 = {
  {
    "primaryAudioSource",
    ford_message_descriptions::Enum,
    false,
    false
  },
  Enums::Common__PrimaryAudioSource__entries
};
const ford_message_descriptions::ParameterDescription Common__DeviceStatus__parameter11 = {
  "eCallEventActive",
  ford_message_descriptions::Boolean,
  false,
  false
};
const ford_message_descriptions::ParameterDescription* Structs::Common__DeviceStatus__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&Common__DeviceStatus__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&Common__DeviceStatus__parameter2,
  (const ford_message_descriptions::ParameterDescription*)&Common__DeviceStatus__parameter3,
  (const ford_message_descriptions::ParameterDescription*)&Common__DeviceStatus__parameter4,
  (const ford_message_descriptions::ParameterDescription*)&Common__DeviceStatus__parameter5,
  (const ford_message_descriptions::ParameterDescription*)&Common__DeviceStatus__parameter6,
  (const ford_message_descriptions::ParameterDescription*)&Common__DeviceStatus__parameter7,
  (const ford_message_descriptions::ParameterDescription*)&Common__DeviceStatus__parameter8,
  (const ford_message_descriptions::ParameterDescription*)&Common__DeviceStatus__parameter9,
  (const ford_message_descriptions::ParameterDescription*)&Common__DeviceStatus__parameter10,
  (const ford_message_descriptions::ParameterDescription*)&Common__DeviceStatus__parameter11,
  NULL };

const ford_message_descriptions::EnumDescription Common__TextFieldStruct__parameter1 = {
  {
    "fieldName",
    ford_message_descriptions::Enum,
    false,
    true
  },
  Enums::Common__TextFieldName__entries
};
const ford_message_descriptions::ParameterDescription Common__TextFieldStruct__parameter2 = {
  "fieldText",
  ford_message_descriptions::String,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* Structs::Common__TextFieldStruct__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&Common__TextFieldStruct__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&Common__TextFieldStruct__parameter2,
  NULL };

const ford_message_descriptions::StructDescription Common__Turn__parameter1 = {
  {
    "navigationText",
    ford_message_descriptions::Struct,
    false,
    false
  },
  Structs::Common__TextFieldStruct__parameters
};
const ford_message_descriptions::StructDescription Common__Turn__parameter2 = {
  {
    "turnIcon",
    ford_message_descriptions::Struct,
    false,
    false
  },
  Structs::Common__Image__parameters
};
const ford_message_descriptions::ParameterDescription* Structs::Common__Turn__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&Common__Turn__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&Common__Turn__parameter2,
  NULL };

ford_message_descriptions::EnumDescription::Entry Common__ButtonEventMode__entry1 = {"BUTTONUP", 1};
ford_message_descriptions::EnumDescription::Entry Common__ButtonEventMode__entry2 = {"BUTTONDOWN", 2};
const ford_message_descriptions::EnumDescription::Entry* Enums::Common__ButtonEventMode__entries[] = {
  &Common__ButtonEventMode__entry1,
  &Common__ButtonEventMode__entry2,
  NULL };

ford_message_descriptions::EnumDescription::Entry Common__IgnitionStatus__entry1 = {"UNKNOWN", 1};
ford_message_descriptions::EnumDescription::Entry Common__IgnitionStatus__entry2 = {"OFF", 2};
ford_message_descriptions::EnumDescription::Entry Common__IgnitionStatus__entry3 = {"ACCESSORY", 3};
ford_message_descriptions::EnumDescription::Entry Common__IgnitionStatus__entry4 = {"RUN", 4};
ford_message_descriptions::EnumDescription::Entry Common__IgnitionStatus__entry5 = {"START", 5};
ford_message_descriptions::EnumDescription::Entry Common__IgnitionStatus__entry6 = {"INVALID", 6};
const ford_message_descriptions::EnumDescription::Entry* Enums::Common__IgnitionStatus__entries[] = {
  &Common__IgnitionStatus__entry1,
  &Common__IgnitionStatus__entry2,
  &Common__IgnitionStatus__entry3,
  &Common__IgnitionStatus__entry4,
  &Common__IgnitionStatus__entry5,
  &Common__IgnitionStatus__entry6,
  NULL };

ford_message_descriptions::EnumDescription::Entry Common__ApplicationsCloseReason__entry1 = {"IGNITION_OFF", 1};
ford_message_descriptions::EnumDescription::Entry Common__ApplicationsCloseReason__entry2 = {"MASTER_RESET", 2};
ford_message_descriptions::EnumDescription::Entry Common__ApplicationsCloseReason__entry3 = {"FACTORY_DEFAULTS", 3};
const ford_message_descriptions::EnumDescription::Entry* Enums::Common__ApplicationsCloseReason__entries[] = {
  &Common__ApplicationsCloseReason__entry1,
  &Common__ApplicationsCloseReason__entry2,
  &Common__ApplicationsCloseReason__entry3,
  NULL };

ford_message_descriptions::EnumDescription::Entry Common__PRNDL__entry1 = {"PARK", 1};
ford_message_descriptions::EnumDescription::Entry Common__PRNDL__entry2 = {"REVERSE", 2};
ford_message_descriptions::EnumDescription::Entry Common__PRNDL__entry3 = {"NEUTRAL", 3};
ford_message_descriptions::EnumDescription::Entry Common__PRNDL__entry4 = {"DRIVE", 4};
ford_message_descriptions::EnumDescription::Entry Common__PRNDL__entry5 = {"SPORT", 5};
ford_message_descriptions::EnumDescription::Entry Common__PRNDL__entry6 = {"LOWGEAR", 6};
ford_message_descriptions::EnumDescription::Entry Common__PRNDL__entry7 = {"FIRST", 7};
ford_message_descriptions::EnumDescription::Entry Common__PRNDL__entry8 = {"SECOND", 8};
ford_message_descriptions::EnumDescription::Entry Common__PRNDL__entry9 = {"THIRD", 9};
ford_message_descriptions::EnumDescription::Entry Common__PRNDL__entry10 = {"FOURTH", 10};
ford_message_descriptions::EnumDescription::Entry Common__PRNDL__entry11 = {"FIFTH", 11};
ford_message_descriptions::EnumDescription::Entry Common__PRNDL__entry12 = {"SIXTH", 12};
const ford_message_descriptions::EnumDescription::Entry* Enums::Common__PRNDL__entries[] = {
  &Common__PRNDL__entry1,
  &Common__PRNDL__entry2,
  &Common__PRNDL__entry3,
  &Common__PRNDL__entry4,
  &Common__PRNDL__entry5,
  &Common__PRNDL__entry6,
  &Common__PRNDL__entry7,
  &Common__PRNDL__entry8,
  &Common__PRNDL__entry9,
  &Common__PRNDL__entry10,
  &Common__PRNDL__entry11,
  &Common__PRNDL__entry12,
  NULL };

ford_message_descriptions::EnumDescription::Entry Common__ButtonName__entry1 = {"OK", 1};
ford_message_descriptions::EnumDescription::Entry Common__ButtonName__entry2 = {"SEEKLEFT", 2};
ford_message_descriptions::EnumDescription::Entry Common__ButtonName__entry3 = {"SEEKRIGHT", 3};
ford_message_descriptions::EnumDescription::Entry Common__ButtonName__entry4 = {"TUNEUP", 4};
ford_message_descriptions::EnumDescription::Entry Common__ButtonName__entry5 = {"TUNEDOWN", 5};
ford_message_descriptions::EnumDescription::Entry Common__ButtonName__entry6 = {"PRESET_0", 6};
ford_message_descriptions::EnumDescription::Entry Common__ButtonName__entry7 = {"PRESET_1", 7};
ford_message_descriptions::EnumDescription::Entry Common__ButtonName__entry8 = {"PRESET_2", 8};
ford_message_descriptions::EnumDescription::Entry Common__ButtonName__entry9 = {"PRESET_3", 9};
ford_message_descriptions::EnumDescription::Entry Common__ButtonName__entry10 = {"PRESET_4", 10};
ford_message_descriptions::EnumDescription::Entry Common__ButtonName__entry11 = {"PRESET_5", 11};
ford_message_descriptions::EnumDescription::Entry Common__ButtonName__entry12 = {"PRESET_6", 12};
ford_message_descriptions::EnumDescription::Entry Common__ButtonName__entry13 = {"PRESET_7", 13};
ford_message_descriptions::EnumDescription::Entry Common__ButtonName__entry14 = {"PRESET_8", 14};
ford_message_descriptions::EnumDescription::Entry Common__ButtonName__entry15 = {"PRESET_9", 15};
ford_message_descriptions::EnumDescription::Entry Common__ButtonName__entry16 = {"CUSTOM_BUTTON", 16};
const ford_message_descriptions::EnumDescription::Entry* Enums::Common__ButtonName__entries[] = {
  &Common__ButtonName__entry1,
  &Common__ButtonName__entry2,
  &Common__ButtonName__entry3,
  &Common__ButtonName__entry4,
  &Common__ButtonName__entry5,
  &Common__ButtonName__entry6,
  &Common__ButtonName__entry7,
  &Common__ButtonName__entry8,
  &Common__ButtonName__entry9,
  &Common__ButtonName__entry10,
  &Common__ButtonName__entry11,
  &Common__ButtonName__entry12,
  &Common__ButtonName__entry13,
  &Common__ButtonName__entry14,
  &Common__ButtonName__entry15,
  &Common__ButtonName__entry16,
  NULL };

ford_message_descriptions::EnumDescription::Entry Common__TextFieldName__entry1 = {"mainField1", 1};
ford_message_descriptions::EnumDescription::Entry Common__TextFieldName__entry2 = {"mainField2", 2};
ford_message_descriptions::EnumDescription::Entry Common__TextFieldName__entry3 = {"mainField3", 3};
ford_message_descriptions::EnumDescription::Entry Common__TextFieldName__entry4 = {"mainField4", 4};
ford_message_descriptions::EnumDescription::Entry Common__TextFieldName__entry5 = {"statusBar", 5};
ford_message_descriptions::EnumDescription::Entry Common__TextFieldName__entry6 = {"mediaClock", 6};
ford_message_descriptions::EnumDescription::Entry Common__TextFieldName__entry7 = {"mediaTrack", 7};
ford_message_descriptions::EnumDescription::Entry Common__TextFieldName__entry8 = {"alertText1", 8};
ford_message_descriptions::EnumDescription::Entry Common__TextFieldName__entry9 = {"alertText2", 9};
ford_message_descriptions::EnumDescription::Entry Common__TextFieldName__entry10 = {"alertText3", 10};
ford_message_descriptions::EnumDescription::Entry Common__TextFieldName__entry11 = {"scrollableMessageBody", 11};
ford_message_descriptions::EnumDescription::Entry Common__TextFieldName__entry12 = {"initialInteractionText", 12};
ford_message_descriptions::EnumDescription::Entry Common__TextFieldName__entry13 = {"navigationText1", 13};
ford_message_descriptions::EnumDescription::Entry Common__TextFieldName__entry14 = {"navigationText2", 14};
ford_message_descriptions::EnumDescription::Entry Common__TextFieldName__entry15 = {"ETA", 15};
ford_message_descriptions::EnumDescription::Entry Common__TextFieldName__entry16 = {"totalDistance", 16};
ford_message_descriptions::EnumDescription::Entry Common__TextFieldName__entry17 = {"navigationText", 17};
ford_message_descriptions::EnumDescription::Entry Common__TextFieldName__entry18 = {"audioPassThruDisplayText1", 18};
ford_message_descriptions::EnumDescription::Entry Common__TextFieldName__entry19 = {"audioPassThruDisplayText2", 19};
ford_message_descriptions::EnumDescription::Entry Common__TextFieldName__entry20 = {"sliderHeader", 20};
ford_message_descriptions::EnumDescription::Entry Common__TextFieldName__entry21 = {"sliderFooter", 21};
ford_message_descriptions::EnumDescription::Entry Common__TextFieldName__entry22 = {"notificationText", 22};
const ford_message_descriptions::EnumDescription::Entry* Enums::Common__TextFieldName__entries[] = {
  &Common__TextFieldName__entry1,
  &Common__TextFieldName__entry2,
  &Common__TextFieldName__entry3,
  &Common__TextFieldName__entry4,
  &Common__TextFieldName__entry5,
  &Common__TextFieldName__entry6,
  &Common__TextFieldName__entry7,
  &Common__TextFieldName__entry8,
  &Common__TextFieldName__entry9,
  &Common__TextFieldName__entry10,
  &Common__TextFieldName__entry11,
  &Common__TextFieldName__entry12,
  &Common__TextFieldName__entry13,
  &Common__TextFieldName__entry14,
  &Common__TextFieldName__entry15,
  &Common__TextFieldName__entry16,
  &Common__TextFieldName__entry17,
  &Common__TextFieldName__entry18,
  &Common__TextFieldName__entry19,
  &Common__TextFieldName__entry20,
  &Common__TextFieldName__entry21,
  &Common__TextFieldName__entry22,
  NULL };

ford_message_descriptions::EnumDescription::Entry Common__Language__entry1 = {"EN-US", 1};
ford_message_descriptions::EnumDescription::Entry Common__Language__entry2 = {"ES-MX", 2};
ford_message_descriptions::EnumDescription::Entry Common__Language__entry3 = {"FR-CA", 3};
ford_message_descriptions::EnumDescription::Entry Common__Language__entry4 = {"DE-DE", 4};
ford_message_descriptions::EnumDescription::Entry Common__Language__entry5 = {"ES-ES", 5};
ford_message_descriptions::EnumDescription::Entry Common__Language__entry6 = {"EN-GB", 6};
ford_message_descriptions::EnumDescription::Entry Common__Language__entry7 = {"RU-RU", 7};
ford_message_descriptions::EnumDescription::Entry Common__Language__entry8 = {"TR-TR", 8};
ford_message_descriptions::EnumDescription::Entry Common__Language__entry9 = {"PL-PL", 9};
ford_message_descriptions::EnumDescription::Entry Common__Language__entry10 = {"FR-FR", 10};
ford_message_descriptions::EnumDescription::Entry Common__Language__entry11 = {"IT-IT", 11};
ford_message_descriptions::EnumDescription::Entry Common__Language__entry12 = {"SV-SE", 12};
ford_message_descriptions::EnumDescription::Entry Common__Language__entry13 = {"PT-PT", 13};
ford_message_descriptions::EnumDescription::Entry Common__Language__entry14 = {"NL-NL", 14};
ford_message_descriptions::EnumDescription::Entry Common__Language__entry15 = {"EN-AU", 15};
ford_message_descriptions::EnumDescription::Entry Common__Language__entry16 = {"ZH-CN", 16};
ford_message_descriptions::EnumDescription::Entry Common__Language__entry17 = {"ZH-TW", 17};
ford_message_descriptions::EnumDescription::Entry Common__Language__entry18 = {"JA-JP", 18};
ford_message_descriptions::EnumDescription::Entry Common__Language__entry19 = {"AR-SA", 19};
ford_message_descriptions::EnumDescription::Entry Common__Language__entry20 = {"KO-KR", 20};
ford_message_descriptions::EnumDescription::Entry Common__Language__entry21 = {"PT-BR", 21};
ford_message_descriptions::EnumDescription::Entry Common__Language__entry22 = {"CS-CZ", 22};
ford_message_descriptions::EnumDescription::Entry Common__Language__entry23 = {"DA-DK", 23};
ford_message_descriptions::EnumDescription::Entry Common__Language__entry24 = {"NO-NO", 24};
const ford_message_descriptions::EnumDescription::Entry* Enums::Common__Language__entries[] = {
  &Common__Language__entry1,
  &Common__Language__entry2,
  &Common__Language__entry3,
  &Common__Language__entry4,
  &Common__Language__entry5,
  &Common__Language__entry6,
  &Common__Language__entry7,
  &Common__Language__entry8,
  &Common__Language__entry9,
  &Common__Language__entry10,
  &Common__Language__entry11,
  &Common__Language__entry12,
  &Common__Language__entry13,
  &Common__Language__entry14,
  &Common__Language__entry15,
  &Common__Language__entry16,
  &Common__Language__entry17,
  &Common__Language__entry18,
  &Common__Language__entry19,
  &Common__Language__entry20,
  &Common__Language__entry21,
  &Common__Language__entry22,
  &Common__Language__entry23,
  &Common__Language__entry24,
  NULL };

ford_message_descriptions::EnumDescription::Entry Common__TBTState__entry1 = {"ROUTE_UPDATE_REQUEST", 1};
ford_message_descriptions::EnumDescription::Entry Common__TBTState__entry2 = {"ROUTE_ACCEPTED", 2};
ford_message_descriptions::EnumDescription::Entry Common__TBTState__entry3 = {"ROUTE_REFUSED", 3};
ford_message_descriptions::EnumDescription::Entry Common__TBTState__entry4 = {"ROUTE_CANCELLED", 4};
ford_message_descriptions::EnumDescription::Entry Common__TBTState__entry5 = {"ETA_REQUEST", 5};
ford_message_descriptions::EnumDescription::Entry Common__TBTState__entry6 = {"NEXT_TURN_REQUEST", 6};
ford_message_descriptions::EnumDescription::Entry Common__TBTState__entry7 = {"ROUTE_STATUS_REQUEST", 7};
ford_message_descriptions::EnumDescription::Entry Common__TBTState__entry8 = {"ROUTE_SUMMARY_REQUEST", 8};
ford_message_descriptions::EnumDescription::Entry Common__TBTState__entry9 = {"TRIP_STATUS_REQUEST", 9};
ford_message_descriptions::EnumDescription::Entry Common__TBTState__entry10 = {"ROUTE_UPDATE_REQUEST_TIMEOUT", 10};
const ford_message_descriptions::EnumDescription::Entry* Enums::Common__TBTState__entries[] = {
  &Common__TBTState__entry1,
  &Common__TBTState__entry2,
  &Common__TBTState__entry3,
  &Common__TBTState__entry4,
  &Common__TBTState__entry5,
  &Common__TBTState__entry6,
  &Common__TBTState__entry7,
  &Common__TBTState__entry8,
  &Common__TBTState__entry9,
  &Common__TBTState__entry10,
  NULL };

ford_message_descriptions::EnumDescription::Entry Common__Result__entry1 = {"SUCCESS", 0};
ford_message_descriptions::EnumDescription::Entry Common__Result__entry2 = {"UNSUPPORTED_REQUEST", 1};
ford_message_descriptions::EnumDescription::Entry Common__Result__entry3 = {"UNSUPPORTED_RESOURCE", 2};
ford_message_descriptions::EnumDescription::Entry Common__Result__entry4 = {"DISALLOWED", 3};
ford_message_descriptions::EnumDescription::Entry Common__Result__entry5 = {"REJECTED", 4};
ford_message_descriptions::EnumDescription::Entry Common__Result__entry6 = {"ABORTED", 5};
ford_message_descriptions::EnumDescription::Entry Common__Result__entry7 = {"IGNORED", 6};
ford_message_descriptions::EnumDescription::Entry Common__Result__entry8 = {"RETRY", 7};
ford_message_descriptions::EnumDescription::Entry Common__Result__entry9 = {"IN_USE", 8};
ford_message_descriptions::EnumDescription::Entry Common__Result__entry10 = {"DATA_NOT_AVAILABLE", 9};
ford_message_descriptions::EnumDescription::Entry Common__Result__entry11 = {"TIMED_OUT", 10};
ford_message_descriptions::EnumDescription::Entry Common__Result__entry12 = {"INVALID_DATA", 11};
ford_message_descriptions::EnumDescription::Entry Common__Result__entry13 = {"CHAR_LIMIT_EXCEEDED", 12};
ford_message_descriptions::EnumDescription::Entry Common__Result__entry14 = {"INVALID_ID", 13};
ford_message_descriptions::EnumDescription::Entry Common__Result__entry15 = {"DUPLICATE_NAME", 14};
ford_message_descriptions::EnumDescription::Entry Common__Result__entry16 = {"APPLICATION_NOT_REGISTERED", 15};
ford_message_descriptions::EnumDescription::Entry Common__Result__entry17 = {"WRONG_LANGUAGE", 16};
ford_message_descriptions::EnumDescription::Entry Common__Result__entry18 = {"OUT_OF_MEMORY", 17};
ford_message_descriptions::EnumDescription::Entry Common__Result__entry19 = {"TOO_MANY_PENDING_REQUESTS", 18};
ford_message_descriptions::EnumDescription::Entry Common__Result__entry20 = {"NO_APPS_REGISTERED", 19};
ford_message_descriptions::EnumDescription::Entry Common__Result__entry21 = {"NO_DEVICES_CONNECTED", 20};
ford_message_descriptions::EnumDescription::Entry Common__Result__entry22 = {"WARNINGS", 21};
ford_message_descriptions::EnumDescription::Entry Common__Result__entry23 = {"GENERIC_ERROR", 22};
ford_message_descriptions::EnumDescription::Entry Common__Result__entry24 = {"USER_DISALLOWED", 23};
const ford_message_descriptions::EnumDescription::Entry* Enums::Common__Result__entries[] = {
  &Common__Result__entry1,
  &Common__Result__entry2,
  &Common__Result__entry3,
  &Common__Result__entry4,
  &Common__Result__entry5,
  &Common__Result__entry6,
  &Common__Result__entry7,
  &Common__Result__entry8,
  &Common__Result__entry9,
  &Common__Result__entry10,
  &Common__Result__entry11,
  &Common__Result__entry12,
  &Common__Result__entry13,
  &Common__Result__entry14,
  &Common__Result__entry15,
  &Common__Result__entry16,
  &Common__Result__entry17,
  &Common__Result__entry18,
  &Common__Result__entry19,
  &Common__Result__entry20,
  &Common__Result__entry21,
  &Common__Result__entry22,
  &Common__Result__entry23,
  &Common__Result__entry24,
  NULL };

ford_message_descriptions::EnumDescription::Entry Common__VehicleDataResultCode__entry1 = {"SUCCESS", 1};
ford_message_descriptions::EnumDescription::Entry Common__VehicleDataResultCode__entry2 = {"TRUNCATED_DATA", 2};
ford_message_descriptions::EnumDescription::Entry Common__VehicleDataResultCode__entry3 = {"DISALLOWED", 3};
ford_message_descriptions::EnumDescription::Entry Common__VehicleDataResultCode__entry4 = {"USER_DISALLOWED", 4};
ford_message_descriptions::EnumDescription::Entry Common__VehicleDataResultCode__entry5 = {"INVALID_ID", 5};
ford_message_descriptions::EnumDescription::Entry Common__VehicleDataResultCode__entry6 = {"VEHICLE_DATA_NOT_AVAILABLE", 6};
ford_message_descriptions::EnumDescription::Entry Common__VehicleDataResultCode__entry7 = {"DATA_ALREADY_SUBSCRIBED", 7};
ford_message_descriptions::EnumDescription::Entry Common__VehicleDataResultCode__entry8 = {"DATA_NOT_SUBSCRIBED", 8};
ford_message_descriptions::EnumDescription::Entry Common__VehicleDataResultCode__entry9 = {"IGNORED", 9};
const ford_message_descriptions::EnumDescription::Entry* Enums::Common__VehicleDataResultCode__entries[] = {
  &Common__VehicleDataResultCode__entry1,
  &Common__VehicleDataResultCode__entry2,
  &Common__VehicleDataResultCode__entry3,
  &Common__VehicleDataResultCode__entry4,
  &Common__VehicleDataResultCode__entry5,
  &Common__VehicleDataResultCode__entry6,
  &Common__VehicleDataResultCode__entry7,
  &Common__VehicleDataResultCode__entry8,
  &Common__VehicleDataResultCode__entry9,
  NULL };

ford_message_descriptions::EnumDescription::Entry Common__ClockUpdateMode__entry1 = {"COUNTUP", 1};
ford_message_descriptions::EnumDescription::Entry Common__ClockUpdateMode__entry2 = {"COUNTDOWN", 2};
ford_message_descriptions::EnumDescription::Entry Common__ClockUpdateMode__entry3 = {"PAUSE", 3};
ford_message_descriptions::EnumDescription::Entry Common__ClockUpdateMode__entry4 = {"RESUME", 4};
ford_message_descriptions::EnumDescription::Entry Common__ClockUpdateMode__entry5 = {"CLEAR", 5};
const ford_message_descriptions::EnumDescription::Entry* Enums::Common__ClockUpdateMode__entries[] = {
  &Common__ClockUpdateMode__entry1,
  &Common__ClockUpdateMode__entry2,
  &Common__ClockUpdateMode__entry3,
  &Common__ClockUpdateMode__entry4,
  &Common__ClockUpdateMode__entry5,
  NULL };

ford_message_descriptions::EnumDescription::Entry Common__IgnitionStableStatus__entry1 = {"IGNITION_SWITCH_NOT_STABLE", 1};
ford_message_descriptions::EnumDescription::Entry Common__IgnitionStableStatus__entry2 = {"IGNITION_SWITCH_STABLE", 2};
ford_message_descriptions::EnumDescription::Entry Common__IgnitionStableStatus__entry3 = {"MISSING_FROM_TRANSMITTER", 3};
const ford_message_descriptions::EnumDescription::Entry* Enums::Common__IgnitionStableStatus__entries[] = {
  &Common__IgnitionStableStatus__entry1,
  &Common__IgnitionStableStatus__entry2,
  &Common__IgnitionStableStatus__entry3,
  NULL };

ford_message_descriptions::EnumDescription::Entry Common__TextAlignment__entry1 = {"LEFT_ALIGNED", 1};
ford_message_descriptions::EnumDescription::Entry Common__TextAlignment__entry2 = {"RIGHT_ALIGNED", 2};
ford_message_descriptions::EnumDescription::Entry Common__TextAlignment__entry3 = {"CENTERED", 3};
const ford_message_descriptions::EnumDescription::Entry* Enums::Common__TextAlignment__entries[] = {
  &Common__TextAlignment__entry1,
  &Common__TextAlignment__entry2,
  &Common__TextAlignment__entry3,
  NULL };

ford_message_descriptions::EnumDescription::Entry Common__ButtonPressMode__entry1 = {"LONG", 1};
ford_message_descriptions::EnumDescription::Entry Common__ButtonPressMode__entry2 = {"SHORT", 2};
const ford_message_descriptions::EnumDescription::Entry* Enums::Common__ButtonPressMode__entries[] = {
  &Common__ButtonPressMode__entry1,
  &Common__ButtonPressMode__entry2,
  NULL };

ford_message_descriptions::EnumDescription::Entry Common__SystemContext__entry1 = {"MAIN", 1};
ford_message_descriptions::EnumDescription::Entry Common__SystemContext__entry2 = {"VRSESSION", 2};
ford_message_descriptions::EnumDescription::Entry Common__SystemContext__entry3 = {"MENU", 3};
ford_message_descriptions::EnumDescription::Entry Common__SystemContext__entry4 = {"HMI_OBSCURED", 4};
ford_message_descriptions::EnumDescription::Entry Common__SystemContext__entry5 = {"ALERT", 5};
const ford_message_descriptions::EnumDescription::Entry* Enums::Common__SystemContext__entries[] = {
  &Common__SystemContext__entry1,
  &Common__SystemContext__entry2,
  &Common__SystemContext__entry3,
  &Common__SystemContext__entry4,
  &Common__SystemContext__entry5,
  NULL };

ford_message_descriptions::EnumDescription::Entry Common__MediaClockFormat__entry1 = {"CLOCK1", 1};
ford_message_descriptions::EnumDescription::Entry Common__MediaClockFormat__entry2 = {"CLOCK2", 2};
ford_message_descriptions::EnumDescription::Entry Common__MediaClockFormat__entry3 = {"CLOCK3", 3};
ford_message_descriptions::EnumDescription::Entry Common__MediaClockFormat__entry4 = {"CLOCKTEXT1", 4};
ford_message_descriptions::EnumDescription::Entry Common__MediaClockFormat__entry5 = {"CLOCKTEXT2", 5};
ford_message_descriptions::EnumDescription::Entry Common__MediaClockFormat__entry6 = {"CLOCKTEXT3", 6};
ford_message_descriptions::EnumDescription::Entry Common__MediaClockFormat__entry7 = {"CLOCKTEXT4", 7};
const ford_message_descriptions::EnumDescription::Entry* Enums::Common__MediaClockFormat__entries[] = {
  &Common__MediaClockFormat__entry1,
  &Common__MediaClockFormat__entry2,
  &Common__MediaClockFormat__entry3,
  &Common__MediaClockFormat__entry4,
  &Common__MediaClockFormat__entry5,
  &Common__MediaClockFormat__entry6,
  &Common__MediaClockFormat__entry7,
  NULL };

ford_message_descriptions::EnumDescription::Entry Common__AppHMIType__entry1 = {"DEFAULT", 1};
ford_message_descriptions::EnumDescription::Entry Common__AppHMIType__entry2 = {"COMMUNICATION", 2};
ford_message_descriptions::EnumDescription::Entry Common__AppHMIType__entry3 = {"MEDIA", 3};
ford_message_descriptions::EnumDescription::Entry Common__AppHMIType__entry4 = {"MESSAGING", 4};
ford_message_descriptions::EnumDescription::Entry Common__AppHMIType__entry5 = {"NAVIGATION", 5};
ford_message_descriptions::EnumDescription::Entry Common__AppHMIType__entry6 = {"INFORMATION", 6};
ford_message_descriptions::EnumDescription::Entry Common__AppHMIType__entry7 = {"SOCIAL", 7};
ford_message_descriptions::EnumDescription::Entry Common__AppHMIType__entry8 = {"BACKGROUND_PROCESS", 8};
ford_message_descriptions::EnumDescription::Entry Common__AppHMIType__entry9 = {"TESTING", 9};
ford_message_descriptions::EnumDescription::Entry Common__AppHMIType__entry10 = {"SYSTEM", 10};
const ford_message_descriptions::EnumDescription::Entry* Enums::Common__AppHMIType__entries[] = {
  &Common__AppHMIType__entry1,
  &Common__AppHMIType__entry2,
  &Common__AppHMIType__entry3,
  &Common__AppHMIType__entry4,
  &Common__AppHMIType__entry5,
  &Common__AppHMIType__entry6,
  &Common__AppHMIType__entry7,
  &Common__AppHMIType__entry8,
  &Common__AppHMIType__entry9,
  &Common__AppHMIType__entry10,
  NULL };

ford_message_descriptions::EnumDescription::Entry Common__PrimaryAudioSource__entry1 = {"NO_SOURCE_SELECTED", 1};
ford_message_descriptions::EnumDescription::Entry Common__PrimaryAudioSource__entry2 = {"USB", 2};
ford_message_descriptions::EnumDescription::Entry Common__PrimaryAudioSource__entry3 = {"USB2", 3};
ford_message_descriptions::EnumDescription::Entry Common__PrimaryAudioSource__entry4 = {"BLUETOOTH_STEREO_BTST", 4};
ford_message_descriptions::EnumDescription::Entry Common__PrimaryAudioSource__entry5 = {"LINE_IN", 5};
ford_message_descriptions::EnumDescription::Entry Common__PrimaryAudioSource__entry6 = {"IPOD", 6};
ford_message_descriptions::EnumDescription::Entry Common__PrimaryAudioSource__entry7 = {"MOBILE_APP", 7};
const ford_message_descriptions::EnumDescription::Entry* Enums::Common__PrimaryAudioSource__entries[] = {
  &Common__PrimaryAudioSource__entry1,
  &Common__PrimaryAudioSource__entry2,
  &Common__PrimaryAudioSource__entry3,
  &Common__PrimaryAudioSource__entry4,
  &Common__PrimaryAudioSource__entry5,
  &Common__PrimaryAudioSource__entry6,
  &Common__PrimaryAudioSource__entry7,
  NULL };

ford_message_descriptions::EnumDescription::Entry Common__CompassDirection__entry1 = {"NORTH", 1};
ford_message_descriptions::EnumDescription::Entry Common__CompassDirection__entry2 = {"NORTHWEST", 2};
ford_message_descriptions::EnumDescription::Entry Common__CompassDirection__entry3 = {"WEST", 3};
ford_message_descriptions::EnumDescription::Entry Common__CompassDirection__entry4 = {"SOUTHWEST", 4};
ford_message_descriptions::EnumDescription::Entry Common__CompassDirection__entry5 = {"SOUTH", 5};
ford_message_descriptions::EnumDescription::Entry Common__CompassDirection__entry6 = {"SOUTHEAST", 6};
ford_message_descriptions::EnumDescription::Entry Common__CompassDirection__entry7 = {"EAST", 7};
ford_message_descriptions::EnumDescription::Entry Common__CompassDirection__entry8 = {"NORTHEAST", 8};
const ford_message_descriptions::EnumDescription::Entry* Enums::Common__CompassDirection__entries[] = {
  &Common__CompassDirection__entry1,
  &Common__CompassDirection__entry2,
  &Common__CompassDirection__entry3,
  &Common__CompassDirection__entry4,
  &Common__CompassDirection__entry5,
  &Common__CompassDirection__entry6,
  &Common__CompassDirection__entry7,
  &Common__CompassDirection__entry8,
  NULL };

ford_message_descriptions::EnumDescription::Entry Common__SystemAction__entry1 = {"DEFAULT_ACTION", 1};
ford_message_descriptions::EnumDescription::Entry Common__SystemAction__entry2 = {"STEAL_FOCUS", 2};
ford_message_descriptions::EnumDescription::Entry Common__SystemAction__entry3 = {"KEEP_CONTEXT", 3};
const ford_message_descriptions::EnumDescription::Entry* Enums::Common__SystemAction__entries[] = {
  &Common__SystemAction__entry1,
  &Common__SystemAction__entry2,
  &Common__SystemAction__entry3,
  NULL };

ford_message_descriptions::EnumDescription::Entry Common__DriverDistractionState__entry1 = {"DD_ON", 1};
ford_message_descriptions::EnumDescription::Entry Common__DriverDistractionState__entry2 = {"DD_OFF", 2};
const ford_message_descriptions::EnumDescription::Entry* Enums::Common__DriverDistractionState__entries[] = {
  &Common__DriverDistractionState__entry1,
  &Common__DriverDistractionState__entry2,
  NULL };

ford_message_descriptions::EnumDescription::Entry Common__ImageType__entry1 = {"STATIC", 1};
ford_message_descriptions::EnumDescription::Entry Common__ImageType__entry2 = {"DYNAMIC", 2};
const ford_message_descriptions::EnumDescription::Entry* Enums::Common__ImageType__entries[] = {
  &Common__ImageType__entry1,
  &Common__ImageType__entry2,
  NULL };

ford_message_descriptions::EnumDescription::Entry Common__WiperStatus__entry1 = {"OFF", 1};
ford_message_descriptions::EnumDescription::Entry Common__WiperStatus__entry2 = {"AUTO_OFF", 2};
ford_message_descriptions::EnumDescription::Entry Common__WiperStatus__entry3 = {"OFF_MOVING", 3};
ford_message_descriptions::EnumDescription::Entry Common__WiperStatus__entry4 = {"MAN_INT_OFF", 4};
ford_message_descriptions::EnumDescription::Entry Common__WiperStatus__entry5 = {"MAN_INT_ON", 5};
ford_message_descriptions::EnumDescription::Entry Common__WiperStatus__entry6 = {"MAN_LOW", 6};
ford_message_descriptions::EnumDescription::Entry Common__WiperStatus__entry7 = {"MAN_HIGH", 7};
ford_message_descriptions::EnumDescription::Entry Common__WiperStatus__entry8 = {"MAN_FLICK", 8};
ford_message_descriptions::EnumDescription::Entry Common__WiperStatus__entry9 = {"WASH", 9};
ford_message_descriptions::EnumDescription::Entry Common__WiperStatus__entry10 = {"AUTO_LOW", 10};
ford_message_descriptions::EnumDescription::Entry Common__WiperStatus__entry11 = {"AUTO_HIGH", 11};
ford_message_descriptions::EnumDescription::Entry Common__WiperStatus__entry12 = {"COURTESYWIPE", 12};
ford_message_descriptions::EnumDescription::Entry Common__WiperStatus__entry13 = {"AUTO_ADJUST", 13};
ford_message_descriptions::EnumDescription::Entry Common__WiperStatus__entry14 = {"STALLED", 14};
ford_message_descriptions::EnumDescription::Entry Common__WiperStatus__entry15 = {"NO_DATA_EXISTS", 15};
const ford_message_descriptions::EnumDescription::Entry* Enums::Common__WiperStatus__entries[] = {
  &Common__WiperStatus__entry1,
  &Common__WiperStatus__entry2,
  &Common__WiperStatus__entry3,
  &Common__WiperStatus__entry4,
  &Common__WiperStatus__entry5,
  &Common__WiperStatus__entry6,
  &Common__WiperStatus__entry7,
  &Common__WiperStatus__entry8,
  &Common__WiperStatus__entry9,
  &Common__WiperStatus__entry10,
  &Common__WiperStatus__entry11,
  &Common__WiperStatus__entry12,
  &Common__WiperStatus__entry13,
  &Common__WiperStatus__entry14,
  &Common__WiperStatus__entry15,
  NULL };

ford_message_descriptions::EnumDescription::Entry Common__VehicleDataStatus__entry1 = {"NO_DATA_EXISTS", 1};
ford_message_descriptions::EnumDescription::Entry Common__VehicleDataStatus__entry2 = {"OFF", 2};
ford_message_descriptions::EnumDescription::Entry Common__VehicleDataStatus__entry3 = {"ON", 3};
const ford_message_descriptions::EnumDescription::Entry* Enums::Common__VehicleDataStatus__entries[] = {
  &Common__VehicleDataStatus__entry1,
  &Common__VehicleDataStatus__entry2,
  &Common__VehicleDataStatus__entry3,
  NULL };

ford_message_descriptions::EnumDescription::Entry Common__VehicleDataEventStatus__entry1 = {"NO_EVENT", 1};
ford_message_descriptions::EnumDescription::Entry Common__VehicleDataEventStatus__entry2 = {"NO", 2};
ford_message_descriptions::EnumDescription::Entry Common__VehicleDataEventStatus__entry3 = {"YES", 3};
ford_message_descriptions::EnumDescription::Entry Common__VehicleDataEventStatus__entry4 = {"NOT_SUPPORTED", 4};
ford_message_descriptions::EnumDescription::Entry Common__VehicleDataEventStatus__entry5 = {"FAULT", 5};
const ford_message_descriptions::EnumDescription::Entry* Enums::Common__VehicleDataEventStatus__entries[] = {
  &Common__VehicleDataEventStatus__entry1,
  &Common__VehicleDataEventStatus__entry2,
  &Common__VehicleDataEventStatus__entry3,
  &Common__VehicleDataEventStatus__entry4,
  &Common__VehicleDataEventStatus__entry5,
  NULL };

ford_message_descriptions::EnumDescription::Entry Common__ComponentVolumeStatus__entry1 = {"UNKNOWN", 1};
ford_message_descriptions::EnumDescription::Entry Common__ComponentVolumeStatus__entry2 = {"NORMAL", 2};
ford_message_descriptions::EnumDescription::Entry Common__ComponentVolumeStatus__entry3 = {"LOW", 3};
ford_message_descriptions::EnumDescription::Entry Common__ComponentVolumeStatus__entry4 = {"FAULT", 4};
ford_message_descriptions::EnumDescription::Entry Common__ComponentVolumeStatus__entry5 = {"ALERT", 5};
ford_message_descriptions::EnumDescription::Entry Common__ComponentVolumeStatus__entry6 = {"NOT_SUPPORTED", 6};
const ford_message_descriptions::EnumDescription::Entry* Enums::Common__ComponentVolumeStatus__entries[] = {
  &Common__ComponentVolumeStatus__entry1,
  &Common__ComponentVolumeStatus__entry2,
  &Common__ComponentVolumeStatus__entry3,
  &Common__ComponentVolumeStatus__entry4,
  &Common__ComponentVolumeStatus__entry5,
  &Common__ComponentVolumeStatus__entry6,
  NULL };

ford_message_descriptions::EnumDescription::Entry Common__SoftButtonType__entry1 = {"TEXT", 1};
ford_message_descriptions::EnumDescription::Entry Common__SoftButtonType__entry2 = {"IMAGE", 2};
ford_message_descriptions::EnumDescription::Entry Common__SoftButtonType__entry3 = {"BOTH", 3};
const ford_message_descriptions::EnumDescription::Entry* Enums::Common__SoftButtonType__entries[] = {
  &Common__SoftButtonType__entry1,
  &Common__SoftButtonType__entry2,
  &Common__SoftButtonType__entry3,
  NULL };

ford_message_descriptions::EnumDescription::Entry Common__Dimension__entry1 = {"NO_FIX", 1};
ford_message_descriptions::EnumDescription::Entry Common__Dimension__entry2 = {"2D", 2};
ford_message_descriptions::EnumDescription::Entry Common__Dimension__entry3 = {"3D", 3};
const ford_message_descriptions::EnumDescription::Entry* Enums::Common__Dimension__entries[] = {
  &Common__Dimension__entry1,
  &Common__Dimension__entry2,
  &Common__Dimension__entry3,
  NULL };

ford_message_descriptions::EnumDescription::Entry Common__WarningLightStatus__entry1 = {"OFF", 1};
ford_message_descriptions::EnumDescription::Entry Common__WarningLightStatus__entry2 = {"ON", 2};
ford_message_descriptions::EnumDescription::Entry Common__WarningLightStatus__entry3 = {"FLASH", 3};
const ford_message_descriptions::EnumDescription::Entry* Enums::Common__WarningLightStatus__entries[] = {
  &Common__WarningLightStatus__entry1,
  &Common__WarningLightStatus__entry2,
  &Common__WarningLightStatus__entry3,
  NULL };

ford_message_descriptions::EnumDescription::Entry Common__DeviceLevelStatus__entry1 = {"ZERO_LEVEL_BARS", 1};
ford_message_descriptions::EnumDescription::Entry Common__DeviceLevelStatus__entry2 = {"ONE_LEVEL_BARS", 2};
ford_message_descriptions::EnumDescription::Entry Common__DeviceLevelStatus__entry3 = {"TWO_LEVEL_BARS", 3};
ford_message_descriptions::EnumDescription::Entry Common__DeviceLevelStatus__entry4 = {"THREE_LEVEL_BARS", 4};
ford_message_descriptions::EnumDescription::Entry Common__DeviceLevelStatus__entry5 = {"FOUR_LEVEL_BARS", 5};
ford_message_descriptions::EnumDescription::Entry Common__DeviceLevelStatus__entry6 = {"NOT_PROVIDED", 6};
const ford_message_descriptions::EnumDescription::Entry* Enums::Common__DeviceLevelStatus__entries[] = {
  &Common__DeviceLevelStatus__entry1,
  &Common__DeviceLevelStatus__entry2,
  &Common__DeviceLevelStatus__entry3,
  &Common__DeviceLevelStatus__entry4,
  &Common__DeviceLevelStatus__entry5,
  &Common__DeviceLevelStatus__entry6,
  NULL };

ford_message_descriptions::EnumDescription::Entry Common__DeactivateReason__entry1 = {"AUDIO", 1};
ford_message_descriptions::EnumDescription::Entry Common__DeactivateReason__entry2 = {"PHONECALL", 2};
ford_message_descriptions::EnumDescription::Entry Common__DeactivateReason__entry3 = {"NAVIGATIONMAP", 3};
ford_message_descriptions::EnumDescription::Entry Common__DeactivateReason__entry4 = {"PHONEMENU", 4};
ford_message_descriptions::EnumDescription::Entry Common__DeactivateReason__entry5 = {"SYNCSETTINGS", 5};
ford_message_descriptions::EnumDescription::Entry Common__DeactivateReason__entry6 = {"GENERAL", 6};
const ford_message_descriptions::EnumDescription::Entry* Enums::Common__DeactivateReason__entries[] = {
  &Common__DeactivateReason__entry1,
  &Common__DeactivateReason__entry2,
  &Common__DeactivateReason__entry3,
  &Common__DeactivateReason__entry4,
  &Common__DeactivateReason__entry5,
  &Common__DeactivateReason__entry6,
  NULL };

ford_message_descriptions::EnumDescription::Entry Common__DisplayType__entry1 = {"CID", 1};
ford_message_descriptions::EnumDescription::Entry Common__DisplayType__entry2 = {"TYPE2", 2};
ford_message_descriptions::EnumDescription::Entry Common__DisplayType__entry3 = {"TYPE5", 3};
ford_message_descriptions::EnumDescription::Entry Common__DisplayType__entry4 = {"NGN", 4};
ford_message_descriptions::EnumDescription::Entry Common__DisplayType__entry5 = {"GEN2_8_DMA", 5};
ford_message_descriptions::EnumDescription::Entry Common__DisplayType__entry6 = {"GEN2_6_DMA", 6};
ford_message_descriptions::EnumDescription::Entry Common__DisplayType__entry7 = {"MFD3", 7};
ford_message_descriptions::EnumDescription::Entry Common__DisplayType__entry8 = {"MFD4", 8};
ford_message_descriptions::EnumDescription::Entry Common__DisplayType__entry9 = {"MFD5", 9};
const ford_message_descriptions::EnumDescription::Entry* Enums::Common__DisplayType__entries[] = {
  &Common__DisplayType__entry1,
  &Common__DisplayType__entry2,
  &Common__DisplayType__entry3,
  &Common__DisplayType__entry4,
  &Common__DisplayType__entry5,
  &Common__DisplayType__entry6,
  &Common__DisplayType__entry7,
  &Common__DisplayType__entry8,
  &Common__DisplayType__entry9,
  NULL };

ford_message_descriptions::EnumDescription::Entry Common__HmiZoneCapabilities__entry1 = {"FRONT", 1};
ford_message_descriptions::EnumDescription::Entry Common__HmiZoneCapabilities__entry2 = {"BACK", 2};
const ford_message_descriptions::EnumDescription::Entry* Enums::Common__HmiZoneCapabilities__entries[] = {
  &Common__HmiZoneCapabilities__entry1,
  &Common__HmiZoneCapabilities__entry2,
  NULL };

const ford_message_descriptions::ParameterDescription* Buttons__GetCapabilities__request__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription Buttons__GetCapabilities__request = {
  "Buttons",
  "GetCapabilities",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::Buttons_GetCapabilities,
  Buttons__GetCapabilities__request__parameters
};

const ford_message_descriptions::StructDescription Buttons__GetCapabilities__response__parameter1 = {
  {
    "capabilities",
    ford_message_descriptions::Struct,
    true,
    true
  },
  Structs::Common__ButtonCapabilities__parameters
};
const ford_message_descriptions::StructDescription Buttons__GetCapabilities__response__parameter2 = {
  {
    "presetBankCapabilities",
    ford_message_descriptions::Struct,
    false,
    false
  },
  Structs::Common__PresetBankCapabilities__parameters
};
const ford_message_descriptions::ParameterDescription* Buttons__GetCapabilities__response__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&Buttons__GetCapabilities__response__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&Buttons__GetCapabilities__response__parameter2,
  NULL };
const ford_message_descriptions::MessageDescription Buttons__GetCapabilities__response = {
  "Buttons",
  "GetCapabilities",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::Buttons_GetCapabilities,
  Buttons__GetCapabilities__response__parameters
};

const ford_message_descriptions::EnumDescription Buttons__OnButtonEvent__notification__parameter1 = {
  {
    "name",
    ford_message_descriptions::Enum,
    false,
    true
  },
  Enums::Common__ButtonName__entries
};
const ford_message_descriptions::EnumDescription Buttons__OnButtonEvent__notification__parameter2 = {
  {
    "mode",
    ford_message_descriptions::Enum,
    false,
    true
  },
  Enums::Common__ButtonEventMode__entries
};
const ford_message_descriptions::ParameterDescription Buttons__OnButtonEvent__notification__parameter3 = {
  "customButtonID",
  ford_message_descriptions::Integer,
  false,
  false
};
const ford_message_descriptions::ParameterDescription* Buttons__OnButtonEvent__notification__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&Buttons__OnButtonEvent__notification__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&Buttons__OnButtonEvent__notification__parameter2,
  (const ford_message_descriptions::ParameterDescription*)&Buttons__OnButtonEvent__notification__parameter3,
  NULL };
const ford_message_descriptions::MessageDescription Buttons__OnButtonEvent__notification = {
  "Buttons",
  "OnButtonEvent",
  hmi_apis::messageType::notification,
  hmi_apis::FunctionID::Buttons_OnButtonEvent,
  Buttons__OnButtonEvent__notification__parameters
};

const ford_message_descriptions::EnumDescription Buttons__OnButtonPress__notification__parameter1 = {
  {
    "name",
    ford_message_descriptions::Enum,
    false,
    true
  },
  Enums::Common__ButtonName__entries
};
const ford_message_descriptions::EnumDescription Buttons__OnButtonPress__notification__parameter2 = {
  {
    "mode",
    ford_message_descriptions::Enum,
    false,
    true
  },
  Enums::Common__ButtonPressMode__entries
};
const ford_message_descriptions::ParameterDescription Buttons__OnButtonPress__notification__parameter3 = {
  "customButtonID",
  ford_message_descriptions::Integer,
  false,
  false
};
const ford_message_descriptions::ParameterDescription* Buttons__OnButtonPress__notification__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&Buttons__OnButtonPress__notification__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&Buttons__OnButtonPress__notification__parameter2,
  (const ford_message_descriptions::ParameterDescription*)&Buttons__OnButtonPress__notification__parameter3,
  NULL };
const ford_message_descriptions::MessageDescription Buttons__OnButtonPress__notification = {
  "Buttons",
  "OnButtonPress",
  hmi_apis::messageType::notification,
  hmi_apis::FunctionID::Buttons_OnButtonPress,
  Buttons__OnButtonPress__notification__parameters
};

const ford_message_descriptions::ParameterDescription* BasicCommunication__OnReady__notification__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription BasicCommunication__OnReady__notification = {
  "BasicCommunication",
  "OnReady",
  hmi_apis::messageType::notification,
  hmi_apis::FunctionID::BasicCommunication_OnReady,
  BasicCommunication__OnReady__notification__parameters
};

const ford_message_descriptions::ParameterDescription* BasicCommunication__OnStartDeviceDiscovery__notification__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription BasicCommunication__OnStartDeviceDiscovery__notification = {
  "BasicCommunication",
  "OnStartDeviceDiscovery",
  hmi_apis::messageType::notification,
  hmi_apis::FunctionID::BasicCommunication_OnStartDeviceDiscovery,
  BasicCommunication__OnStartDeviceDiscovery__notification__parameters
};

const ford_message_descriptions::StructDescription BasicCommunication__UpdateDeviceList__request__parameter1 = {
  {
    "deviceList",
    ford_message_descriptions::Struct,
    true,
    true
  },
  Structs::Common__DeviceInfo__parameters
};
const ford_message_descriptions::ParameterDescription* BasicCommunication__UpdateDeviceList__request__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&BasicCommunication__UpdateDeviceList__request__parameter1,
  NULL };
const ford_message_descriptions::MessageDescription BasicCommunication__UpdateDeviceList__request = {
  "BasicCommunication",
  "UpdateDeviceList",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::BasicCommunication_UpdateDeviceList,
  BasicCommunication__UpdateDeviceList__request__parameters
};

const ford_message_descriptions::ParameterDescription* BasicCommunication__UpdateDeviceList__response__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription BasicCommunication__UpdateDeviceList__response = {
  "BasicCommunication",
  "UpdateDeviceList",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::BasicCommunication_UpdateDeviceList,
  BasicCommunication__UpdateDeviceList__response__parameters
};

const ford_message_descriptions::ParameterDescription* BasicCommunication__OnUpdateDeviceList__notification__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription BasicCommunication__OnUpdateDeviceList__notification = {
  "BasicCommunication",
  "OnUpdateDeviceList",
  hmi_apis::messageType::notification,
  hmi_apis::FunctionID::BasicCommunication_OnUpdateDeviceList,
  BasicCommunication__OnUpdateDeviceList__notification__parameters
};

const ford_message_descriptions::StructDescription BasicCommunication__AllowDeviceToConnect__request__parameter1 = {
  {
    "device",
    ford_message_descriptions::Struct,
    false,
    true
  },
  Structs::Common__DeviceInfo__parameters
};
const ford_message_descriptions::ParameterDescription* BasicCommunication__AllowDeviceToConnect__request__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&BasicCommunication__AllowDeviceToConnect__request__parameter1,
  NULL };
const ford_message_descriptions::MessageDescription BasicCommunication__AllowDeviceToConnect__request = {
  "BasicCommunication",
  "AllowDeviceToConnect",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::BasicCommunication_AllowDeviceToConnect,
  BasicCommunication__AllowDeviceToConnect__request__parameters
};

const ford_message_descriptions::ParameterDescription BasicCommunication__AllowDeviceToConnect__response__parameter1 = {
  "allow",
  ford_message_descriptions::Boolean,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* BasicCommunication__AllowDeviceToConnect__response__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&BasicCommunication__AllowDeviceToConnect__response__parameter1,
  NULL };
const ford_message_descriptions::MessageDescription BasicCommunication__AllowDeviceToConnect__response = {
  "BasicCommunication",
  "AllowDeviceToConnect",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::BasicCommunication_AllowDeviceToConnect,
  BasicCommunication__AllowDeviceToConnect__response__parameters
};

const ford_message_descriptions::StructDescription BasicCommunication__OnDeviceChosen__notification__parameter1 = {
  {
    "deviceInfo",
    ford_message_descriptions::Struct,
    false,
    true
  },
  Structs::Common__DeviceInfo__parameters
};
const ford_message_descriptions::ParameterDescription* BasicCommunication__OnDeviceChosen__notification__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&BasicCommunication__OnDeviceChosen__notification__parameter1,
  NULL };
const ford_message_descriptions::MessageDescription BasicCommunication__OnDeviceChosen__notification = {
  "BasicCommunication",
  "OnDeviceChosen",
  hmi_apis::messageType::notification,
  hmi_apis::FunctionID::BasicCommunication_OnDeviceChosen,
  BasicCommunication__OnDeviceChosen__notification__parameters
};

const ford_message_descriptions::StructDescription BasicCommunication__OnFindApplications__notification__parameter1 = {
  {
    "deviceInfo",
    ford_message_descriptions::Struct,
    false,
    true
  },
  Structs::Common__DeviceInfo__parameters
};
const ford_message_descriptions::ParameterDescription* BasicCommunication__OnFindApplications__notification__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&BasicCommunication__OnFindApplications__notification__parameter1,
  NULL };
const ford_message_descriptions::MessageDescription BasicCommunication__OnFindApplications__notification = {
  "BasicCommunication",
  "OnFindApplications",
  hmi_apis::messageType::notification,
  hmi_apis::FunctionID::BasicCommunication_OnFindApplications,
  BasicCommunication__OnFindApplications__notification__parameters
};

const ford_message_descriptions::StructDescription BasicCommunication__UpdateAppList__request__parameter1 = {
  {
    "applications",
    ford_message_descriptions::Struct,
    true,
    true
  },
  Structs::Common__HMIApplication__parameters
};
const ford_message_descriptions::ParameterDescription* BasicCommunication__UpdateAppList__request__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&BasicCommunication__UpdateAppList__request__parameter1,
  NULL };
const ford_message_descriptions::MessageDescription BasicCommunication__UpdateAppList__request = {
  "BasicCommunication",
  "UpdateAppList",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::BasicCommunication_UpdateAppList,
  BasicCommunication__UpdateAppList__request__parameters
};

const ford_message_descriptions::ParameterDescription* BasicCommunication__UpdateAppList__response__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription BasicCommunication__UpdateAppList__response = {
  "BasicCommunication",
  "UpdateAppList",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::BasicCommunication_UpdateAppList,
  BasicCommunication__UpdateAppList__response__parameters
};

const ford_message_descriptions::ParameterDescription BasicCommunication__OnAppActivated__notification__parameter1 = {
  "appID",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* BasicCommunication__OnAppActivated__notification__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&BasicCommunication__OnAppActivated__notification__parameter1,
  NULL };
const ford_message_descriptions::MessageDescription BasicCommunication__OnAppActivated__notification = {
  "BasicCommunication",
  "OnAppActivated",
  hmi_apis::messageType::notification,
  hmi_apis::FunctionID::BasicCommunication_OnAppActivated,
  BasicCommunication__OnAppActivated__notification__parameters
};

const ford_message_descriptions::ParameterDescription BasicCommunication__OnAppDeactivated__notification__parameter1 = {
  "appID",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::EnumDescription BasicCommunication__OnAppDeactivated__notification__parameter2 = {
  {
    "reason",
    ford_message_descriptions::Enum,
    false,
    true
  },
  Enums::Common__DeactivateReason__entries
};
const ford_message_descriptions::ParameterDescription* BasicCommunication__OnAppDeactivated__notification__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&BasicCommunication__OnAppDeactivated__notification__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&BasicCommunication__OnAppDeactivated__notification__parameter2,
  NULL };
const ford_message_descriptions::MessageDescription BasicCommunication__OnAppDeactivated__notification = {
  "BasicCommunication",
  "OnAppDeactivated",
  hmi_apis::messageType::notification,
  hmi_apis::FunctionID::BasicCommunication_OnAppDeactivated,
  BasicCommunication__OnAppDeactivated__notification__parameters
};

const ford_message_descriptions::StructDescription BasicCommunication__OnAppRegistered__notification__parameter1 = {
  {
    "application",
    ford_message_descriptions::Struct,
    false,
    true
  },
  Structs::Common__HMIApplication__parameters
};
const ford_message_descriptions::ParameterDescription* BasicCommunication__OnAppRegistered__notification__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&BasicCommunication__OnAppRegistered__notification__parameter1,
  NULL };
const ford_message_descriptions::MessageDescription BasicCommunication__OnAppRegistered__notification = {
  "BasicCommunication",
  "OnAppRegistered",
  hmi_apis::messageType::notification,
  hmi_apis::FunctionID::BasicCommunication_OnAppRegistered,
  BasicCommunication__OnAppRegistered__notification__parameters
};

const ford_message_descriptions::ParameterDescription BasicCommunication__OnAppUnregistered__notification__parameter1 = {
  "appID",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* BasicCommunication__OnAppUnregistered__notification__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&BasicCommunication__OnAppUnregistered__notification__parameter1,
  NULL };
const ford_message_descriptions::MessageDescription BasicCommunication__OnAppUnregistered__notification = {
  "BasicCommunication",
  "OnAppUnregistered",
  hmi_apis::messageType::notification,
  hmi_apis::FunctionID::BasicCommunication_OnAppUnregistered,
  BasicCommunication__OnAppUnregistered__notification__parameters
};

const ford_message_descriptions::ParameterDescription BasicCommunication__OnExitApplication__notification__parameter1 = {
  "appID",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* BasicCommunication__OnExitApplication__notification__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&BasicCommunication__OnExitApplication__notification__parameter1,
  NULL };
const ford_message_descriptions::MessageDescription BasicCommunication__OnExitApplication__notification = {
  "BasicCommunication",
  "OnExitApplication",
  hmi_apis::messageType::notification,
  hmi_apis::FunctionID::BasicCommunication_OnExitApplication,
  BasicCommunication__OnExitApplication__notification__parameters
};

const ford_message_descriptions::EnumDescription BasicCommunication__OnExitAllApplications__notification__parameter1 = {
  {
    "reason",
    ford_message_descriptions::Enum,
    false,
    true
  },
  Enums::Common__ApplicationsCloseReason__entries
};
const ford_message_descriptions::ParameterDescription* BasicCommunication__OnExitAllApplications__notification__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&BasicCommunication__OnExitAllApplications__notification__parameter1,
  NULL };
const ford_message_descriptions::MessageDescription BasicCommunication__OnExitAllApplications__notification = {
  "BasicCommunication",
  "OnExitAllApplications",
  hmi_apis::messageType::notification,
  hmi_apis::FunctionID::BasicCommunication_OnExitAllApplications,
  BasicCommunication__OnExitAllApplications__notification__parameters
};

const ford_message_descriptions::ParameterDescription* BasicCommunication__MixingAudioSupported__request__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription BasicCommunication__MixingAudioSupported__request = {
  "BasicCommunication",
  "MixingAudioSupported",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::BasicCommunication_MixingAudioSupported,
  BasicCommunication__MixingAudioSupported__request__parameters
};

const ford_message_descriptions::ParameterDescription BasicCommunication__MixingAudioSupported__response__parameter1 = {
  "attenuatedSupported",
  ford_message_descriptions::Boolean,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* BasicCommunication__MixingAudioSupported__response__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&BasicCommunication__MixingAudioSupported__response__parameter1,
  NULL };
const ford_message_descriptions::MessageDescription BasicCommunication__MixingAudioSupported__response = {
  "BasicCommunication",
  "MixingAudioSupported",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::BasicCommunication_MixingAudioSupported,
  BasicCommunication__MixingAudioSupported__response__parameters
};

const ford_message_descriptions::ParameterDescription* BasicCommunication__PlayTone__notification__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription BasicCommunication__PlayTone__notification = {
  "BasicCommunication",
  "PlayTone",
  hmi_apis::messageType::notification,
  hmi_apis::FunctionID::BasicCommunication_PlayTone,
  BasicCommunication__PlayTone__notification__parameters
};

const ford_message_descriptions::ParameterDescription* BasicCommunication__AllowAllApps__request__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription BasicCommunication__AllowAllApps__request = {
  "BasicCommunication",
  "AllowAllApps",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::BasicCommunication_AllowAllApps,
  BasicCommunication__AllowAllApps__request__parameters
};

const ford_message_descriptions::ParameterDescription BasicCommunication__AllowAllApps__response__parameter1 = {
  "allowed",
  ford_message_descriptions::Boolean,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* BasicCommunication__AllowAllApps__response__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&BasicCommunication__AllowAllApps__response__parameter1,
  NULL };
const ford_message_descriptions::MessageDescription BasicCommunication__AllowAllApps__response = {
  "BasicCommunication",
  "AllowAllApps",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::BasicCommunication_AllowAllApps,
  BasicCommunication__AllowAllApps__response__parameters
};

const ford_message_descriptions::StructDescription BasicCommunication__AllowApp__request__parameter1 = {
  {
    "app",
    ford_message_descriptions::Struct,
    false,
    true
  },
  Structs::Common__HMIApplication__parameters
};
const ford_message_descriptions::ParameterDescription BasicCommunication__AllowApp__request__parameter2 = {
  "appPermissions",
  ford_message_descriptions::String,
  true,
  false
};
const ford_message_descriptions::ParameterDescription* BasicCommunication__AllowApp__request__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&BasicCommunication__AllowApp__request__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&BasicCommunication__AllowApp__request__parameter2,
  NULL };
const ford_message_descriptions::MessageDescription BasicCommunication__AllowApp__request = {
  "BasicCommunication",
  "AllowApp",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::BasicCommunication_AllowApp,
  BasicCommunication__AllowApp__request__parameters
};

const ford_message_descriptions::ParameterDescription BasicCommunication__AllowApp__response__parameter1 = {
  "allowed",
  ford_message_descriptions::Boolean,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* BasicCommunication__AllowApp__response__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&BasicCommunication__AllowApp__response__parameter1,
  NULL };
const ford_message_descriptions::MessageDescription BasicCommunication__AllowApp__response = {
  "BasicCommunication",
  "AllowApp",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::BasicCommunication_AllowApp,
  BasicCommunication__AllowApp__response__parameters
};

const ford_message_descriptions::ParameterDescription* VR__IsReady__request__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription VR__IsReady__request = {
  "VR",
  "IsReady",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::VR_IsReady,
  VR__IsReady__request__parameters
};

const ford_message_descriptions::ParameterDescription VR__IsReady__response__parameter1 = {
  "available",
  ford_message_descriptions::Boolean,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* VR__IsReady__response__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&VR__IsReady__response__parameter1,
  NULL };
const ford_message_descriptions::MessageDescription VR__IsReady__response = {
  "VR",
  "IsReady",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::VR_IsReady,
  VR__IsReady__response__parameters
};

const ford_message_descriptions::ParameterDescription* VR__Started__notification__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription VR__Started__notification = {
  "VR",
  "Started",
  hmi_apis::messageType::notification,
  hmi_apis::FunctionID::VR_Started,
  VR__Started__notification__parameters
};

const ford_message_descriptions::ParameterDescription* VR__Stopped__notification__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription VR__Stopped__notification = {
  "VR",
  "Stopped",
  hmi_apis::messageType::notification,
  hmi_apis::FunctionID::VR_Stopped,
  VR__Stopped__notification__parameters
};

const ford_message_descriptions::ParameterDescription VR__AddCommand__request__parameter1 = {
  "cmdID",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription VR__AddCommand__request__parameter2 = {
  "vrCommands",
  ford_message_descriptions::String,
  true,
  true
};
const ford_message_descriptions::ParameterDescription VR__AddCommand__request__parameter3 = {
  "appID",
  ford_message_descriptions::Integer,
  false,
  false
};
const ford_message_descriptions::ParameterDescription* VR__AddCommand__request__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&VR__AddCommand__request__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&VR__AddCommand__request__parameter2,
  (const ford_message_descriptions::ParameterDescription*)&VR__AddCommand__request__parameter3,
  NULL };
const ford_message_descriptions::MessageDescription VR__AddCommand__request = {
  "VR",
  "AddCommand",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::VR_AddCommand,
  VR__AddCommand__request__parameters
};

const ford_message_descriptions::ParameterDescription* VR__AddCommand__response__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription VR__AddCommand__response = {
  "VR",
  "AddCommand",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::VR_AddCommand,
  VR__AddCommand__response__parameters
};

const ford_message_descriptions::ParameterDescription VR__DeleteCommand__request__parameter1 = {
  "cmdID",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription VR__DeleteCommand__request__parameter2 = {
  "appID",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* VR__DeleteCommand__request__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&VR__DeleteCommand__request__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&VR__DeleteCommand__request__parameter2,
  NULL };
const ford_message_descriptions::MessageDescription VR__DeleteCommand__request = {
  "VR",
  "DeleteCommand",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::VR_DeleteCommand,
  VR__DeleteCommand__request__parameters
};

const ford_message_descriptions::ParameterDescription* VR__DeleteCommand__response__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription VR__DeleteCommand__response = {
  "VR",
  "DeleteCommand",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::VR_DeleteCommand,
  VR__DeleteCommand__response__parameters
};

const ford_message_descriptions::ParameterDescription VR__OnCommand__notification__parameter1 = {
  "cmdID",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription VR__OnCommand__notification__parameter2 = {
  "appID",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* VR__OnCommand__notification__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&VR__OnCommand__notification__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&VR__OnCommand__notification__parameter2,
  NULL };
const ford_message_descriptions::MessageDescription VR__OnCommand__notification = {
  "VR",
  "OnCommand",
  hmi_apis::messageType::notification,
  hmi_apis::FunctionID::VR_OnCommand,
  VR__OnCommand__notification__parameters
};

const ford_message_descriptions::EnumDescription VR__ChangeRegistration__request__parameter1 = {
  {
    "language",
    ford_message_descriptions::Enum,
    false,
    true
  },
  Enums::Common__Language__entries
};
const ford_message_descriptions::ParameterDescription VR__ChangeRegistration__request__parameter2 = {
  "appID",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* VR__ChangeRegistration__request__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&VR__ChangeRegistration__request__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&VR__ChangeRegistration__request__parameter2,
  NULL };
const ford_message_descriptions::MessageDescription VR__ChangeRegistration__request = {
  "VR",
  "ChangeRegistration",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::VR_ChangeRegistration,
  VR__ChangeRegistration__request__parameters
};

const ford_message_descriptions::ParameterDescription* VR__ChangeRegistration__response__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription VR__ChangeRegistration__response = {
  "VR",
  "ChangeRegistration",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::VR_ChangeRegistration,
  VR__ChangeRegistration__response__parameters
};

const ford_message_descriptions::EnumDescription VR__OnLanguageChange__notification__parameter1 = {
  {
    "language",
    ford_message_descriptions::Enum,
    false,
    true
  },
  Enums::Common__Language__entries
};
const ford_message_descriptions::ParameterDescription* VR__OnLanguageChange__notification__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&VR__OnLanguageChange__notification__parameter1,
  NULL };
const ford_message_descriptions::MessageDescription VR__OnLanguageChange__notification = {
  "VR",
  "OnLanguageChange",
  hmi_apis::messageType::notification,
  hmi_apis::FunctionID::VR_OnLanguageChange,
  VR__OnLanguageChange__notification__parameters
};

const ford_message_descriptions::ParameterDescription* VR__GetSupportedLanguages__request__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription VR__GetSupportedLanguages__request = {
  "VR",
  "GetSupportedLanguages",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::VR_GetSupportedLanguages,
  VR__GetSupportedLanguages__request__parameters
};

const ford_message_descriptions::EnumDescription VR__GetSupportedLanguages__response__parameter1 = {
  {
    "languages",
    ford_message_descriptions::Enum,
    true,
    true
  },
  Enums::Common__Language__entries
};
const ford_message_descriptions::ParameterDescription* VR__GetSupportedLanguages__response__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&VR__GetSupportedLanguages__response__parameter1,
  NULL };
const ford_message_descriptions::MessageDescription VR__GetSupportedLanguages__response = {
  "VR",
  "GetSupportedLanguages",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::VR_GetSupportedLanguages,
  VR__GetSupportedLanguages__response__parameters
};

const ford_message_descriptions::ParameterDescription* VR__GetLanguage__request__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription VR__GetLanguage__request = {
  "VR",
  "GetLanguage",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::VR_GetLanguage,
  VR__GetLanguage__request__parameters
};

const ford_message_descriptions::EnumDescription VR__GetLanguage__response__parameter1 = {
  {
    "language",
    ford_message_descriptions::Enum,
    false,
    true
  },
  Enums::Common__Language__entries
};
const ford_message_descriptions::ParameterDescription* VR__GetLanguage__response__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&VR__GetLanguage__response__parameter1,
  NULL };
const ford_message_descriptions::MessageDescription VR__GetLanguage__response = {
  "VR",
  "GetLanguage",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::VR_GetLanguage,
  VR__GetLanguage__response__parameters
};

const ford_message_descriptions::ParameterDescription* TTS__IsReady__request__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription TTS__IsReady__request = {
  "TTS",
  "IsReady",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::TTS_IsReady,
  TTS__IsReady__request__parameters
};

const ford_message_descriptions::ParameterDescription TTS__IsReady__response__parameter1 = {
  "available",
  ford_message_descriptions::Boolean,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* TTS__IsReady__response__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&TTS__IsReady__response__parameter1,
  NULL };
const ford_message_descriptions::MessageDescription TTS__IsReady__response = {
  "TTS",
  "IsReady",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::TTS_IsReady,
  TTS__IsReady__response__parameters
};

const ford_message_descriptions::StructDescription TTS__Speak__request__parameter1 = {
  {
    "ttsChunks",
    ford_message_descriptions::Struct,
    true,
    true
  },
  Structs::Common__TTSChunk__parameters
};
const ford_message_descriptions::ParameterDescription TTS__Speak__request__parameter2 = {
  "appID",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* TTS__Speak__request__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&TTS__Speak__request__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&TTS__Speak__request__parameter2,
  NULL };
const ford_message_descriptions::MessageDescription TTS__Speak__request = {
  "TTS",
  "Speak",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::TTS_Speak,
  TTS__Speak__request__parameters
};

const ford_message_descriptions::ParameterDescription* TTS__Speak__response__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription TTS__Speak__response = {
  "TTS",
  "Speak",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::TTS_Speak,
  TTS__Speak__response__parameters
};

const ford_message_descriptions::ParameterDescription* TTS__StopSpeaking__request__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription TTS__StopSpeaking__request = {
  "TTS",
  "StopSpeaking",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::TTS_StopSpeaking,
  TTS__StopSpeaking__request__parameters
};

const ford_message_descriptions::ParameterDescription* TTS__StopSpeaking__response__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription TTS__StopSpeaking__response = {
  "TTS",
  "StopSpeaking",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::TTS_StopSpeaking,
  TTS__StopSpeaking__response__parameters
};

const ford_message_descriptions::EnumDescription TTS__ChangeRegistration__request__parameter1 = {
  {
    "language",
    ford_message_descriptions::Enum,
    false,
    true
  },
  Enums::Common__Language__entries
};
const ford_message_descriptions::ParameterDescription TTS__ChangeRegistration__request__parameter2 = {
  "appID",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* TTS__ChangeRegistration__request__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&TTS__ChangeRegistration__request__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&TTS__ChangeRegistration__request__parameter2,
  NULL };
const ford_message_descriptions::MessageDescription TTS__ChangeRegistration__request = {
  "TTS",
  "ChangeRegistration",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::TTS_ChangeRegistration,
  TTS__ChangeRegistration__request__parameters
};

const ford_message_descriptions::ParameterDescription* TTS__ChangeRegistration__response__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription TTS__ChangeRegistration__response = {
  "TTS",
  "ChangeRegistration",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::TTS_ChangeRegistration,
  TTS__ChangeRegistration__response__parameters
};

const ford_message_descriptions::EnumDescription TTS__OnLanguageChange__notification__parameter1 = {
  {
    "language",
    ford_message_descriptions::Enum,
    false,
    true
  },
  Enums::Common__Language__entries
};
const ford_message_descriptions::ParameterDescription* TTS__OnLanguageChange__notification__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&TTS__OnLanguageChange__notification__parameter1,
  NULL };
const ford_message_descriptions::MessageDescription TTS__OnLanguageChange__notification = {
  "TTS",
  "OnLanguageChange",
  hmi_apis::messageType::notification,
  hmi_apis::FunctionID::TTS_OnLanguageChange,
  TTS__OnLanguageChange__notification__parameters
};

const ford_message_descriptions::ParameterDescription* TTS__GetSupportedLanguages__request__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription TTS__GetSupportedLanguages__request = {
  "TTS",
  "GetSupportedLanguages",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::TTS_GetSupportedLanguages,
  TTS__GetSupportedLanguages__request__parameters
};

const ford_message_descriptions::EnumDescription TTS__GetSupportedLanguages__response__parameter1 = {
  {
    "languages",
    ford_message_descriptions::Enum,
    true,
    true
  },
  Enums::Common__Language__entries
};
const ford_message_descriptions::ParameterDescription* TTS__GetSupportedLanguages__response__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&TTS__GetSupportedLanguages__response__parameter1,
  NULL };
const ford_message_descriptions::MessageDescription TTS__GetSupportedLanguages__response = {
  "TTS",
  "GetSupportedLanguages",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::TTS_GetSupportedLanguages,
  TTS__GetSupportedLanguages__response__parameters
};

const ford_message_descriptions::ParameterDescription* TTS__GetLanguage__request__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription TTS__GetLanguage__request = {
  "TTS",
  "GetLanguage",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::TTS_GetLanguage,
  TTS__GetLanguage__request__parameters
};

const ford_message_descriptions::EnumDescription TTS__GetLanguage__response__parameter1 = {
  {
    "language",
    ford_message_descriptions::Enum,
    false,
    true
  },
  Enums::Common__Language__entries
};
const ford_message_descriptions::ParameterDescription* TTS__GetLanguage__response__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&TTS__GetLanguage__response__parameter1,
  NULL };
const ford_message_descriptions::MessageDescription TTS__GetLanguage__response = {
  "TTS",
  "GetLanguage",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::TTS_GetLanguage,
  TTS__GetLanguage__response__parameters
};

const ford_message_descriptions::StructDescription TTS__SetGlobalProperties__request__parameter1 = {
  {
    "helpPrompt",
    ford_message_descriptions::Struct,
    true,
    false
  },
  Structs::Common__TTSChunk__parameters
};
const ford_message_descriptions::StructDescription TTS__SetGlobalProperties__request__parameter2 = {
  {
    "timeoutPrompt",
    ford_message_descriptions::Struct,
    true,
    false
  },
  Structs::Common__TTSChunk__parameters
};
const ford_message_descriptions::ParameterDescription TTS__SetGlobalProperties__request__parameter3 = {
  "appID",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* TTS__SetGlobalProperties__request__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&TTS__SetGlobalProperties__request__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&TTS__SetGlobalProperties__request__parameter2,
  (const ford_message_descriptions::ParameterDescription*)&TTS__SetGlobalProperties__request__parameter3,
  NULL };
const ford_message_descriptions::MessageDescription TTS__SetGlobalProperties__request = {
  "TTS",
  "SetGlobalProperties",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::TTS_SetGlobalProperties,
  TTS__SetGlobalProperties__request__parameters
};

const ford_message_descriptions::ParameterDescription* TTS__SetGlobalProperties__response__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription TTS__SetGlobalProperties__response = {
  "TTS",
  "SetGlobalProperties",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::TTS_SetGlobalProperties,
  TTS__SetGlobalProperties__response__parameters
};

const ford_message_descriptions::StructDescription UI__Alert__request__parameter1 = {
  {
    "alertStrings",
    ford_message_descriptions::Struct,
    true,
    true
  },
  Structs::Common__TextFieldStruct__parameters
};
const ford_message_descriptions::ParameterDescription UI__Alert__request__parameter2 = {
  "duration",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::StructDescription UI__Alert__request__parameter3 = {
  {
    "softButtons",
    ford_message_descriptions::Struct,
    true,
    false
  },
  Structs::Common__SoftButton__parameters
};
const ford_message_descriptions::ParameterDescription UI__Alert__request__parameter4 = {
  "appID",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* UI__Alert__request__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&UI__Alert__request__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&UI__Alert__request__parameter2,
  (const ford_message_descriptions::ParameterDescription*)&UI__Alert__request__parameter3,
  (const ford_message_descriptions::ParameterDescription*)&UI__Alert__request__parameter4,
  NULL };
const ford_message_descriptions::MessageDescription UI__Alert__request = {
  "UI",
  "Alert",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::UI_Alert,
  UI__Alert__request__parameters
};

const ford_message_descriptions::ParameterDescription UI__Alert__response__parameter1 = {
  "tryAgainTime",
  ford_message_descriptions::Integer,
  false,
  false
};
const ford_message_descriptions::ParameterDescription* UI__Alert__response__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&UI__Alert__response__parameter1,
  NULL };
const ford_message_descriptions::MessageDescription UI__Alert__response = {
  "UI",
  "Alert",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::UI_Alert,
  UI__Alert__response__parameters
};

const ford_message_descriptions::StructDescription UI__ShowNotification__notification__parameter1 = {
  {
    "text",
    ford_message_descriptions::Struct,
    false,
    true
  },
  Structs::Common__TextFieldStruct__parameters
};
const ford_message_descriptions::StructDescription UI__ShowNotification__notification__parameter2 = {
  {
    "icon",
    ford_message_descriptions::Struct,
    false,
    false
  },
  Structs::Common__Image__parameters
};
const ford_message_descriptions::ParameterDescription UI__ShowNotification__notification__parameter3 = {
  "timeout",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* UI__ShowNotification__notification__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&UI__ShowNotification__notification__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&UI__ShowNotification__notification__parameter2,
  (const ford_message_descriptions::ParameterDescription*)&UI__ShowNotification__notification__parameter3,
  NULL };
const ford_message_descriptions::MessageDescription UI__ShowNotification__notification = {
  "UI",
  "ShowNotification",
  hmi_apis::messageType::notification,
  hmi_apis::FunctionID::UI_ShowNotification,
  UI__ShowNotification__notification__parameters
};

const ford_message_descriptions::StructDescription UI__Show__request__parameter1 = {
  {
    "showStrings",
    ford_message_descriptions::Struct,
    true,
    true
  },
  Structs::Common__TextFieldStruct__parameters
};
const ford_message_descriptions::EnumDescription UI__Show__request__parameter2 = {
  {
    "alignment",
    ford_message_descriptions::Enum,
    false,
    false
  },
  Enums::Common__TextAlignment__entries
};
const ford_message_descriptions::StructDescription UI__Show__request__parameter3 = {
  {
    "graphic",
    ford_message_descriptions::Struct,
    false,
    false
  },
  Structs::Common__Image__parameters
};
const ford_message_descriptions::StructDescription UI__Show__request__parameter4 = {
  {
    "softButtons",
    ford_message_descriptions::Struct,
    true,
    false
  },
  Structs::Common__SoftButton__parameters
};
const ford_message_descriptions::ParameterDescription UI__Show__request__parameter5 = {
  "customPresets",
  ford_message_descriptions::String,
  true,
  false
};
const ford_message_descriptions::ParameterDescription UI__Show__request__parameter6 = {
  "appID",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* UI__Show__request__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&UI__Show__request__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&UI__Show__request__parameter2,
  (const ford_message_descriptions::ParameterDescription*)&UI__Show__request__parameter3,
  (const ford_message_descriptions::ParameterDescription*)&UI__Show__request__parameter4,
  (const ford_message_descriptions::ParameterDescription*)&UI__Show__request__parameter5,
  (const ford_message_descriptions::ParameterDescription*)&UI__Show__request__parameter6,
  NULL };
const ford_message_descriptions::MessageDescription UI__Show__request = {
  "UI",
  "Show",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::UI_Show,
  UI__Show__request__parameters
};

const ford_message_descriptions::ParameterDescription* UI__Show__response__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription UI__Show__response = {
  "UI",
  "Show",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::UI_Show,
  UI__Show__response__parameters
};

const ford_message_descriptions::ParameterDescription UI__AddCommand__request__parameter1 = {
  "cmdID",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::StructDescription UI__AddCommand__request__parameter2 = {
  {
    "menuParams",
    ford_message_descriptions::Struct,
    false,
    false
  },
  Structs::Common__MenuParams__parameters
};
const ford_message_descriptions::StructDescription UI__AddCommand__request__parameter3 = {
  {
    "cmdIcon",
    ford_message_descriptions::Struct,
    false,
    false
  },
  Structs::Common__Image__parameters
};
const ford_message_descriptions::ParameterDescription UI__AddCommand__request__parameter4 = {
  "appID",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* UI__AddCommand__request__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&UI__AddCommand__request__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&UI__AddCommand__request__parameter2,
  (const ford_message_descriptions::ParameterDescription*)&UI__AddCommand__request__parameter3,
  (const ford_message_descriptions::ParameterDescription*)&UI__AddCommand__request__parameter4,
  NULL };
const ford_message_descriptions::MessageDescription UI__AddCommand__request = {
  "UI",
  "AddCommand",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::UI_AddCommand,
  UI__AddCommand__request__parameters
};

const ford_message_descriptions::ParameterDescription* UI__AddCommand__response__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription UI__AddCommand__response = {
  "UI",
  "AddCommand",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::UI_AddCommand,
  UI__AddCommand__response__parameters
};

const ford_message_descriptions::ParameterDescription UI__DeleteCommand__request__parameter1 = {
  "cmdID",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription UI__DeleteCommand__request__parameter2 = {
  "appID",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* UI__DeleteCommand__request__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&UI__DeleteCommand__request__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&UI__DeleteCommand__request__parameter2,
  NULL };
const ford_message_descriptions::MessageDescription UI__DeleteCommand__request = {
  "UI",
  "DeleteCommand",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::UI_DeleteCommand,
  UI__DeleteCommand__request__parameters
};

const ford_message_descriptions::ParameterDescription* UI__DeleteCommand__response__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription UI__DeleteCommand__response = {
  "UI",
  "DeleteCommand",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::UI_DeleteCommand,
  UI__DeleteCommand__response__parameters
};

const ford_message_descriptions::ParameterDescription UI__AddSubMenu__request__parameter1 = {
  "menuID",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::StructDescription UI__AddSubMenu__request__parameter2 = {
  {
    "menuParams",
    ford_message_descriptions::Struct,
    false,
    true
  },
  Structs::Common__MenuParams__parameters
};
const ford_message_descriptions::ParameterDescription UI__AddSubMenu__request__parameter3 = {
  "appID",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* UI__AddSubMenu__request__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&UI__AddSubMenu__request__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&UI__AddSubMenu__request__parameter2,
  (const ford_message_descriptions::ParameterDescription*)&UI__AddSubMenu__request__parameter3,
  NULL };
const ford_message_descriptions::MessageDescription UI__AddSubMenu__request = {
  "UI",
  "AddSubMenu",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::UI_AddSubMenu,
  UI__AddSubMenu__request__parameters
};

const ford_message_descriptions::ParameterDescription* UI__AddSubMenu__response__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription UI__AddSubMenu__response = {
  "UI",
  "AddSubMenu",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::UI_AddSubMenu,
  UI__AddSubMenu__response__parameters
};

const ford_message_descriptions::ParameterDescription UI__DeleteSubMenu__request__parameter1 = {
  "menuID",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription UI__DeleteSubMenu__request__parameter2 = {
  "appID",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* UI__DeleteSubMenu__request__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&UI__DeleteSubMenu__request__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&UI__DeleteSubMenu__request__parameter2,
  NULL };
const ford_message_descriptions::MessageDescription UI__DeleteSubMenu__request = {
  "UI",
  "DeleteSubMenu",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::UI_DeleteSubMenu,
  UI__DeleteSubMenu__request__parameters
};

const ford_message_descriptions::ParameterDescription* UI__DeleteSubMenu__response__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription UI__DeleteSubMenu__response = {
  "UI",
  "DeleteSubMenu",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::UI_DeleteSubMenu,
  UI__DeleteSubMenu__response__parameters
};

const ford_message_descriptions::StructDescription UI__PerformInteraction__request__parameter1 = {
  {
    "initialText",
    ford_message_descriptions::Struct,
    false,
    true
  },
  Structs::Common__TextFieldStruct__parameters
};
const ford_message_descriptions::StructDescription UI__PerformInteraction__request__parameter2 = {
  {
    "choiceSet",
    ford_message_descriptions::Struct,
    true,
    true
  },
  Structs::Common__Choice__parameters
};
const ford_message_descriptions::StructDescription UI__PerformInteraction__request__parameter3 = {
  {
    "vrHelp",
    ford_message_descriptions::Struct,
    true,
    false
  },
  Structs::Common__VrHelpItem__parameters
};
const ford_message_descriptions::ParameterDescription UI__PerformInteraction__request__parameter4 = {
  "timeout",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription UI__PerformInteraction__request__parameter5 = {
  "appID",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* UI__PerformInteraction__request__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&UI__PerformInteraction__request__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&UI__PerformInteraction__request__parameter2,
  (const ford_message_descriptions::ParameterDescription*)&UI__PerformInteraction__request__parameter3,
  (const ford_message_descriptions::ParameterDescription*)&UI__PerformInteraction__request__parameter4,
  (const ford_message_descriptions::ParameterDescription*)&UI__PerformInteraction__request__parameter5,
  NULL };
const ford_message_descriptions::MessageDescription UI__PerformInteraction__request = {
  "UI",
  "PerformInteraction",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::UI_PerformInteraction,
  UI__PerformInteraction__request__parameters
};

const ford_message_descriptions::ParameterDescription UI__PerformInteraction__response__parameter1 = {
  "choiceID",
  ford_message_descriptions::Integer,
  false,
  false
};
const ford_message_descriptions::ParameterDescription* UI__PerformInteraction__response__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&UI__PerformInteraction__response__parameter1,
  NULL };
const ford_message_descriptions::MessageDescription UI__PerformInteraction__response = {
  "UI",
  "PerformInteraction",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::UI_PerformInteraction,
  UI__PerformInteraction__response__parameters
};

const ford_message_descriptions::StructDescription UI__SetMediaClockTimer__request__parameter1 = {
  {
    "startTime",
    ford_message_descriptions::Struct,
    false,
    false
  },
  Structs::Common__TimeFormat__parameters
};
const ford_message_descriptions::EnumDescription UI__SetMediaClockTimer__request__parameter2 = {
  {
    "updateMode",
    ford_message_descriptions::Enum,
    false,
    true
  },
  Enums::Common__ClockUpdateMode__entries
};
const ford_message_descriptions::ParameterDescription UI__SetMediaClockTimer__request__parameter3 = {
  "appID",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* UI__SetMediaClockTimer__request__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&UI__SetMediaClockTimer__request__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&UI__SetMediaClockTimer__request__parameter2,
  (const ford_message_descriptions::ParameterDescription*)&UI__SetMediaClockTimer__request__parameter3,
  NULL };
const ford_message_descriptions::MessageDescription UI__SetMediaClockTimer__request = {
  "UI",
  "SetMediaClockTimer",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::UI_SetMediaClockTimer,
  UI__SetMediaClockTimer__request__parameters
};

const ford_message_descriptions::ParameterDescription* UI__SetMediaClockTimer__response__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription UI__SetMediaClockTimer__response = {
  "UI",
  "SetMediaClockTimer",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::UI_SetMediaClockTimer,
  UI__SetMediaClockTimer__response__parameters
};

const ford_message_descriptions::ParameterDescription UI__SetGlobalProperties__request__parameter1 = {
  "vrHelpTitle",
  ford_message_descriptions::String,
  false,
  false
};
const ford_message_descriptions::StructDescription UI__SetGlobalProperties__request__parameter2 = {
  {
    "vrHelp",
    ford_message_descriptions::Struct,
    true,
    false
  },
  Structs::Common__VrHelpItem__parameters
};
const ford_message_descriptions::ParameterDescription UI__SetGlobalProperties__request__parameter3 = {
  "appID",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* UI__SetGlobalProperties__request__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&UI__SetGlobalProperties__request__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&UI__SetGlobalProperties__request__parameter2,
  (const ford_message_descriptions::ParameterDescription*)&UI__SetGlobalProperties__request__parameter3,
  NULL };
const ford_message_descriptions::MessageDescription UI__SetGlobalProperties__request = {
  "UI",
  "SetGlobalProperties",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::UI_SetGlobalProperties,
  UI__SetGlobalProperties__request__parameters
};

const ford_message_descriptions::ParameterDescription* UI__SetGlobalProperties__response__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription UI__SetGlobalProperties__response = {
  "UI",
  "SetGlobalProperties",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::UI_SetGlobalProperties,
  UI__SetGlobalProperties__response__parameters
};

const ford_message_descriptions::ParameterDescription UI__ShowVrHelp__request__parameter1 = {
  "vrHelpTitle",
  ford_message_descriptions::String,
  false,
  false
};
const ford_message_descriptions::StructDescription UI__ShowVrHelp__request__parameter2 = {
  {
    "vrHelp",
    ford_message_descriptions::Struct,
    true,
    false
  },
  Structs::Common__VrHelpItem__parameters
};
const ford_message_descriptions::ParameterDescription UI__ShowVrHelp__request__parameter3 = {
  "appID",
  ford_message_descriptions::Integer,
  false,
  false
};
const ford_message_descriptions::ParameterDescription* UI__ShowVrHelp__request__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&UI__ShowVrHelp__request__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&UI__ShowVrHelp__request__parameter2,
  (const ford_message_descriptions::ParameterDescription*)&UI__ShowVrHelp__request__parameter3,
  NULL };
const ford_message_descriptions::MessageDescription UI__ShowVrHelp__request = {
  "UI",
  "ShowVrHelp",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::UI_ShowVrHelp,
  UI__ShowVrHelp__request__parameters
};

const ford_message_descriptions::ParameterDescription* UI__ShowVrHelp__response__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription UI__ShowVrHelp__response = {
  "UI",
  "ShowVrHelp",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::UI_ShowVrHelp,
  UI__ShowVrHelp__response__parameters
};

const ford_message_descriptions::ParameterDescription UI__OnCommand__notification__parameter1 = {
  "cmdID",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription UI__OnCommand__notification__parameter2 = {
  "appID",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* UI__OnCommand__notification__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&UI__OnCommand__notification__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&UI__OnCommand__notification__parameter2,
  NULL };
const ford_message_descriptions::MessageDescription UI__OnCommand__notification = {
  "UI",
  "OnCommand",
  hmi_apis::messageType::notification,
  hmi_apis::FunctionID::UI_OnCommand,
  UI__OnCommand__notification__parameters
};

const ford_message_descriptions::EnumDescription UI__OnSystemContext__notification__parameter1 = {
  {
    "systemContext",
    ford_message_descriptions::Enum,
    false,
    true
  },
  Enums::Common__SystemContext__entries
};
const ford_message_descriptions::ParameterDescription* UI__OnSystemContext__notification__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&UI__OnSystemContext__notification__parameter1,
  NULL };
const ford_message_descriptions::MessageDescription UI__OnSystemContext__notification = {
  "UI",
  "OnSystemContext",
  hmi_apis::messageType::notification,
  hmi_apis::FunctionID::UI_OnSystemContext,
  UI__OnSystemContext__notification__parameters
};

const ford_message_descriptions::ParameterDescription* UI__GetCapabilities__request__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription UI__GetCapabilities__request = {
  "UI",
  "GetCapabilities",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::UI_GetCapabilities,
  UI__GetCapabilities__request__parameters
};

const ford_message_descriptions::StructDescription UI__GetCapabilities__response__parameter1 = {
  {
    "displayCapabilities",
    ford_message_descriptions::Struct,
    false,
    true
  },
  Structs::Common__DisplayCapabilities__parameters
};
const ford_message_descriptions::EnumDescription UI__GetCapabilities__response__parameter2 = {
  {
    "hmiZoneCapabilities",
    ford_message_descriptions::Enum,
    false,
    true
  },
  Enums::Common__HmiZoneCapabilities__entries
};
const ford_message_descriptions::StructDescription UI__GetCapabilities__response__parameter3 = {
  {
    "softButtonCapabilities",
    ford_message_descriptions::Struct,
    false,
    false
  },
  Structs::Common__SoftButtonCapabilities__parameters
};
const ford_message_descriptions::ParameterDescription* UI__GetCapabilities__response__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&UI__GetCapabilities__response__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&UI__GetCapabilities__response__parameter2,
  (const ford_message_descriptions::ParameterDescription*)&UI__GetCapabilities__response__parameter3,
  NULL };
const ford_message_descriptions::MessageDescription UI__GetCapabilities__response = {
  "UI",
  "GetCapabilities",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::UI_GetCapabilities,
  UI__GetCapabilities__response__parameters
};

const ford_message_descriptions::EnumDescription UI__ChangeRegistration__request__parameter1 = {
  {
    "language",
    ford_message_descriptions::Enum,
    false,
    true
  },
  Enums::Common__Language__entries
};
const ford_message_descriptions::ParameterDescription UI__ChangeRegistration__request__parameter2 = {
  "appID",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* UI__ChangeRegistration__request__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&UI__ChangeRegistration__request__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&UI__ChangeRegistration__request__parameter2,
  NULL };
const ford_message_descriptions::MessageDescription UI__ChangeRegistration__request = {
  "UI",
  "ChangeRegistration",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::UI_ChangeRegistration,
  UI__ChangeRegistration__request__parameters
};

const ford_message_descriptions::ParameterDescription* UI__ChangeRegistration__response__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription UI__ChangeRegistration__response = {
  "UI",
  "ChangeRegistration",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::UI_ChangeRegistration,
  UI__ChangeRegistration__response__parameters
};

const ford_message_descriptions::EnumDescription UI__OnLanguageChange__notification__parameter1 = {
  {
    "language",
    ford_message_descriptions::Enum,
    false,
    true
  },
  Enums::Common__Language__entries
};
const ford_message_descriptions::ParameterDescription* UI__OnLanguageChange__notification__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&UI__OnLanguageChange__notification__parameter1,
  NULL };
const ford_message_descriptions::MessageDescription UI__OnLanguageChange__notification = {
  "UI",
  "OnLanguageChange",
  hmi_apis::messageType::notification,
  hmi_apis::FunctionID::UI_OnLanguageChange,
  UI__OnLanguageChange__notification__parameters
};

const ford_message_descriptions::ParameterDescription* UI__GetSupportedLanguages__request__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription UI__GetSupportedLanguages__request = {
  "UI",
  "GetSupportedLanguages",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::UI_GetSupportedLanguages,
  UI__GetSupportedLanguages__request__parameters
};

const ford_message_descriptions::EnumDescription UI__GetSupportedLanguages__response__parameter1 = {
  {
    "languages",
    ford_message_descriptions::Enum,
    true,
    true
  },
  Enums::Common__Language__entries
};
const ford_message_descriptions::ParameterDescription* UI__GetSupportedLanguages__response__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&UI__GetSupportedLanguages__response__parameter1,
  NULL };
const ford_message_descriptions::MessageDescription UI__GetSupportedLanguages__response = {
  "UI",
  "GetSupportedLanguages",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::UI_GetSupportedLanguages,
  UI__GetSupportedLanguages__response__parameters
};

const ford_message_descriptions::ParameterDescription* UI__GetLanguage__request__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription UI__GetLanguage__request = {
  "UI",
  "GetLanguage",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::UI_GetLanguage,
  UI__GetLanguage__request__parameters
};

const ford_message_descriptions::EnumDescription UI__GetLanguage__response__parameter1 = {
  {
    "language",
    ford_message_descriptions::Enum,
    false,
    true
  },
  Enums::Common__Language__entries
};
const ford_message_descriptions::ParameterDescription* UI__GetLanguage__response__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&UI__GetLanguage__response__parameter1,
  NULL };
const ford_message_descriptions::MessageDescription UI__GetLanguage__response = {
  "UI",
  "GetLanguage",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::UI_GetLanguage,
  UI__GetLanguage__response__parameters
};

const ford_message_descriptions::EnumDescription UI__OnDriverDistraction__notification__parameter1 = {
  {
    "state",
    ford_message_descriptions::Enum,
    false,
    true
  },
  Enums::Common__DriverDistractionState__entries
};
const ford_message_descriptions::ParameterDescription* UI__OnDriverDistraction__notification__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&UI__OnDriverDistraction__notification__parameter1,
  NULL };
const ford_message_descriptions::MessageDescription UI__OnDriverDistraction__notification = {
  "UI",
  "OnDriverDistraction",
  hmi_apis::messageType::notification,
  hmi_apis::FunctionID::UI_OnDriverDistraction,
  UI__OnDriverDistraction__notification__parameters
};

const ford_message_descriptions::StructDescription UI__SetAppIcon__request__parameter1 = {
  {
    "syncFileName",
    ford_message_descriptions::Struct,
    false,
    true
  },
  Structs::Common__Image__parameters
};
const ford_message_descriptions::ParameterDescription UI__SetAppIcon__request__parameter2 = {
  "appID",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* UI__SetAppIcon__request__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&UI__SetAppIcon__request__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&UI__SetAppIcon__request__parameter2,
  NULL };
const ford_message_descriptions::MessageDescription UI__SetAppIcon__request = {
  "UI",
  "SetAppIcon",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::UI_SetAppIcon,
  UI__SetAppIcon__request__parameters
};

const ford_message_descriptions::ParameterDescription* UI__SetAppIcon__response__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription UI__SetAppIcon__response = {
  "UI",
  "SetAppIcon",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::UI_SetAppIcon,
  UI__SetAppIcon__response__parameters
};

const ford_message_descriptions::ParameterDescription UI__Slider__request__parameter1 = {
  "numTicks",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription UI__Slider__request__parameter2 = {
  "position",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription UI__Slider__request__parameter3 = {
  "sliderHeader",
  ford_message_descriptions::String,
  false,
  true
};
const ford_message_descriptions::ParameterDescription UI__Slider__request__parameter4 = {
  "sliderFooter",
  ford_message_descriptions::String,
  true,
  false
};
const ford_message_descriptions::ParameterDescription UI__Slider__request__parameter5 = {
  "timeout",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription UI__Slider__request__parameter6 = {
  "appID",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* UI__Slider__request__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&UI__Slider__request__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&UI__Slider__request__parameter2,
  (const ford_message_descriptions::ParameterDescription*)&UI__Slider__request__parameter3,
  (const ford_message_descriptions::ParameterDescription*)&UI__Slider__request__parameter4,
  (const ford_message_descriptions::ParameterDescription*)&UI__Slider__request__parameter5,
  (const ford_message_descriptions::ParameterDescription*)&UI__Slider__request__parameter6,
  NULL };
const ford_message_descriptions::MessageDescription UI__Slider__request = {
  "UI",
  "Slider",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::UI_Slider,
  UI__Slider__request__parameters
};

const ford_message_descriptions::ParameterDescription UI__Slider__response__parameter1 = {
  "sliderPosition",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* UI__Slider__response__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&UI__Slider__response__parameter1,
  NULL };
const ford_message_descriptions::MessageDescription UI__Slider__response = {
  "UI",
  "Slider",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::UI_Slider,
  UI__Slider__response__parameters
};

const ford_message_descriptions::StructDescription UI__ScrollableMessage__request__parameter1 = {
  {
    "messageText",
    ford_message_descriptions::Struct,
    false,
    true
  },
  Structs::Common__TextFieldStruct__parameters
};
const ford_message_descriptions::ParameterDescription UI__ScrollableMessage__request__parameter2 = {
  "timeout",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::StructDescription UI__ScrollableMessage__request__parameter3 = {
  {
    "softButtons",
    ford_message_descriptions::Struct,
    true,
    false
  },
  Structs::Common__SoftButton__parameters
};
const ford_message_descriptions::ParameterDescription UI__ScrollableMessage__request__parameter4 = {
  "appID",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* UI__ScrollableMessage__request__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&UI__ScrollableMessage__request__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&UI__ScrollableMessage__request__parameter2,
  (const ford_message_descriptions::ParameterDescription*)&UI__ScrollableMessage__request__parameter3,
  (const ford_message_descriptions::ParameterDescription*)&UI__ScrollableMessage__request__parameter4,
  NULL };
const ford_message_descriptions::MessageDescription UI__ScrollableMessage__request = {
  "UI",
  "ScrollableMessage",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::UI_ScrollableMessage,
  UI__ScrollableMessage__request__parameters
};

const ford_message_descriptions::ParameterDescription* UI__ScrollableMessage__response__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription UI__ScrollableMessage__response = {
  "UI",
  "ScrollableMessage",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::UI_ScrollableMessage,
  UI__ScrollableMessage__response__parameters
};

const ford_message_descriptions::StructDescription UI__PerformAudioPassThru__request__parameter1 = {
  {
    "audioPassThruDisplayTexts",
    ford_message_descriptions::Struct,
    true,
    true
  },
  Structs::Common__TextFieldStruct__parameters
};
const ford_message_descriptions::ParameterDescription UI__PerformAudioPassThru__request__parameter2 = {
  "maxDuration",
  ford_message_descriptions::Integer,
  false,
  false
};
const ford_message_descriptions::ParameterDescription* UI__PerformAudioPassThru__request__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&UI__PerformAudioPassThru__request__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&UI__PerformAudioPassThru__request__parameter2,
  NULL };
const ford_message_descriptions::MessageDescription UI__PerformAudioPassThru__request = {
  "UI",
  "PerformAudioPassThru",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::UI_PerformAudioPassThru,
  UI__PerformAudioPassThru__request__parameters
};

const ford_message_descriptions::ParameterDescription* UI__PerformAudioPassThru__response__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription UI__PerformAudioPassThru__response = {
  "UI",
  "PerformAudioPassThru",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::UI_PerformAudioPassThru,
  UI__PerformAudioPassThru__response__parameters
};

const ford_message_descriptions::ParameterDescription* UI__EndAudioPassThru__request__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription UI__EndAudioPassThru__request = {
  "UI",
  "EndAudioPassThru",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::UI_EndAudioPassThru,
  UI__EndAudioPassThru__request__parameters
};

const ford_message_descriptions::ParameterDescription* UI__EndAudioPassThru__response__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription UI__EndAudioPassThru__response = {
  "UI",
  "EndAudioPassThru",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::UI_EndAudioPassThru,
  UI__EndAudioPassThru__response__parameters
};

const ford_message_descriptions::ParameterDescription* UI__IsReady__request__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription UI__IsReady__request = {
  "UI",
  "IsReady",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::UI_IsReady,
  UI__IsReady__request__parameters
};

const ford_message_descriptions::ParameterDescription UI__IsReady__response__parameter1 = {
  "available",
  ford_message_descriptions::Boolean,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* UI__IsReady__response__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&UI__IsReady__response__parameter1,
  NULL };
const ford_message_descriptions::MessageDescription UI__IsReady__response = {
  "UI",
  "IsReady",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::UI_IsReady,
  UI__IsReady__response__parameters
};

const ford_message_descriptions::ParameterDescription* UI__ClosePopUp__request__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription UI__ClosePopUp__request = {
  "UI",
  "ClosePopUp",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::UI_ClosePopUp,
  UI__ClosePopUp__request__parameters
};

const ford_message_descriptions::ParameterDescription* UI__ClosePopUp__response__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription UI__ClosePopUp__response = {
  "UI",
  "ClosePopUp",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::UI_ClosePopUp,
  UI__ClosePopUp__response__parameters
};

const ford_message_descriptions::ParameterDescription* Navigation__IsReady__request__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription Navigation__IsReady__request = {
  "Navigation",
  "IsReady",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::Navigation_IsReady,
  Navigation__IsReady__request__parameters
};

const ford_message_descriptions::ParameterDescription Navigation__IsReady__response__parameter1 = {
  "available",
  ford_message_descriptions::Boolean,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* Navigation__IsReady__response__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&Navigation__IsReady__response__parameter1,
  NULL };
const ford_message_descriptions::MessageDescription Navigation__IsReady__response = {
  "Navigation",
  "IsReady",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::Navigation_IsReady,
  Navigation__IsReady__response__parameters
};

const ford_message_descriptions::StructDescription Navigation__ShowConstantTBT__request__parameter1 = {
  {
    "navigationTexts",
    ford_message_descriptions::Struct,
    true,
    true
  },
  Structs::Common__TextFieldStruct__parameters
};
const ford_message_descriptions::StructDescription Navigation__ShowConstantTBT__request__parameter2 = {
  {
    "turnIcon",
    ford_message_descriptions::Struct,
    false,
    false
  },
  Structs::Common__Image__parameters
};
const ford_message_descriptions::ParameterDescription Navigation__ShowConstantTBT__request__parameter3 = {
  "distanceToManeuver",
  ford_message_descriptions::Float,
  false,
  true
};
const ford_message_descriptions::ParameterDescription Navigation__ShowConstantTBT__request__parameter4 = {
  "distanceToManeuverScale",
  ford_message_descriptions::Float,
  false,
  true
};
const ford_message_descriptions::ParameterDescription Navigation__ShowConstantTBT__request__parameter5 = {
  "maneuverComplete",
  ford_message_descriptions::Boolean,
  false,
  false
};
const ford_message_descriptions::StructDescription Navigation__ShowConstantTBT__request__parameter6 = {
  {
    "softButtons",
    ford_message_descriptions::Struct,
    true,
    false
  },
  Structs::Common__SoftButton__parameters
};
const ford_message_descriptions::ParameterDescription Navigation__ShowConstantTBT__request__parameter7 = {
  "appID",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* Navigation__ShowConstantTBT__request__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&Navigation__ShowConstantTBT__request__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&Navigation__ShowConstantTBT__request__parameter2,
  (const ford_message_descriptions::ParameterDescription*)&Navigation__ShowConstantTBT__request__parameter3,
  (const ford_message_descriptions::ParameterDescription*)&Navigation__ShowConstantTBT__request__parameter4,
  (const ford_message_descriptions::ParameterDescription*)&Navigation__ShowConstantTBT__request__parameter5,
  (const ford_message_descriptions::ParameterDescription*)&Navigation__ShowConstantTBT__request__parameter6,
  (const ford_message_descriptions::ParameterDescription*)&Navigation__ShowConstantTBT__request__parameter7,
  NULL };
const ford_message_descriptions::MessageDescription Navigation__ShowConstantTBT__request = {
  "Navigation",
  "ShowConstantTBT",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::Navigation_ShowConstantTBT,
  Navigation__ShowConstantTBT__request__parameters
};

const ford_message_descriptions::ParameterDescription* Navigation__ShowConstantTBT__response__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription Navigation__ShowConstantTBT__response = {
  "Navigation",
  "ShowConstantTBT",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::Navigation_ShowConstantTBT,
  Navigation__ShowConstantTBT__response__parameters
};

const ford_message_descriptions::StructDescription Navigation__AlertManeuver__request__parameter1 = {
  {
    "softButtons",
    ford_message_descriptions::Struct,
    true,
    false
  },
  Structs::Common__SoftButton__parameters
};
const ford_message_descriptions::ParameterDescription* Navigation__AlertManeuver__request__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&Navigation__AlertManeuver__request__parameter1,
  NULL };
const ford_message_descriptions::MessageDescription Navigation__AlertManeuver__request = {
  "Navigation",
  "AlertManeuver",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::Navigation_AlertManeuver,
  Navigation__AlertManeuver__request__parameters
};

const ford_message_descriptions::ParameterDescription* Navigation__AlertManeuver__response__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription Navigation__AlertManeuver__response = {
  "Navigation",
  "AlertManeuver",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::Navigation_AlertManeuver,
  Navigation__AlertManeuver__response__parameters
};

const ford_message_descriptions::StructDescription Navigation__UpdateTurnList__request__parameter1 = {
  {
    "turnList",
    ford_message_descriptions::Struct,
    true,
    true
  },
  Structs::Common__Turn__parameters
};
const ford_message_descriptions::StructDescription Navigation__UpdateTurnList__request__parameter2 = {
  {
    "softButtons",
    ford_message_descriptions::Struct,
    true,
    false
  },
  Structs::Common__SoftButton__parameters
};
const ford_message_descriptions::ParameterDescription Navigation__UpdateTurnList__request__parameter3 = {
  "appID",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* Navigation__UpdateTurnList__request__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&Navigation__UpdateTurnList__request__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&Navigation__UpdateTurnList__request__parameter2,
  (const ford_message_descriptions::ParameterDescription*)&Navigation__UpdateTurnList__request__parameter3,
  NULL };
const ford_message_descriptions::MessageDescription Navigation__UpdateTurnList__request = {
  "Navigation",
  "UpdateTurnList",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::Navigation_UpdateTurnList,
  Navigation__UpdateTurnList__request__parameters
};

const ford_message_descriptions::ParameterDescription* Navigation__UpdateTurnList__response__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription Navigation__UpdateTurnList__response = {
  "Navigation",
  "UpdateTurnList",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::Navigation_UpdateTurnList,
  Navigation__UpdateTurnList__response__parameters
};

const ford_message_descriptions::EnumDescription Navigation__OnTBTClientState__notification__parameter1 = {
  {
    "state",
    ford_message_descriptions::Enum,
    false,
    true
  },
  Enums::Common__TBTState__entries
};
const ford_message_descriptions::ParameterDescription* Navigation__OnTBTClientState__notification__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&Navigation__OnTBTClientState__notification__parameter1,
  NULL };
const ford_message_descriptions::MessageDescription Navigation__OnTBTClientState__notification = {
  "Navigation",
  "OnTBTClientState",
  hmi_apis::messageType::notification,
  hmi_apis::FunctionID::Navigation_OnTBTClientState,
  Navigation__OnTBTClientState__notification__parameters
};

const ford_message_descriptions::ParameterDescription* VehicleInfo__IsReady__request__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription VehicleInfo__IsReady__request = {
  "VehicleInfo",
  "IsReady",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::VehicleInfo_IsReady,
  VehicleInfo__IsReady__request__parameters
};

const ford_message_descriptions::ParameterDescription VehicleInfo__IsReady__response__parameter1 = {
  "available",
  ford_message_descriptions::Boolean,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* VehicleInfo__IsReady__response__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__IsReady__response__parameter1,
  NULL };
const ford_message_descriptions::MessageDescription VehicleInfo__IsReady__response = {
  "VehicleInfo",
  "IsReady",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::VehicleInfo_IsReady,
  VehicleInfo__IsReady__response__parameters
};

const ford_message_descriptions::ParameterDescription* VehicleInfo__GetVehicleType__request__parameters[] = {
  NULL };
const ford_message_descriptions::MessageDescription VehicleInfo__GetVehicleType__request = {
  "VehicleInfo",
  "GetVehicleType",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::VehicleInfo_GetVehicleType,
  VehicleInfo__GetVehicleType__request__parameters
};

const ford_message_descriptions::StructDescription VehicleInfo__GetVehicleType__response__parameter1 = {
  {
    "vehicleType",
    ford_message_descriptions::Struct,
    false,
    true
  },
  Structs::Common__VehicleType__parameters
};
const ford_message_descriptions::ParameterDescription* VehicleInfo__GetVehicleType__response__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleType__response__parameter1,
  NULL };
const ford_message_descriptions::MessageDescription VehicleInfo__GetVehicleType__response = {
  "VehicleInfo",
  "GetVehicleType",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::VehicleInfo_GetVehicleType,
  VehicleInfo__GetVehicleType__response__parameters
};

const ford_message_descriptions::ParameterDescription VehicleInfo__ReadDID__request__parameter1 = {
  "ecuName",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription VehicleInfo__ReadDID__request__parameter2 = {
  "didLocation",
  ford_message_descriptions::Integer,
  true,
  true
};
const ford_message_descriptions::ParameterDescription VehicleInfo__ReadDID__request__parameter3 = {
  "appID",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* VehicleInfo__ReadDID__request__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__ReadDID__request__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__ReadDID__request__parameter2,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__ReadDID__request__parameter3,
  NULL };
const ford_message_descriptions::MessageDescription VehicleInfo__ReadDID__request = {
  "VehicleInfo",
  "ReadDID",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::VehicleInfo_ReadDID,
  VehicleInfo__ReadDID__request__parameters
};

const ford_message_descriptions::StructDescription VehicleInfo__ReadDID__response__parameter1 = {
  {
    "didResult",
    ford_message_descriptions::Struct,
    true,
    false
  },
  Structs::Common__DIDResult__parameters
};
const ford_message_descriptions::ParameterDescription* VehicleInfo__ReadDID__response__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__ReadDID__response__parameter1,
  NULL };
const ford_message_descriptions::MessageDescription VehicleInfo__ReadDID__response = {
  "VehicleInfo",
  "ReadDID",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::VehicleInfo_ReadDID,
  VehicleInfo__ReadDID__response__parameters
};

const ford_message_descriptions::ParameterDescription VehicleInfo__GetDTCs__request__parameter1 = {
  "ecuName",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription VehicleInfo__GetDTCs__request__parameter2 = {
  "dtcMask",
  ford_message_descriptions::Integer,
  false,
  false
};
const ford_message_descriptions::ParameterDescription VehicleInfo__GetDTCs__request__parameter3 = {
  "appID",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* VehicleInfo__GetDTCs__request__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetDTCs__request__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetDTCs__request__parameter2,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetDTCs__request__parameter3,
  NULL };
const ford_message_descriptions::MessageDescription VehicleInfo__GetDTCs__request = {
  "VehicleInfo",
  "GetDTCs",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::VehicleInfo_GetDTCs,
  VehicleInfo__GetDTCs__request__parameters
};

const ford_message_descriptions::ParameterDescription VehicleInfo__GetDTCs__response__parameter1 = {
  "ecuHeader",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription VehicleInfo__GetDTCs__response__parameter2 = {
  "dtc",
  ford_message_descriptions::String,
  true,
  false
};
const ford_message_descriptions::ParameterDescription* VehicleInfo__GetDTCs__response__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetDTCs__response__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetDTCs__response__parameter2,
  NULL };
const ford_message_descriptions::MessageDescription VehicleInfo__GetDTCs__response = {
  "VehicleInfo",
  "GetDTCs",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::VehicleInfo_GetDTCs,
  VehicleInfo__GetDTCs__response__parameters
};

const ford_message_descriptions::ParameterDescription VehicleInfo__GetVehicleData__request__parameter1 = {
  "gps",
  ford_message_descriptions::Boolean,
  false,
  false
};
const ford_message_descriptions::ParameterDescription VehicleInfo__GetVehicleData__request__parameter2 = {
  "speed",
  ford_message_descriptions::Boolean,
  false,
  false
};
const ford_message_descriptions::ParameterDescription VehicleInfo__GetVehicleData__request__parameter3 = {
  "rpm",
  ford_message_descriptions::Boolean,
  false,
  false
};
const ford_message_descriptions::ParameterDescription VehicleInfo__GetVehicleData__request__parameter4 = {
  "fuelLevel",
  ford_message_descriptions::Boolean,
  false,
  false
};
const ford_message_descriptions::ParameterDescription VehicleInfo__GetVehicleData__request__parameter5 = {
  "fuelLevel_State",
  ford_message_descriptions::Boolean,
  false,
  false
};
const ford_message_descriptions::ParameterDescription VehicleInfo__GetVehicleData__request__parameter6 = {
  "instantFuelConsumption",
  ford_message_descriptions::Boolean,
  false,
  false
};
const ford_message_descriptions::ParameterDescription VehicleInfo__GetVehicleData__request__parameter7 = {
  "externalTemperature",
  ford_message_descriptions::Boolean,
  false,
  false
};
const ford_message_descriptions::ParameterDescription VehicleInfo__GetVehicleData__request__parameter8 = {
  "vin",
  ford_message_descriptions::Boolean,
  false,
  false
};
const ford_message_descriptions::ParameterDescription VehicleInfo__GetVehicleData__request__parameter9 = {
  "prndl",
  ford_message_descriptions::Boolean,
  false,
  false
};
const ford_message_descriptions::ParameterDescription VehicleInfo__GetVehicleData__request__parameter10 = {
  "tirePressure",
  ford_message_descriptions::Boolean,
  false,
  false
};
const ford_message_descriptions::ParameterDescription VehicleInfo__GetVehicleData__request__parameter11 = {
  "odometer",
  ford_message_descriptions::Boolean,
  false,
  false
};
const ford_message_descriptions::ParameterDescription VehicleInfo__GetVehicleData__request__parameter12 = {
  "beltStatus",
  ford_message_descriptions::Boolean,
  false,
  false
};
const ford_message_descriptions::ParameterDescription VehicleInfo__GetVehicleData__request__parameter13 = {
  "bodyInformation",
  ford_message_descriptions::Boolean,
  false,
  false
};
const ford_message_descriptions::ParameterDescription VehicleInfo__GetVehicleData__request__parameter14 = {
  "deviceStatus",
  ford_message_descriptions::Boolean,
  false,
  false
};
const ford_message_descriptions::ParameterDescription VehicleInfo__GetVehicleData__request__parameter15 = {
  "driverBraking",
  ford_message_descriptions::Boolean,
  false,
  false
};
const ford_message_descriptions::ParameterDescription VehicleInfo__GetVehicleData__request__parameter16 = {
  "wiperStatus",
  ford_message_descriptions::Boolean,
  false,
  false
};
const ford_message_descriptions::ParameterDescription VehicleInfo__GetVehicleData__request__parameter17 = {
  "headLampStatus",
  ford_message_descriptions::Boolean,
  false,
  false
};
const ford_message_descriptions::ParameterDescription VehicleInfo__GetVehicleData__request__parameter18 = {
  "engineTorque",
  ford_message_descriptions::Boolean,
  false,
  false
};
const ford_message_descriptions::ParameterDescription VehicleInfo__GetVehicleData__request__parameter19 = {
  "accPedalPosition",
  ford_message_descriptions::Boolean,
  false,
  false
};
const ford_message_descriptions::ParameterDescription VehicleInfo__GetVehicleData__request__parameter20 = {
  "steeringWheelAngle",
  ford_message_descriptions::Boolean,
  false,
  false
};
const ford_message_descriptions::ParameterDescription VehicleInfo__GetVehicleData__request__parameter21 = {
  "myKey",
  ford_message_descriptions::Boolean,
  false,
  false
};
const ford_message_descriptions::ParameterDescription VehicleInfo__GetVehicleData__request__parameter22 = {
  "appID",
  ford_message_descriptions::Integer,
  false,
  true
};
const ford_message_descriptions::ParameterDescription* VehicleInfo__GetVehicleData__request__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleData__request__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleData__request__parameter2,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleData__request__parameter3,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleData__request__parameter4,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleData__request__parameter5,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleData__request__parameter6,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleData__request__parameter7,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleData__request__parameter8,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleData__request__parameter9,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleData__request__parameter10,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleData__request__parameter11,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleData__request__parameter12,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleData__request__parameter13,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleData__request__parameter14,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleData__request__parameter15,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleData__request__parameter16,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleData__request__parameter17,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleData__request__parameter18,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleData__request__parameter19,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleData__request__parameter20,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleData__request__parameter21,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleData__request__parameter22,
  NULL };
const ford_message_descriptions::MessageDescription VehicleInfo__GetVehicleData__request = {
  "VehicleInfo",
  "GetVehicleData",
  hmi_apis::messageType::request,
  hmi_apis::FunctionID::VehicleInfo_GetVehicleData,
  VehicleInfo__GetVehicleData__request__parameters
};

const ford_message_descriptions::StructDescription VehicleInfo__GetVehicleData__response__parameter1 = {
  {
    "gps",
    ford_message_descriptions::Struct,
    false,
    false
  },
  Structs::Common__GPSData__parameters
};
const ford_message_descriptions::ParameterDescription VehicleInfo__GetVehicleData__response__parameter2 = {
  "speed",
  ford_message_descriptions::Float,
  false,
  false
};
const ford_message_descriptions::ParameterDescription VehicleInfo__GetVehicleData__response__parameter3 = {
  "rpm",
  ford_message_descriptions::Integer,
  false,
  false
};
const ford_message_descriptions::ParameterDescription VehicleInfo__GetVehicleData__response__parameter4 = {
  "fuelLevel",
  ford_message_descriptions::Float,
  false,
  false
};
const ford_message_descriptions::EnumDescription VehicleInfo__GetVehicleData__response__parameter5 = {
  {
    "fuelLevel_State",
    ford_message_descriptions::Enum,
    false,
    false
  },
  Enums::Common__ComponentVolumeStatus__entries
};
const ford_message_descriptions::ParameterDescription VehicleInfo__GetVehicleData__response__parameter6 = {
  "instantFuelConsumption",
  ford_message_descriptions::Float,
  false,
  false
};
const ford_message_descriptions::ParameterDescription VehicleInfo__GetVehicleData__response__parameter7 = {
  "externalTemperature",
  ford_message_descriptions::Float,
  false,
  false
};
const ford_message_descriptions::ParameterDescription VehicleInfo__GetVehicleData__response__parameter8 = {
  "vin",
  ford_message_descriptions::String,
  false,
  false
};
const ford_message_descriptions::EnumDescription VehicleInfo__GetVehicleData__response__parameter9 = {
  {
    "prndl",
    ford_message_descriptions::Enum,
    false,
    false
  },
  Enums::Common__PRNDL__entries
};
const ford_message_descriptions::StructDescription VehicleInfo__GetVehicleData__response__parameter10 = {
  {
    "tirePressure",
    ford_message_descriptions::Struct,
    false,
    false
  },
  Structs::Common__TireStatus__parameters
};
const ford_message_descriptions::ParameterDescription VehicleInfo__GetVehicleData__response__parameter11 = {
  "odometer",
  ford_message_descriptions::Integer,
  false,
  false
};
const ford_message_descriptions::StructDescription VehicleInfo__GetVehicleData__response__parameter12 = {
  {
    "beltStatus",
    ford_message_descriptions::Struct,
    false,
    false
  },
  Structs::Common__BeltStatus__parameters
};
const ford_message_descriptions::StructDescription VehicleInfo__GetVehicleData__response__parameter13 = {
  {
    "bodyInformation",
    ford_message_descriptions::Struct,
    false,
    false
  },
  Structs::Common__BodyInformation__parameters
};
const ford_message_descriptions::StructDescription VehicleInfo__GetVehicleData__response__parameter14 = {
  {
    "deviceStatus",
    ford_message_descriptions::Struct,
    false,
    false
  },
  Structs::Common__DeviceStatus__parameters
};
const ford_message_descriptions::EnumDescription VehicleInfo__GetVehicleData__response__parameter15 = {
  {
    "driverBraking",
    ford_message_descriptions::Enum,
    false,
    false
  },
  Enums::Common__VehicleDataEventStatus__entries
};
const ford_message_descriptions::EnumDescription VehicleInfo__GetVehicleData__response__parameter16 = {
  {
    "wiperStatus",
    ford_message_descriptions::Enum,
    false,
    false
  },
  Enums::Common__WiperStatus__entries
};
const ford_message_descriptions::StructDescription VehicleInfo__GetVehicleData__response__parameter17 = {
  {
    "headLampStatus",
    ford_message_descriptions::Struct,
    false,
    false
  },
  Structs::Common__HeadLampStatus__parameters
};
const ford_message_descriptions::ParameterDescription VehicleInfo__GetVehicleData__response__parameter18 = {
  "engineTorque",
  ford_message_descriptions::Float,
  false,
  false
};
const ford_message_descriptions::ParameterDescription VehicleInfo__GetVehicleData__response__parameter19 = {
  "accPedalPosition",
  ford_message_descriptions::Float,
  false,
  false
};
const ford_message_descriptions::ParameterDescription VehicleInfo__GetVehicleData__response__parameter20 = {
  "steeringWheelAngle",
  ford_message_descriptions::Float,
  false,
  false
};
const ford_message_descriptions::StructDescription VehicleInfo__GetVehicleData__response__parameter21 = {
  {
    "myKey",
    ford_message_descriptions::Struct,
    false,
    false
  },
  Structs::Common__MyKey__parameters
};
const ford_message_descriptions::ParameterDescription* VehicleInfo__GetVehicleData__response__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleData__response__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleData__response__parameter2,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleData__response__parameter3,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleData__response__parameter4,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleData__response__parameter5,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleData__response__parameter6,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleData__response__parameter7,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleData__response__parameter8,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleData__response__parameter9,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleData__response__parameter10,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleData__response__parameter11,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleData__response__parameter12,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleData__response__parameter13,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleData__response__parameter14,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleData__response__parameter15,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleData__response__parameter16,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleData__response__parameter17,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleData__response__parameter18,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleData__response__parameter19,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleData__response__parameter20,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__GetVehicleData__response__parameter21,
  NULL };
const ford_message_descriptions::MessageDescription VehicleInfo__GetVehicleData__response = {
  "VehicleInfo",
  "GetVehicleData",
  hmi_apis::messageType::response,
  hmi_apis::FunctionID::VehicleInfo_GetVehicleData,
  VehicleInfo__GetVehicleData__response__parameters
};

const ford_message_descriptions::StructDescription VehicleInfo__OnVehicleData__notification__parameter1 = {
  {
    "gps",
    ford_message_descriptions::Struct,
    false,
    false
  },
  Structs::Common__GPSData__parameters
};
const ford_message_descriptions::ParameterDescription VehicleInfo__OnVehicleData__notification__parameter2 = {
  "speed",
  ford_message_descriptions::Float,
  false,
  false
};
const ford_message_descriptions::ParameterDescription VehicleInfo__OnVehicleData__notification__parameter3 = {
  "rpm",
  ford_message_descriptions::Integer,
  false,
  false
};
const ford_message_descriptions::ParameterDescription VehicleInfo__OnVehicleData__notification__parameter4 = {
  "fuelLevel",
  ford_message_descriptions::Float,
  false,
  false
};
const ford_message_descriptions::EnumDescription VehicleInfo__OnVehicleData__notification__parameter5 = {
  {
    "fuelLevel_State",
    ford_message_descriptions::Enum,
    false,
    false
  },
  Enums::Common__ComponentVolumeStatus__entries
};
const ford_message_descriptions::ParameterDescription VehicleInfo__OnVehicleData__notification__parameter6 = {
  "instantFuelConsumption",
  ford_message_descriptions::Float,
  false,
  false
};
const ford_message_descriptions::ParameterDescription VehicleInfo__OnVehicleData__notification__parameter7 = {
  "externalTemperature",
  ford_message_descriptions::Float,
  false,
  false
};
const ford_message_descriptions::ParameterDescription VehicleInfo__OnVehicleData__notification__parameter8 = {
  "vin",
  ford_message_descriptions::String,
  false,
  false
};
const ford_message_descriptions::EnumDescription VehicleInfo__OnVehicleData__notification__parameter9 = {
  {
    "prndl",
    ford_message_descriptions::Enum,
    false,
    false
  },
  Enums::Common__PRNDL__entries
};
const ford_message_descriptions::StructDescription VehicleInfo__OnVehicleData__notification__parameter10 = {
  {
    "tirePressure",
    ford_message_descriptions::Struct,
    false,
    false
  },
  Structs::Common__TireStatus__parameters
};
const ford_message_descriptions::ParameterDescription VehicleInfo__OnVehicleData__notification__parameter11 = {
  "odometer",
  ford_message_descriptions::Integer,
  false,
  false
};
const ford_message_descriptions::StructDescription VehicleInfo__OnVehicleData__notification__parameter12 = {
  {
    "beltStatus",
    ford_message_descriptions::Struct,
    false,
    false
  },
  Structs::Common__BeltStatus__parameters
};
const ford_message_descriptions::StructDescription VehicleInfo__OnVehicleData__notification__parameter13 = {
  {
    "bodyInformation",
    ford_message_descriptions::Struct,
    false,
    false
  },
  Structs::Common__BodyInformation__parameters
};
const ford_message_descriptions::StructDescription VehicleInfo__OnVehicleData__notification__parameter14 = {
  {
    "deviceStatus",
    ford_message_descriptions::Struct,
    false,
    false
  },
  Structs::Common__DeviceStatus__parameters
};
const ford_message_descriptions::EnumDescription VehicleInfo__OnVehicleData__notification__parameter15 = {
  {
    "driverBraking",
    ford_message_descriptions::Enum,
    false,
    false
  },
  Enums::Common__VehicleDataEventStatus__entries
};
const ford_message_descriptions::EnumDescription VehicleInfo__OnVehicleData__notification__parameter16 = {
  {
    "wiperStatus",
    ford_message_descriptions::Enum,
    false,
    false
  },
  Enums::Common__WiperStatus__entries
};
const ford_message_descriptions::StructDescription VehicleInfo__OnVehicleData__notification__parameter17 = {
  {
    "headLampStatus",
    ford_message_descriptions::Struct,
    false,
    false
  },
  Structs::Common__HeadLampStatus__parameters
};
const ford_message_descriptions::ParameterDescription VehicleInfo__OnVehicleData__notification__parameter18 = {
  "engineTorque",
  ford_message_descriptions::Float,
  false,
  false
};
const ford_message_descriptions::ParameterDescription VehicleInfo__OnVehicleData__notification__parameter19 = {
  "accPedalPosition",
  ford_message_descriptions::Float,
  false,
  false
};
const ford_message_descriptions::ParameterDescription VehicleInfo__OnVehicleData__notification__parameter20 = {
  "steeringWheelAngle",
  ford_message_descriptions::Float,
  false,
  false
};
const ford_message_descriptions::StructDescription VehicleInfo__OnVehicleData__notification__parameter21 = {
  {
    "myKey",
    ford_message_descriptions::Struct,
    false,
    false
  },
  Structs::Common__MyKey__parameters
};
const ford_message_descriptions::ParameterDescription* VehicleInfo__OnVehicleData__notification__parameters[] = {
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__OnVehicleData__notification__parameter1,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__OnVehicleData__notification__parameter2,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__OnVehicleData__notification__parameter3,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__OnVehicleData__notification__parameter4,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__OnVehicleData__notification__parameter5,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__OnVehicleData__notification__parameter6,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__OnVehicleData__notification__parameter7,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__OnVehicleData__notification__parameter8,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__OnVehicleData__notification__parameter9,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__OnVehicleData__notification__parameter10,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__OnVehicleData__notification__parameter11,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__OnVehicleData__notification__parameter12,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__OnVehicleData__notification__parameter13,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__OnVehicleData__notification__parameter14,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__OnVehicleData__notification__parameter15,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__OnVehicleData__notification__parameter16,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__OnVehicleData__notification__parameter17,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__OnVehicleData__notification__parameter18,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__OnVehicleData__notification__parameter19,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__OnVehicleData__notification__parameter20,
  (const ford_message_descriptions::ParameterDescription*)&VehicleInfo__OnVehicleData__notification__parameter21,
  NULL };
const ford_message_descriptions::MessageDescription VehicleInfo__OnVehicleData__notification = {
  "VehicleInfo",
  "OnVehicleData",
  hmi_apis::messageType::notification,
  hmi_apis::FunctionID::VehicleInfo_OnVehicleData,
  VehicleInfo__OnVehicleData__notification__parameters
};

}

namespace ford_message_descriptions {

const MessageDescription* message_descriptions[] = {
  &Buttons__GetCapabilities__request,
  &Buttons__GetCapabilities__response,
  &Buttons__OnButtonEvent__notification,
  &Buttons__OnButtonPress__notification,
  &BasicCommunication__OnReady__notification,
  &BasicCommunication__OnStartDeviceDiscovery__notification,
  &BasicCommunication__UpdateDeviceList__request,
  &BasicCommunication__UpdateDeviceList__response,
  &BasicCommunication__OnUpdateDeviceList__notification,
  &BasicCommunication__AllowDeviceToConnect__request,
  &BasicCommunication__AllowDeviceToConnect__response,
  &BasicCommunication__OnDeviceChosen__notification,
  &BasicCommunication__OnFindApplications__notification,
  &BasicCommunication__UpdateAppList__request,
  &BasicCommunication__UpdateAppList__response,
  &BasicCommunication__OnAppActivated__notification,
  &BasicCommunication__OnAppDeactivated__notification,
  &BasicCommunication__OnAppRegistered__notification,
  &BasicCommunication__OnAppUnregistered__notification,
  &BasicCommunication__OnExitApplication__notification,
  &BasicCommunication__OnExitAllApplications__notification,
  &BasicCommunication__MixingAudioSupported__request,
  &BasicCommunication__MixingAudioSupported__response,
  &BasicCommunication__PlayTone__notification,
  &BasicCommunication__AllowAllApps__request,
  &BasicCommunication__AllowAllApps__response,
  &BasicCommunication__AllowApp__request,
  &BasicCommunication__AllowApp__response,
  &VR__IsReady__request,
  &VR__IsReady__response,
  &VR__Started__notification,
  &VR__Stopped__notification,
  &VR__AddCommand__request,
  &VR__AddCommand__response,
  &VR__DeleteCommand__request,
  &VR__DeleteCommand__response,
  &VR__OnCommand__notification,
  &VR__ChangeRegistration__request,
  &VR__ChangeRegistration__response,
  &VR__OnLanguageChange__notification,
  &VR__GetSupportedLanguages__request,
  &VR__GetSupportedLanguages__response,
  &VR__GetLanguage__request,
  &VR__GetLanguage__response,
  &TTS__IsReady__request,
  &TTS__IsReady__response,
  &TTS__Speak__request,
  &TTS__Speak__response,
  &TTS__StopSpeaking__request,
  &TTS__StopSpeaking__response,
  &TTS__ChangeRegistration__request,
  &TTS__ChangeRegistration__response,
  &TTS__OnLanguageChange__notification,
  &TTS__GetSupportedLanguages__request,
  &TTS__GetSupportedLanguages__response,
  &TTS__GetLanguage__request,
  &TTS__GetLanguage__response,
  &TTS__SetGlobalProperties__request,
  &TTS__SetGlobalProperties__response,
  &UI__Alert__request,
  &UI__Alert__response,
  &UI__ShowNotification__notification,
  &UI__Show__request,
  &UI__Show__response,
  &UI__AddCommand__request,
  &UI__AddCommand__response,
  &UI__DeleteCommand__request,
  &UI__DeleteCommand__response,
  &UI__AddSubMenu__request,
  &UI__AddSubMenu__response,
  &UI__DeleteSubMenu__request,
  &UI__DeleteSubMenu__response,
  &UI__PerformInteraction__request,
  &UI__PerformInteraction__response,
  &UI__SetMediaClockTimer__request,
  &UI__SetMediaClockTimer__response,
  &UI__SetGlobalProperties__request,
  &UI__SetGlobalProperties__response,
  &UI__ShowVrHelp__request,
  &UI__ShowVrHelp__response,
  &UI__OnCommand__notification,
  &UI__OnSystemContext__notification,
  &UI__GetCapabilities__request,
  &UI__GetCapabilities__response,
  &UI__ChangeRegistration__request,
  &UI__ChangeRegistration__response,
  &UI__OnLanguageChange__notification,
  &UI__GetSupportedLanguages__request,
  &UI__GetSupportedLanguages__response,
  &UI__GetLanguage__request,
  &UI__GetLanguage__response,
  &UI__OnDriverDistraction__notification,
  &UI__SetAppIcon__request,
  &UI__SetAppIcon__response,
  &UI__Slider__request,
  &UI__Slider__response,
  &UI__ScrollableMessage__request,
  &UI__ScrollableMessage__response,
  &UI__PerformAudioPassThru__request,
  &UI__PerformAudioPassThru__response,
  &UI__EndAudioPassThru__request,
  &UI__EndAudioPassThru__response,
  &UI__IsReady__request,
  &UI__IsReady__response,
  &UI__ClosePopUp__request,
  &UI__ClosePopUp__response,
  &Navigation__IsReady__request,
  &Navigation__IsReady__response,
  &Navigation__ShowConstantTBT__request,
  &Navigation__ShowConstantTBT__response,
  &Navigation__AlertManeuver__request,
  &Navigation__AlertManeuver__response,
  &Navigation__UpdateTurnList__request,
  &Navigation__UpdateTurnList__response,
  &Navigation__OnTBTClientState__notification,
  &VehicleInfo__IsReady__request,
  &VehicleInfo__IsReady__response,
  &VehicleInfo__GetVehicleType__request,
  &VehicleInfo__GetVehicleType__response,
  &VehicleInfo__ReadDID__request,
  &VehicleInfo__ReadDID__response,
  &VehicleInfo__GetDTCs__request,
  &VehicleInfo__GetDTCs__response,
  &VehicleInfo__GetVehicleData__request,
  &VehicleInfo__GetVehicleData__response,
  &VehicleInfo__OnVehicleData__notification,
  NULL
};
}

