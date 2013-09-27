/**
 * \file qml_dbus.cpp
 * \brief Generated source file that contain interfaces.
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
 * @file qml_dbus.cc
 * @brief Generated QDbus adaptors source file
 *
 * This file is a part of HMI D-Bus layer.
 */

#include "qml_dbus.h"

QDBusArgument& operator << (QDBusArgument& arg, const Common_SingleTireStatus& v) {
  arg.beginStructure();
  arg << v.status;
  arg.endStructure();
  return arg;
}
const QDBusArgument& operator >> (const QDBusArgument& arg, Common_SingleTireStatus& v) {
  arg.beginStructure();
  arg >> v.status;
  arg.endStructure();
  return arg;
}
QVariant ValueToVariant(const Common_SingleTireStatus& v) {
  QVariantMap map;
  PutArgToMap(map, "status", v.status);
  return QVariant::fromValue(map);
}
QVariant ValueToVariant(const QList<Common_SingleTireStatus>& v) {
  QList<QVariant> ret;
  for (QList<Common_SingleTireStatus>::const_iterator i = v.begin(); i != v.end(); ++i) ret.append(ValueToVariant(*i));
  return QVariant::fromValue(ret);
}
void PutArgToMap(QVariantMap& map, const char* name, const Common_SingleTireStatus& v) {
  map.insert(name, ValueToVariant(v));
}
bool VariantToValue(const QVariant& variant, Common_SingleTireStatus& v) {
  if (variant.type() != QVariant::Map) return false;
  QVariantMap map = variant.toMap();
  if (!GetArgFromMap(map, "status", v.status)) return false;
  return true;
}
bool VariantToValue(const QVariant& variant, QList<Common_SingleTireStatus>& v) {
  if (variant.type() != QVariant::List) return false;
  QList<QVariant> list = variant.toList();
  for (QList<QVariant>::const_iterator i = list.begin(); i != list.end(); ++i) {
    Common_SingleTireStatus s;
    if (!VariantToValue(*i, s)) return false;
    v.append(s);
  }
  return true;
}
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_SingleTireStatus& v) {
  QVariantMap::const_iterator it = map.find(name);
  if (map.end() == it) return false;
  if (it->type() != QVariant::Map) return false;
  const QVariantMap& inmap = it->toMap();
  if (!GetArgFromMap(inmap, "status", v.status)) return false;
  return true;
}

QDBusArgument& operator << (QDBusArgument& arg, const Common_TTSChunk& v) {
  arg.beginStructure();
  arg << v.text;
  arg.endStructure();
  return arg;
}
const QDBusArgument& operator >> (const QDBusArgument& arg, Common_TTSChunk& v) {
  arg.beginStructure();
  arg >> v.text;
  arg.endStructure();
  return arg;
}
QVariant ValueToVariant(const Common_TTSChunk& v) {
  QVariantMap map;
  PutArgToMap(map, "text", v.text);
  return QVariant::fromValue(map);
}
QVariant ValueToVariant(const QList<Common_TTSChunk>& v) {
  QList<QVariant> ret;
  for (QList<Common_TTSChunk>::const_iterator i = v.begin(); i != v.end(); ++i) ret.append(ValueToVariant(*i));
  return QVariant::fromValue(ret);
}
void PutArgToMap(QVariantMap& map, const char* name, const Common_TTSChunk& v) {
  map.insert(name, ValueToVariant(v));
}
bool VariantToValue(const QVariant& variant, Common_TTSChunk& v) {
  if (variant.type() != QVariant::Map) return false;
  QVariantMap map = variant.toMap();
  if (!GetArgFromMap(map, "text", v.text)) return false;
  return true;
}
bool VariantToValue(const QVariant& variant, QList<Common_TTSChunk>& v) {
  if (variant.type() != QVariant::List) return false;
  QList<QVariant> list = variant.toList();
  for (QList<QVariant>::const_iterator i = list.begin(); i != list.end(); ++i) {
    Common_TTSChunk s;
    if (!VariantToValue(*i, s)) return false;
    v.append(s);
  }
  return true;
}
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_TTSChunk& v) {
  QVariantMap::const_iterator it = map.find(name);
  if (map.end() == it) return false;
  if (it->type() != QVariant::Map) return false;
  const QVariantMap& inmap = it->toMap();
  if (!GetArgFromMap(inmap, "text", v.text)) return false;
  return true;
}

QDBusArgument& operator << (QDBusArgument& arg, const Common_BodyInformation& v) {
  arg.beginStructure();
  arg << v.parkBrakeActive;
  arg << v.ignitionStableStatus;
  arg << v.ignitionStatus;
  arg.endStructure();
  return arg;
}
const QDBusArgument& operator >> (const QDBusArgument& arg, Common_BodyInformation& v) {
  arg.beginStructure();
  arg >> v.parkBrakeActive;
  arg >> v.ignitionStableStatus;
  arg >> v.ignitionStatus;
  arg.endStructure();
  return arg;
}
QVariant ValueToVariant(const Common_BodyInformation& v) {
  QVariantMap map;
  PutArgToMap(map, "parkBrakeActive", v.parkBrakeActive);
  PutArgToMap(map, "ignitionStableStatus", v.ignitionStableStatus);
  PutArgToMap(map, "ignitionStatus", v.ignitionStatus);
  return QVariant::fromValue(map);
}
QVariant ValueToVariant(const QList<Common_BodyInformation>& v) {
  QList<QVariant> ret;
  for (QList<Common_BodyInformation>::const_iterator i = v.begin(); i != v.end(); ++i) ret.append(ValueToVariant(*i));
  return QVariant::fromValue(ret);
}
void PutArgToMap(QVariantMap& map, const char* name, const Common_BodyInformation& v) {
  map.insert(name, ValueToVariant(v));
}
bool VariantToValue(const QVariant& variant, Common_BodyInformation& v) {
  if (variant.type() != QVariant::Map) return false;
  QVariantMap map = variant.toMap();
  if (!GetArgFromMap(map, "parkBrakeActive", v.parkBrakeActive)) return false;
  if (!GetArgFromMap(map, "ignitionStableStatus", v.ignitionStableStatus)) return false;
  if (!GetArgFromMap(map, "ignitionStatus", v.ignitionStatus)) return false;
  return true;
}
bool VariantToValue(const QVariant& variant, QList<Common_BodyInformation>& v) {
  if (variant.type() != QVariant::List) return false;
  QList<QVariant> list = variant.toList();
  for (QList<QVariant>::const_iterator i = list.begin(); i != list.end(); ++i) {
    Common_BodyInformation s;
    if (!VariantToValue(*i, s)) return false;
    v.append(s);
  }
  return true;
}
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_BodyInformation& v) {
  QVariantMap::const_iterator it = map.find(name);
  if (map.end() == it) return false;
  if (it->type() != QVariant::Map) return false;
  const QVariantMap& inmap = it->toMap();
  if (!GetArgFromMap(inmap, "parkBrakeActive", v.parkBrakeActive)) return false;
  if (!GetArgFromMap(inmap, "ignitionStableStatus", v.ignitionStableStatus)) return false;
  if (!GetArgFromMap(inmap, "ignitionStatus", v.ignitionStatus)) return false;
  return true;
}

QDBusArgument& operator << (QDBusArgument& arg, const Common_Image& v) {
  arg.beginStructure();
  arg << v.value;
  arg << v.imageType;
  arg.endStructure();
  return arg;
}
const QDBusArgument& operator >> (const QDBusArgument& arg, Common_Image& v) {
  arg.beginStructure();
  arg >> v.value;
  arg >> v.imageType;
  arg.endStructure();
  return arg;
}
QVariant ValueToVariant(const Common_Image& v) {
  QVariantMap map;
  PutArgToMap(map, "value", v.value);
  PutArgToMap(map, "imageType", v.imageType);
  return QVariant::fromValue(map);
}
QVariant ValueToVariant(const QList<Common_Image>& v) {
  QList<QVariant> ret;
  for (QList<Common_Image>::const_iterator i = v.begin(); i != v.end(); ++i) ret.append(ValueToVariant(*i));
  return QVariant::fromValue(ret);
}
void PutArgToMap(QVariantMap& map, const char* name, const Common_Image& v) {
  map.insert(name, ValueToVariant(v));
}
bool VariantToValue(const QVariant& variant, Common_Image& v) {
  if (variant.type() != QVariant::Map) return false;
  QVariantMap map = variant.toMap();
  if (!GetArgFromMap(map, "value", v.value)) return false;
  if (!GetArgFromMap(map, "imageType", v.imageType)) return false;
  return true;
}
bool VariantToValue(const QVariant& variant, QList<Common_Image>& v) {
  if (variant.type() != QVariant::List) return false;
  QList<QVariant> list = variant.toList();
  for (QList<QVariant>::const_iterator i = list.begin(); i != list.end(); ++i) {
    Common_Image s;
    if (!VariantToValue(*i, s)) return false;
    v.append(s);
  }
  return true;
}
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_Image& v) {
  QVariantMap::const_iterator it = map.find(name);
  if (map.end() == it) return false;
  if (it->type() != QVariant::Map) return false;
  const QVariantMap& inmap = it->toMap();
  if (!GetArgFromMap(inmap, "value", v.value)) return false;
  if (!GetArgFromMap(inmap, "imageType", v.imageType)) return false;
  return true;
}

QDBusArgument& operator << (QDBusArgument& arg, const Common_VrHelpItem& v) {
  arg.beginStructure();
  arg << v.text;
  arg << v.image;
  arg << v.position;
  arg.endStructure();
  return arg;
}
const QDBusArgument& operator >> (const QDBusArgument& arg, Common_VrHelpItem& v) {
  arg.beginStructure();
  arg >> v.text;
  arg >> v.image;
  arg >> v.position;
  arg.endStructure();
  return arg;
}
QVariant ValueToVariant(const Common_VrHelpItem& v) {
  QVariantMap map;
  PutArgToMap(map, "text", v.text);
  PutArgToMap(map, "image", v.image);
  PutArgToMap(map, "position", v.position);
  return QVariant::fromValue(map);
}
QVariant ValueToVariant(const QList<Common_VrHelpItem>& v) {
  QList<QVariant> ret;
  for (QList<Common_VrHelpItem>::const_iterator i = v.begin(); i != v.end(); ++i) ret.append(ValueToVariant(*i));
  return QVariant::fromValue(ret);
}
void PutArgToMap(QVariantMap& map, const char* name, const Common_VrHelpItem& v) {
  map.insert(name, ValueToVariant(v));
}
bool VariantToValue(const QVariant& variant, Common_VrHelpItem& v) {
  if (variant.type() != QVariant::Map) return false;
  QVariantMap map = variant.toMap();
  if (!GetArgFromMap(map, "text", v.text)) return false;
  if (!GetArgFromMap(map, "image", v.image)) return false;
  if (!GetArgFromMap(map, "position", v.position)) return false;
  return true;
}
bool VariantToValue(const QVariant& variant, QList<Common_VrHelpItem>& v) {
  if (variant.type() != QVariant::List) return false;
  QList<QVariant> list = variant.toList();
  for (QList<QVariant>::const_iterator i = list.begin(); i != list.end(); ++i) {
    Common_VrHelpItem s;
    if (!VariantToValue(*i, s)) return false;
    v.append(s);
  }
  return true;
}
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_VrHelpItem& v) {
  QVariantMap::const_iterator it = map.find(name);
  if (map.end() == it) return false;
  if (it->type() != QVariant::Map) return false;
  const QVariantMap& inmap = it->toMap();
  if (!GetArgFromMap(inmap, "text", v.text)) return false;
  if (!GetArgFromMap(inmap, "image", v.image)) return false;
  if (!GetArgFromMap(inmap, "position", v.position)) return false;
  return true;
}

QDBusArgument& operator << (QDBusArgument& arg, const Common_HeadLampStatus& v) {
  arg.beginStructure();
  arg << v.lowBeamsOn;
  arg << v.highBeamsOn;
  arg.endStructure();
  return arg;
}
const QDBusArgument& operator >> (const QDBusArgument& arg, Common_HeadLampStatus& v) {
  arg.beginStructure();
  arg >> v.lowBeamsOn;
  arg >> v.highBeamsOn;
  arg.endStructure();
  return arg;
}
QVariant ValueToVariant(const Common_HeadLampStatus& v) {
  QVariantMap map;
  PutArgToMap(map, "lowBeamsOn", v.lowBeamsOn);
  PutArgToMap(map, "highBeamsOn", v.highBeamsOn);
  return QVariant::fromValue(map);
}
QVariant ValueToVariant(const QList<Common_HeadLampStatus>& v) {
  QList<QVariant> ret;
  for (QList<Common_HeadLampStatus>::const_iterator i = v.begin(); i != v.end(); ++i) ret.append(ValueToVariant(*i));
  return QVariant::fromValue(ret);
}
void PutArgToMap(QVariantMap& map, const char* name, const Common_HeadLampStatus& v) {
  map.insert(name, ValueToVariant(v));
}
bool VariantToValue(const QVariant& variant, Common_HeadLampStatus& v) {
  if (variant.type() != QVariant::Map) return false;
  QVariantMap map = variant.toMap();
  if (!GetArgFromMap(map, "lowBeamsOn", v.lowBeamsOn)) return false;
  if (!GetArgFromMap(map, "highBeamsOn", v.highBeamsOn)) return false;
  return true;
}
bool VariantToValue(const QVariant& variant, QList<Common_HeadLampStatus>& v) {
  if (variant.type() != QVariant::List) return false;
  QList<QVariant> list = variant.toList();
  for (QList<QVariant>::const_iterator i = list.begin(); i != list.end(); ++i) {
    Common_HeadLampStatus s;
    if (!VariantToValue(*i, s)) return false;
    v.append(s);
  }
  return true;
}
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_HeadLampStatus& v) {
  QVariantMap::const_iterator it = map.find(name);
  if (map.end() == it) return false;
  if (it->type() != QVariant::Map) return false;
  const QVariantMap& inmap = it->toMap();
  if (!GetArgFromMap(inmap, "lowBeamsOn", v.lowBeamsOn)) return false;
  if (!GetArgFromMap(inmap, "highBeamsOn", v.highBeamsOn)) return false;
  return true;
}

QDBusArgument& operator << (QDBusArgument& arg, const Common_TimeFormat& v) {
  arg.beginStructure();
  arg << v.hours;
  arg << v.minutes;
  arg << v.seconds;
  arg.endStructure();
  return arg;
}
const QDBusArgument& operator >> (const QDBusArgument& arg, Common_TimeFormat& v) {
  arg.beginStructure();
  arg >> v.hours;
  arg >> v.minutes;
  arg >> v.seconds;
  arg.endStructure();
  return arg;
}
QVariant ValueToVariant(const Common_TimeFormat& v) {
  QVariantMap map;
  PutArgToMap(map, "hours", v.hours);
  PutArgToMap(map, "minutes", v.minutes);
  PutArgToMap(map, "seconds", v.seconds);
  return QVariant::fromValue(map);
}
QVariant ValueToVariant(const QList<Common_TimeFormat>& v) {
  QList<QVariant> ret;
  for (QList<Common_TimeFormat>::const_iterator i = v.begin(); i != v.end(); ++i) ret.append(ValueToVariant(*i));
  return QVariant::fromValue(ret);
}
void PutArgToMap(QVariantMap& map, const char* name, const Common_TimeFormat& v) {
  map.insert(name, ValueToVariant(v));
}
bool VariantToValue(const QVariant& variant, Common_TimeFormat& v) {
  if (variant.type() != QVariant::Map) return false;
  QVariantMap map = variant.toMap();
  if (!GetArgFromMap(map, "hours", v.hours)) return false;
  if (!GetArgFromMap(map, "minutes", v.minutes)) return false;
  if (!GetArgFromMap(map, "seconds", v.seconds)) return false;
  return true;
}
bool VariantToValue(const QVariant& variant, QList<Common_TimeFormat>& v) {
  if (variant.type() != QVariant::List) return false;
  QList<QVariant> list = variant.toList();
  for (QList<QVariant>::const_iterator i = list.begin(); i != list.end(); ++i) {
    Common_TimeFormat s;
    if (!VariantToValue(*i, s)) return false;
    v.append(s);
  }
  return true;
}
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_TimeFormat& v) {
  QVariantMap::const_iterator it = map.find(name);
  if (map.end() == it) return false;
  if (it->type() != QVariant::Map) return false;
  const QVariantMap& inmap = it->toMap();
  if (!GetArgFromMap(inmap, "hours", v.hours)) return false;
  if (!GetArgFromMap(inmap, "minutes", v.minutes)) return false;
  if (!GetArgFromMap(inmap, "seconds", v.seconds)) return false;
  return true;
}

QDBusArgument& operator << (QDBusArgument& arg, const Common_GPSData& v) {
  arg.beginStructure();
  arg << v.longitudeDegrees;
  arg << v.latitudeDegrees;
  arg << v.utcYear;
  arg << v.utcMonth;
  arg << v.utcDay;
  arg << v.utcHours;
  arg << v.utcMinutes;
  arg << v.utcSeconds;
  arg << v.compassDirection;
  arg << v.pdop;
  arg << v.hdop;
  arg << v.vdop;
  arg << v.actual;
  arg << v.satellites;
  arg << v.dimension;
  arg << v.altitude;
  arg << v.heading;
  arg << v.speed;
  arg.endStructure();
  return arg;
}
const QDBusArgument& operator >> (const QDBusArgument& arg, Common_GPSData& v) {
  arg.beginStructure();
  arg >> v.longitudeDegrees;
  arg >> v.latitudeDegrees;
  arg >> v.utcYear;
  arg >> v.utcMonth;
  arg >> v.utcDay;
  arg >> v.utcHours;
  arg >> v.utcMinutes;
  arg >> v.utcSeconds;
  arg >> v.compassDirection;
  arg >> v.pdop;
  arg >> v.hdop;
  arg >> v.vdop;
  arg >> v.actual;
  arg >> v.satellites;
  arg >> v.dimension;
  arg >> v.altitude;
  arg >> v.heading;
  arg >> v.speed;
  arg.endStructure();
  return arg;
}
QVariant ValueToVariant(const Common_GPSData& v) {
  QVariantMap map;
  PutArgToMap(map, "longitudeDegrees", v.longitudeDegrees);
  PutArgToMap(map, "latitudeDegrees", v.latitudeDegrees);
  PutArgToMap(map, "utcYear", v.utcYear);
  PutArgToMap(map, "utcMonth", v.utcMonth);
  PutArgToMap(map, "utcDay", v.utcDay);
  PutArgToMap(map, "utcHours", v.utcHours);
  PutArgToMap(map, "utcMinutes", v.utcMinutes);
  PutArgToMap(map, "utcSeconds", v.utcSeconds);
  PutArgToMap(map, "compassDirection", v.compassDirection);
  PutArgToMap(map, "pdop", v.pdop);
  PutArgToMap(map, "hdop", v.hdop);
  PutArgToMap(map, "vdop", v.vdop);
  PutArgToMap(map, "actual", v.actual);
  PutArgToMap(map, "satellites", v.satellites);
  PutArgToMap(map, "dimension", v.dimension);
  PutArgToMap(map, "altitude", v.altitude);
  PutArgToMap(map, "heading", v.heading);
  PutArgToMap(map, "speed", v.speed);
  return QVariant::fromValue(map);
}
QVariant ValueToVariant(const QList<Common_GPSData>& v) {
  QList<QVariant> ret;
  for (QList<Common_GPSData>::const_iterator i = v.begin(); i != v.end(); ++i) ret.append(ValueToVariant(*i));
  return QVariant::fromValue(ret);
}
void PutArgToMap(QVariantMap& map, const char* name, const Common_GPSData& v) {
  map.insert(name, ValueToVariant(v));
}
bool VariantToValue(const QVariant& variant, Common_GPSData& v) {
  if (variant.type() != QVariant::Map) return false;
  QVariantMap map = variant.toMap();
  if (!GetArgFromMap(map, "longitudeDegrees", v.longitudeDegrees)) return false;
  if (!GetArgFromMap(map, "latitudeDegrees", v.latitudeDegrees)) return false;
  if (!GetArgFromMap(map, "utcYear", v.utcYear)) return false;
  if (!GetArgFromMap(map, "utcMonth", v.utcMonth)) return false;
  if (!GetArgFromMap(map, "utcDay", v.utcDay)) return false;
  if (!GetArgFromMap(map, "utcHours", v.utcHours)) return false;
  if (!GetArgFromMap(map, "utcMinutes", v.utcMinutes)) return false;
  if (!GetArgFromMap(map, "utcSeconds", v.utcSeconds)) return false;
  if (!GetArgFromMap(map, "compassDirection", v.compassDirection)) return false;
  if (!GetArgFromMap(map, "pdop", v.pdop)) return false;
  if (!GetArgFromMap(map, "hdop", v.hdop)) return false;
  if (!GetArgFromMap(map, "vdop", v.vdop)) return false;
  if (!GetArgFromMap(map, "actual", v.actual)) return false;
  if (!GetArgFromMap(map, "satellites", v.satellites)) return false;
  if (!GetArgFromMap(map, "dimension", v.dimension)) return false;
  if (!GetArgFromMap(map, "altitude", v.altitude)) return false;
  if (!GetArgFromMap(map, "heading", v.heading)) return false;
  if (!GetArgFromMap(map, "speed", v.speed)) return false;
  return true;
}
bool VariantToValue(const QVariant& variant, QList<Common_GPSData>& v) {
  if (variant.type() != QVariant::List) return false;
  QList<QVariant> list = variant.toList();
  for (QList<QVariant>::const_iterator i = list.begin(); i != list.end(); ++i) {
    Common_GPSData s;
    if (!VariantToValue(*i, s)) return false;
    v.append(s);
  }
  return true;
}
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_GPSData& v) {
  QVariantMap::const_iterator it = map.find(name);
  if (map.end() == it) return false;
  if (it->type() != QVariant::Map) return false;
  const QVariantMap& inmap = it->toMap();
  if (!GetArgFromMap(inmap, "longitudeDegrees", v.longitudeDegrees)) return false;
  if (!GetArgFromMap(inmap, "latitudeDegrees", v.latitudeDegrees)) return false;
  if (!GetArgFromMap(inmap, "utcYear", v.utcYear)) return false;
  if (!GetArgFromMap(inmap, "utcMonth", v.utcMonth)) return false;
  if (!GetArgFromMap(inmap, "utcDay", v.utcDay)) return false;
  if (!GetArgFromMap(inmap, "utcHours", v.utcHours)) return false;
  if (!GetArgFromMap(inmap, "utcMinutes", v.utcMinutes)) return false;
  if (!GetArgFromMap(inmap, "utcSeconds", v.utcSeconds)) return false;
  if (!GetArgFromMap(inmap, "compassDirection", v.compassDirection)) return false;
  if (!GetArgFromMap(inmap, "pdop", v.pdop)) return false;
  if (!GetArgFromMap(inmap, "hdop", v.hdop)) return false;
  if (!GetArgFromMap(inmap, "vdop", v.vdop)) return false;
  if (!GetArgFromMap(inmap, "actual", v.actual)) return false;
  if (!GetArgFromMap(inmap, "satellites", v.satellites)) return false;
  if (!GetArgFromMap(inmap, "dimension", v.dimension)) return false;
  if (!GetArgFromMap(inmap, "altitude", v.altitude)) return false;
  if (!GetArgFromMap(inmap, "heading", v.heading)) return false;
  if (!GetArgFromMap(inmap, "speed", v.speed)) return false;
  return true;
}

QDBusArgument& operator << (QDBusArgument& arg, const Common_SoftButton& v) {
  arg.beginStructure();
  arg << v.type;
  arg << v.text;
  arg << v.image;
  arg << v.isHighlighted;
  arg << v.softButtonID;
  arg << v.systemAction;
  arg.endStructure();
  return arg;
}
const QDBusArgument& operator >> (const QDBusArgument& arg, Common_SoftButton& v) {
  arg.beginStructure();
  arg >> v.type;
  arg >> v.text;
  arg >> v.image;
  arg >> v.isHighlighted;
  arg >> v.softButtonID;
  arg >> v.systemAction;
  arg.endStructure();
  return arg;
}
QVariant ValueToVariant(const Common_SoftButton& v) {
  QVariantMap map;
  PutArgToMap(map, "type", v.type);
  PutArgToMap(map, "text", v.text);
  PutArgToMap(map, "image", v.image);
  PutArgToMap(map, "isHighlighted", v.isHighlighted);
  PutArgToMap(map, "softButtonID", v.softButtonID);
  PutArgToMap(map, "systemAction", v.systemAction);
  return QVariant::fromValue(map);
}
QVariant ValueToVariant(const QList<Common_SoftButton>& v) {
  QList<QVariant> ret;
  for (QList<Common_SoftButton>::const_iterator i = v.begin(); i != v.end(); ++i) ret.append(ValueToVariant(*i));
  return QVariant::fromValue(ret);
}
void PutArgToMap(QVariantMap& map, const char* name, const Common_SoftButton& v) {
  map.insert(name, ValueToVariant(v));
}
bool VariantToValue(const QVariant& variant, Common_SoftButton& v) {
  if (variant.type() != QVariant::Map) return false;
  QVariantMap map = variant.toMap();
  if (!GetArgFromMap(map, "type", v.type)) return false;
  if (!GetArgFromMap(map, "text", v.text)) return false;
  if (!GetArgFromMap(map, "image", v.image)) return false;
  if (!GetArgFromMap(map, "isHighlighted", v.isHighlighted)) return false;
  if (!GetArgFromMap(map, "softButtonID", v.softButtonID)) return false;
  if (!GetArgFromMap(map, "systemAction", v.systemAction)) return false;
  return true;
}
bool VariantToValue(const QVariant& variant, QList<Common_SoftButton>& v) {
  if (variant.type() != QVariant::List) return false;
  QList<QVariant> list = variant.toList();
  for (QList<QVariant>::const_iterator i = list.begin(); i != list.end(); ++i) {
    Common_SoftButton s;
    if (!VariantToValue(*i, s)) return false;
    v.append(s);
  }
  return true;
}
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_SoftButton& v) {
  QVariantMap::const_iterator it = map.find(name);
  if (map.end() == it) return false;
  if (it->type() != QVariant::Map) return false;
  const QVariantMap& inmap = it->toMap();
  if (!GetArgFromMap(inmap, "type", v.type)) return false;
  if (!GetArgFromMap(inmap, "text", v.text)) return false;
  if (!GetArgFromMap(inmap, "image", v.image)) return false;
  if (!GetArgFromMap(inmap, "isHighlighted", v.isHighlighted)) return false;
  if (!GetArgFromMap(inmap, "softButtonID", v.softButtonID)) return false;
  if (!GetArgFromMap(inmap, "systemAction", v.systemAction)) return false;
  return true;
}

QDBusArgument& operator << (QDBusArgument& arg, const Common_DIDResult& v) {
  arg.beginStructure();
  arg << v.resultCode;
  arg << v.didLocation;
  arg << v.data;
  arg.endStructure();
  return arg;
}
const QDBusArgument& operator >> (const QDBusArgument& arg, Common_DIDResult& v) {
  arg.beginStructure();
  arg >> v.resultCode;
  arg >> v.didLocation;
  arg >> v.data;
  arg.endStructure();
  return arg;
}
QVariant ValueToVariant(const Common_DIDResult& v) {
  QVariantMap map;
  PutArgToMap(map, "resultCode", v.resultCode);
  PutArgToMap(map, "didLocation", v.didLocation);
  PutArgToMap(map, "data", v.data);
  return QVariant::fromValue(map);
}
QVariant ValueToVariant(const QList<Common_DIDResult>& v) {
  QList<QVariant> ret;
  for (QList<Common_DIDResult>::const_iterator i = v.begin(); i != v.end(); ++i) ret.append(ValueToVariant(*i));
  return QVariant::fromValue(ret);
}
void PutArgToMap(QVariantMap& map, const char* name, const Common_DIDResult& v) {
  map.insert(name, ValueToVariant(v));
}
bool VariantToValue(const QVariant& variant, Common_DIDResult& v) {
  if (variant.type() != QVariant::Map) return false;
  QVariantMap map = variant.toMap();
  if (!GetArgFromMap(map, "resultCode", v.resultCode)) return false;
  if (!GetArgFromMap(map, "didLocation", v.didLocation)) return false;
  if (!GetArgFromMap(map, "data", v.data)) return false;
  return true;
}
bool VariantToValue(const QVariant& variant, QList<Common_DIDResult>& v) {
  if (variant.type() != QVariant::List) return false;
  QList<QVariant> list = variant.toList();
  for (QList<QVariant>::const_iterator i = list.begin(); i != list.end(); ++i) {
    Common_DIDResult s;
    if (!VariantToValue(*i, s)) return false;
    v.append(s);
  }
  return true;
}
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_DIDResult& v) {
  QVariantMap::const_iterator it = map.find(name);
  if (map.end() == it) return false;
  if (it->type() != QVariant::Map) return false;
  const QVariantMap& inmap = it->toMap();
  if (!GetArgFromMap(inmap, "resultCode", v.resultCode)) return false;
  if (!GetArgFromMap(inmap, "didLocation", v.didLocation)) return false;
  if (!GetArgFromMap(inmap, "data", v.data)) return false;
  return true;
}

QDBusArgument& operator << (QDBusArgument& arg, const Common_PresetBankCapabilities& v) {
  arg.beginStructure();
  arg << v.onScreenPresetsAvailable;
  arg.endStructure();
  return arg;
}
const QDBusArgument& operator >> (const QDBusArgument& arg, Common_PresetBankCapabilities& v) {
  arg.beginStructure();
  arg >> v.onScreenPresetsAvailable;
  arg.endStructure();
  return arg;
}
QVariant ValueToVariant(const Common_PresetBankCapabilities& v) {
  QVariantMap map;
  PutArgToMap(map, "onScreenPresetsAvailable", v.onScreenPresetsAvailable);
  return QVariant::fromValue(map);
}
QVariant ValueToVariant(const QList<Common_PresetBankCapabilities>& v) {
  QList<QVariant> ret;
  for (QList<Common_PresetBankCapabilities>::const_iterator i = v.begin(); i != v.end(); ++i) ret.append(ValueToVariant(*i));
  return QVariant::fromValue(ret);
}
void PutArgToMap(QVariantMap& map, const char* name, const Common_PresetBankCapabilities& v) {
  map.insert(name, ValueToVariant(v));
}
bool VariantToValue(const QVariant& variant, Common_PresetBankCapabilities& v) {
  if (variant.type() != QVariant::Map) return false;
  QVariantMap map = variant.toMap();
  if (!GetArgFromMap(map, "onScreenPresetsAvailable", v.onScreenPresetsAvailable)) return false;
  return true;
}
bool VariantToValue(const QVariant& variant, QList<Common_PresetBankCapabilities>& v) {
  if (variant.type() != QVariant::List) return false;
  QList<QVariant> list = variant.toList();
  for (QList<QVariant>::const_iterator i = list.begin(); i != list.end(); ++i) {
    Common_PresetBankCapabilities s;
    if (!VariantToValue(*i, s)) return false;
    v.append(s);
  }
  return true;
}
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_PresetBankCapabilities& v) {
  QVariantMap::const_iterator it = map.find(name);
  if (map.end() == it) return false;
  if (it->type() != QVariant::Map) return false;
  const QVariantMap& inmap = it->toMap();
  if (!GetArgFromMap(inmap, "onScreenPresetsAvailable", v.onScreenPresetsAvailable)) return false;
  return true;
}

QDBusArgument& operator << (QDBusArgument& arg, const Common_HMIApplication& v) {
  arg.beginStructure();
  arg << v.appName;
  arg << v.ngnMediaScreenAppName;
  arg << v.icon;
  arg << v.deviceName;
  arg << v.appID;
  arg << v.hmiDisplayLanguageDesired;
  arg << v.isMediaApplication;
  arg << v.appType;
  arg.endStructure();
  return arg;
}
const QDBusArgument& operator >> (const QDBusArgument& arg, Common_HMIApplication& v) {
  arg.beginStructure();
  arg >> v.appName;
  arg >> v.ngnMediaScreenAppName;
  arg >> v.icon;
  arg >> v.deviceName;
  arg >> v.appID;
  arg >> v.hmiDisplayLanguageDesired;
  arg >> v.isMediaApplication;
  arg >> v.appType;
  arg.endStructure();
  return arg;
}
QVariant ValueToVariant(const Common_HMIApplication& v) {
  QVariantMap map;
  PutArgToMap(map, "appName", v.appName);
  PutArgToMap(map, "ngnMediaScreenAppName", v.ngnMediaScreenAppName);
  PutArgToMap(map, "icon", v.icon);
  PutArgToMap(map, "deviceName", v.deviceName);
  PutArgToMap(map, "appID", v.appID);
  PutArgToMap(map, "hmiDisplayLanguageDesired", v.hmiDisplayLanguageDesired);
  PutArgToMap(map, "isMediaApplication", v.isMediaApplication);
  PutArgToMap(map, "appType", v.appType);
  return QVariant::fromValue(map);
}
QVariant ValueToVariant(const QList<Common_HMIApplication>& v) {
  QList<QVariant> ret;
  for (QList<Common_HMIApplication>::const_iterator i = v.begin(); i != v.end(); ++i) ret.append(ValueToVariant(*i));
  return QVariant::fromValue(ret);
}
void PutArgToMap(QVariantMap& map, const char* name, const Common_HMIApplication& v) {
  map.insert(name, ValueToVariant(v));
}
bool VariantToValue(const QVariant& variant, Common_HMIApplication& v) {
  if (variant.type() != QVariant::Map) return false;
  QVariantMap map = variant.toMap();
  if (!GetArgFromMap(map, "appName", v.appName)) return false;
  if (!GetArgFromMap(map, "ngnMediaScreenAppName", v.ngnMediaScreenAppName)) return false;
  if (!GetArgFromMap(map, "icon", v.icon)) return false;
  if (!GetArgFromMap(map, "deviceName", v.deviceName)) return false;
  if (!GetArgFromMap(map, "appID", v.appID)) return false;
  if (!GetArgFromMap(map, "hmiDisplayLanguageDesired", v.hmiDisplayLanguageDesired)) return false;
  if (!GetArgFromMap(map, "isMediaApplication", v.isMediaApplication)) return false;
  if (!GetArgFromMap(map, "appType", v.appType)) return false;
  return true;
}
bool VariantToValue(const QVariant& variant, QList<Common_HMIApplication>& v) {
  if (variant.type() != QVariant::List) return false;
  QList<QVariant> list = variant.toList();
  for (QList<QVariant>::const_iterator i = list.begin(); i != list.end(); ++i) {
    Common_HMIApplication s;
    if (!VariantToValue(*i, s)) return false;
    v.append(s);
  }
  return true;
}
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_HMIApplication& v) {
  QVariantMap::const_iterator it = map.find(name);
  if (map.end() == it) return false;
  if (it->type() != QVariant::Map) return false;
  const QVariantMap& inmap = it->toMap();
  if (!GetArgFromMap(inmap, "appName", v.appName)) return false;
  if (!GetArgFromMap(inmap, "ngnMediaScreenAppName", v.ngnMediaScreenAppName)) return false;
  if (!GetArgFromMap(inmap, "icon", v.icon)) return false;
  if (!GetArgFromMap(inmap, "deviceName", v.deviceName)) return false;
  if (!GetArgFromMap(inmap, "appID", v.appID)) return false;
  if (!GetArgFromMap(inmap, "hmiDisplayLanguageDesired", v.hmiDisplayLanguageDesired)) return false;
  if (!GetArgFromMap(inmap, "isMediaApplication", v.isMediaApplication)) return false;
  if (!GetArgFromMap(inmap, "appType", v.appType)) return false;
  return true;
}

QDBusArgument& operator << (QDBusArgument& arg, const Common_Choice& v) {
  arg.beginStructure();
  arg << v.choiceID;
  arg << v.menuName;
  arg << v.image;
  arg.endStructure();
  return arg;
}
const QDBusArgument& operator >> (const QDBusArgument& arg, Common_Choice& v) {
  arg.beginStructure();
  arg >> v.choiceID;
  arg >> v.menuName;
  arg >> v.image;
  arg.endStructure();
  return arg;
}
QVariant ValueToVariant(const Common_Choice& v) {
  QVariantMap map;
  PutArgToMap(map, "choiceID", v.choiceID);
  PutArgToMap(map, "menuName", v.menuName);
  PutArgToMap(map, "image", v.image);
  return QVariant::fromValue(map);
}
QVariant ValueToVariant(const QList<Common_Choice>& v) {
  QList<QVariant> ret;
  for (QList<Common_Choice>::const_iterator i = v.begin(); i != v.end(); ++i) ret.append(ValueToVariant(*i));
  return QVariant::fromValue(ret);
}
void PutArgToMap(QVariantMap& map, const char* name, const Common_Choice& v) {
  map.insert(name, ValueToVariant(v));
}
bool VariantToValue(const QVariant& variant, Common_Choice& v) {
  if (variant.type() != QVariant::Map) return false;
  QVariantMap map = variant.toMap();
  if (!GetArgFromMap(map, "choiceID", v.choiceID)) return false;
  if (!GetArgFromMap(map, "menuName", v.menuName)) return false;
  if (!GetArgFromMap(map, "image", v.image)) return false;
  return true;
}
bool VariantToValue(const QVariant& variant, QList<Common_Choice>& v) {
  if (variant.type() != QVariant::List) return false;
  QList<QVariant> list = variant.toList();
  for (QList<QVariant>::const_iterator i = list.begin(); i != list.end(); ++i) {
    Common_Choice s;
    if (!VariantToValue(*i, s)) return false;
    v.append(s);
  }
  return true;
}
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_Choice& v) {
  QVariantMap::const_iterator it = map.find(name);
  if (map.end() == it) return false;
  if (it->type() != QVariant::Map) return false;
  const QVariantMap& inmap = it->toMap();
  if (!GetArgFromMap(inmap, "choiceID", v.choiceID)) return false;
  if (!GetArgFromMap(inmap, "menuName", v.menuName)) return false;
  if (!GetArgFromMap(inmap, "image", v.image)) return false;
  return true;
}

QDBusArgument& operator << (QDBusArgument& arg, const Common_TireStatus& v) {
  arg.beginStructure();
  arg << v.pressureTelltale;
  arg << v.leftFront;
  arg << v.rightFront;
  arg << v.leftRear;
  arg << v.rightRear;
  arg << v.innerLeftRear;
  arg << v.innerRightRear;
  arg.endStructure();
  return arg;
}
const QDBusArgument& operator >> (const QDBusArgument& arg, Common_TireStatus& v) {
  arg.beginStructure();
  arg >> v.pressureTelltale;
  arg >> v.leftFront;
  arg >> v.rightFront;
  arg >> v.leftRear;
  arg >> v.rightRear;
  arg >> v.innerLeftRear;
  arg >> v.innerRightRear;
  arg.endStructure();
  return arg;
}
QVariant ValueToVariant(const Common_TireStatus& v) {
  QVariantMap map;
  PutArgToMap(map, "pressureTelltale", v.pressureTelltale);
  PutArgToMap(map, "leftFront", v.leftFront);
  PutArgToMap(map, "rightFront", v.rightFront);
  PutArgToMap(map, "leftRear", v.leftRear);
  PutArgToMap(map, "rightRear", v.rightRear);
  PutArgToMap(map, "innerLeftRear", v.innerLeftRear);
  PutArgToMap(map, "innerRightRear", v.innerRightRear);
  return QVariant::fromValue(map);
}
QVariant ValueToVariant(const QList<Common_TireStatus>& v) {
  QList<QVariant> ret;
  for (QList<Common_TireStatus>::const_iterator i = v.begin(); i != v.end(); ++i) ret.append(ValueToVariant(*i));
  return QVariant::fromValue(ret);
}
void PutArgToMap(QVariantMap& map, const char* name, const Common_TireStatus& v) {
  map.insert(name, ValueToVariant(v));
}
bool VariantToValue(const QVariant& variant, Common_TireStatus& v) {
  if (variant.type() != QVariant::Map) return false;
  QVariantMap map = variant.toMap();
  if (!GetArgFromMap(map, "pressureTelltale", v.pressureTelltale)) return false;
  if (!GetArgFromMap(map, "leftFront", v.leftFront)) return false;
  if (!GetArgFromMap(map, "rightFront", v.rightFront)) return false;
  if (!GetArgFromMap(map, "leftRear", v.leftRear)) return false;
  if (!GetArgFromMap(map, "rightRear", v.rightRear)) return false;
  if (!GetArgFromMap(map, "innerLeftRear", v.innerLeftRear)) return false;
  if (!GetArgFromMap(map, "innerRightRear", v.innerRightRear)) return false;
  return true;
}
bool VariantToValue(const QVariant& variant, QList<Common_TireStatus>& v) {
  if (variant.type() != QVariant::List) return false;
  QList<QVariant> list = variant.toList();
  for (QList<QVariant>::const_iterator i = list.begin(); i != list.end(); ++i) {
    Common_TireStatus s;
    if (!VariantToValue(*i, s)) return false;
    v.append(s);
  }
  return true;
}
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_TireStatus& v) {
  QVariantMap::const_iterator it = map.find(name);
  if (map.end() == it) return false;
  if (it->type() != QVariant::Map) return false;
  const QVariantMap& inmap = it->toMap();
  if (!GetArgFromMap(inmap, "pressureTelltale", v.pressureTelltale)) return false;
  if (!GetArgFromMap(inmap, "leftFront", v.leftFront)) return false;
  if (!GetArgFromMap(inmap, "rightFront", v.rightFront)) return false;
  if (!GetArgFromMap(inmap, "leftRear", v.leftRear)) return false;
  if (!GetArgFromMap(inmap, "rightRear", v.rightRear)) return false;
  if (!GetArgFromMap(inmap, "innerLeftRear", v.innerLeftRear)) return false;
  if (!GetArgFromMap(inmap, "innerRightRear", v.innerRightRear)) return false;
  return true;
}

QDBusArgument& operator << (QDBusArgument& arg, const Common_ButtonCapabilities& v) {
  arg.beginStructure();
  arg << v.name;
  arg << v.shortPressAvailable;
  arg << v.longPressAvailable;
  arg << v.upDownAvailable;
  arg.endStructure();
  return arg;
}
const QDBusArgument& operator >> (const QDBusArgument& arg, Common_ButtonCapabilities& v) {
  arg.beginStructure();
  arg >> v.name;
  arg >> v.shortPressAvailable;
  arg >> v.longPressAvailable;
  arg >> v.upDownAvailable;
  arg.endStructure();
  return arg;
}
QVariant ValueToVariant(const Common_ButtonCapabilities& v) {
  QVariantMap map;
  PutArgToMap(map, "name", v.name);
  PutArgToMap(map, "shortPressAvailable", v.shortPressAvailable);
  PutArgToMap(map, "longPressAvailable", v.longPressAvailable);
  PutArgToMap(map, "upDownAvailable", v.upDownAvailable);
  return QVariant::fromValue(map);
}
QVariant ValueToVariant(const QList<Common_ButtonCapabilities>& v) {
  QList<QVariant> ret;
  for (QList<Common_ButtonCapabilities>::const_iterator i = v.begin(); i != v.end(); ++i) ret.append(ValueToVariant(*i));
  return QVariant::fromValue(ret);
}
void PutArgToMap(QVariantMap& map, const char* name, const Common_ButtonCapabilities& v) {
  map.insert(name, ValueToVariant(v));
}
bool VariantToValue(const QVariant& variant, Common_ButtonCapabilities& v) {
  if (variant.type() != QVariant::Map) return false;
  QVariantMap map = variant.toMap();
  if (!GetArgFromMap(map, "name", v.name)) return false;
  if (!GetArgFromMap(map, "shortPressAvailable", v.shortPressAvailable)) return false;
  if (!GetArgFromMap(map, "longPressAvailable", v.longPressAvailable)) return false;
  if (!GetArgFromMap(map, "upDownAvailable", v.upDownAvailable)) return false;
  return true;
}
bool VariantToValue(const QVariant& variant, QList<Common_ButtonCapabilities>& v) {
  if (variant.type() != QVariant::List) return false;
  QList<QVariant> list = variant.toList();
  for (QList<QVariant>::const_iterator i = list.begin(); i != list.end(); ++i) {
    Common_ButtonCapabilities s;
    if (!VariantToValue(*i, s)) return false;
    v.append(s);
  }
  return true;
}
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_ButtonCapabilities& v) {
  QVariantMap::const_iterator it = map.find(name);
  if (map.end() == it) return false;
  if (it->type() != QVariant::Map) return false;
  const QVariantMap& inmap = it->toMap();
  if (!GetArgFromMap(inmap, "name", v.name)) return false;
  if (!GetArgFromMap(inmap, "shortPressAvailable", v.shortPressAvailable)) return false;
  if (!GetArgFromMap(inmap, "longPressAvailable", v.longPressAvailable)) return false;
  if (!GetArgFromMap(inmap, "upDownAvailable", v.upDownAvailable)) return false;
  return true;
}

QDBusArgument& operator << (QDBusArgument& arg, const Common_BeltStatus& v) {
  arg.beginStructure();
  arg << v.driverBeltDeployed;
  arg << v.passengerBeltDeployed;
  arg << v.passengerBuckleBelted;
  arg << v.driverBuckleBelted;
  arg << v.leftRow2BuckleBelted;
  arg << v.passengerChildDetected;
  arg << v.rightRow2BuckleBelted;
  arg << v.middleRow2BuckleBelted;
  arg << v.middleRow3BuckleBelted;
  arg << v.leftRow3BuckleBelted;
  arg << v.rightRow3BuckleBelted;
  arg << v.leftRearInflatableBelted;
  arg << v.rightRearInflatableBelted;
  arg << v.middleRow1BeltDeployed;
  arg << v.middleRow1BuckleBelted;
  arg.endStructure();
  return arg;
}
const QDBusArgument& operator >> (const QDBusArgument& arg, Common_BeltStatus& v) {
  arg.beginStructure();
  arg >> v.driverBeltDeployed;
  arg >> v.passengerBeltDeployed;
  arg >> v.passengerBuckleBelted;
  arg >> v.driverBuckleBelted;
  arg >> v.leftRow2BuckleBelted;
  arg >> v.passengerChildDetected;
  arg >> v.rightRow2BuckleBelted;
  arg >> v.middleRow2BuckleBelted;
  arg >> v.middleRow3BuckleBelted;
  arg >> v.leftRow3BuckleBelted;
  arg >> v.rightRow3BuckleBelted;
  arg >> v.leftRearInflatableBelted;
  arg >> v.rightRearInflatableBelted;
  arg >> v.middleRow1BeltDeployed;
  arg >> v.middleRow1BuckleBelted;
  arg.endStructure();
  return arg;
}
QVariant ValueToVariant(const Common_BeltStatus& v) {
  QVariantMap map;
  PutArgToMap(map, "driverBeltDeployed", v.driverBeltDeployed);
  PutArgToMap(map, "passengerBeltDeployed", v.passengerBeltDeployed);
  PutArgToMap(map, "passengerBuckleBelted", v.passengerBuckleBelted);
  PutArgToMap(map, "driverBuckleBelted", v.driverBuckleBelted);
  PutArgToMap(map, "leftRow2BuckleBelted", v.leftRow2BuckleBelted);
  PutArgToMap(map, "passengerChildDetected", v.passengerChildDetected);
  PutArgToMap(map, "rightRow2BuckleBelted", v.rightRow2BuckleBelted);
  PutArgToMap(map, "middleRow2BuckleBelted", v.middleRow2BuckleBelted);
  PutArgToMap(map, "middleRow3BuckleBelted", v.middleRow3BuckleBelted);
  PutArgToMap(map, "leftRow3BuckleBelted", v.leftRow3BuckleBelted);
  PutArgToMap(map, "rightRow3BuckleBelted", v.rightRow3BuckleBelted);
  PutArgToMap(map, "leftRearInflatableBelted", v.leftRearInflatableBelted);
  PutArgToMap(map, "rightRearInflatableBelted", v.rightRearInflatableBelted);
  PutArgToMap(map, "middleRow1BeltDeployed", v.middleRow1BeltDeployed);
  PutArgToMap(map, "middleRow1BuckleBelted", v.middleRow1BuckleBelted);
  return QVariant::fromValue(map);
}
QVariant ValueToVariant(const QList<Common_BeltStatus>& v) {
  QList<QVariant> ret;
  for (QList<Common_BeltStatus>::const_iterator i = v.begin(); i != v.end(); ++i) ret.append(ValueToVariant(*i));
  return QVariant::fromValue(ret);
}
void PutArgToMap(QVariantMap& map, const char* name, const Common_BeltStatus& v) {
  map.insert(name, ValueToVariant(v));
}
bool VariantToValue(const QVariant& variant, Common_BeltStatus& v) {
  if (variant.type() != QVariant::Map) return false;
  QVariantMap map = variant.toMap();
  if (!GetArgFromMap(map, "driverBeltDeployed", v.driverBeltDeployed)) return false;
  if (!GetArgFromMap(map, "passengerBeltDeployed", v.passengerBeltDeployed)) return false;
  if (!GetArgFromMap(map, "passengerBuckleBelted", v.passengerBuckleBelted)) return false;
  if (!GetArgFromMap(map, "driverBuckleBelted", v.driverBuckleBelted)) return false;
  if (!GetArgFromMap(map, "leftRow2BuckleBelted", v.leftRow2BuckleBelted)) return false;
  if (!GetArgFromMap(map, "passengerChildDetected", v.passengerChildDetected)) return false;
  if (!GetArgFromMap(map, "rightRow2BuckleBelted", v.rightRow2BuckleBelted)) return false;
  if (!GetArgFromMap(map, "middleRow2BuckleBelted", v.middleRow2BuckleBelted)) return false;
  if (!GetArgFromMap(map, "middleRow3BuckleBelted", v.middleRow3BuckleBelted)) return false;
  if (!GetArgFromMap(map, "leftRow3BuckleBelted", v.leftRow3BuckleBelted)) return false;
  if (!GetArgFromMap(map, "rightRow3BuckleBelted", v.rightRow3BuckleBelted)) return false;
  if (!GetArgFromMap(map, "leftRearInflatableBelted", v.leftRearInflatableBelted)) return false;
  if (!GetArgFromMap(map, "rightRearInflatableBelted", v.rightRearInflatableBelted)) return false;
  if (!GetArgFromMap(map, "middleRow1BeltDeployed", v.middleRow1BeltDeployed)) return false;
  if (!GetArgFromMap(map, "middleRow1BuckleBelted", v.middleRow1BuckleBelted)) return false;
  return true;
}
bool VariantToValue(const QVariant& variant, QList<Common_BeltStatus>& v) {
  if (variant.type() != QVariant::List) return false;
  QList<QVariant> list = variant.toList();
  for (QList<QVariant>::const_iterator i = list.begin(); i != list.end(); ++i) {
    Common_BeltStatus s;
    if (!VariantToValue(*i, s)) return false;
    v.append(s);
  }
  return true;
}
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_BeltStatus& v) {
  QVariantMap::const_iterator it = map.find(name);
  if (map.end() == it) return false;
  if (it->type() != QVariant::Map) return false;
  const QVariantMap& inmap = it->toMap();
  if (!GetArgFromMap(inmap, "driverBeltDeployed", v.driverBeltDeployed)) return false;
  if (!GetArgFromMap(inmap, "passengerBeltDeployed", v.passengerBeltDeployed)) return false;
  if (!GetArgFromMap(inmap, "passengerBuckleBelted", v.passengerBuckleBelted)) return false;
  if (!GetArgFromMap(inmap, "driverBuckleBelted", v.driverBuckleBelted)) return false;
  if (!GetArgFromMap(inmap, "leftRow2BuckleBelted", v.leftRow2BuckleBelted)) return false;
  if (!GetArgFromMap(inmap, "passengerChildDetected", v.passengerChildDetected)) return false;
  if (!GetArgFromMap(inmap, "rightRow2BuckleBelted", v.rightRow2BuckleBelted)) return false;
  if (!GetArgFromMap(inmap, "middleRow2BuckleBelted", v.middleRow2BuckleBelted)) return false;
  if (!GetArgFromMap(inmap, "middleRow3BuckleBelted", v.middleRow3BuckleBelted)) return false;
  if (!GetArgFromMap(inmap, "leftRow3BuckleBelted", v.leftRow3BuckleBelted)) return false;
  if (!GetArgFromMap(inmap, "rightRow3BuckleBelted", v.rightRow3BuckleBelted)) return false;
  if (!GetArgFromMap(inmap, "leftRearInflatableBelted", v.leftRearInflatableBelted)) return false;
  if (!GetArgFromMap(inmap, "rightRearInflatableBelted", v.rightRearInflatableBelted)) return false;
  if (!GetArgFromMap(inmap, "middleRow1BeltDeployed", v.middleRow1BeltDeployed)) return false;
  if (!GetArgFromMap(inmap, "middleRow1BuckleBelted", v.middleRow1BuckleBelted)) return false;
  return true;
}

QDBusArgument& operator << (QDBusArgument& arg, const Common_DeviceInfo& v) {
  arg.beginStructure();
  arg << v.name;
  arg << v.id;
  arg.endStructure();
  return arg;
}
const QDBusArgument& operator >> (const QDBusArgument& arg, Common_DeviceInfo& v) {
  arg.beginStructure();
  arg >> v.name;
  arg >> v.id;
  arg.endStructure();
  return arg;
}
QVariant ValueToVariant(const Common_DeviceInfo& v) {
  QVariantMap map;
  PutArgToMap(map, "name", v.name);
  PutArgToMap(map, "id", v.id);
  return QVariant::fromValue(map);
}
QVariant ValueToVariant(const QList<Common_DeviceInfo>& v) {
  QList<QVariant> ret;
  for (QList<Common_DeviceInfo>::const_iterator i = v.begin(); i != v.end(); ++i) ret.append(ValueToVariant(*i));
  return QVariant::fromValue(ret);
}
void PutArgToMap(QVariantMap& map, const char* name, const Common_DeviceInfo& v) {
  map.insert(name, ValueToVariant(v));
}
bool VariantToValue(const QVariant& variant, Common_DeviceInfo& v) {
  if (variant.type() != QVariant::Map) return false;
  QVariantMap map = variant.toMap();
  if (!GetArgFromMap(map, "name", v.name)) return false;
  if (!GetArgFromMap(map, "id", v.id)) return false;
  return true;
}
bool VariantToValue(const QVariant& variant, QList<Common_DeviceInfo>& v) {
  if (variant.type() != QVariant::List) return false;
  QList<QVariant> list = variant.toList();
  for (QList<QVariant>::const_iterator i = list.begin(); i != list.end(); ++i) {
    Common_DeviceInfo s;
    if (!VariantToValue(*i, s)) return false;
    v.append(s);
  }
  return true;
}
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_DeviceInfo& v) {
  QVariantMap::const_iterator it = map.find(name);
  if (map.end() == it) return false;
  if (it->type() != QVariant::Map) return false;
  const QVariantMap& inmap = it->toMap();
  if (!GetArgFromMap(inmap, "name", v.name)) return false;
  if (!GetArgFromMap(inmap, "id", v.id)) return false;
  return true;
}

QDBusArgument& operator << (QDBusArgument& arg, const Common_MenuParams& v) {
  arg.beginStructure();
  arg << v.parentID;
  arg << v.position;
  arg << v.menuName;
  arg.endStructure();
  return arg;
}
const QDBusArgument& operator >> (const QDBusArgument& arg, Common_MenuParams& v) {
  arg.beginStructure();
  arg >> v.parentID;
  arg >> v.position;
  arg >> v.menuName;
  arg.endStructure();
  return arg;
}
QVariant ValueToVariant(const Common_MenuParams& v) {
  QVariantMap map;
  PutArgToMap(map, "parentID", v.parentID);
  PutArgToMap(map, "position", v.position);
  PutArgToMap(map, "menuName", v.menuName);
  return QVariant::fromValue(map);
}
QVariant ValueToVariant(const QList<Common_MenuParams>& v) {
  QList<QVariant> ret;
  for (QList<Common_MenuParams>::const_iterator i = v.begin(); i != v.end(); ++i) ret.append(ValueToVariant(*i));
  return QVariant::fromValue(ret);
}
void PutArgToMap(QVariantMap& map, const char* name, const Common_MenuParams& v) {
  map.insert(name, ValueToVariant(v));
}
bool VariantToValue(const QVariant& variant, Common_MenuParams& v) {
  if (variant.type() != QVariant::Map) return false;
  QVariantMap map = variant.toMap();
  if (!GetArgFromMap(map, "parentID", v.parentID)) return false;
  if (!GetArgFromMap(map, "position", v.position)) return false;
  if (!GetArgFromMap(map, "menuName", v.menuName)) return false;
  return true;
}
bool VariantToValue(const QVariant& variant, QList<Common_MenuParams>& v) {
  if (variant.type() != QVariant::List) return false;
  QList<QVariant> list = variant.toList();
  for (QList<QVariant>::const_iterator i = list.begin(); i != list.end(); ++i) {
    Common_MenuParams s;
    if (!VariantToValue(*i, s)) return false;
    v.append(s);
  }
  return true;
}
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_MenuParams& v) {
  QVariantMap::const_iterator it = map.find(name);
  if (map.end() == it) return false;
  if (it->type() != QVariant::Map) return false;
  const QVariantMap& inmap = it->toMap();
  if (!GetArgFromMap(inmap, "parentID", v.parentID)) return false;
  if (!GetArgFromMap(inmap, "position", v.position)) return false;
  if (!GetArgFromMap(inmap, "menuName", v.menuName)) return false;
  return true;
}

QDBusArgument& operator << (QDBusArgument& arg, const Common_SoftButtonCapabilities& v) {
  arg.beginStructure();
  arg << v.shortPressAvailable;
  arg << v.longPressAvailable;
  arg << v.upDownAvailable;
  arg << v.imageSupported;
  arg.endStructure();
  return arg;
}
const QDBusArgument& operator >> (const QDBusArgument& arg, Common_SoftButtonCapabilities& v) {
  arg.beginStructure();
  arg >> v.shortPressAvailable;
  arg >> v.longPressAvailable;
  arg >> v.upDownAvailable;
  arg >> v.imageSupported;
  arg.endStructure();
  return arg;
}
QVariant ValueToVariant(const Common_SoftButtonCapabilities& v) {
  QVariantMap map;
  PutArgToMap(map, "shortPressAvailable", v.shortPressAvailable);
  PutArgToMap(map, "longPressAvailable", v.longPressAvailable);
  PutArgToMap(map, "upDownAvailable", v.upDownAvailable);
  PutArgToMap(map, "imageSupported", v.imageSupported);
  return QVariant::fromValue(map);
}
QVariant ValueToVariant(const QList<Common_SoftButtonCapabilities>& v) {
  QList<QVariant> ret;
  for (QList<Common_SoftButtonCapabilities>::const_iterator i = v.begin(); i != v.end(); ++i) ret.append(ValueToVariant(*i));
  return QVariant::fromValue(ret);
}
void PutArgToMap(QVariantMap& map, const char* name, const Common_SoftButtonCapabilities& v) {
  map.insert(name, ValueToVariant(v));
}
bool VariantToValue(const QVariant& variant, Common_SoftButtonCapabilities& v) {
  if (variant.type() != QVariant::Map) return false;
  QVariantMap map = variant.toMap();
  if (!GetArgFromMap(map, "shortPressAvailable", v.shortPressAvailable)) return false;
  if (!GetArgFromMap(map, "longPressAvailable", v.longPressAvailable)) return false;
  if (!GetArgFromMap(map, "upDownAvailable", v.upDownAvailable)) return false;
  if (!GetArgFromMap(map, "imageSupported", v.imageSupported)) return false;
  return true;
}
bool VariantToValue(const QVariant& variant, QList<Common_SoftButtonCapabilities>& v) {
  if (variant.type() != QVariant::List) return false;
  QList<QVariant> list = variant.toList();
  for (QList<QVariant>::const_iterator i = list.begin(); i != list.end(); ++i) {
    Common_SoftButtonCapabilities s;
    if (!VariantToValue(*i, s)) return false;
    v.append(s);
  }
  return true;
}
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_SoftButtonCapabilities& v) {
  QVariantMap::const_iterator it = map.find(name);
  if (map.end() == it) return false;
  if (it->type() != QVariant::Map) return false;
  const QVariantMap& inmap = it->toMap();
  if (!GetArgFromMap(inmap, "shortPressAvailable", v.shortPressAvailable)) return false;
  if (!GetArgFromMap(inmap, "longPressAvailable", v.longPressAvailable)) return false;
  if (!GetArgFromMap(inmap, "upDownAvailable", v.upDownAvailable)) return false;
  if (!GetArgFromMap(inmap, "imageSupported", v.imageSupported)) return false;
  return true;
}

QDBusArgument& operator << (QDBusArgument& arg, const Common_DisplayCapabilities& v) {
  arg.beginStructure();
  arg << v.displayType;
  arg << v.textFields;
  arg << v.mediaClockFormats;
  arg << v.imageCapabilities;
  arg.endStructure();
  return arg;
}
const QDBusArgument& operator >> (const QDBusArgument& arg, Common_DisplayCapabilities& v) {
  arg.beginStructure();
  arg >> v.displayType;
  arg >> v.textFields;
  arg >> v.mediaClockFormats;
  arg >> v.imageCapabilities;
  arg.endStructure();
  return arg;
}
QVariant ValueToVariant(const Common_DisplayCapabilities& v) {
  QVariantMap map;
  PutArgToMap(map, "displayType", v.displayType);
  PutArgToMap(map, "textFields", v.textFields);
  PutArgToMap(map, "mediaClockFormats", v.mediaClockFormats);
  PutArgToMap(map, "imageCapabilities", v.imageCapabilities);
  return QVariant::fromValue(map);
}
QVariant ValueToVariant(const QList<Common_DisplayCapabilities>& v) {
  QList<QVariant> ret;
  for (QList<Common_DisplayCapabilities>::const_iterator i = v.begin(); i != v.end(); ++i) ret.append(ValueToVariant(*i));
  return QVariant::fromValue(ret);
}
void PutArgToMap(QVariantMap& map, const char* name, const Common_DisplayCapabilities& v) {
  map.insert(name, ValueToVariant(v));
}
bool VariantToValue(const QVariant& variant, Common_DisplayCapabilities& v) {
  if (variant.type() != QVariant::Map) return false;
  QVariantMap map = variant.toMap();
  if (!GetArgFromMap(map, "displayType", v.displayType)) return false;
  if (!GetArgFromMap(map, "textFields", v.textFields)) return false;
  if (!GetArgFromMap(map, "mediaClockFormats", v.mediaClockFormats)) return false;
  if (!GetArgFromMap(map, "imageCapabilities", v.imageCapabilities)) return false;
  return true;
}
bool VariantToValue(const QVariant& variant, QList<Common_DisplayCapabilities>& v) {
  if (variant.type() != QVariant::List) return false;
  QList<QVariant> list = variant.toList();
  for (QList<QVariant>::const_iterator i = list.begin(); i != list.end(); ++i) {
    Common_DisplayCapabilities s;
    if (!VariantToValue(*i, s)) return false;
    v.append(s);
  }
  return true;
}
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_DisplayCapabilities& v) {
  QVariantMap::const_iterator it = map.find(name);
  if (map.end() == it) return false;
  if (it->type() != QVariant::Map) return false;
  const QVariantMap& inmap = it->toMap();
  if (!GetArgFromMap(inmap, "displayType", v.displayType)) return false;
  if (!GetArgFromMap(inmap, "textFields", v.textFields)) return false;
  if (!GetArgFromMap(inmap, "mediaClockFormats", v.mediaClockFormats)) return false;
  if (!GetArgFromMap(inmap, "imageCapabilities", v.imageCapabilities)) return false;
  return true;
}

QDBusArgument& operator << (QDBusArgument& arg, const Common_VehicleType& v) {
  arg.beginStructure();
  arg << v.make;
  arg << v.model;
  arg << v.modelYear;
  arg << v.trim;
  arg.endStructure();
  return arg;
}
const QDBusArgument& operator >> (const QDBusArgument& arg, Common_VehicleType& v) {
  arg.beginStructure();
  arg >> v.make;
  arg >> v.model;
  arg >> v.modelYear;
  arg >> v.trim;
  arg.endStructure();
  return arg;
}
QVariant ValueToVariant(const Common_VehicleType& v) {
  QVariantMap map;
  PutArgToMap(map, "make", v.make);
  PutArgToMap(map, "model", v.model);
  PutArgToMap(map, "modelYear", v.modelYear);
  PutArgToMap(map, "trim", v.trim);
  return QVariant::fromValue(map);
}
QVariant ValueToVariant(const QList<Common_VehicleType>& v) {
  QList<QVariant> ret;
  for (QList<Common_VehicleType>::const_iterator i = v.begin(); i != v.end(); ++i) ret.append(ValueToVariant(*i));
  return QVariant::fromValue(ret);
}
void PutArgToMap(QVariantMap& map, const char* name, const Common_VehicleType& v) {
  map.insert(name, ValueToVariant(v));
}
bool VariantToValue(const QVariant& variant, Common_VehicleType& v) {
  if (variant.type() != QVariant::Map) return false;
  QVariantMap map = variant.toMap();
  if (!GetArgFromMap(map, "make", v.make)) return false;
  if (!GetArgFromMap(map, "model", v.model)) return false;
  if (!GetArgFromMap(map, "modelYear", v.modelYear)) return false;
  if (!GetArgFromMap(map, "trim", v.trim)) return false;
  return true;
}
bool VariantToValue(const QVariant& variant, QList<Common_VehicleType>& v) {
  if (variant.type() != QVariant::List) return false;
  QList<QVariant> list = variant.toList();
  for (QList<QVariant>::const_iterator i = list.begin(); i != list.end(); ++i) {
    Common_VehicleType s;
    if (!VariantToValue(*i, s)) return false;
    v.append(s);
  }
  return true;
}
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_VehicleType& v) {
  QVariantMap::const_iterator it = map.find(name);
  if (map.end() == it) return false;
  if (it->type() != QVariant::Map) return false;
  const QVariantMap& inmap = it->toMap();
  if (!GetArgFromMap(inmap, "make", v.make)) return false;
  if (!GetArgFromMap(inmap, "model", v.model)) return false;
  if (!GetArgFromMap(inmap, "modelYear", v.modelYear)) return false;
  if (!GetArgFromMap(inmap, "trim", v.trim)) return false;
  return true;
}

QDBusArgument& operator << (QDBusArgument& arg, const Common_MyKey& v) {
  arg.beginStructure();
  arg << v.e911Override;
  arg.endStructure();
  return arg;
}
const QDBusArgument& operator >> (const QDBusArgument& arg, Common_MyKey& v) {
  arg.beginStructure();
  arg >> v.e911Override;
  arg.endStructure();
  return arg;
}
QVariant ValueToVariant(const Common_MyKey& v) {
  QVariantMap map;
  PutArgToMap(map, "e911Override", v.e911Override);
  return QVariant::fromValue(map);
}
QVariant ValueToVariant(const QList<Common_MyKey>& v) {
  QList<QVariant> ret;
  for (QList<Common_MyKey>::const_iterator i = v.begin(); i != v.end(); ++i) ret.append(ValueToVariant(*i));
  return QVariant::fromValue(ret);
}
void PutArgToMap(QVariantMap& map, const char* name, const Common_MyKey& v) {
  map.insert(name, ValueToVariant(v));
}
bool VariantToValue(const QVariant& variant, Common_MyKey& v) {
  if (variant.type() != QVariant::Map) return false;
  QVariantMap map = variant.toMap();
  if (!GetArgFromMap(map, "e911Override", v.e911Override)) return false;
  return true;
}
bool VariantToValue(const QVariant& variant, QList<Common_MyKey>& v) {
  if (variant.type() != QVariant::List) return false;
  QList<QVariant> list = variant.toList();
  for (QList<QVariant>::const_iterator i = list.begin(); i != list.end(); ++i) {
    Common_MyKey s;
    if (!VariantToValue(*i, s)) return false;
    v.append(s);
  }
  return true;
}
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_MyKey& v) {
  QVariantMap::const_iterator it = map.find(name);
  if (map.end() == it) return false;
  if (it->type() != QVariant::Map) return false;
  const QVariantMap& inmap = it->toMap();
  if (!GetArgFromMap(inmap, "e911Override", v.e911Override)) return false;
  return true;
}

QDBusArgument& operator << (QDBusArgument& arg, const Common_DeviceStatus& v) {
  arg.beginStructure();
  arg << v.voiceRecOn;
  arg << v.btIconOn;
  arg << v.callActive;
  arg << v.phoneRoaming;
  arg << v.textMsgAvailable;
  arg << v.battLevelStatus;
  arg << v.stereoAudioOutputMuted;
  arg << v.monoAudioOutputMuted;
  arg << v.signalLevelStatus;
  arg << v.primaryAudioSource;
  arg << v.eCallEventActive;
  arg.endStructure();
  return arg;
}
const QDBusArgument& operator >> (const QDBusArgument& arg, Common_DeviceStatus& v) {
  arg.beginStructure();
  arg >> v.voiceRecOn;
  arg >> v.btIconOn;
  arg >> v.callActive;
  arg >> v.phoneRoaming;
  arg >> v.textMsgAvailable;
  arg >> v.battLevelStatus;
  arg >> v.stereoAudioOutputMuted;
  arg >> v.monoAudioOutputMuted;
  arg >> v.signalLevelStatus;
  arg >> v.primaryAudioSource;
  arg >> v.eCallEventActive;
  arg.endStructure();
  return arg;
}
QVariant ValueToVariant(const Common_DeviceStatus& v) {
  QVariantMap map;
  PutArgToMap(map, "voiceRecOn", v.voiceRecOn);
  PutArgToMap(map, "btIconOn", v.btIconOn);
  PutArgToMap(map, "callActive", v.callActive);
  PutArgToMap(map, "phoneRoaming", v.phoneRoaming);
  PutArgToMap(map, "textMsgAvailable", v.textMsgAvailable);
  PutArgToMap(map, "battLevelStatus", v.battLevelStatus);
  PutArgToMap(map, "stereoAudioOutputMuted", v.stereoAudioOutputMuted);
  PutArgToMap(map, "monoAudioOutputMuted", v.monoAudioOutputMuted);
  PutArgToMap(map, "signalLevelStatus", v.signalLevelStatus);
  PutArgToMap(map, "primaryAudioSource", v.primaryAudioSource);
  PutArgToMap(map, "eCallEventActive", v.eCallEventActive);
  return QVariant::fromValue(map);
}
QVariant ValueToVariant(const QList<Common_DeviceStatus>& v) {
  QList<QVariant> ret;
  for (QList<Common_DeviceStatus>::const_iterator i = v.begin(); i != v.end(); ++i) ret.append(ValueToVariant(*i));
  return QVariant::fromValue(ret);
}
void PutArgToMap(QVariantMap& map, const char* name, const Common_DeviceStatus& v) {
  map.insert(name, ValueToVariant(v));
}
bool VariantToValue(const QVariant& variant, Common_DeviceStatus& v) {
  if (variant.type() != QVariant::Map) return false;
  QVariantMap map = variant.toMap();
  if (!GetArgFromMap(map, "voiceRecOn", v.voiceRecOn)) return false;
  if (!GetArgFromMap(map, "btIconOn", v.btIconOn)) return false;
  if (!GetArgFromMap(map, "callActive", v.callActive)) return false;
  if (!GetArgFromMap(map, "phoneRoaming", v.phoneRoaming)) return false;
  if (!GetArgFromMap(map, "textMsgAvailable", v.textMsgAvailable)) return false;
  if (!GetArgFromMap(map, "battLevelStatus", v.battLevelStatus)) return false;
  if (!GetArgFromMap(map, "stereoAudioOutputMuted", v.stereoAudioOutputMuted)) return false;
  if (!GetArgFromMap(map, "monoAudioOutputMuted", v.monoAudioOutputMuted)) return false;
  if (!GetArgFromMap(map, "signalLevelStatus", v.signalLevelStatus)) return false;
  if (!GetArgFromMap(map, "primaryAudioSource", v.primaryAudioSource)) return false;
  if (!GetArgFromMap(map, "eCallEventActive", v.eCallEventActive)) return false;
  return true;
}
bool VariantToValue(const QVariant& variant, QList<Common_DeviceStatus>& v) {
  if (variant.type() != QVariant::List) return false;
  QList<QVariant> list = variant.toList();
  for (QList<QVariant>::const_iterator i = list.begin(); i != list.end(); ++i) {
    Common_DeviceStatus s;
    if (!VariantToValue(*i, s)) return false;
    v.append(s);
  }
  return true;
}
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_DeviceStatus& v) {
  QVariantMap::const_iterator it = map.find(name);
  if (map.end() == it) return false;
  if (it->type() != QVariant::Map) return false;
  const QVariantMap& inmap = it->toMap();
  if (!GetArgFromMap(inmap, "voiceRecOn", v.voiceRecOn)) return false;
  if (!GetArgFromMap(inmap, "btIconOn", v.btIconOn)) return false;
  if (!GetArgFromMap(inmap, "callActive", v.callActive)) return false;
  if (!GetArgFromMap(inmap, "phoneRoaming", v.phoneRoaming)) return false;
  if (!GetArgFromMap(inmap, "textMsgAvailable", v.textMsgAvailable)) return false;
  if (!GetArgFromMap(inmap, "battLevelStatus", v.battLevelStatus)) return false;
  if (!GetArgFromMap(inmap, "stereoAudioOutputMuted", v.stereoAudioOutputMuted)) return false;
  if (!GetArgFromMap(inmap, "monoAudioOutputMuted", v.monoAudioOutputMuted)) return false;
  if (!GetArgFromMap(inmap, "signalLevelStatus", v.signalLevelStatus)) return false;
  if (!GetArgFromMap(inmap, "primaryAudioSource", v.primaryAudioSource)) return false;
  if (!GetArgFromMap(inmap, "eCallEventActive", v.eCallEventActive)) return false;
  return true;
}

QDBusArgument& operator << (QDBusArgument& arg, const Common_TextFieldStruct& v) {
  arg.beginStructure();
  arg << v.fieldName;
  arg << v.fieldText;
  arg.endStructure();
  return arg;
}
const QDBusArgument& operator >> (const QDBusArgument& arg, Common_TextFieldStruct& v) {
  arg.beginStructure();
  arg >> v.fieldName;
  arg >> v.fieldText;
  arg.endStructure();
  return arg;
}
QVariant ValueToVariant(const Common_TextFieldStruct& v) {
  QVariantMap map;
  PutArgToMap(map, "fieldName", v.fieldName);
  PutArgToMap(map, "fieldText", v.fieldText);
  return QVariant::fromValue(map);
}
QVariant ValueToVariant(const QList<Common_TextFieldStruct>& v) {
  QList<QVariant> ret;
  for (QList<Common_TextFieldStruct>::const_iterator i = v.begin(); i != v.end(); ++i) ret.append(ValueToVariant(*i));
  return QVariant::fromValue(ret);
}
void PutArgToMap(QVariantMap& map, const char* name, const Common_TextFieldStruct& v) {
  map.insert(name, ValueToVariant(v));
}
bool VariantToValue(const QVariant& variant, Common_TextFieldStruct& v) {
  if (variant.type() != QVariant::Map) return false;
  QVariantMap map = variant.toMap();
  if (!GetArgFromMap(map, "fieldName", v.fieldName)) return false;
  if (!GetArgFromMap(map, "fieldText", v.fieldText)) return false;
  return true;
}
bool VariantToValue(const QVariant& variant, QList<Common_TextFieldStruct>& v) {
  if (variant.type() != QVariant::List) return false;
  QList<QVariant> list = variant.toList();
  for (QList<QVariant>::const_iterator i = list.begin(); i != list.end(); ++i) {
    Common_TextFieldStruct s;
    if (!VariantToValue(*i, s)) return false;
    v.append(s);
  }
  return true;
}
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_TextFieldStruct& v) {
  QVariantMap::const_iterator it = map.find(name);
  if (map.end() == it) return false;
  if (it->type() != QVariant::Map) return false;
  const QVariantMap& inmap = it->toMap();
  if (!GetArgFromMap(inmap, "fieldName", v.fieldName)) return false;
  if (!GetArgFromMap(inmap, "fieldText", v.fieldText)) return false;
  return true;
}

QDBusArgument& operator << (QDBusArgument& arg, const Common_Turn& v) {
  arg.beginStructure();
  arg << v.navigationText;
  arg << v.turnIcon;
  arg.endStructure();
  return arg;
}
const QDBusArgument& operator >> (const QDBusArgument& arg, Common_Turn& v) {
  arg.beginStructure();
  arg >> v.navigationText;
  arg >> v.turnIcon;
  arg.endStructure();
  return arg;
}
QVariant ValueToVariant(const Common_Turn& v) {
  QVariantMap map;
  PutArgToMap(map, "navigationText", v.navigationText);
  PutArgToMap(map, "turnIcon", v.turnIcon);
  return QVariant::fromValue(map);
}
QVariant ValueToVariant(const QList<Common_Turn>& v) {
  QList<QVariant> ret;
  for (QList<Common_Turn>::const_iterator i = v.begin(); i != v.end(); ++i) ret.append(ValueToVariant(*i));
  return QVariant::fromValue(ret);
}
void PutArgToMap(QVariantMap& map, const char* name, const Common_Turn& v) {
  map.insert(name, ValueToVariant(v));
}
bool VariantToValue(const QVariant& variant, Common_Turn& v) {
  if (variant.type() != QVariant::Map) return false;
  QVariantMap map = variant.toMap();
  if (!GetArgFromMap(map, "navigationText", v.navigationText)) return false;
  if (!GetArgFromMap(map, "turnIcon", v.turnIcon)) return false;
  return true;
}
bool VariantToValue(const QVariant& variant, QList<Common_Turn>& v) {
  if (variant.type() != QVariant::List) return false;
  QList<QVariant> list = variant.toList();
  for (QList<QVariant>::const_iterator i = list.begin(); i != list.end(); ++i) {
    Common_Turn s;
    if (!VariantToValue(*i, s)) return false;
    v.append(s);
  }
  return true;
}
bool GetArgFromMap(const QVariantMap& map, const char* name, Common_Turn& v) {
  QVariantMap::const_iterator it = map.find(name);
  if (map.end() == it) return false;
  if (it->type() != QVariant::Map) return false;
  const QVariantMap& inmap = it->toMap();
  if (!GetArgFromMap(inmap, "navigationText", v.navigationText)) return false;
  if (!GetArgFromMap(inmap, "turnIcon", v.turnIcon)) return false;
  return true;
}

ButtonsAdaptor::ButtonsAdaptor(QObject* parent) : QDBusAbstractAdaptor(parent) {}
void ButtonsAdaptor::SetApi(QQuickItem* api) {
  api_ = api;
  connect(api_, SIGNAL(onButtonEvent(int, int, QVariant)), this, SLOT(OnButtonEvent_qml(int, int, QVariant)));
  connect(api_, SIGNAL(onButtonPress(int, int, QVariant)), this, SLOT(OnButtonPress_qml(int, int, QVariant)));
}

QList<Common_ButtonCapabilities>  ButtonsAdaptor::GetCapabilities(OptionalArgument<Common_PresetBankCapabilities> & presetBankCapabilities_out) {
  QList<Common_ButtonCapabilities>  ret;
  QVariantMap in_arg;
  QVariant out_arg_v;
  if (!QMetaObject::invokeMethod(api_, "getCapabilities",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return ret;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return ret;}
  QVariantMap out_arg = out_arg_v.toMap();
  if (!GetArgFromMap(out_arg, "presetBankCapabilities", presetBankCapabilities_out)) {RaiseDbusError(this); return ret;}
  if (!GetArgFromMap(out_arg, "capabilities", ret)) RaiseDbusError(this);
  return ret;
}

void ButtonsAdaptor::OnButtonEvent_qml(int name, int mode, QVariant customButtonID) {
  OptionalArgument<int>  p_customButtonID;
  p_customButtonID.presence = !customButtonID.isNull();
  if (p_customButtonID.presence) {
    if (!VariantToValue(customButtonID, p_customButtonID.val)) return;
  }
  emit OnButtonEvent(name, mode, p_customButtonID);
}

void ButtonsAdaptor::OnButtonPress_qml(int name, int mode, QVariant customButtonID) {
  OptionalArgument<int>  p_customButtonID;
  p_customButtonID.presence = !customButtonID.isNull();
  if (p_customButtonID.presence) {
    if (!VariantToValue(customButtonID, p_customButtonID.val)) return;
  }
  emit OnButtonPress(name, mode, p_customButtonID);
}

BasicCommunicationAdaptor::BasicCommunicationAdaptor(QObject* parent) : QDBusAbstractAdaptor(parent) {}
void BasicCommunicationAdaptor::SetApi(QQuickItem* api) {
  api_ = api;
  connect(api_, SIGNAL(onReady()), this, SLOT(OnReady_qml()));
  connect(api_, SIGNAL(onStartDeviceDiscovery()), this, SLOT(OnStartDeviceDiscovery_qml()));
  connect(api_, SIGNAL(onUpdateDeviceList()), this, SLOT(OnUpdateDeviceList_qml()));
  connect(api_, SIGNAL(onDeviceChosen(QVariant)), this, SLOT(OnDeviceChosen_qml(QVariant)));
  connect(api_, SIGNAL(onFindApplications(QVariant)), this, SLOT(OnFindApplications_qml(QVariant)));
  connect(api_, SIGNAL(onAppActivated(int)), this, SLOT(OnAppActivated_qml(int)));
  connect(api_, SIGNAL(onAppDeactivated(int, int)), this, SLOT(OnAppDeactivated_qml(int, int)));
  connect(api_, SIGNAL(onAppRegistered(QVariant)), this, SLOT(OnAppRegistered_qml(QVariant)));
  connect(api_, SIGNAL(onAppUnregistered(int)), this, SLOT(OnAppUnregistered_qml(int)));
  connect(api_, SIGNAL(onExitApplication(int)), this, SLOT(OnExitApplication_qml(int)));
  connect(api_, SIGNAL(onExitAllApplications(int)), this, SLOT(OnExitAllApplications_qml(int)));
  connect(api_, SIGNAL(playTone()), this, SLOT(PlayTone_qml()));
}

void BasicCommunicationAdaptor::UpdateDeviceList(QList<Common_DeviceInfo>  deviceList_in) {
  QVariantMap in_arg;
  QVariant out_arg_v;
  PutArgToMap(in_arg, "deviceList", deviceList_in);
  if (!QMetaObject::invokeMethod(api_, "updateDeviceList",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return;}
  QVariantMap out_arg = out_arg_v.toMap();
}

bool BasicCommunicationAdaptor::AllowDeviceToConnect(Common_DeviceInfo device_in) {
  bool ret;
  QVariantMap in_arg;
  QVariant out_arg_v;
  PutArgToMap(in_arg, "device", device_in);
  if (!QMetaObject::invokeMethod(api_, "allowDeviceToConnect",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return ret;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return ret;}
  QVariantMap out_arg = out_arg_v.toMap();
  if (!GetArgFromMap(out_arg, "allow", ret)) RaiseDbusError(this);
  return ret;
}

void BasicCommunicationAdaptor::UpdateAppList(QList<Common_HMIApplication>  applications_in) {
  QVariantMap in_arg;
  QVariant out_arg_v;
  PutArgToMap(in_arg, "applications", applications_in);
  if (!QMetaObject::invokeMethod(api_, "updateAppList",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return;}
  QVariantMap out_arg = out_arg_v.toMap();
}

void BasicCommunicationAdaptor::ActivateApp(int appID_in) {
  QVariantMap in_arg;
  QVariant out_arg_v;
  PutArgToMap(in_arg, "appID", appID_in);
  if (!QMetaObject::invokeMethod(api_, "activateApp",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return;}
  QVariantMap out_arg = out_arg_v.toMap();
}

bool BasicCommunicationAdaptor::MixingAudioSupported() {
  bool ret;
  QVariantMap in_arg;
  QVariant out_arg_v;
  if (!QMetaObject::invokeMethod(api_, "mixingAudioSupported",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return ret;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return ret;}
  QVariantMap out_arg = out_arg_v.toMap();
  if (!GetArgFromMap(out_arg, "attenuatedSupported", ret)) RaiseDbusError(this);
  return ret;
}

bool BasicCommunicationAdaptor::AllowAllApps() {
  bool ret;
  QVariantMap in_arg;
  QVariant out_arg_v;
  if (!QMetaObject::invokeMethod(api_, "allowAllApps",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return ret;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return ret;}
  QVariantMap out_arg = out_arg_v.toMap();
  if (!GetArgFromMap(out_arg, "allowed", ret)) RaiseDbusError(this);
  return ret;
}

bool BasicCommunicationAdaptor::AllowApp(Common_HMIApplication app_in, OptionalArgument<QList<QString> >  appPermissions_in) {
  bool ret;
  QVariantMap in_arg;
  QVariant out_arg_v;
  PutArgToMap(in_arg, "app", app_in);
  PutArgToMap(in_arg, "appPermissions", appPermissions_in);
  if (!QMetaObject::invokeMethod(api_, "allowApp",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return ret;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return ret;}
  QVariantMap out_arg = out_arg_v.toMap();
  if (!GetArgFromMap(out_arg, "allowed", ret)) RaiseDbusError(this);
  return ret;
}

void BasicCommunicationAdaptor::OnReady_qml() {
  emit OnReady();
}

void BasicCommunicationAdaptor::OnStartDeviceDiscovery_qml() {
  emit OnStartDeviceDiscovery();
}

void BasicCommunicationAdaptor::OnUpdateDeviceList_qml() {
  emit OnUpdateDeviceList();
}

void BasicCommunicationAdaptor::OnDeviceChosen_qml(QVariant deviceInfo) {
  Common_DeviceInfo p_deviceInfo;
  if (!VariantToValue(deviceInfo, p_deviceInfo)) return;
  emit OnDeviceChosen(p_deviceInfo);
}

void BasicCommunicationAdaptor::OnFindApplications_qml(QVariant deviceInfo) {
  Common_DeviceInfo p_deviceInfo;
  if (!VariantToValue(deviceInfo, p_deviceInfo)) return;
  emit OnFindApplications(p_deviceInfo);
}

void BasicCommunicationAdaptor::OnAppActivated_qml(int appID) {
  emit OnAppActivated(appID);
}

void BasicCommunicationAdaptor::OnAppDeactivated_qml(int appID, int reason) {
  emit OnAppDeactivated(appID, reason);
}

void BasicCommunicationAdaptor::OnAppRegistered_qml(QVariant application) {
  Common_HMIApplication p_application;
  if (!VariantToValue(application, p_application)) return;
  emit OnAppRegistered(p_application);
}

void BasicCommunicationAdaptor::OnAppUnregistered_qml(int appID) {
  emit OnAppUnregistered(appID);
}

void BasicCommunicationAdaptor::OnExitApplication_qml(int appID) {
  emit OnExitApplication(appID);
}

void BasicCommunicationAdaptor::OnExitAllApplications_qml(int reason) {
  emit OnExitAllApplications(reason);
}

void BasicCommunicationAdaptor::PlayTone_qml() {
  emit PlayTone();
}

VRAdaptor::VRAdaptor(QObject* parent) : QDBusAbstractAdaptor(parent) {}
void VRAdaptor::SetApi(QQuickItem* api) {
  api_ = api;
  connect(api_, SIGNAL(started()), this, SLOT(Started_qml()));
  connect(api_, SIGNAL(stopped()), this, SLOT(Stopped_qml()));
  connect(api_, SIGNAL(onCommand(int, int)), this, SLOT(OnCommand_qml(int, int)));
  connect(api_, SIGNAL(onLanguageChange(int)), this, SLOT(OnLanguageChange_qml(int)));
}

bool VRAdaptor::IsReady() {
  bool ret;
  QVariantMap in_arg;
  QVariant out_arg_v;
  if (!QMetaObject::invokeMethod(api_, "isReady",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return ret;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return ret;}
  QVariantMap out_arg = out_arg_v.toMap();
  if (!GetArgFromMap(out_arg, "available", ret)) RaiseDbusError(this);
  return ret;
}

void VRAdaptor::AddCommand(int cmdID_in, QList<QString>  vrCommands_in, OptionalArgument<int>  appID_in) {
  QVariantMap in_arg;
  QVariant out_arg_v;
  PutArgToMap(in_arg, "cmdID", cmdID_in);
  PutArgToMap(in_arg, "vrCommands", vrCommands_in);
  PutArgToMap(in_arg, "appID", appID_in);
  if (!QMetaObject::invokeMethod(api_, "addCommand",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return;}
  QVariantMap out_arg = out_arg_v.toMap();
}

void VRAdaptor::DeleteCommand(int cmdID_in, int appID_in) {
  QVariantMap in_arg;
  QVariant out_arg_v;
  PutArgToMap(in_arg, "cmdID", cmdID_in);
  PutArgToMap(in_arg, "appID", appID_in);
  if (!QMetaObject::invokeMethod(api_, "deleteCommand",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return;}
  QVariantMap out_arg = out_arg_v.toMap();
}

void VRAdaptor::ChangeRegistration(int language_in, int appID_in) {
  QVariantMap in_arg;
  QVariant out_arg_v;
  PutArgToMap(in_arg, "language", language_in);
  PutArgToMap(in_arg, "appID", appID_in);
  if (!QMetaObject::invokeMethod(api_, "changeRegistration",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return;}
  QVariantMap out_arg = out_arg_v.toMap();
}

QList<int>  VRAdaptor::GetSupportedLanguages() {
  QList<int>  ret;
  QVariantMap in_arg;
  QVariant out_arg_v;
  if (!QMetaObject::invokeMethod(api_, "getSupportedLanguages",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return ret;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return ret;}
  QVariantMap out_arg = out_arg_v.toMap();
  if (!GetArgFromMap(out_arg, "languages", ret)) RaiseDbusError(this);
  return ret;
}

int VRAdaptor::GetLanguage() {
  int ret;
  QVariantMap in_arg;
  QVariant out_arg_v;
  if (!QMetaObject::invokeMethod(api_, "getLanguage",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return ret;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return ret;}
  QVariantMap out_arg = out_arg_v.toMap();
  if (!GetArgFromMap(out_arg, "language", ret)) RaiseDbusError(this);
  return ret;
}

void VRAdaptor::Started_qml() {
  emit Started();
}

void VRAdaptor::Stopped_qml() {
  emit Stopped();
}

void VRAdaptor::OnCommand_qml(int cmdID, int appID) {
  emit OnCommand(cmdID, appID);
}

void VRAdaptor::OnLanguageChange_qml(int language) {
  emit OnLanguageChange(language);
}

TTSAdaptor::TTSAdaptor(QObject* parent) : QDBusAbstractAdaptor(parent) {}
void TTSAdaptor::SetApi(QQuickItem* api) {
  api_ = api;
  connect(api_, SIGNAL(onLanguageChange(int)), this, SLOT(OnLanguageChange_qml(int)));
}

bool TTSAdaptor::IsReady() {
  bool ret;
  QVariantMap in_arg;
  QVariant out_arg_v;
  if (!QMetaObject::invokeMethod(api_, "isReady",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return ret;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return ret;}
  QVariantMap out_arg = out_arg_v.toMap();
  if (!GetArgFromMap(out_arg, "available", ret)) RaiseDbusError(this);
  return ret;
}

void TTSAdaptor::Speak(QList<Common_TTSChunk>  ttsChunks_in, int appID_in) {
  QVariantMap in_arg;
  QVariant out_arg_v;
  PutArgToMap(in_arg, "ttsChunks", ttsChunks_in);
  PutArgToMap(in_arg, "appID", appID_in);
  if (!QMetaObject::invokeMethod(api_, "speak",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return;}
  QVariantMap out_arg = out_arg_v.toMap();
}

void TTSAdaptor::StopSpeaking() {
  QVariantMap in_arg;
  QVariant out_arg_v;
  if (!QMetaObject::invokeMethod(api_, "stopSpeaking",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return;}
  QVariantMap out_arg = out_arg_v.toMap();
}

void TTSAdaptor::ChangeRegistration(int language_in, int appID_in) {
  QVariantMap in_arg;
  QVariant out_arg_v;
  PutArgToMap(in_arg, "language", language_in);
  PutArgToMap(in_arg, "appID", appID_in);
  if (!QMetaObject::invokeMethod(api_, "changeRegistration",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return;}
  QVariantMap out_arg = out_arg_v.toMap();
}

QList<int>  TTSAdaptor::GetSupportedLanguages() {
  QList<int>  ret;
  QVariantMap in_arg;
  QVariant out_arg_v;
  if (!QMetaObject::invokeMethod(api_, "getSupportedLanguages",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return ret;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return ret;}
  QVariantMap out_arg = out_arg_v.toMap();
  if (!GetArgFromMap(out_arg, "languages", ret)) RaiseDbusError(this);
  return ret;
}

int TTSAdaptor::GetLanguage() {
  int ret;
  QVariantMap in_arg;
  QVariant out_arg_v;
  if (!QMetaObject::invokeMethod(api_, "getLanguage",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return ret;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return ret;}
  QVariantMap out_arg = out_arg_v.toMap();
  if (!GetArgFromMap(out_arg, "language", ret)) RaiseDbusError(this);
  return ret;
}

void TTSAdaptor::SetGlobalProperties(OptionalArgument<QList<Common_TTSChunk> >  helpPrompt_in, OptionalArgument<QList<Common_TTSChunk> >  timeoutPrompt_in, int appID_in) {
  QVariantMap in_arg;
  QVariant out_arg_v;
  PutArgToMap(in_arg, "helpPrompt", helpPrompt_in);
  PutArgToMap(in_arg, "timeoutPrompt", timeoutPrompt_in);
  PutArgToMap(in_arg, "appID", appID_in);
  if (!QMetaObject::invokeMethod(api_, "setGlobalProperties",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return;}
  QVariantMap out_arg = out_arg_v.toMap();
}

void TTSAdaptor::OnLanguageChange_qml(int language) {
  emit OnLanguageChange(language);
}

UIAdaptor::UIAdaptor(QObject* parent) : QDBusAbstractAdaptor(parent) {}
void UIAdaptor::SetApi(QQuickItem* api) {
  api_ = api;
  connect(api_, SIGNAL(showNotification(QVariant, QVariant, int)), this, SLOT(ShowNotification_qml(QVariant, QVariant, int)));
  connect(api_, SIGNAL(onCommand(int, int)), this, SLOT(OnCommand_qml(int, int)));
  connect(api_, SIGNAL(onSystemContext(int)), this, SLOT(OnSystemContext_qml(int)));
  connect(api_, SIGNAL(onLanguageChange(int)), this, SLOT(OnLanguageChange_qml(int)));
  connect(api_, SIGNAL(onDriverDistraction(int)), this, SLOT(OnDriverDistraction_qml(int)));
}

OptionalArgument<int>  UIAdaptor::Alert(QList<Common_TextFieldStruct>  alertStrings_in, int duration_in, OptionalArgument<QList<Common_SoftButton> >  softButtons_in, int appID_in) {
  OptionalArgument<int>  ret;
  QVariantMap in_arg;
  QVariant out_arg_v;
  PutArgToMap(in_arg, "alertStrings", alertStrings_in);
  PutArgToMap(in_arg, "duration", duration_in);
  PutArgToMap(in_arg, "softButtons", softButtons_in);
  PutArgToMap(in_arg, "appID", appID_in);
  if (!QMetaObject::invokeMethod(api_, "alert",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return ret;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return ret;}
  QVariantMap out_arg = out_arg_v.toMap();
  if (!GetArgFromMap(out_arg, "tryAgainTime", ret)) RaiseDbusError(this);
  return ret;
}

void UIAdaptor::Show(QList<Common_TextFieldStruct>  showStrings_in, OptionalArgument<int>  alignment_in, OptionalArgument<Common_Image>  graphic_in, OptionalArgument<QList<Common_SoftButton> >  softButtons_in, OptionalArgument<QList<QString> >  customPresets_in, int appID_in) {
  QVariantMap in_arg;
  QVariant out_arg_v;
  PutArgToMap(in_arg, "showStrings", showStrings_in);
  PutArgToMap(in_arg, "alignment", alignment_in);
  PutArgToMap(in_arg, "graphic", graphic_in);
  PutArgToMap(in_arg, "softButtons", softButtons_in);
  PutArgToMap(in_arg, "customPresets", customPresets_in);
  PutArgToMap(in_arg, "appID", appID_in);
  if (!QMetaObject::invokeMethod(api_, "show",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return;}
  QVariantMap out_arg = out_arg_v.toMap();
}

void UIAdaptor::AddCommand(int cmdID_in, OptionalArgument<Common_MenuParams>  menuParams_in, OptionalArgument<Common_Image>  cmdIcon_in, int appID_in) {
  QVariantMap in_arg;
  QVariant out_arg_v;
  PutArgToMap(in_arg, "cmdID", cmdID_in);
  PutArgToMap(in_arg, "menuParams", menuParams_in);
  PutArgToMap(in_arg, "cmdIcon", cmdIcon_in);
  PutArgToMap(in_arg, "appID", appID_in);
  if (!QMetaObject::invokeMethod(api_, "addCommand",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return;}
  QVariantMap out_arg = out_arg_v.toMap();
}

void UIAdaptor::DeleteCommand(int cmdID_in, int appID_in) {
  QVariantMap in_arg;
  QVariant out_arg_v;
  PutArgToMap(in_arg, "cmdID", cmdID_in);
  PutArgToMap(in_arg, "appID", appID_in);
  if (!QMetaObject::invokeMethod(api_, "deleteCommand",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return;}
  QVariantMap out_arg = out_arg_v.toMap();
}

void UIAdaptor::AddSubMenu(int menuID_in, Common_MenuParams menuParams_in, int appID_in) {
  QVariantMap in_arg;
  QVariant out_arg_v;
  PutArgToMap(in_arg, "menuID", menuID_in);
  PutArgToMap(in_arg, "menuParams", menuParams_in);
  PutArgToMap(in_arg, "appID", appID_in);
  if (!QMetaObject::invokeMethod(api_, "addSubMenu",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return;}
  QVariantMap out_arg = out_arg_v.toMap();
}

void UIAdaptor::DeleteSubMenu(int menuID_in, int appID_in) {
  QVariantMap in_arg;
  QVariant out_arg_v;
  PutArgToMap(in_arg, "menuID", menuID_in);
  PutArgToMap(in_arg, "appID", appID_in);
  if (!QMetaObject::invokeMethod(api_, "deleteSubMenu",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return;}
  QVariantMap out_arg = out_arg_v.toMap();
}

OptionalArgument<int>  UIAdaptor::PerformInteraction(Common_TextFieldStruct initialText_in, QList<Common_Choice>  choiceSet_in, OptionalArgument<QList<Common_VrHelpItem> >  vrHelp_in, int timeout_in, int appID_in) {
  OptionalArgument<int>  ret;
  QVariantMap in_arg;
  QVariant out_arg_v;
  PutArgToMap(in_arg, "initialText", initialText_in);
  PutArgToMap(in_arg, "choiceSet", choiceSet_in);
  PutArgToMap(in_arg, "vrHelp", vrHelp_in);
  PutArgToMap(in_arg, "timeout", timeout_in);
  PutArgToMap(in_arg, "appID", appID_in);
  if (!QMetaObject::invokeMethod(api_, "performInteraction",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return ret;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return ret;}
  QVariantMap out_arg = out_arg_v.toMap();
  if (!GetArgFromMap(out_arg, "choiceID", ret)) RaiseDbusError(this);
  return ret;
}

void UIAdaptor::SetMediaClockTimer(OptionalArgument<Common_TimeFormat>  startTime_in, int updateMode_in, int appID_in) {
  QVariantMap in_arg;
  QVariant out_arg_v;
  PutArgToMap(in_arg, "startTime", startTime_in);
  PutArgToMap(in_arg, "updateMode", updateMode_in);
  PutArgToMap(in_arg, "appID", appID_in);
  if (!QMetaObject::invokeMethod(api_, "setMediaClockTimer",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return;}
  QVariantMap out_arg = out_arg_v.toMap();
}

void UIAdaptor::SetGlobalProperties(OptionalArgument<QString>  vrHelpTitle_in, OptionalArgument<QList<Common_VrHelpItem> >  vrHelp_in, int appID_in) {
  QVariantMap in_arg;
  QVariant out_arg_v;
  PutArgToMap(in_arg, "vrHelpTitle", vrHelpTitle_in);
  PutArgToMap(in_arg, "vrHelp", vrHelp_in);
  PutArgToMap(in_arg, "appID", appID_in);
  if (!QMetaObject::invokeMethod(api_, "setGlobalProperties",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return;}
  QVariantMap out_arg = out_arg_v.toMap();
}

void UIAdaptor::ShowVrHelp(OptionalArgument<QString>  vrHelpTitle_in, OptionalArgument<QList<Common_VrHelpItem> >  vrHelp_in, OptionalArgument<int>  appID_in) {
  QVariantMap in_arg;
  QVariant out_arg_v;
  PutArgToMap(in_arg, "vrHelpTitle", vrHelpTitle_in);
  PutArgToMap(in_arg, "vrHelp", vrHelp_in);
  PutArgToMap(in_arg, "appID", appID_in);
  if (!QMetaObject::invokeMethod(api_, "showVrHelp",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return;}
  QVariantMap out_arg = out_arg_v.toMap();
}

Common_DisplayCapabilities UIAdaptor::GetCapabilities(int& hmiZoneCapabilities_out, OptionalArgument<Common_SoftButtonCapabilities> & softButtonCapabilities_out) {
  Common_DisplayCapabilities ret;
  QVariantMap in_arg;
  QVariant out_arg_v;
  if (!QMetaObject::invokeMethod(api_, "getCapabilities",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return ret;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return ret;}
  QVariantMap out_arg = out_arg_v.toMap();
  if (!GetArgFromMap(out_arg, "hmiZoneCapabilities", hmiZoneCapabilities_out)) {RaiseDbusError(this); return ret;}
  if (!GetArgFromMap(out_arg, "softButtonCapabilities", softButtonCapabilities_out)) {RaiseDbusError(this); return ret;}
  if (!GetArgFromMap(out_arg, "displayCapabilities", ret)) RaiseDbusError(this);
  return ret;
}

void UIAdaptor::ChangeRegistration(int language_in, int appID_in) {
  QVariantMap in_arg;
  QVariant out_arg_v;
  PutArgToMap(in_arg, "language", language_in);
  PutArgToMap(in_arg, "appID", appID_in);
  if (!QMetaObject::invokeMethod(api_, "changeRegistration",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return;}
  QVariantMap out_arg = out_arg_v.toMap();
}

QList<int>  UIAdaptor::GetSupportedLanguages() {
  QList<int>  ret;
  QVariantMap in_arg;
  QVariant out_arg_v;
  if (!QMetaObject::invokeMethod(api_, "getSupportedLanguages",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return ret;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return ret;}
  QVariantMap out_arg = out_arg_v.toMap();
  if (!GetArgFromMap(out_arg, "languages", ret)) RaiseDbusError(this);
  return ret;
}

int UIAdaptor::GetLanguage() {
  int ret;
  QVariantMap in_arg;
  QVariant out_arg_v;
  if (!QMetaObject::invokeMethod(api_, "getLanguage",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return ret;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return ret;}
  QVariantMap out_arg = out_arg_v.toMap();
  if (!GetArgFromMap(out_arg, "language", ret)) RaiseDbusError(this);
  return ret;
}

void UIAdaptor::SetAppIcon(Common_Image syncFileName_in, int appID_in) {
  QVariantMap in_arg;
  QVariant out_arg_v;
  PutArgToMap(in_arg, "syncFileName", syncFileName_in);
  PutArgToMap(in_arg, "appID", appID_in);
  if (!QMetaObject::invokeMethod(api_, "setAppIcon",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return;}
  QVariantMap out_arg = out_arg_v.toMap();
}

int UIAdaptor::Slider(int numTicks_in, int position_in, QString sliderHeader_in, OptionalArgument<QList<QString> >  sliderFooter_in, int timeout_in, int appID_in) {
  int ret;
  QVariantMap in_arg;
  QVariant out_arg_v;
  PutArgToMap(in_arg, "numTicks", numTicks_in);
  PutArgToMap(in_arg, "position", position_in);
  PutArgToMap(in_arg, "sliderHeader", sliderHeader_in);
  PutArgToMap(in_arg, "sliderFooter", sliderFooter_in);
  PutArgToMap(in_arg, "timeout", timeout_in);
  PutArgToMap(in_arg, "appID", appID_in);
  if (!QMetaObject::invokeMethod(api_, "slider",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return ret;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return ret;}
  QVariantMap out_arg = out_arg_v.toMap();
  if (!GetArgFromMap(out_arg, "sliderPosition", ret)) RaiseDbusError(this);
  return ret;
}

void UIAdaptor::ScrollableMessage(Common_TextFieldStruct messageText_in, int timeout_in, OptionalArgument<QList<Common_SoftButton> >  softButtons_in, int appID_in) {
  QVariantMap in_arg;
  QVariant out_arg_v;
  PutArgToMap(in_arg, "messageText", messageText_in);
  PutArgToMap(in_arg, "timeout", timeout_in);
  PutArgToMap(in_arg, "softButtons", softButtons_in);
  PutArgToMap(in_arg, "appID", appID_in);
  if (!QMetaObject::invokeMethod(api_, "scrollableMessage",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return;}
  QVariantMap out_arg = out_arg_v.toMap();
}

void UIAdaptor::PerformAudioPassThru(QList<Common_TextFieldStruct>  audioPassThruDisplayTexts_in, OptionalArgument<int>  maxDuration_in) {
  QVariantMap in_arg;
  QVariant out_arg_v;
  PutArgToMap(in_arg, "audioPassThruDisplayTexts", audioPassThruDisplayTexts_in);
  PutArgToMap(in_arg, "maxDuration", maxDuration_in);
  if (!QMetaObject::invokeMethod(api_, "performAudioPassThru",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return;}
  QVariantMap out_arg = out_arg_v.toMap();
}

void UIAdaptor::EndAudioPassThru() {
  QVariantMap in_arg;
  QVariant out_arg_v;
  if (!QMetaObject::invokeMethod(api_, "endAudioPassThru",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return;}
  QVariantMap out_arg = out_arg_v.toMap();
}

bool UIAdaptor::IsReady() {
  bool ret;
  QVariantMap in_arg;
  QVariant out_arg_v;
  if (!QMetaObject::invokeMethod(api_, "isReady",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return ret;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return ret;}
  QVariantMap out_arg = out_arg_v.toMap();
  if (!GetArgFromMap(out_arg, "available", ret)) RaiseDbusError(this);
  return ret;
}

void UIAdaptor::ClosePopUp() {
  QVariantMap in_arg;
  QVariant out_arg_v;
  if (!QMetaObject::invokeMethod(api_, "closePopUp",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return;}
  QVariantMap out_arg = out_arg_v.toMap();
}

void UIAdaptor::ShowNotification_qml(QVariant text, QVariant icon, int timeout) {
  Common_TextFieldStruct p_text;
  if (!VariantToValue(text, p_text)) return;
  OptionalArgument<Common_Image>  p_icon;
  p_icon.presence = !icon.isNull();
  if (p_icon.presence) {
    if (!VariantToValue(icon, p_icon.val)) return;
  }
  emit ShowNotification(p_text, p_icon, timeout);
}

void UIAdaptor::OnCommand_qml(int cmdID, int appID) {
  emit OnCommand(cmdID, appID);
}

void UIAdaptor::OnSystemContext_qml(int systemContext) {
  emit OnSystemContext(systemContext);
}

void UIAdaptor::OnLanguageChange_qml(int language) {
  emit OnLanguageChange(language);
}

void UIAdaptor::OnDriverDistraction_qml(int state) {
  emit OnDriverDistraction(state);
}

NavigationAdaptor::NavigationAdaptor(QObject* parent) : QDBusAbstractAdaptor(parent) {}
void NavigationAdaptor::SetApi(QQuickItem* api) {
  api_ = api;
  connect(api_, SIGNAL(onTBTClientState(int)), this, SLOT(OnTBTClientState_qml(int)));
}

bool NavigationAdaptor::IsReady() {
  bool ret;
  QVariantMap in_arg;
  QVariant out_arg_v;
  if (!QMetaObject::invokeMethod(api_, "isReady",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return ret;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return ret;}
  QVariantMap out_arg = out_arg_v.toMap();
  if (!GetArgFromMap(out_arg, "available", ret)) RaiseDbusError(this);
  return ret;
}

void NavigationAdaptor::ShowConstantTBT(QList<Common_TextFieldStruct>  navigationTexts_in, OptionalArgument<Common_Image>  turnIcon_in, double distanceToManeuver_in, double distanceToManeuverScale_in, OptionalArgument<bool>  maneuverComplete_in, OptionalArgument<QList<Common_SoftButton> >  softButtons_in, int appID_in) {
  QVariantMap in_arg;
  QVariant out_arg_v;
  PutArgToMap(in_arg, "navigationTexts", navigationTexts_in);
  PutArgToMap(in_arg, "turnIcon", turnIcon_in);
  PutArgToMap(in_arg, "distanceToManeuver", distanceToManeuver_in);
  PutArgToMap(in_arg, "distanceToManeuverScale", distanceToManeuverScale_in);
  PutArgToMap(in_arg, "maneuverComplete", maneuverComplete_in);
  PutArgToMap(in_arg, "softButtons", softButtons_in);
  PutArgToMap(in_arg, "appID", appID_in);
  if (!QMetaObject::invokeMethod(api_, "showConstantTBT",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return;}
  QVariantMap out_arg = out_arg_v.toMap();
}

void NavigationAdaptor::AlertManeuver(OptionalArgument<QList<Common_SoftButton> >  softButtons_in) {
  QVariantMap in_arg;
  QVariant out_arg_v;
  PutArgToMap(in_arg, "softButtons", softButtons_in);
  if (!QMetaObject::invokeMethod(api_, "alertManeuver",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return;}
  QVariantMap out_arg = out_arg_v.toMap();
}

void NavigationAdaptor::UpdateTurnList(QList<Common_Turn>  turnList_in, OptionalArgument<QList<Common_SoftButton> >  softButtons_in, int appID_in) {
  QVariantMap in_arg;
  QVariant out_arg_v;
  PutArgToMap(in_arg, "turnList", turnList_in);
  PutArgToMap(in_arg, "softButtons", softButtons_in);
  PutArgToMap(in_arg, "appID", appID_in);
  if (!QMetaObject::invokeMethod(api_, "updateTurnList",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return;}
  QVariantMap out_arg = out_arg_v.toMap();
}

void NavigationAdaptor::OnTBTClientState_qml(int state) {
  emit OnTBTClientState(state);
}

VehicleInfoAdaptor::VehicleInfoAdaptor(QObject* parent) : QDBusAbstractAdaptor(parent) {}
void VehicleInfoAdaptor::SetApi(QQuickItem* api) {
  api_ = api;
  connect(api_, SIGNAL(onVehicleData(QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant)), this, SLOT(OnVehicleData_qml(QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant)));
}

bool VehicleInfoAdaptor::IsReady() {
  bool ret;
  QVariantMap in_arg;
  QVariant out_arg_v;
  if (!QMetaObject::invokeMethod(api_, "isReady",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return ret;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return ret;}
  QVariantMap out_arg = out_arg_v.toMap();
  if (!GetArgFromMap(out_arg, "available", ret)) RaiseDbusError(this);
  return ret;
}

Common_VehicleType VehicleInfoAdaptor::GetVehicleType() {
  Common_VehicleType ret;
  QVariantMap in_arg;
  QVariant out_arg_v;
  if (!QMetaObject::invokeMethod(api_, "getVehicleType",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return ret;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return ret;}
  QVariantMap out_arg = out_arg_v.toMap();
  if (!GetArgFromMap(out_arg, "vehicleType", ret)) RaiseDbusError(this);
  return ret;
}

OptionalArgument<QList<Common_DIDResult> >  VehicleInfoAdaptor::ReadDID(int ecuName_in, QList<int>  didLocation_in, int appID_in) {
  OptionalArgument<QList<Common_DIDResult> >  ret;
  QVariantMap in_arg;
  QVariant out_arg_v;
  PutArgToMap(in_arg, "ecuName", ecuName_in);
  PutArgToMap(in_arg, "didLocation", didLocation_in);
  PutArgToMap(in_arg, "appID", appID_in);
  if (!QMetaObject::invokeMethod(api_, "readDID",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return ret;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return ret;}
  QVariantMap out_arg = out_arg_v.toMap();
  if (!GetArgFromMap(out_arg, "didResult", ret)) RaiseDbusError(this);
  return ret;
}

int VehicleInfoAdaptor::GetDTCs(int ecuName_in, OptionalArgument<int>  dtcMask_in, int appID_in, OptionalArgument<QList<QString> > & dtc_out) {
  int ret;
  QVariantMap in_arg;
  QVariant out_arg_v;
  PutArgToMap(in_arg, "ecuName", ecuName_in);
  PutArgToMap(in_arg, "dtcMask", dtcMask_in);
  PutArgToMap(in_arg, "appID", appID_in);
  if (!QMetaObject::invokeMethod(api_, "getDTCs",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return ret;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return ret;}
  QVariantMap out_arg = out_arg_v.toMap();
  if (!GetArgFromMap(out_arg, "dtc", dtc_out)) {RaiseDbusError(this); return ret;}
  if (!GetArgFromMap(out_arg, "ecuHeader", ret)) RaiseDbusError(this);
  return ret;
}

OptionalArgument<Common_GPSData>  VehicleInfoAdaptor::GetVehicleData(OptionalArgument<bool>  gps_in, OptionalArgument<bool>  speed_in, OptionalArgument<bool>  rpm_in, OptionalArgument<bool>  fuelLevel_in, OptionalArgument<bool>  fuelLevel_State_in, OptionalArgument<bool>  instantFuelConsumption_in, OptionalArgument<bool>  externalTemperature_in, OptionalArgument<bool>  vin_in, OptionalArgument<bool>  prndl_in, OptionalArgument<bool>  tirePressure_in, OptionalArgument<bool>  odometer_in, OptionalArgument<bool>  beltStatus_in, OptionalArgument<bool>  bodyInformation_in, OptionalArgument<bool>  deviceStatus_in, OptionalArgument<bool>  driverBraking_in, OptionalArgument<bool>  wiperStatus_in, OptionalArgument<bool>  headLampStatus_in, OptionalArgument<bool>  engineTorque_in, OptionalArgument<bool>  accPedalPosition_in, OptionalArgument<bool>  steeringWheelAngle_in, OptionalArgument<bool>  myKey_in, int appID_in, OptionalArgument<double> & speed_out, OptionalArgument<int> & rpm_out, OptionalArgument<double> & fuelLevel_out, OptionalArgument<int> & fuelLevel_State_out, OptionalArgument<double> & instantFuelConsumption_out, OptionalArgument<double> & externalTemperature_out, OptionalArgument<QString> & vin_out, OptionalArgument<int> & prndl_out, OptionalArgument<Common_TireStatus> & tirePressure_out, OptionalArgument<int> & odometer_out, OptionalArgument<Common_BeltStatus> & beltStatus_out, OptionalArgument<Common_BodyInformation> & bodyInformation_out, OptionalArgument<Common_DeviceStatus> & deviceStatus_out, OptionalArgument<int> & driverBraking_out, OptionalArgument<int> & wiperStatus_out, OptionalArgument<Common_HeadLampStatus> & headLampStatus_out, OptionalArgument<double> & engineTorque_out, OptionalArgument<double> & accPedalPosition_out, OptionalArgument<double> & steeringWheelAngle_out, OptionalArgument<Common_MyKey> & myKey_out) {
  OptionalArgument<Common_GPSData>  ret;
  QVariantMap in_arg;
  QVariant out_arg_v;
  PutArgToMap(in_arg, "gps", gps_in);
  PutArgToMap(in_arg, "speed", speed_in);
  PutArgToMap(in_arg, "rpm", rpm_in);
  PutArgToMap(in_arg, "fuelLevel", fuelLevel_in);
  PutArgToMap(in_arg, "fuelLevel_State", fuelLevel_State_in);
  PutArgToMap(in_arg, "instantFuelConsumption", instantFuelConsumption_in);
  PutArgToMap(in_arg, "externalTemperature", externalTemperature_in);
  PutArgToMap(in_arg, "vin", vin_in);
  PutArgToMap(in_arg, "prndl", prndl_in);
  PutArgToMap(in_arg, "tirePressure", tirePressure_in);
  PutArgToMap(in_arg, "odometer", odometer_in);
  PutArgToMap(in_arg, "beltStatus", beltStatus_in);
  PutArgToMap(in_arg, "bodyInformation", bodyInformation_in);
  PutArgToMap(in_arg, "deviceStatus", deviceStatus_in);
  PutArgToMap(in_arg, "driverBraking", driverBraking_in);
  PutArgToMap(in_arg, "wiperStatus", wiperStatus_in);
  PutArgToMap(in_arg, "headLampStatus", headLampStatus_in);
  PutArgToMap(in_arg, "engineTorque", engineTorque_in);
  PutArgToMap(in_arg, "accPedalPosition", accPedalPosition_in);
  PutArgToMap(in_arg, "steeringWheelAngle", steeringWheelAngle_in);
  PutArgToMap(in_arg, "myKey", myKey_in);
  PutArgToMap(in_arg, "appID", appID_in);
  if (!QMetaObject::invokeMethod(api_, "getVehicleData",Qt::BlockingQueuedConnection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {RaiseDbusError(this); return ret;}
  if (out_arg_v.type() != QVariant::Map) {RaiseDbusError(this); return ret;}
  QVariantMap out_arg = out_arg_v.toMap();
  if (!GetArgFromMap(out_arg, "speed", speed_out)) {RaiseDbusError(this); return ret;}
  if (!GetArgFromMap(out_arg, "rpm", rpm_out)) {RaiseDbusError(this); return ret;}
  if (!GetArgFromMap(out_arg, "fuelLevel", fuelLevel_out)) {RaiseDbusError(this); return ret;}
  if (!GetArgFromMap(out_arg, "fuelLevel_State", fuelLevel_State_out)) {RaiseDbusError(this); return ret;}
  if (!GetArgFromMap(out_arg, "instantFuelConsumption", instantFuelConsumption_out)) {RaiseDbusError(this); return ret;}
  if (!GetArgFromMap(out_arg, "externalTemperature", externalTemperature_out)) {RaiseDbusError(this); return ret;}
  if (!GetArgFromMap(out_arg, "vin", vin_out)) {RaiseDbusError(this); return ret;}
  if (!GetArgFromMap(out_arg, "prndl", prndl_out)) {RaiseDbusError(this); return ret;}
  if (!GetArgFromMap(out_arg, "tirePressure", tirePressure_out)) {RaiseDbusError(this); return ret;}
  if (!GetArgFromMap(out_arg, "odometer", odometer_out)) {RaiseDbusError(this); return ret;}
  if (!GetArgFromMap(out_arg, "beltStatus", beltStatus_out)) {RaiseDbusError(this); return ret;}
  if (!GetArgFromMap(out_arg, "bodyInformation", bodyInformation_out)) {RaiseDbusError(this); return ret;}
  if (!GetArgFromMap(out_arg, "deviceStatus", deviceStatus_out)) {RaiseDbusError(this); return ret;}
  if (!GetArgFromMap(out_arg, "driverBraking", driverBraking_out)) {RaiseDbusError(this); return ret;}
  if (!GetArgFromMap(out_arg, "wiperStatus", wiperStatus_out)) {RaiseDbusError(this); return ret;}
  if (!GetArgFromMap(out_arg, "headLampStatus", headLampStatus_out)) {RaiseDbusError(this); return ret;}
  if (!GetArgFromMap(out_arg, "engineTorque", engineTorque_out)) {RaiseDbusError(this); return ret;}
  if (!GetArgFromMap(out_arg, "accPedalPosition", accPedalPosition_out)) {RaiseDbusError(this); return ret;}
  if (!GetArgFromMap(out_arg, "steeringWheelAngle", steeringWheelAngle_out)) {RaiseDbusError(this); return ret;}
  if (!GetArgFromMap(out_arg, "myKey", myKey_out)) {RaiseDbusError(this); return ret;}
  if (!GetArgFromMap(out_arg, "gps", ret)) RaiseDbusError(this);
  return ret;
}

void VehicleInfoAdaptor::OnVehicleData_qml(QVariant gps, QVariant speed, QVariant rpm, QVariant fuelLevel, QVariant fuelLevel_State, QVariant instantFuelConsumption, QVariant externalTemperature, QVariant vin, QVariant prndl, QVariant tirePressure, QVariant odometer, QVariant beltStatus, QVariant bodyInformation, QVariant deviceStatus, QVariant driverBraking, QVariant wiperStatus, QVariant headLampStatus, QVariant engineTorque, QVariant accPedalPosition, QVariant steeringWheelAngle, QVariant myKey) {
  OptionalArgument<Common_GPSData>  p_gps;
  p_gps.presence = !gps.isNull();
  if (p_gps.presence) {
    if (!VariantToValue(gps, p_gps.val)) return;
  }
  OptionalArgument<double>  p_speed;
  p_speed.presence = !speed.isNull();
  if (p_speed.presence) {
    if (!VariantToValue(speed, p_speed.val)) return;
  }
  OptionalArgument<int>  p_rpm;
  p_rpm.presence = !rpm.isNull();
  if (p_rpm.presence) {
    if (!VariantToValue(rpm, p_rpm.val)) return;
  }
  OptionalArgument<double>  p_fuelLevel;
  p_fuelLevel.presence = !fuelLevel.isNull();
  if (p_fuelLevel.presence) {
    if (!VariantToValue(fuelLevel, p_fuelLevel.val)) return;
  }
  OptionalArgument<int>  p_fuelLevel_State;
  p_fuelLevel_State.presence = !fuelLevel_State.isNull();
  if (p_fuelLevel_State.presence) {
    if (!VariantToValue(fuelLevel_State, p_fuelLevel_State.val)) return;
  }
  OptionalArgument<double>  p_instantFuelConsumption;
  p_instantFuelConsumption.presence = !instantFuelConsumption.isNull();
  if (p_instantFuelConsumption.presence) {
    if (!VariantToValue(instantFuelConsumption, p_instantFuelConsumption.val)) return;
  }
  OptionalArgument<double>  p_externalTemperature;
  p_externalTemperature.presence = !externalTemperature.isNull();
  if (p_externalTemperature.presence) {
    if (!VariantToValue(externalTemperature, p_externalTemperature.val)) return;
  }
  OptionalArgument<QString>  p_vin;
  p_vin.presence = !vin.isNull();
  if (p_vin.presence) {
    if (!VariantToValue(vin, p_vin.val)) return;
  }
  OptionalArgument<int>  p_prndl;
  p_prndl.presence = !prndl.isNull();
  if (p_prndl.presence) {
    if (!VariantToValue(prndl, p_prndl.val)) return;
  }
  OptionalArgument<Common_TireStatus>  p_tirePressure;
  p_tirePressure.presence = !tirePressure.isNull();
  if (p_tirePressure.presence) {
    if (!VariantToValue(tirePressure, p_tirePressure.val)) return;
  }
  OptionalArgument<int>  p_odometer;
  p_odometer.presence = !odometer.isNull();
  if (p_odometer.presence) {
    if (!VariantToValue(odometer, p_odometer.val)) return;
  }
  OptionalArgument<Common_BeltStatus>  p_beltStatus;
  p_beltStatus.presence = !beltStatus.isNull();
  if (p_beltStatus.presence) {
    if (!VariantToValue(beltStatus, p_beltStatus.val)) return;
  }
  OptionalArgument<Common_BodyInformation>  p_bodyInformation;
  p_bodyInformation.presence = !bodyInformation.isNull();
  if (p_bodyInformation.presence) {
    if (!VariantToValue(bodyInformation, p_bodyInformation.val)) return;
  }
  OptionalArgument<Common_DeviceStatus>  p_deviceStatus;
  p_deviceStatus.presence = !deviceStatus.isNull();
  if (p_deviceStatus.presence) {
    if (!VariantToValue(deviceStatus, p_deviceStatus.val)) return;
  }
  OptionalArgument<int>  p_driverBraking;
  p_driverBraking.presence = !driverBraking.isNull();
  if (p_driverBraking.presence) {
    if (!VariantToValue(driverBraking, p_driverBraking.val)) return;
  }
  OptionalArgument<int>  p_wiperStatus;
  p_wiperStatus.presence = !wiperStatus.isNull();
  if (p_wiperStatus.presence) {
    if (!VariantToValue(wiperStatus, p_wiperStatus.val)) return;
  }
  OptionalArgument<Common_HeadLampStatus>  p_headLampStatus;
  p_headLampStatus.presence = !headLampStatus.isNull();
  if (p_headLampStatus.presence) {
    if (!VariantToValue(headLampStatus, p_headLampStatus.val)) return;
  }
  OptionalArgument<double>  p_engineTorque;
  p_engineTorque.presence = !engineTorque.isNull();
  if (p_engineTorque.presence) {
    if (!VariantToValue(engineTorque, p_engineTorque.val)) return;
  }
  OptionalArgument<double>  p_accPedalPosition;
  p_accPedalPosition.presence = !accPedalPosition.isNull();
  if (p_accPedalPosition.presence) {
    if (!VariantToValue(accPedalPosition, p_accPedalPosition.val)) return;
  }
  OptionalArgument<double>  p_steeringWheelAngle;
  p_steeringWheelAngle.presence = !steeringWheelAngle.isNull();
  if (p_steeringWheelAngle.presence) {
    if (!VariantToValue(steeringWheelAngle, p_steeringWheelAngle.val)) return;
  }
  OptionalArgument<Common_MyKey>  p_myKey;
  p_myKey.presence = !myKey.isNull();
  if (p_myKey.presence) {
    if (!VariantToValue(myKey, p_myKey.val)) return;
  }
  emit OnVehicleData(p_gps, p_speed, p_rpm, p_fuelLevel, p_fuelLevel_State, p_instantFuelConsumption, p_externalTemperature, p_vin, p_prndl, p_tirePressure, p_odometer, p_beltStatus, p_bodyInformation, p_deviceStatus, p_driverBraking, p_wiperStatus, p_headLampStatus, p_engineTorque, p_accPedalPosition, p_steeringWheelAngle, p_myKey);
}

void RegisterDbusMetatypes() {
qDBusRegisterMetaType<Common_SingleTireStatus>();
qDBusRegisterMetaType<OptionalArgument<Common_SingleTireStatus> >();
qDBusRegisterMetaType<QList<Common_SingleTireStatus> >();
qDBusRegisterMetaType<OptionalArgument<QList<Common_SingleTireStatus> > >();
qDBusRegisterMetaType<Common_TTSChunk>();
qDBusRegisterMetaType<OptionalArgument<Common_TTSChunk> >();
qDBusRegisterMetaType<QList<Common_TTSChunk> >();
qDBusRegisterMetaType<OptionalArgument<QList<Common_TTSChunk> > >();
qDBusRegisterMetaType<Common_BodyInformation>();
qDBusRegisterMetaType<OptionalArgument<Common_BodyInformation> >();
qDBusRegisterMetaType<QList<Common_BodyInformation> >();
qDBusRegisterMetaType<OptionalArgument<QList<Common_BodyInformation> > >();
qDBusRegisterMetaType<Common_Image>();
qDBusRegisterMetaType<OptionalArgument<Common_Image> >();
qDBusRegisterMetaType<QList<Common_Image> >();
qDBusRegisterMetaType<OptionalArgument<QList<Common_Image> > >();
qDBusRegisterMetaType<Common_VrHelpItem>();
qDBusRegisterMetaType<OptionalArgument<Common_VrHelpItem> >();
qDBusRegisterMetaType<QList<Common_VrHelpItem> >();
qDBusRegisterMetaType<OptionalArgument<QList<Common_VrHelpItem> > >();
qDBusRegisterMetaType<Common_HeadLampStatus>();
qDBusRegisterMetaType<OptionalArgument<Common_HeadLampStatus> >();
qDBusRegisterMetaType<QList<Common_HeadLampStatus> >();
qDBusRegisterMetaType<OptionalArgument<QList<Common_HeadLampStatus> > >();
qDBusRegisterMetaType<Common_TimeFormat>();
qDBusRegisterMetaType<OptionalArgument<Common_TimeFormat> >();
qDBusRegisterMetaType<QList<Common_TimeFormat> >();
qDBusRegisterMetaType<OptionalArgument<QList<Common_TimeFormat> > >();
qDBusRegisterMetaType<Common_GPSData>();
qDBusRegisterMetaType<OptionalArgument<Common_GPSData> >();
qDBusRegisterMetaType<QList<Common_GPSData> >();
qDBusRegisterMetaType<OptionalArgument<QList<Common_GPSData> > >();
qDBusRegisterMetaType<Common_SoftButton>();
qDBusRegisterMetaType<OptionalArgument<Common_SoftButton> >();
qDBusRegisterMetaType<QList<Common_SoftButton> >();
qDBusRegisterMetaType<OptionalArgument<QList<Common_SoftButton> > >();
qDBusRegisterMetaType<Common_DIDResult>();
qDBusRegisterMetaType<OptionalArgument<Common_DIDResult> >();
qDBusRegisterMetaType<QList<Common_DIDResult> >();
qDBusRegisterMetaType<OptionalArgument<QList<Common_DIDResult> > >();
qDBusRegisterMetaType<Common_PresetBankCapabilities>();
qDBusRegisterMetaType<OptionalArgument<Common_PresetBankCapabilities> >();
qDBusRegisterMetaType<QList<Common_PresetBankCapabilities> >();
qDBusRegisterMetaType<OptionalArgument<QList<Common_PresetBankCapabilities> > >();
qDBusRegisterMetaType<Common_HMIApplication>();
qDBusRegisterMetaType<OptionalArgument<Common_HMIApplication> >();
qDBusRegisterMetaType<QList<Common_HMIApplication> >();
qDBusRegisterMetaType<OptionalArgument<QList<Common_HMIApplication> > >();
qDBusRegisterMetaType<Common_Choice>();
qDBusRegisterMetaType<OptionalArgument<Common_Choice> >();
qDBusRegisterMetaType<QList<Common_Choice> >();
qDBusRegisterMetaType<OptionalArgument<QList<Common_Choice> > >();
qDBusRegisterMetaType<Common_TireStatus>();
qDBusRegisterMetaType<OptionalArgument<Common_TireStatus> >();
qDBusRegisterMetaType<QList<Common_TireStatus> >();
qDBusRegisterMetaType<OptionalArgument<QList<Common_TireStatus> > >();
qDBusRegisterMetaType<Common_ButtonCapabilities>();
qDBusRegisterMetaType<OptionalArgument<Common_ButtonCapabilities> >();
qDBusRegisterMetaType<QList<Common_ButtonCapabilities> >();
qDBusRegisterMetaType<OptionalArgument<QList<Common_ButtonCapabilities> > >();
qDBusRegisterMetaType<Common_BeltStatus>();
qDBusRegisterMetaType<OptionalArgument<Common_BeltStatus> >();
qDBusRegisterMetaType<QList<Common_BeltStatus> >();
qDBusRegisterMetaType<OptionalArgument<QList<Common_BeltStatus> > >();
qDBusRegisterMetaType<Common_DeviceInfo>();
qDBusRegisterMetaType<OptionalArgument<Common_DeviceInfo> >();
qDBusRegisterMetaType<QList<Common_DeviceInfo> >();
qDBusRegisterMetaType<OptionalArgument<QList<Common_DeviceInfo> > >();
qDBusRegisterMetaType<Common_MenuParams>();
qDBusRegisterMetaType<OptionalArgument<Common_MenuParams> >();
qDBusRegisterMetaType<QList<Common_MenuParams> >();
qDBusRegisterMetaType<OptionalArgument<QList<Common_MenuParams> > >();
qDBusRegisterMetaType<Common_SoftButtonCapabilities>();
qDBusRegisterMetaType<OptionalArgument<Common_SoftButtonCapabilities> >();
qDBusRegisterMetaType<QList<Common_SoftButtonCapabilities> >();
qDBusRegisterMetaType<OptionalArgument<QList<Common_SoftButtonCapabilities> > >();
qDBusRegisterMetaType<Common_DisplayCapabilities>();
qDBusRegisterMetaType<OptionalArgument<Common_DisplayCapabilities> >();
qDBusRegisterMetaType<QList<Common_DisplayCapabilities> >();
qDBusRegisterMetaType<OptionalArgument<QList<Common_DisplayCapabilities> > >();
qDBusRegisterMetaType<Common_VehicleType>();
qDBusRegisterMetaType<OptionalArgument<Common_VehicleType> >();
qDBusRegisterMetaType<QList<Common_VehicleType> >();
qDBusRegisterMetaType<OptionalArgument<QList<Common_VehicleType> > >();
qDBusRegisterMetaType<Common_MyKey>();
qDBusRegisterMetaType<OptionalArgument<Common_MyKey> >();
qDBusRegisterMetaType<QList<Common_MyKey> >();
qDBusRegisterMetaType<OptionalArgument<QList<Common_MyKey> > >();
qDBusRegisterMetaType<Common_DeviceStatus>();
qDBusRegisterMetaType<OptionalArgument<Common_DeviceStatus> >();
qDBusRegisterMetaType<QList<Common_DeviceStatus> >();
qDBusRegisterMetaType<OptionalArgument<QList<Common_DeviceStatus> > >();
qDBusRegisterMetaType<Common_TextFieldStruct>();
qDBusRegisterMetaType<OptionalArgument<Common_TextFieldStruct> >();
qDBusRegisterMetaType<QList<Common_TextFieldStruct> >();
qDBusRegisterMetaType<OptionalArgument<QList<Common_TextFieldStruct> > >();
qDBusRegisterMetaType<Common_Turn>();
qDBusRegisterMetaType<OptionalArgument<Common_Turn> >();
qDBusRegisterMetaType<QList<Common_Turn> >();
qDBusRegisterMetaType<OptionalArgument<QList<Common_Turn> > >();
}
