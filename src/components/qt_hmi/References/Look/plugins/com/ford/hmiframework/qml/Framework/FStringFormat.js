var leadSection = ""
var trailSection = ""
var varName = ""
var formatSection = ""

var digits = 0
var digitZeros = 0
var decimals = 0
var decimalsZeros = 0
var decimalSymbol = "."

function splitFormatString(format) {
    // we need to analyze the structure of the string first
    // a typical notation is like this: "{distance:#00.0#} km", where
    // "{" "}" variable limiters
    // "distance" variable name, this will be discarded and has not effect
    // "#" non-leading/-trailing zero character
    // "0" leading/trailing zero character
    // " km" additional text which will remain unchanged
    leadSection = ""
    trailSection = ""
    varName = ""
    formatSection = ""

    var startPos = 0;
    var searchStep = 0;
    for(var i = 0; i < format.length; ++i)
    {
        // search for the opening "{"
        if(searchStep === 0) {
            if(format.charAt(i)==='{') {
                leadSection = format.slice(startPos, i);
                startPos = i + 1;
                searchStep++;
            }
        }
        // search for the ":" or the closing "}"
        else if(searchStep === 1) {
            if(format.charAt(i) === '}') {
                formatSection = format.slice(startPos, i);
                startPos = i + 1;
                trailSection = format.slice(startPos)
                break;
            }
            else if(format.charAt(i) === ':') {
                varName = format.slice(startPos, i);
                startPos = i + 1;
                searchStep++;
            }
        }
        else if(searchStep === 2) {
            if(format.charAt(i) === '}') {
                formatSection = format.slice(startPos, i);
                startPos = i + 1;
                trailSection = format.slice(startPos)
                break;
            }
        }
    }

    // finally we need to create the formatting string for sprintf out of the formatSection
    digits = 0;
    digitZeros = 0;
    decimals = 0;
    decimalsZeros = 0;
    searchStep = 0;
    for(var i = 0; i < formatSection.length; ++i) {
        if(searchStep === 0) {
            if(formatSection.charAt(i) === '#') {
                digits++;
            }
            else if(formatSection.charAt(i) === '0') {
                digits++;
                digitZeros++;
            }
            else if(formatSection.charAt(i) === '.' || formatSection.charAt(i) === ',') {
                decimalSymbol = formatSection.charAt(i);
                searchStep++;
            }
        }
        else if(searchStep === 1) {
            if(formatSection.charAt(i) === '#') {
                decimals++;
            }
            else if(formatSection.charAt(i) === '0') {
                decimals++;
                decimalsZeros++;
            }
        }
    }
}

function formatOutput(data)
{
    // format the output, first the digits
    var digitString = Math.floor(data).toString();
    if(digitString.length < digitZeros) {
        // zero pad the string
        var temp = "";
        for(var i = 0; i < digitZeros - digitString.length; ++i) {
            temp += '0';
        }
        digitString = temp + digitString;
    }
    else {
        digitString = digitString.slice(-digits);
    }

    var decimalString = (data - Math.floor(data)).toFixed(decimals).slice(2);
    if (decimalString.length > decimalsZeros) {
        for (var i = decimalString.length - 1; i >= decimalsZeros; i--)
        {
            if(decimalString.charAt(i) === '0')
            {
                decimalString = decimalString.slice(0, -1);
            }
        }
    }
    else {
        for (var i = decimalString.length; i < decimalsZeros; i++)
        {
            decimalString += '0';
        }
    }

    if (decimalString.length > 0) {
        return leadSection + digitString + decimalSymbol + decimalString + trailSection;
    }
    else {
        return leadSection + digitString + trailSection;
    }
}
