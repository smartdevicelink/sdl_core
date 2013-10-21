/**
 * @file Constants.js
 * @brief Namespace of constants.
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
.pragma library

// Colors

// Main color of HMI model(now it ~lightblue). Buttons, text, icons of this color.
var primaryColor = "#1d81d5"

// Secondary color of HMI model, backgroud of this color.
var secondaryColor = "black"

// Color of text at buttonts that are inactive (at phone keyboard, when no any contact started with current letter).
var inactiveButtonTextColor = "grey"

// Color of text at contact list screen view (Name, number and number type).
var contactTextColor = "lightblue"

// Color of text at buttons, when they are pressed.
var pressedButtonTextColor = "black"

// Color of text at "white" buttons, for all other buttons (which most) use primaryColor.
var releasedButtonTextColor = "white"

// Transparent background.
var transparentColor = "#000000"
//--------------------------------------------------------------------------------------

// Other

// Font size for most of text in HMI model.
var fontSize = 20

// Offset from the edges of the screen.
var margin = 30

// Spacing between contacts at contact screen view.
var contactListSpacing = 30

// Minimal width and height of screen (screen wont resize, if height and width are lower).
var mainScreenMinWidth = 800
var mainScreenMiHeight = 600

// Duration of appearance of icon/button at icon/button menus.
var animationDuration = 500

// Width for long oval button
var longOvalButtonWidth = 163

// Size icon for buttons
var iconButtonSize = 30

// Size icon for item list
var iconItemListSize = 40

// Height item list
var itemListHeight = 60

// Spasing between item on views
var generalSpacing = 10

// -------------------------

var sliderBarWidth = 400
var sliderBarHeight = 50
var sliderBarRadius = 3
var sliderBarBorderWidth = 1
var sliderTextColor = "white"
var sliderBarBorderColor = "white"
var sliderBarFillColor = "white"
