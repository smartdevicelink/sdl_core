<head>
<script language="JavaScript" >
function tFont(x)
    {
    switch(x)
        {
    case "Error":
        return "<font color=\"red\">";
    case "Warning":
        return "<font color=\"darkred\">";
    case "Info":
        return "<font color=\"blue\">";
    case "Note":
        return "<font color=\"purple\">";
    default:
        return "<font>";
        }
    }
function typeFont(x)
    {
    document.write( tFont(x) );
    }
</script>
</head>
