// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import "FStringFormat.js" as CB

QtObject {
    // output string for which is used as output
    property string output

    // this string defines the format of the output
    property string format

    // this is the data to be formatted into the output
    property variant data


    onFormatChanged: {
        CB.splitFormatString(format);
    }

    onDataChanged: {
        doFormat();
    }

    function doFormat()
    {
        output = CB.formatOutput(data);
    }
}
