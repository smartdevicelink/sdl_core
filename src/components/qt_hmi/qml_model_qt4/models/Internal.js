/**
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

var MenuItemType = {
    MI_NODE: 0,
    MI_SUBMENU: 1,
    MI_PARENT: 2
}

function activePageChoose(flickElement, pages) {
    if (flickElement.contentX <= 0) {
        return 0
    }
    else if ( (flickElement.contentWidth - flickElement.contentX) < flickElement.width) {
        return pages -1
    }
    else {
        if ( (flickElement.contentX % flickElement.width) >= (1/6 * flickElement.width)) {
            return Math.ceil(flickElement.contentX / flickElement.width)
        }
        else if ( (flickElement.contentX % flickElement.width) > 0) {
            return Math.floor(flickElement.contentX / flickElement.width)
        }
    }
}

var MediaClockUpdateMode = {
    MCU_COUNTUP: 0,
    MCU_COUNTDOWN: 1,
}

var MediaClockRunningMode = {
    MCR_RUNNING: 0,
    MCR_STOPPED: 1
}

function appendVrHelpItem (arrayElement, index, array) {
    this.append({
        text: arrayElement.text,
        image: arrayElement.image ? arrayElement.image : "",
        position: arrayElement.position
        })
}

function checkBit(value, bitPosition) {
    return (value & (1 << bitPosition))
}

function getArrayForPresetRow() {
    var array = []
    for (var i = 0; i < dataContainer.currentApplication.customPresets.count; i++) {
        array.push(dataContainer.currentApplication.customPresets.get(i).text)
    }
    return array
}

function pad (string, length, lead) {
    if (!lead) {
        lead = '0'
    }
    var paddedString = "" + string
    while (paddedString.length < length) {
        paddedString = lead + paddedString
    }
    return paddedString
}

function hmsTime (hours, minutes, seconds) {
    return 60 * (60 * hours + minutes) + seconds
}

function hmsTimeToString (hmsTime) {
    if (hmsTime === -1) {
        return "0:00:00"
    }

    var _time = hmsTime
    var seconds = _time % 60
    _time -= seconds
    _time /= 60
    var minutes = _time % 60
    _time -= minutes
    _time /= 60
    var hours = _time
    var string = ""
    string += hours
    string += ":"
    string += pad(minutes, 2)
    string += ":"
    string += pad(seconds, 2)
    return string
}

function stringToHmsTime (hmsString) {
    console.debug("enter: \"" + hmsString + "\"")
    var substrings = hmsString.split(":")
    var seconds = substrings.length > 0 ? parseInt(substrings.pop()) : 0
    if (seconds >= 60) {
        console.log("incorrect field \"seconds\": " + seconds)
        console.debug("exit")
        return 0
    }
    var minutes = substrings.length > 0 ? parseInt(substrings.pop()) : 0
    if (minutes >= 60) {
        console.log("incorrect field \"minutes\": " + minutes)
        console.debug("exit")
        return 0
    }
    var hours = substrings.length > 0 ? parseInt(substrings.pop()) : 0
    console.debug("exit")
    return hmsTime(hours, minutes, seconds)
}

function mediaClockUpdateModeToString (mediaClockUpdateMode) {
    switch (mediaClockUpdateMode) {
        case MediaClockUpdateMode.MCU_COUNTUP:
            return "Internal.MediaClockUpdateMode.MCU_COUNTUP"
        case MediaClockUpdateMode.MCU_COUNTDOWN:
            return "Internal.MediaClockUpdateMode.MCU_COUNTDOWN"
        default:
            return ""
    }
}

function mediaClockRunningModeToString (mediaClockRunningMode) {
    switch (mediaClockRunningMode) {
        case MediaClockRunningMode.MCR_RUNNING:
            return "Internal.MediaClockRunningMode.MCR_RUNNING"
        case MediaClockRunningMode.MCR_STOPPED:
            return "Internal.MediaClockRunningMode.MCR_STOPPED"
        default:
            return ""
    }
}
