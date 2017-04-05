pragma Singleton
import QtQuick 2.0

QtObject {
    property int rowSpacing: 2 //spacing between UI elements
    property int radius: 3 //radius for UI elements
    property int margins: 10 //margins for UI elements
    property int dataStructureContainerMargin: 20 //margins for data structure container
    property int columnSpacing: 10 //spacing for column structures
    property int buttonWidth: 150 //width for buttons
    property int windowHeight: 600 //height for main window
    property int componentHeight: 400 //height for Tab view

    property int customInputWidthInput: 140 //width for Input for CustomInput element
    property string inputBorderColor: "green" //border color for CustomInput element

    property int textBoxHeight: 25 //height for LabelText element
    property int textBoxWidth: 255 //width for LabelText element
    property int labelWidth: 120 //width for text in LabelText element
    property int labelTextLeftMargin: 10 //left margin for LabelText element
    property int labelTextLabelLeftMargin: 5 //left margin for text in LabelText element
    property string colorCastomInputBackground: "#E2F2FD" //color for input background for LabelText element
    property string colorLabelTextBackground: "#C6DCEC" //color for label background for LabelText element
    property string colorCastomInputText: "#1C405B" //color for Input background for LabelText element

    property string colorButtonGradTop: "#006F97" //Dark blue color for button
    property string colorButtonGradBot: "#5D97C3" //Light blue color for button and Logger tab border
    property string colorButtonBorder: "#1B6CA9" //Dark blue color for button border
    property string colorButtonText: "#CBE4F7" //Light blue color for button Text, column label, Logger label
    property string colorMainDarkGradient: "#1C496C" //Dark blue color for columns, TabViews and Logger
    property string colorMainLightGradient: "#0091AE" //Light blue color for columns and TabViews

    property string colorLogError: "#FF9595" //Red color for logger
    property string colorLogReceived: "#95FFAB" //Green color for logger
    property string colorLogSend: "#94FFF9" //Blue color for logger
}
