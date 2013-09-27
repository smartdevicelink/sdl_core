/**
 * \file qml_dbus.h
 * \brief Genereted header that contain interfaces.
 * Copyright (c) 2013, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file qml_dbus.h
 * @brief Generated QDbus adaptors header file
 *
 * This file is a part of HMI D-Bus layer.
 */

#ifndef SRC_COMPONENTS_DBUS_QML_DBUS_H_
#define SRC_COMPONENTS_DBUS_QML_DBUS_H_

#include <QDBusArgument>
#include <QDBusAbstractAdaptor>
#include <QDBusMetaType>
#include <QQuickItem>
#include "qml_dbus_common.h"

struct Common_SingleTireStatus {
  int status;
};
QDBusArgument& operator << (QDBusArgument&, const Common_SingleTireStatus&);
const QDBusArgument& operator >> (const QDBusArgument&, Common_SingleTireStatus&);
void PutArgToMap(QVariantMap& map, const char* name, const Common_SingleTireStatus& v);
QVariant ValueToVariant(Common_SingleTireStatus& v);
QVariant ValueToVariant(QList<Common_SingleTireStatus >& v);
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_SingleTireStatus& v);
bool VariantToValue(const QVariant& variant, Common_SingleTireStatus& v);
bool VariantToValue(const QVariant& variant, QList<Common_SingleTireStatus >& v);
struct Common_TTSChunk {
  QString text;
};
QDBusArgument& operator << (QDBusArgument&, const Common_TTSChunk&);
const QDBusArgument& operator >> (const QDBusArgument&, Common_TTSChunk&);
void PutArgToMap(QVariantMap& map, const char* name, const Common_TTSChunk& v);
QVariant ValueToVariant(Common_TTSChunk& v);
QVariant ValueToVariant(QList<Common_TTSChunk >& v);
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_TTSChunk& v);
bool VariantToValue(const QVariant& variant, Common_TTSChunk& v);
bool VariantToValue(const QVariant& variant, QList<Common_TTSChunk >& v);
struct Common_BodyInformation {
  bool parkBrakeActive;
  int ignitionStableStatus;
  int ignitionStatus;
};
QDBusArgument& operator << (QDBusArgument&, const Common_BodyInformation&);
const QDBusArgument& operator >> (const QDBusArgument&, Common_BodyInformation&);
void PutArgToMap(QVariantMap& map, const char* name, const Common_BodyInformation& v);
QVariant ValueToVariant(Common_BodyInformation& v);
QVariant ValueToVariant(QList<Common_BodyInformation >& v);
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_BodyInformation& v);
bool VariantToValue(const QVariant& variant, Common_BodyInformation& v);
bool VariantToValue(const QVariant& variant, QList<Common_BodyInformation >& v);
struct Common_Image {
  QString value;
  int imageType;
};
QDBusArgument& operator << (QDBusArgument&, const Common_Image&);
const QDBusArgument& operator >> (const QDBusArgument&, Common_Image&);
void PutArgToMap(QVariantMap& map, const char* name, const Common_Image& v);
QVariant ValueToVariant(Common_Image& v);
QVariant ValueToVariant(QList<Common_Image >& v);
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_Image& v);
bool VariantToValue(const QVariant& variant, Common_Image& v);
bool VariantToValue(const QVariant& variant, QList<Common_Image >& v);
struct Common_VrHelpItem {
  QString text;
  OptionalArgument<Common_Image>  image;
  int position;
};
QDBusArgument& operator << (QDBusArgument&, const Common_VrHelpItem&);
const QDBusArgument& operator >> (const QDBusArgument&, Common_VrHelpItem&);
void PutArgToMap(QVariantMap& map, const char* name, const Common_VrHelpItem& v);
QVariant ValueToVariant(Common_VrHelpItem& v);
QVariant ValueToVariant(QList<Common_VrHelpItem >& v);
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_VrHelpItem& v);
bool VariantToValue(const QVariant& variant, Common_VrHelpItem& v);
bool VariantToValue(const QVariant& variant, QList<Common_VrHelpItem >& v);
struct Common_HeadLampStatus {
  bool lowBeamsOn;
  bool highBeamsOn;
};
QDBusArgument& operator << (QDBusArgument&, const Common_HeadLampStatus&);
const QDBusArgument& operator >> (const QDBusArgument&, Common_HeadLampStatus&);
void PutArgToMap(QVariantMap& map, const char* name, const Common_HeadLampStatus& v);
QVariant ValueToVariant(Common_HeadLampStatus& v);
QVariant ValueToVariant(QList<Common_HeadLampStatus >& v);
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_HeadLampStatus& v);
bool VariantToValue(const QVariant& variant, Common_HeadLampStatus& v);
bool VariantToValue(const QVariant& variant, QList<Common_HeadLampStatus >& v);
struct Common_TimeFormat {
  int hours;
  int minutes;
  int seconds;
};
QDBusArgument& operator << (QDBusArgument&, const Common_TimeFormat&);
const QDBusArgument& operator >> (const QDBusArgument&, Common_TimeFormat&);
void PutArgToMap(QVariantMap& map, const char* name, const Common_TimeFormat& v);
QVariant ValueToVariant(Common_TimeFormat& v);
QVariant ValueToVariant(QList<Common_TimeFormat >& v);
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_TimeFormat& v);
bool VariantToValue(const QVariant& variant, Common_TimeFormat& v);
bool VariantToValue(const QVariant& variant, QList<Common_TimeFormat >& v);
struct Common_GPSData {
  double longitudeDegrees;
  double latitudeDegrees;
  int utcYear;
  int utcMonth;
  int utcDay;
  int utcHours;
  int utcMinutes;
  int utcSeconds;
  int compassDirection;
  double pdop;
  double hdop;
  double vdop;
  bool actual;
  int satellites;
  int dimension;
  double altitude;
  double heading;
  double speed;
};
QDBusArgument& operator << (QDBusArgument&, const Common_GPSData&);
const QDBusArgument& operator >> (const QDBusArgument&, Common_GPSData&);
void PutArgToMap(QVariantMap& map, const char* name, const Common_GPSData& v);
QVariant ValueToVariant(Common_GPSData& v);
QVariant ValueToVariant(QList<Common_GPSData >& v);
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_GPSData& v);
bool VariantToValue(const QVariant& variant, Common_GPSData& v);
bool VariantToValue(const QVariant& variant, QList<Common_GPSData >& v);
struct Common_SoftButton {
  int type;
  OptionalArgument<QString>  text;
  OptionalArgument<Common_Image>  image;
  OptionalArgument<bool>  isHighlighted;
  int softButtonID;
  OptionalArgument<int>  systemAction;
};
QDBusArgument& operator << (QDBusArgument&, const Common_SoftButton&);
const QDBusArgument& operator >> (const QDBusArgument&, Common_SoftButton&);
void PutArgToMap(QVariantMap& map, const char* name, const Common_SoftButton& v);
QVariant ValueToVariant(Common_SoftButton& v);
QVariant ValueToVariant(QList<Common_SoftButton >& v);
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_SoftButton& v);
bool VariantToValue(const QVariant& variant, Common_SoftButton& v);
bool VariantToValue(const QVariant& variant, QList<Common_SoftButton >& v);
struct Common_DIDResult {
  int resultCode;
  int didLocation;
  OptionalArgument<QString>  data;
};
QDBusArgument& operator << (QDBusArgument&, const Common_DIDResult&);
const QDBusArgument& operator >> (const QDBusArgument&, Common_DIDResult&);
void PutArgToMap(QVariantMap& map, const char* name, const Common_DIDResult& v);
QVariant ValueToVariant(Common_DIDResult& v);
QVariant ValueToVariant(QList<Common_DIDResult >& v);
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_DIDResult& v);
bool VariantToValue(const QVariant& variant, Common_DIDResult& v);
bool VariantToValue(const QVariant& variant, QList<Common_DIDResult >& v);
struct Common_PresetBankCapabilities {
  bool onScreenPresetsAvailable;
};
QDBusArgument& operator << (QDBusArgument&, const Common_PresetBankCapabilities&);
const QDBusArgument& operator >> (const QDBusArgument&, Common_PresetBankCapabilities&);
void PutArgToMap(QVariantMap& map, const char* name, const Common_PresetBankCapabilities& v);
QVariant ValueToVariant(Common_PresetBankCapabilities& v);
QVariant ValueToVariant(QList<Common_PresetBankCapabilities >& v);
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_PresetBankCapabilities& v);
bool VariantToValue(const QVariant& variant, Common_PresetBankCapabilities& v);
bool VariantToValue(const QVariant& variant, QList<Common_PresetBankCapabilities >& v);
struct Common_HMIApplication {
  QString appName;
  OptionalArgument<QString>  ngnMediaScreenAppName;
  OptionalArgument<QString>  icon;
  QString deviceName;
  int appID;
  int hmiDisplayLanguageDesired;
  bool isMediaApplication;
  OptionalArgument<QList<int> >  appType;
};
QDBusArgument& operator << (QDBusArgument&, const Common_HMIApplication&);
const QDBusArgument& operator >> (const QDBusArgument&, Common_HMIApplication&);
void PutArgToMap(QVariantMap& map, const char* name, const Common_HMIApplication& v);
QVariant ValueToVariant(Common_HMIApplication& v);
QVariant ValueToVariant(QList<Common_HMIApplication >& v);
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_HMIApplication& v);
bool VariantToValue(const QVariant& variant, Common_HMIApplication& v);
bool VariantToValue(const QVariant& variant, QList<Common_HMIApplication >& v);
struct Common_Choice {
  int choiceID;
  OptionalArgument<QString>  menuName;
  OptionalArgument<Common_Image>  image;
};
QDBusArgument& operator << (QDBusArgument&, const Common_Choice&);
const QDBusArgument& operator >> (const QDBusArgument&, Common_Choice&);
void PutArgToMap(QVariantMap& map, const char* name, const Common_Choice& v);
QVariant ValueToVariant(Common_Choice& v);
QVariant ValueToVariant(QList<Common_Choice >& v);
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_Choice& v);
bool VariantToValue(const QVariant& variant, Common_Choice& v);
bool VariantToValue(const QVariant& variant, QList<Common_Choice >& v);
struct Common_TireStatus {
  int pressureTelltale;
  Common_SingleTireStatus leftFront;
  Common_SingleTireStatus rightFront;
  Common_SingleTireStatus leftRear;
  Common_SingleTireStatus rightRear;
  Common_SingleTireStatus innerLeftRear;
  Common_SingleTireStatus innerRightRear;
};
QDBusArgument& operator << (QDBusArgument&, const Common_TireStatus&);
const QDBusArgument& operator >> (const QDBusArgument&, Common_TireStatus&);
void PutArgToMap(QVariantMap& map, const char* name, const Common_TireStatus& v);
QVariant ValueToVariant(Common_TireStatus& v);
QVariant ValueToVariant(QList<Common_TireStatus >& v);
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_TireStatus& v);
bool VariantToValue(const QVariant& variant, Common_TireStatus& v);
bool VariantToValue(const QVariant& variant, QList<Common_TireStatus >& v);
struct Common_ButtonCapabilities {
  int name;
  bool shortPressAvailable;
  bool longPressAvailable;
  bool upDownAvailable;
};
QDBusArgument& operator << (QDBusArgument&, const Common_ButtonCapabilities&);
const QDBusArgument& operator >> (const QDBusArgument&, Common_ButtonCapabilities&);
void PutArgToMap(QVariantMap& map, const char* name, const Common_ButtonCapabilities& v);
QVariant ValueToVariant(Common_ButtonCapabilities& v);
QVariant ValueToVariant(QList<Common_ButtonCapabilities >& v);
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_ButtonCapabilities& v);
bool VariantToValue(const QVariant& variant, Common_ButtonCapabilities& v);
bool VariantToValue(const QVariant& variant, QList<Common_ButtonCapabilities >& v);
struct Common_BeltStatus {
  OptionalArgument<int>  driverBeltDeployed;
  OptionalArgument<int>  passengerBeltDeployed;
  OptionalArgument<int>  passengerBuckleBelted;
  OptionalArgument<int>  driverBuckleBelted;
  OptionalArgument<int>  leftRow2BuckleBelted;
  OptionalArgument<int>  passengerChildDetected;
  OptionalArgument<int>  rightRow2BuckleBelted;
  OptionalArgument<int>  middleRow2BuckleBelted;
  OptionalArgument<int>  middleRow3BuckleBelted;
  OptionalArgument<int>  leftRow3BuckleBelted;
  OptionalArgument<int>  rightRow3BuckleBelted;
  OptionalArgument<int>  leftRearInflatableBelted;
  OptionalArgument<int>  rightRearInflatableBelted;
  OptionalArgument<int>  middleRow1BeltDeployed;
  OptionalArgument<int>  middleRow1BuckleBelted;
};
QDBusArgument& operator << (QDBusArgument&, const Common_BeltStatus&);
const QDBusArgument& operator >> (const QDBusArgument&, Common_BeltStatus&);
void PutArgToMap(QVariantMap& map, const char* name, const Common_BeltStatus& v);
QVariant ValueToVariant(Common_BeltStatus& v);
QVariant ValueToVariant(QList<Common_BeltStatus >& v);
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_BeltStatus& v);
bool VariantToValue(const QVariant& variant, Common_BeltStatus& v);
bool VariantToValue(const QVariant& variant, QList<Common_BeltStatus >& v);
struct Common_DeviceInfo {
  QString name;
  int id;
};
QDBusArgument& operator << (QDBusArgument&, const Common_DeviceInfo&);
const QDBusArgument& operator >> (const QDBusArgument&, Common_DeviceInfo&);
void PutArgToMap(QVariantMap& map, const char* name, const Common_DeviceInfo& v);
QVariant ValueToVariant(Common_DeviceInfo& v);
QVariant ValueToVariant(QList<Common_DeviceInfo >& v);
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_DeviceInfo& v);
bool VariantToValue(const QVariant& variant, Common_DeviceInfo& v);
bool VariantToValue(const QVariant& variant, QList<Common_DeviceInfo >& v);
struct Common_MenuParams {
  OptionalArgument<int>  parentID;
  OptionalArgument<int>  position;
  QString menuName;
};
QDBusArgument& operator << (QDBusArgument&, const Common_MenuParams&);
const QDBusArgument& operator >> (const QDBusArgument&, Common_MenuParams&);
void PutArgToMap(QVariantMap& map, const char* name, const Common_MenuParams& v);
QVariant ValueToVariant(Common_MenuParams& v);
QVariant ValueToVariant(QList<Common_MenuParams >& v);
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_MenuParams& v);
bool VariantToValue(const QVariant& variant, Common_MenuParams& v);
bool VariantToValue(const QVariant& variant, QList<Common_MenuParams >& v);
struct Common_SoftButtonCapabilities {
  bool shortPressAvailable;
  bool longPressAvailable;
  bool upDownAvailable;
  bool imageSupported;
};
QDBusArgument& operator << (QDBusArgument&, const Common_SoftButtonCapabilities&);
const QDBusArgument& operator >> (const QDBusArgument&, Common_SoftButtonCapabilities&);
void PutArgToMap(QVariantMap& map, const char* name, const Common_SoftButtonCapabilities& v);
QVariant ValueToVariant(Common_SoftButtonCapabilities& v);
QVariant ValueToVariant(QList<Common_SoftButtonCapabilities >& v);
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_SoftButtonCapabilities& v);
bool VariantToValue(const QVariant& variant, Common_SoftButtonCapabilities& v);
bool VariantToValue(const QVariant& variant, QList<Common_SoftButtonCapabilities >& v);
struct Common_DisplayCapabilities {
  int displayType;
  QList<int>  textFields;
  QList<int>  mediaClockFormats;
  QList<int>  imageCapabilities;
};
QDBusArgument& operator << (QDBusArgument&, const Common_DisplayCapabilities&);
const QDBusArgument& operator >> (const QDBusArgument&, Common_DisplayCapabilities&);
void PutArgToMap(QVariantMap& map, const char* name, const Common_DisplayCapabilities& v);
QVariant ValueToVariant(Common_DisplayCapabilities& v);
QVariant ValueToVariant(QList<Common_DisplayCapabilities >& v);
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_DisplayCapabilities& v);
bool VariantToValue(const QVariant& variant, Common_DisplayCapabilities& v);
bool VariantToValue(const QVariant& variant, QList<Common_DisplayCapabilities >& v);
struct Common_VehicleType {
  OptionalArgument<QString>  make;
  OptionalArgument<QString>  model;
  OptionalArgument<QString>  modelYear;
  OptionalArgument<QString>  trim;
};
QDBusArgument& operator << (QDBusArgument&, const Common_VehicleType&);
const QDBusArgument& operator >> (const QDBusArgument&, Common_VehicleType&);
void PutArgToMap(QVariantMap& map, const char* name, const Common_VehicleType& v);
QVariant ValueToVariant(Common_VehicleType& v);
QVariant ValueToVariant(QList<Common_VehicleType >& v);
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_VehicleType& v);
bool VariantToValue(const QVariant& variant, Common_VehicleType& v);
bool VariantToValue(const QVariant& variant, QList<Common_VehicleType >& v);
struct Common_MyKey {
  int e911Override;
};
QDBusArgument& operator << (QDBusArgument&, const Common_MyKey&);
const QDBusArgument& operator >> (const QDBusArgument&, Common_MyKey&);
void PutArgToMap(QVariantMap& map, const char* name, const Common_MyKey& v);
QVariant ValueToVariant(Common_MyKey& v);
QVariant ValueToVariant(QList<Common_MyKey >& v);
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_MyKey& v);
bool VariantToValue(const QVariant& variant, Common_MyKey& v);
bool VariantToValue(const QVariant& variant, QList<Common_MyKey >& v);
struct Common_DeviceStatus {
  OptionalArgument<bool>  voiceRecOn;
  OptionalArgument<bool>  btIconOn;
  OptionalArgument<bool>  callActive;
  OptionalArgument<bool>  phoneRoaming;
  OptionalArgument<bool>  textMsgAvailable;
  OptionalArgument<int>  battLevelStatus;
  OptionalArgument<bool>  stereoAudioOutputMuted;
  OptionalArgument<bool>  monoAudioOutputMuted;
  OptionalArgument<int>  signalLevelStatus;
  OptionalArgument<int>  primaryAudioSource;
  OptionalArgument<bool>  eCallEventActive;
};
QDBusArgument& operator << (QDBusArgument&, const Common_DeviceStatus&);
const QDBusArgument& operator >> (const QDBusArgument&, Common_DeviceStatus&);
void PutArgToMap(QVariantMap& map, const char* name, const Common_DeviceStatus& v);
QVariant ValueToVariant(Common_DeviceStatus& v);
QVariant ValueToVariant(QList<Common_DeviceStatus >& v);
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_DeviceStatus& v);
bool VariantToValue(const QVariant& variant, Common_DeviceStatus& v);
bool VariantToValue(const QVariant& variant, QList<Common_DeviceStatus >& v);
struct Common_TextFieldStruct {
  int fieldName;
  QString fieldText;
};
QDBusArgument& operator << (QDBusArgument&, const Common_TextFieldStruct&);
const QDBusArgument& operator >> (const QDBusArgument&, Common_TextFieldStruct&);
void PutArgToMap(QVariantMap& map, const char* name, const Common_TextFieldStruct& v);
QVariant ValueToVariant(Common_TextFieldStruct& v);
QVariant ValueToVariant(QList<Common_TextFieldStruct >& v);
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_TextFieldStruct& v);
bool VariantToValue(const QVariant& variant, Common_TextFieldStruct& v);
bool VariantToValue(const QVariant& variant, QList<Common_TextFieldStruct >& v);
struct Common_Turn {
  OptionalArgument<Common_TextFieldStruct>  navigationText;
  OptionalArgument<Common_Image>  turnIcon;
};
QDBusArgument& operator << (QDBusArgument&, const Common_Turn&);
const QDBusArgument& operator >> (const QDBusArgument&, Common_Turn&);
void PutArgToMap(QVariantMap& map, const char* name, const Common_Turn& v);
QVariant ValueToVariant(Common_Turn& v);
QVariant ValueToVariant(QList<Common_Turn >& v);
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_Turn& v);
bool VariantToValue(const QVariant& variant, Common_Turn& v);
bool VariantToValue(const QVariant& variant, QList<Common_Turn >& v);
class ButtonsAdaptor : public QDBusAbstractAdaptor {
  Q_OBJECT
  Q_CLASSINFO("D-Bus Interface", "com.ford.sdl.hmi.Buttons")
  Q_CLASSINFO("D-Bus Introspection",
"<interface name=\"com.ford.sdl.hmi.Buttons\">"
"<method name=\"GetCapabilities\">"
"<arg direction=\"out\" name=\"capabilities\" type=\"a(ibbb)\" />"
"<arg direction=\"out\" name=\"presetBankCapabilities\" type=\"(b(b))\" />"
"</method>"
"<signal name=\"OnButtonEvent\">"
"<arg name=\"name\" type=\"i\" />"
"<arg name=\"mode\" type=\"i\" />"
"<arg name=\"customButtonID\" type=\"(bi)\" />"
"</signal>"
"<signal name=\"OnButtonPress\">"
"<arg name=\"name\" type=\"i\" />"
"<arg name=\"mode\" type=\"i\" />"
"<arg name=\"customButtonID\" type=\"(bi)\" />"
"</signal>"
"</interface>"
  )
 public:
  explicit ButtonsAdaptor(QObject *parent = 0);
  void SetApi(QQuickItem*);
 public slots:
  QList<Common_ButtonCapabilities>  GetCapabilities(OptionalArgument<Common_PresetBankCapabilities> & presetBankCapabilities_out);
 signals:
  void OnButtonEvent(int name, int mode, OptionalArgument<int>  customButtonID);
  void OnButtonPress(int name, int mode, OptionalArgument<int>  customButtonID);
 private slots:
  void OnButtonEvent_qml(int name, int mode, QVariant customButtonID);
  void OnButtonPress_qml(int name, int mode, QVariant customButtonID);
 private:
  QQuickItem* api_;
};

class BasicCommunicationAdaptor : public QDBusAbstractAdaptor {
  Q_OBJECT
  Q_CLASSINFO("D-Bus Interface", "com.ford.sdl.hmi.BasicCommunication")
  Q_CLASSINFO("D-Bus Introspection",
"<interface name=\"com.ford.sdl.hmi.BasicCommunication\">"
"<method name=\"UpdateDeviceList\">"
"<arg direction=\"in\" name=\"deviceList\" type=\"a(si)\" />"
"</method>"
"<method name=\"AllowDeviceToConnect\">"
"<arg direction=\"in\" name=\"device\" type=\"(si)\" />"
"<arg direction=\"out\" name=\"allow\" type=\"b\" />"
"</method>"
"<method name=\"UpdateAppList\">"
"<arg direction=\"in\" name=\"applications\" type=\"a(s(bs)(bs)siib(bai))\" />"
"</method>"
"<method name=\"ActivateApp\">"
"<arg direction=\"in\" name=\"appID\" type=\"i\" />"
"</method>"
"<method name=\"MixingAudioSupported\">"
"<arg direction=\"out\" name=\"attenuatedSupported\" type=\"b\" />"
"</method>"
"<method name=\"AllowAllApps\">"
"<arg direction=\"out\" name=\"allowed\" type=\"b\" />"
"</method>"
"<method name=\"AllowApp\">"
"<arg direction=\"in\" name=\"app\" type=\"(s(bs)(bs)siib(bai))\" />"
"<arg direction=\"in\" name=\"appPermissions\" type=\"(bas)\" />"
"<arg direction=\"out\" name=\"allowed\" type=\"b\" />"
"</method>"
"<signal name=\"OnReady\" />"
"<signal name=\"OnStartDeviceDiscovery\" />"
"<signal name=\"OnUpdateDeviceList\" />"
"<signal name=\"OnDeviceChosen\">"
"<arg name=\"deviceInfo\" type=\"(si)\" />"
"</signal>"
"<signal name=\"OnFindApplications\">"
"<arg name=\"deviceInfo\" type=\"(si)\" />"
"</signal>"
"<signal name=\"OnAppActivated\">"
"<arg name=\"appID\" type=\"i\" />"
"</signal>"
"<signal name=\"OnAppDeactivated\">"
"<arg name=\"appID\" type=\"i\" />"
"<arg name=\"reason\" type=\"i\" />"
"</signal>"
"<signal name=\"OnAppRegistered\">"
"<arg name=\"application\" type=\"(s(bs)(bs)siib(bai))\" />"
"</signal>"
"<signal name=\"OnAppUnregistered\">"
"<arg name=\"appID\" type=\"i\" />"
"</signal>"
"<signal name=\"OnExitApplication\">"
"<arg name=\"appID\" type=\"i\" />"
"</signal>"
"<signal name=\"OnExitAllApplications\">"
"<arg name=\"reason\" type=\"i\" />"
"</signal>"
"<signal name=\"PlayTone\" />"
"</interface>"
  )
 public:
  explicit BasicCommunicationAdaptor(QObject *parent = 0);
  void SetApi(QQuickItem*);
 public slots:
  void UpdateDeviceList(QList<Common_DeviceInfo>  deviceList_in);
  bool AllowDeviceToConnect(Common_DeviceInfo device_in);
  void UpdateAppList(QList<Common_HMIApplication>  applications_in);
  void ActivateApp(int appID_in);
  bool MixingAudioSupported();
  bool AllowAllApps();
  bool AllowApp(Common_HMIApplication app_in, OptionalArgument<QList<QString> >  appPermissions_in);
 signals:
  void OnReady();
  void OnStartDeviceDiscovery();
  void OnUpdateDeviceList();
  void OnDeviceChosen(Common_DeviceInfo deviceInfo);
  void OnFindApplications(Common_DeviceInfo deviceInfo);
  void OnAppActivated(int appID);
  void OnAppDeactivated(int appID, int reason);
  void OnAppRegistered(Common_HMIApplication application);
  void OnAppUnregistered(int appID);
  void OnExitApplication(int appID);
  void OnExitAllApplications(int reason);
  void PlayTone();
 private slots:
  void OnReady_qml();
  void OnStartDeviceDiscovery_qml();
  void OnUpdateDeviceList_qml();
  void OnDeviceChosen_qml(QVariant deviceInfo);
  void OnFindApplications_qml(QVariant deviceInfo);
  void OnAppActivated_qml(int appID);
  void OnAppDeactivated_qml(int appID, int reason);
  void OnAppRegistered_qml(QVariant application);
  void OnAppUnregistered_qml(int appID);
  void OnExitApplication_qml(int appID);
  void OnExitAllApplications_qml(int reason);
  void PlayTone_qml();
 private:
  QQuickItem* api_;
};

class VRAdaptor : public QDBusAbstractAdaptor {
  Q_OBJECT
  Q_CLASSINFO("D-Bus Interface", "com.ford.sdl.hmi.VR")
  Q_CLASSINFO("D-Bus Introspection",
"<interface name=\"com.ford.sdl.hmi.VR\">"
"<method name=\"IsReady\">"
"<arg direction=\"out\" name=\"available\" type=\"b\" />"
"</method>"
"<method name=\"AddCommand\">"
"<arg direction=\"in\" name=\"cmdID\" type=\"i\" />"
"<arg direction=\"in\" name=\"vrCommands\" type=\"as\" />"
"<arg direction=\"in\" name=\"appID\" type=\"(bi)\" />"
"</method>"
"<method name=\"DeleteCommand\">"
"<arg direction=\"in\" name=\"cmdID\" type=\"i\" />"
"<arg direction=\"in\" name=\"appID\" type=\"i\" />"
"</method>"
"<method name=\"ChangeRegistration\">"
"<arg direction=\"in\" name=\"language\" type=\"i\" />"
"<arg direction=\"in\" name=\"appID\" type=\"i\" />"
"</method>"
"<method name=\"GetSupportedLanguages\">"
"<arg direction=\"out\" name=\"languages\" type=\"ai\" />"
"</method>"
"<method name=\"GetLanguage\">"
"<arg direction=\"out\" name=\"language\" type=\"i\" />"
"</method>"
"<signal name=\"Started\" />"
"<signal name=\"Stopped\" />"
"<signal name=\"OnCommand\">"
"<arg name=\"cmdID\" type=\"i\" />"
"<arg name=\"appID\" type=\"i\" />"
"</signal>"
"<signal name=\"OnLanguageChange\">"
"<arg name=\"language\" type=\"i\" />"
"</signal>"
"</interface>"
  )
 public:
  explicit VRAdaptor(QObject *parent = 0);
  void SetApi(QQuickItem*);
 public slots:
  bool IsReady();
  void AddCommand(int cmdID_in, QList<QString>  vrCommands_in, OptionalArgument<int>  appID_in);
  void DeleteCommand(int cmdID_in, int appID_in);
  void ChangeRegistration(int language_in, int appID_in);
  QList<int>  GetSupportedLanguages();
  int GetLanguage();
 signals:
  void Started();
  void Stopped();
  void OnCommand(int cmdID, int appID);
  void OnLanguageChange(int language);
 private slots:
  void Started_qml();
  void Stopped_qml();
  void OnCommand_qml(int cmdID, int appID);
  void OnLanguageChange_qml(int language);
 private:
  QQuickItem* api_;
};

class TTSAdaptor : public QDBusAbstractAdaptor {
  Q_OBJECT
  Q_CLASSINFO("D-Bus Interface", "com.ford.sdl.hmi.TTS")
  Q_CLASSINFO("D-Bus Introspection",
"<interface name=\"com.ford.sdl.hmi.TTS\">"
"<method name=\"IsReady\">"
"<arg direction=\"out\" name=\"available\" type=\"b\" />"
"</method>"
"<method name=\"Speak\">"
"<arg direction=\"in\" name=\"ttsChunks\" type=\"a(s)\" />"
"<arg direction=\"in\" name=\"appID\" type=\"i\" />"
"</method>"
"<method name=\"StopSpeaking\" />"
"<method name=\"ChangeRegistration\">"
"<arg direction=\"in\" name=\"language\" type=\"i\" />"
"<arg direction=\"in\" name=\"appID\" type=\"i\" />"
"</method>"
"<method name=\"GetSupportedLanguages\">"
"<arg direction=\"out\" name=\"languages\" type=\"ai\" />"
"</method>"
"<method name=\"GetLanguage\">"
"<arg direction=\"out\" name=\"language\" type=\"i\" />"
"</method>"
"<method name=\"SetGlobalProperties\">"
"<arg direction=\"in\" name=\"helpPrompt\" type=\"(ba(s))\" />"
"<arg direction=\"in\" name=\"timeoutPrompt\" type=\"(ba(s))\" />"
"<arg direction=\"in\" name=\"appID\" type=\"i\" />"
"</method>"
"<signal name=\"OnLanguageChange\">"
"<arg name=\"language\" type=\"i\" />"
"</signal>"
"</interface>"
  )
 public:
  explicit TTSAdaptor(QObject *parent = 0);
  void SetApi(QQuickItem*);
 public slots:
  bool IsReady();
  void Speak(QList<Common_TTSChunk>  ttsChunks_in, int appID_in);
  void StopSpeaking();
  void ChangeRegistration(int language_in, int appID_in);
  QList<int>  GetSupportedLanguages();
  int GetLanguage();
  void SetGlobalProperties(OptionalArgument<QList<Common_TTSChunk> >  helpPrompt_in, OptionalArgument<QList<Common_TTSChunk> >  timeoutPrompt_in, int appID_in);
 signals:
  void OnLanguageChange(int language);
 private slots:
  void OnLanguageChange_qml(int language);
 private:
  QQuickItem* api_;
};

class UIAdaptor : public QDBusAbstractAdaptor {
  Q_OBJECT
  Q_CLASSINFO("D-Bus Interface", "com.ford.sdl.hmi.UI")
  Q_CLASSINFO("D-Bus Introspection",
"<interface name=\"com.ford.sdl.hmi.UI\">"
"<method name=\"Alert\">"
"<arg direction=\"in\" name=\"alertStrings\" type=\"a(is)\" />"
"<arg direction=\"in\" name=\"duration\" type=\"i\" />"
"<arg direction=\"in\" name=\"softButtons\" type=\"(ba(i(bs)(b(si))(bb)i(bi)))\" />"
"<arg direction=\"in\" name=\"appID\" type=\"i\" />"
"<arg direction=\"out\" name=\"tryAgainTime\" type=\"(bi)\" />"
"</method>"
"<method name=\"Show\">"
"<arg direction=\"in\" name=\"showStrings\" type=\"a(is)\" />"
"<arg direction=\"in\" name=\"alignment\" type=\"(bi)\" />"
"<arg direction=\"in\" name=\"graphic\" type=\"(b(si))\" />"
"<arg direction=\"in\" name=\"softButtons\" type=\"(ba(i(bs)(b(si))(bb)i(bi)))\" />"
"<arg direction=\"in\" name=\"customPresets\" type=\"(bas)\" />"
"<arg direction=\"in\" name=\"appID\" type=\"i\" />"
"</method>"
"<method name=\"AddCommand\">"
"<arg direction=\"in\" name=\"cmdID\" type=\"i\" />"
"<arg direction=\"in\" name=\"menuParams\" type=\"(b((bi)(bi)s))\" />"
"<arg direction=\"in\" name=\"cmdIcon\" type=\"(b(si))\" />"
"<arg direction=\"in\" name=\"appID\" type=\"i\" />"
"</method>"
"<method name=\"DeleteCommand\">"
"<arg direction=\"in\" name=\"cmdID\" type=\"i\" />"
"<arg direction=\"in\" name=\"appID\" type=\"i\" />"
"</method>"
"<method name=\"AddSubMenu\">"
"<arg direction=\"in\" name=\"menuID\" type=\"i\" />"
"<arg direction=\"in\" name=\"menuParams\" type=\"((bi)(bi)s)\" />"
"<arg direction=\"in\" name=\"appID\" type=\"i\" />"
"</method>"
"<method name=\"DeleteSubMenu\">"
"<arg direction=\"in\" name=\"menuID\" type=\"i\" />"
"<arg direction=\"in\" name=\"appID\" type=\"i\" />"
"</method>"
"<method name=\"PerformInteraction\">"
"<arg direction=\"in\" name=\"initialText\" type=\"(is)\" />"
"<arg direction=\"in\" name=\"choiceSet\" type=\"a(i(bs)(b(si)))\" />"
"<arg direction=\"in\" name=\"vrHelp\" type=\"(ba(s(b(si))i))\" />"
"<arg direction=\"in\" name=\"timeout\" type=\"i\" />"
"<arg direction=\"in\" name=\"appID\" type=\"i\" />"
"<arg direction=\"out\" name=\"choiceID\" type=\"(bi)\" />"
"</method>"
"<method name=\"SetMediaClockTimer\">"
"<arg direction=\"in\" name=\"startTime\" type=\"(b(iii))\" />"
"<arg direction=\"in\" name=\"updateMode\" type=\"i\" />"
"<arg direction=\"in\" name=\"appID\" type=\"i\" />"
"</method>"
"<method name=\"SetGlobalProperties\">"
"<arg direction=\"in\" name=\"vrHelpTitle\" type=\"(bs)\" />"
"<arg direction=\"in\" name=\"vrHelp\" type=\"(ba(s(b(si))i))\" />"
"<arg direction=\"in\" name=\"appID\" type=\"i\" />"
"</method>"
"<method name=\"ShowVrHelp\">"
"<arg direction=\"in\" name=\"vrHelpTitle\" type=\"(bs)\" />"
"<arg direction=\"in\" name=\"vrHelp\" type=\"(ba(s(b(si))i))\" />"
"<arg direction=\"in\" name=\"appID\" type=\"(bi)\" />"
"</method>"
"<method name=\"GetCapabilities\">"
"<arg direction=\"out\" name=\"displayCapabilities\" type=\"(iaiaiai)\" />"
"<arg direction=\"out\" name=\"hmiZoneCapabilities\" type=\"i\" />"
"<arg direction=\"out\" name=\"softButtonCapabilities\" type=\"(b(bbbb))\" />"
"</method>"
"<method name=\"ChangeRegistration\">"
"<arg direction=\"in\" name=\"language\" type=\"i\" />"
"<arg direction=\"in\" name=\"appID\" type=\"i\" />"
"</method>"
"<method name=\"GetSupportedLanguages\">"
"<arg direction=\"out\" name=\"languages\" type=\"ai\" />"
"</method>"
"<method name=\"GetLanguage\">"
"<arg direction=\"out\" name=\"language\" type=\"i\" />"
"</method>"
"<method name=\"SetAppIcon\">"
"<arg direction=\"in\" name=\"syncFileName\" type=\"(si)\" />"
"<arg direction=\"in\" name=\"appID\" type=\"i\" />"
"</method>"
"<method name=\"Slider\">"
"<arg direction=\"in\" name=\"numTicks\" type=\"i\" />"
"<arg direction=\"in\" name=\"position\" type=\"i\" />"
"<arg direction=\"in\" name=\"sliderHeader\" type=\"s\" />"
"<arg direction=\"in\" name=\"sliderFooter\" type=\"(bas)\" />"
"<arg direction=\"in\" name=\"timeout\" type=\"i\" />"
"<arg direction=\"in\" name=\"appID\" type=\"i\" />"
"<arg direction=\"out\" name=\"sliderPosition\" type=\"i\" />"
"</method>"
"<method name=\"ScrollableMessage\">"
"<arg direction=\"in\" name=\"messageText\" type=\"(is)\" />"
"<arg direction=\"in\" name=\"timeout\" type=\"i\" />"
"<arg direction=\"in\" name=\"softButtons\" type=\"(ba(i(bs)(b(si))(bb)i(bi)))\" />"
"<arg direction=\"in\" name=\"appID\" type=\"i\" />"
"</method>"
"<method name=\"PerformAudioPassThru\">"
"<arg direction=\"in\" name=\"audioPassThruDisplayTexts\" type=\"a(is)\" />"
"<arg direction=\"in\" name=\"maxDuration\" type=\"(bi)\" />"
"</method>"
"<method name=\"EndAudioPassThru\" />"
"<method name=\"IsReady\">"
"<arg direction=\"out\" name=\"available\" type=\"b\" />"
"</method>"
"<method name=\"ClosePopUp\" />"
"<signal name=\"ShowNotification\">"
"<arg name=\"text\" type=\"(is)\" />"
"<arg name=\"icon\" type=\"(b(si))\" />"
"<arg name=\"timeout\" type=\"i\" />"
"</signal>"
"<signal name=\"OnCommand\">"
"<arg name=\"cmdID\" type=\"i\" />"
"<arg name=\"appID\" type=\"i\" />"
"</signal>"
"<signal name=\"OnSystemContext\">"
"<arg name=\"systemContext\" type=\"i\" />"
"</signal>"
"<signal name=\"OnLanguageChange\">"
"<arg name=\"language\" type=\"i\" />"
"</signal>"
"<signal name=\"OnDriverDistraction\">"
"<arg name=\"state\" type=\"i\" />"
"</signal>"
"</interface>"
  )
 public:
  explicit UIAdaptor(QObject *parent = 0);
  void SetApi(QQuickItem*);
 public slots:
  OptionalArgument<int>  Alert(QList<Common_TextFieldStruct>  alertStrings_in, int duration_in, OptionalArgument<QList<Common_SoftButton> >  softButtons_in, int appID_in);
  void Show(QList<Common_TextFieldStruct>  showStrings_in, OptionalArgument<int>  alignment_in, OptionalArgument<Common_Image>  graphic_in, OptionalArgument<QList<Common_SoftButton> >  softButtons_in, OptionalArgument<QList<QString> >  customPresets_in, int appID_in);
  void AddCommand(int cmdID_in, OptionalArgument<Common_MenuParams>  menuParams_in, OptionalArgument<Common_Image>  cmdIcon_in, int appID_in);
  void DeleteCommand(int cmdID_in, int appID_in);
  void AddSubMenu(int menuID_in, Common_MenuParams menuParams_in, int appID_in);
  void DeleteSubMenu(int menuID_in, int appID_in);
  OptionalArgument<int>  PerformInteraction(Common_TextFieldStruct initialText_in, QList<Common_Choice>  choiceSet_in, OptionalArgument<QList<Common_VrHelpItem> >  vrHelp_in, int timeout_in, int appID_in);
  void SetMediaClockTimer(OptionalArgument<Common_TimeFormat>  startTime_in, int updateMode_in, int appID_in);
  void SetGlobalProperties(OptionalArgument<QString>  vrHelpTitle_in, OptionalArgument<QList<Common_VrHelpItem> >  vrHelp_in, int appID_in);
  void ShowVrHelp(OptionalArgument<QString>  vrHelpTitle_in, OptionalArgument<QList<Common_VrHelpItem> >  vrHelp_in, OptionalArgument<int>  appID_in);
  Common_DisplayCapabilities GetCapabilities(int& hmiZoneCapabilities_out, OptionalArgument<Common_SoftButtonCapabilities> & softButtonCapabilities_out);
  void ChangeRegistration(int language_in, int appID_in);
  QList<int>  GetSupportedLanguages();
  int GetLanguage();
  void SetAppIcon(Common_Image syncFileName_in, int appID_in);
  int Slider(int numTicks_in, int position_in, QString sliderHeader_in, OptionalArgument<QList<QString> >  sliderFooter_in, int timeout_in, int appID_in);
  void ScrollableMessage(Common_TextFieldStruct messageText_in, int timeout_in, OptionalArgument<QList<Common_SoftButton> >  softButtons_in, int appID_in);
  void PerformAudioPassThru(QList<Common_TextFieldStruct>  audioPassThruDisplayTexts_in, OptionalArgument<int>  maxDuration_in);
  void EndAudioPassThru();
  bool IsReady();
  void ClosePopUp();
 signals:
  void ShowNotification(Common_TextFieldStruct text, OptionalArgument<Common_Image>  icon, int timeout);
  void OnCommand(int cmdID, int appID);
  void OnSystemContext(int systemContext);
  void OnLanguageChange(int language);
  void OnDriverDistraction(int state);
 private slots:
  void ShowNotification_qml(QVariant text, QVariant icon, int timeout);
  void OnCommand_qml(int cmdID, int appID);
  void OnSystemContext_qml(int systemContext);
  void OnLanguageChange_qml(int language);
  void OnDriverDistraction_qml(int state);
 private:
  QQuickItem* api_;
};

class NavigationAdaptor : public QDBusAbstractAdaptor {
  Q_OBJECT
  Q_CLASSINFO("D-Bus Interface", "com.ford.sdl.hmi.Navigation")
  Q_CLASSINFO("D-Bus Introspection",
"<interface name=\"com.ford.sdl.hmi.Navigation\">"
"<method name=\"IsReady\">"
"<arg direction=\"out\" name=\"available\" type=\"b\" />"
"</method>"
"<method name=\"ShowConstantTBT\">"
"<arg direction=\"in\" name=\"navigationTexts\" type=\"a(is)\" />"
"<arg direction=\"in\" name=\"turnIcon\" type=\"(b(si))\" />"
"<arg direction=\"in\" name=\"distanceToManeuver\" type=\"d\" />"
"<arg direction=\"in\" name=\"distanceToManeuverScale\" type=\"d\" />"
"<arg direction=\"in\" name=\"maneuverComplete\" type=\"(bb)\" />"
"<arg direction=\"in\" name=\"softButtons\" type=\"(ba(i(bs)(b(si))(bb)i(bi)))\" />"
"<arg direction=\"in\" name=\"appID\" type=\"i\" />"
"</method>"
"<method name=\"AlertManeuver\">"
"<arg direction=\"in\" name=\"softButtons\" type=\"(ba(i(bs)(b(si))(bb)i(bi)))\" />"
"</method>"
"<method name=\"UpdateTurnList\">"
"<arg direction=\"in\" name=\"turnList\" type=\"a((b(is))(b(si)))\" />"
"<arg direction=\"in\" name=\"softButtons\" type=\"(ba(i(bs)(b(si))(bb)i(bi)))\" />"
"<arg direction=\"in\" name=\"appID\" type=\"i\" />"
"</method>"
"<signal name=\"OnTBTClientState\">"
"<arg name=\"state\" type=\"i\" />"
"</signal>"
"</interface>"
  )
 public:
  explicit NavigationAdaptor(QObject *parent = 0);
  void SetApi(QQuickItem*);
 public slots:
  bool IsReady();
  void ShowConstantTBT(QList<Common_TextFieldStruct>  navigationTexts_in, OptionalArgument<Common_Image>  turnIcon_in, double distanceToManeuver_in, double distanceToManeuverScale_in, OptionalArgument<bool>  maneuverComplete_in, OptionalArgument<QList<Common_SoftButton> >  softButtons_in, int appID_in);
  void AlertManeuver(OptionalArgument<QList<Common_SoftButton> >  softButtons_in);
  void UpdateTurnList(QList<Common_Turn>  turnList_in, OptionalArgument<QList<Common_SoftButton> >  softButtons_in, int appID_in);
 signals:
  void OnTBTClientState(int state);
 private slots:
  void OnTBTClientState_qml(int state);
 private:
  QQuickItem* api_;
};

class VehicleInfoAdaptor : public QDBusAbstractAdaptor {
  Q_OBJECT
  Q_CLASSINFO("D-Bus Interface", "com.ford.sdl.hmi.VehicleInfo")
  Q_CLASSINFO("D-Bus Introspection",
"<interface name=\"com.ford.sdl.hmi.VehicleInfo\">"
"<method name=\"IsReady\">"
"<arg direction=\"out\" name=\"available\" type=\"b\" />"
"</method>"
"<method name=\"GetVehicleType\">"
"<arg direction=\"out\" name=\"vehicleType\" type=\"((bs)(bs)(bs)(bs))\" />"
"</method>"
"<method name=\"ReadDID\">"
"<arg direction=\"in\" name=\"ecuName\" type=\"i\" />"
"<arg direction=\"in\" name=\"didLocation\" type=\"ai\" />"
"<arg direction=\"in\" name=\"appID\" type=\"i\" />"
"<arg direction=\"out\" name=\"didResult\" type=\"(ba(ii(bs)))\" />"
"</method>"
"<method name=\"GetDTCs\">"
"<arg direction=\"in\" name=\"ecuName\" type=\"i\" />"
"<arg direction=\"in\" name=\"dtcMask\" type=\"(bi)\" />"
"<arg direction=\"in\" name=\"appID\" type=\"i\" />"
"<arg direction=\"out\" name=\"ecuHeader\" type=\"i\" />"
"<arg direction=\"out\" name=\"dtc\" type=\"(bas)\" />"
"</method>"
"<method name=\"GetVehicleData\">"
"<arg direction=\"in\" name=\"gps\" type=\"(bb)\" />"
"<arg direction=\"in\" name=\"speed\" type=\"(bb)\" />"
"<arg direction=\"in\" name=\"rpm\" type=\"(bb)\" />"
"<arg direction=\"in\" name=\"fuelLevel\" type=\"(bb)\" />"
"<arg direction=\"in\" name=\"fuelLevel_State\" type=\"(bb)\" />"
"<arg direction=\"in\" name=\"instantFuelConsumption\" type=\"(bb)\" />"
"<arg direction=\"in\" name=\"externalTemperature\" type=\"(bb)\" />"
"<arg direction=\"in\" name=\"vin\" type=\"(bb)\" />"
"<arg direction=\"in\" name=\"prndl\" type=\"(bb)\" />"
"<arg direction=\"in\" name=\"tirePressure\" type=\"(bb)\" />"
"<arg direction=\"in\" name=\"odometer\" type=\"(bb)\" />"
"<arg direction=\"in\" name=\"beltStatus\" type=\"(bb)\" />"
"<arg direction=\"in\" name=\"bodyInformation\" type=\"(bb)\" />"
"<arg direction=\"in\" name=\"deviceStatus\" type=\"(bb)\" />"
"<arg direction=\"in\" name=\"driverBraking\" type=\"(bb)\" />"
"<arg direction=\"in\" name=\"wiperStatus\" type=\"(bb)\" />"
"<arg direction=\"in\" name=\"headLampStatus\" type=\"(bb)\" />"
"<arg direction=\"in\" name=\"engineTorque\" type=\"(bb)\" />"
"<arg direction=\"in\" name=\"accPedalPosition\" type=\"(bb)\" />"
"<arg direction=\"in\" name=\"steeringWheelAngle\" type=\"(bb)\" />"
"<arg direction=\"in\" name=\"myKey\" type=\"(bb)\" />"
"<arg direction=\"in\" name=\"appID\" type=\"i\" />"
"<arg direction=\"out\" name=\"gps\" type=\"(b(ddiiiiiiidddbiiddd))\" />"
"<arg direction=\"out\" name=\"speed\" type=\"(bd)\" />"
"<arg direction=\"out\" name=\"rpm\" type=\"(bi)\" />"
"<arg direction=\"out\" name=\"fuelLevel\" type=\"(bd)\" />"
"<arg direction=\"out\" name=\"fuelLevel_State\" type=\"(bi)\" />"
"<arg direction=\"out\" name=\"instantFuelConsumption\" type=\"(bd)\" />"
"<arg direction=\"out\" name=\"externalTemperature\" type=\"(bd)\" />"
"<arg direction=\"out\" name=\"vin\" type=\"(bs)\" />"
"<arg direction=\"out\" name=\"prndl\" type=\"(bi)\" />"
"<arg direction=\"out\" name=\"tirePressure\" type=\"(b(i(i)(i)(i)(i)(i)(i)))\" />"
"<arg direction=\"out\" name=\"odometer\" type=\"(bi)\" />"
"<arg direction=\"out\" name=\"beltStatus\" type=\"(b((bi)(bi)(bi)(bi)(bi)(bi)(bi)(bi)(bi)(bi)(bi)(bi)(bi)(bi)(bi)))\" />"
"<arg direction=\"out\" name=\"bodyInformation\" type=\"(b(bii))\" />"
"<arg direction=\"out\" name=\"deviceStatus\" type=\"(b((bb)(bb)(bb)(bb)(bb)(bi)(bb)(bb)(bi)(bi)(bb)))\" />"
"<arg direction=\"out\" name=\"driverBraking\" type=\"(bi)\" />"
"<arg direction=\"out\" name=\"wiperStatus\" type=\"(bi)\" />"
"<arg direction=\"out\" name=\"headLampStatus\" type=\"(b(bb))\" />"
"<arg direction=\"out\" name=\"engineTorque\" type=\"(bd)\" />"
"<arg direction=\"out\" name=\"accPedalPosition\" type=\"(bd)\" />"
"<arg direction=\"out\" name=\"steeringWheelAngle\" type=\"(bd)\" />"
"<arg direction=\"out\" name=\"myKey\" type=\"(b(i))\" />"
"</method>"
"<signal name=\"OnVehicleData\">"
"<arg name=\"gps\" type=\"(b(ddiiiiiiidddbiiddd))\" />"
"<arg name=\"speed\" type=\"(bd)\" />"
"<arg name=\"rpm\" type=\"(bi)\" />"
"<arg name=\"fuelLevel\" type=\"(bd)\" />"
"<arg name=\"fuelLevel_State\" type=\"(bi)\" />"
"<arg name=\"instantFuelConsumption\" type=\"(bd)\" />"
"<arg name=\"externalTemperature\" type=\"(bd)\" />"
"<arg name=\"vin\" type=\"(bs)\" />"
"<arg name=\"prndl\" type=\"(bi)\" />"
"<arg name=\"tirePressure\" type=\"(b(i(i)(i)(i)(i)(i)(i)))\" />"
"<arg name=\"odometer\" type=\"(bi)\" />"
"<arg name=\"beltStatus\" type=\"(b((bi)(bi)(bi)(bi)(bi)(bi)(bi)(bi)(bi)(bi)(bi)(bi)(bi)(bi)(bi)))\" />"
"<arg name=\"bodyInformation\" type=\"(b(bii))\" />"
"<arg name=\"deviceStatus\" type=\"(b((bb)(bb)(bb)(bb)(bb)(bi)(bb)(bb)(bi)(bi)(bb)))\" />"
"<arg name=\"driverBraking\" type=\"(bi)\" />"
"<arg name=\"wiperStatus\" type=\"(bi)\" />"
"<arg name=\"headLampStatus\" type=\"(b(bb))\" />"
"<arg name=\"engineTorque\" type=\"(bd)\" />"
"<arg name=\"accPedalPosition\" type=\"(bd)\" />"
"<arg name=\"steeringWheelAngle\" type=\"(bd)\" />"
"<arg name=\"myKey\" type=\"(b(i))\" />"
"</signal>"
"</interface>"
  )
 public:
  explicit VehicleInfoAdaptor(QObject *parent = 0);
  void SetApi(QQuickItem*);
 public slots:
  bool IsReady();
  Common_VehicleType GetVehicleType();
  OptionalArgument<QList<Common_DIDResult> >  ReadDID(int ecuName_in, QList<int>  didLocation_in, int appID_in);
  int GetDTCs(int ecuName_in, OptionalArgument<int>  dtcMask_in, int appID_in, OptionalArgument<QList<QString> > & dtc_out);
  OptionalArgument<Common_GPSData>  GetVehicleData(OptionalArgument<bool>  gps_in, OptionalArgument<bool>  speed_in, OptionalArgument<bool>  rpm_in, OptionalArgument<bool>  fuelLevel_in, OptionalArgument<bool>  fuelLevel_State_in, OptionalArgument<bool>  instantFuelConsumption_in, OptionalArgument<bool>  externalTemperature_in, OptionalArgument<bool>  vin_in, OptionalArgument<bool>  prndl_in, OptionalArgument<bool>  tirePressure_in, OptionalArgument<bool>  odometer_in, OptionalArgument<bool>  beltStatus_in, OptionalArgument<bool>  bodyInformation_in, OptionalArgument<bool>  deviceStatus_in, OptionalArgument<bool>  driverBraking_in, OptionalArgument<bool>  wiperStatus_in, OptionalArgument<bool>  headLampStatus_in, OptionalArgument<bool>  engineTorque_in, OptionalArgument<bool>  accPedalPosition_in, OptionalArgument<bool>  steeringWheelAngle_in, OptionalArgument<bool>  myKey_in, int appID_in, OptionalArgument<double> & speed_out, OptionalArgument<int> & rpm_out, OptionalArgument<double> & fuelLevel_out, OptionalArgument<int> & fuelLevel_State_out, OptionalArgument<double> & instantFuelConsumption_out, OptionalArgument<double> & externalTemperature_out, OptionalArgument<QString> & vin_out, OptionalArgument<int> & prndl_out, OptionalArgument<Common_TireStatus> & tirePressure_out, OptionalArgument<int> & odometer_out, OptionalArgument<Common_BeltStatus> & beltStatus_out, OptionalArgument<Common_BodyInformation> & bodyInformation_out, OptionalArgument<Common_DeviceStatus> & deviceStatus_out, OptionalArgument<int> & driverBraking_out, OptionalArgument<int> & wiperStatus_out, OptionalArgument<Common_HeadLampStatus> & headLampStatus_out, OptionalArgument<double> & engineTorque_out, OptionalArgument<double> & accPedalPosition_out, OptionalArgument<double> & steeringWheelAngle_out, OptionalArgument<Common_MyKey> & myKey_out);
 signals:
  void OnVehicleData(OptionalArgument<Common_GPSData>  gps, OptionalArgument<double>  speed, OptionalArgument<int>  rpm, OptionalArgument<double>  fuelLevel, OptionalArgument<int>  fuelLevel_State, OptionalArgument<double>  instantFuelConsumption, OptionalArgument<double>  externalTemperature, OptionalArgument<QString>  vin, OptionalArgument<int>  prndl, OptionalArgument<Common_TireStatus>  tirePressure, OptionalArgument<int>  odometer, OptionalArgument<Common_BeltStatus>  beltStatus, OptionalArgument<Common_BodyInformation>  bodyInformation, OptionalArgument<Common_DeviceStatus>  deviceStatus, OptionalArgument<int>  driverBraking, OptionalArgument<int>  wiperStatus, OptionalArgument<Common_HeadLampStatus>  headLampStatus, OptionalArgument<double>  engineTorque, OptionalArgument<double>  accPedalPosition, OptionalArgument<double>  steeringWheelAngle, OptionalArgument<Common_MyKey>  myKey);
 private slots:
  void OnVehicleData_qml(QVariant gps, QVariant speed, QVariant rpm, QVariant fuelLevel, QVariant fuelLevel_State, QVariant instantFuelConsumption, QVariant externalTemperature, QVariant vin, QVariant prndl, QVariant tirePressure, QVariant odometer, QVariant beltStatus, QVariant bodyInformation, QVariant deviceStatus, QVariant driverBraking, QVariant wiperStatus, QVariant headLampStatus, QVariant engineTorque, QVariant accPedalPosition, QVariant steeringWheelAngle, QVariant myKey);
 private:
  QQuickItem* api_;
};

void RegisterDbusMetatypes();
struct ApiAdaptors {
  ButtonsAdaptor* ButtonsAdaptor_;
  BasicCommunicationAdaptor* BasicCommunicationAdaptor_;
  VRAdaptor* VRAdaptor_;
  TTSAdaptor* TTSAdaptor_;
  UIAdaptor* UIAdaptor_;
  NavigationAdaptor* NavigationAdaptor_;
  VehicleInfoAdaptor* VehicleInfoAdaptor_;
  ApiAdaptors() :
    ButtonsAdaptor_(NULL),
    BasicCommunicationAdaptor_(NULL),
    VRAdaptor_(NULL),
    TTSAdaptor_(NULL),
    UIAdaptor_(NULL),
    NavigationAdaptor_(NULL),
    VehicleInfoAdaptor_(NULL)
  {}
  void Init(QObject* p) {
    ButtonsAdaptor_ = new ButtonsAdaptor(p);
    BasicCommunicationAdaptor_ = new BasicCommunicationAdaptor(p);
    VRAdaptor_ = new VRAdaptor(p);
    TTSAdaptor_ = new TTSAdaptor(p);
    UIAdaptor_ = new UIAdaptor(p);
    NavigationAdaptor_ = new NavigationAdaptor(p);
    VehicleInfoAdaptor_ = new VehicleInfoAdaptor(p);
  }
  void SetApi(QObject* p) {
    ButtonsAdaptor_->SetApi(p->findChild<QQuickItem*>("Buttons"));
    BasicCommunicationAdaptor_->SetApi(p->findChild<QQuickItem*>("BasicCommunication"));
    VRAdaptor_->SetApi(p->findChild<QQuickItem*>("VR"));
    TTSAdaptor_->SetApi(p->findChild<QQuickItem*>("TTS"));
    UIAdaptor_->SetApi(p->findChild<QQuickItem*>("UI"));
    NavigationAdaptor_->SetApi(p->findChild<QQuickItem*>("Navigation"));
    VehicleInfoAdaptor_->SetApi(p->findChild<QQuickItem*>("VehicleInfo"));
  }
};

Q_DECLARE_METATYPE(Common_SingleTireStatus)
Q_DECLARE_METATYPE(OptionalArgument<Common_SingleTireStatus>)
Q_DECLARE_METATYPE(QList<Common_SingleTireStatus>)
Q_DECLARE_METATYPE(OptionalArgument<QList<Common_SingleTireStatus> >)

Q_DECLARE_METATYPE(Common_TTSChunk)
Q_DECLARE_METATYPE(OptionalArgument<Common_TTSChunk>)
Q_DECLARE_METATYPE(QList<Common_TTSChunk>)
Q_DECLARE_METATYPE(OptionalArgument<QList<Common_TTSChunk> >)

Q_DECLARE_METATYPE(Common_BodyInformation)
Q_DECLARE_METATYPE(OptionalArgument<Common_BodyInformation>)
Q_DECLARE_METATYPE(QList<Common_BodyInformation>)
Q_DECLARE_METATYPE(OptionalArgument<QList<Common_BodyInformation> >)

Q_DECLARE_METATYPE(Common_Image)
Q_DECLARE_METATYPE(OptionalArgument<Common_Image>)
Q_DECLARE_METATYPE(QList<Common_Image>)
Q_DECLARE_METATYPE(OptionalArgument<QList<Common_Image> >)

Q_DECLARE_METATYPE(Common_VrHelpItem)
Q_DECLARE_METATYPE(OptionalArgument<Common_VrHelpItem>)
Q_DECLARE_METATYPE(QList<Common_VrHelpItem>)
Q_DECLARE_METATYPE(OptionalArgument<QList<Common_VrHelpItem> >)

Q_DECLARE_METATYPE(Common_HeadLampStatus)
Q_DECLARE_METATYPE(OptionalArgument<Common_HeadLampStatus>)
Q_DECLARE_METATYPE(QList<Common_HeadLampStatus>)
Q_DECLARE_METATYPE(OptionalArgument<QList<Common_HeadLampStatus> >)

Q_DECLARE_METATYPE(Common_TimeFormat)
Q_DECLARE_METATYPE(OptionalArgument<Common_TimeFormat>)
Q_DECLARE_METATYPE(QList<Common_TimeFormat>)
Q_DECLARE_METATYPE(OptionalArgument<QList<Common_TimeFormat> >)

Q_DECLARE_METATYPE(Common_GPSData)
Q_DECLARE_METATYPE(OptionalArgument<Common_GPSData>)
Q_DECLARE_METATYPE(QList<Common_GPSData>)
Q_DECLARE_METATYPE(OptionalArgument<QList<Common_GPSData> >)

Q_DECLARE_METATYPE(Common_SoftButton)
Q_DECLARE_METATYPE(OptionalArgument<Common_SoftButton>)
Q_DECLARE_METATYPE(QList<Common_SoftButton>)
Q_DECLARE_METATYPE(OptionalArgument<QList<Common_SoftButton> >)

Q_DECLARE_METATYPE(Common_DIDResult)
Q_DECLARE_METATYPE(OptionalArgument<Common_DIDResult>)
Q_DECLARE_METATYPE(QList<Common_DIDResult>)
Q_DECLARE_METATYPE(OptionalArgument<QList<Common_DIDResult> >)

Q_DECLARE_METATYPE(Common_PresetBankCapabilities)
Q_DECLARE_METATYPE(OptionalArgument<Common_PresetBankCapabilities>)
Q_DECLARE_METATYPE(QList<Common_PresetBankCapabilities>)
Q_DECLARE_METATYPE(OptionalArgument<QList<Common_PresetBankCapabilities> >)

Q_DECLARE_METATYPE(Common_HMIApplication)
Q_DECLARE_METATYPE(OptionalArgument<Common_HMIApplication>)
Q_DECLARE_METATYPE(QList<Common_HMIApplication>)
Q_DECLARE_METATYPE(OptionalArgument<QList<Common_HMIApplication> >)

Q_DECLARE_METATYPE(Common_Choice)
Q_DECLARE_METATYPE(OptionalArgument<Common_Choice>)
Q_DECLARE_METATYPE(QList<Common_Choice>)
Q_DECLARE_METATYPE(OptionalArgument<QList<Common_Choice> >)

Q_DECLARE_METATYPE(Common_TireStatus)
Q_DECLARE_METATYPE(OptionalArgument<Common_TireStatus>)
Q_DECLARE_METATYPE(QList<Common_TireStatus>)
Q_DECLARE_METATYPE(OptionalArgument<QList<Common_TireStatus> >)

Q_DECLARE_METATYPE(Common_ButtonCapabilities)
Q_DECLARE_METATYPE(OptionalArgument<Common_ButtonCapabilities>)
Q_DECLARE_METATYPE(QList<Common_ButtonCapabilities>)
Q_DECLARE_METATYPE(OptionalArgument<QList<Common_ButtonCapabilities> >)

Q_DECLARE_METATYPE(Common_BeltStatus)
Q_DECLARE_METATYPE(OptionalArgument<Common_BeltStatus>)
Q_DECLARE_METATYPE(QList<Common_BeltStatus>)
Q_DECLARE_METATYPE(OptionalArgument<QList<Common_BeltStatus> >)

Q_DECLARE_METATYPE(Common_DeviceInfo)
Q_DECLARE_METATYPE(OptionalArgument<Common_DeviceInfo>)
Q_DECLARE_METATYPE(QList<Common_DeviceInfo>)
Q_DECLARE_METATYPE(OptionalArgument<QList<Common_DeviceInfo> >)

Q_DECLARE_METATYPE(Common_MenuParams)
Q_DECLARE_METATYPE(OptionalArgument<Common_MenuParams>)
Q_DECLARE_METATYPE(QList<Common_MenuParams>)
Q_DECLARE_METATYPE(OptionalArgument<QList<Common_MenuParams> >)

Q_DECLARE_METATYPE(Common_SoftButtonCapabilities)
Q_DECLARE_METATYPE(OptionalArgument<Common_SoftButtonCapabilities>)
Q_DECLARE_METATYPE(QList<Common_SoftButtonCapabilities>)
Q_DECLARE_METATYPE(OptionalArgument<QList<Common_SoftButtonCapabilities> >)

Q_DECLARE_METATYPE(Common_DisplayCapabilities)
Q_DECLARE_METATYPE(OptionalArgument<Common_DisplayCapabilities>)
Q_DECLARE_METATYPE(QList<Common_DisplayCapabilities>)
Q_DECLARE_METATYPE(OptionalArgument<QList<Common_DisplayCapabilities> >)

Q_DECLARE_METATYPE(Common_VehicleType)
Q_DECLARE_METATYPE(OptionalArgument<Common_VehicleType>)
Q_DECLARE_METATYPE(QList<Common_VehicleType>)
Q_DECLARE_METATYPE(OptionalArgument<QList<Common_VehicleType> >)

Q_DECLARE_METATYPE(Common_MyKey)
Q_DECLARE_METATYPE(OptionalArgument<Common_MyKey>)
Q_DECLARE_METATYPE(QList<Common_MyKey>)
Q_DECLARE_METATYPE(OptionalArgument<QList<Common_MyKey> >)

Q_DECLARE_METATYPE(Common_DeviceStatus)
Q_DECLARE_METATYPE(OptionalArgument<Common_DeviceStatus>)
Q_DECLARE_METATYPE(QList<Common_DeviceStatus>)
Q_DECLARE_METATYPE(OptionalArgument<QList<Common_DeviceStatus> >)

Q_DECLARE_METATYPE(Common_TextFieldStruct)
Q_DECLARE_METATYPE(OptionalArgument<Common_TextFieldStruct>)
Q_DECLARE_METATYPE(QList<Common_TextFieldStruct>)
Q_DECLARE_METATYPE(OptionalArgument<QList<Common_TextFieldStruct> >)

Q_DECLARE_METATYPE(Common_Turn)
Q_DECLARE_METATYPE(OptionalArgument<Common_Turn>)
Q_DECLARE_METATYPE(QList<Common_Turn>)
Q_DECLARE_METATYPE(OptionalArgument<QList<Common_Turn> >)

#endif // #ifndef SRC_COMPONENTS_DBUS_QML_DBUS_H_
