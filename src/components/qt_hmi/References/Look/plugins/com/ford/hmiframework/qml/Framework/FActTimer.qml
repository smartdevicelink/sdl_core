// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import com.ford.hmiframework 1.0

FAction {
    id: theAction

    property variant timer
    property int action: 1

    onExecute: {
        switch(action)
        {
        case TM.Start:
            timer.start();
            break;
        case TM.Restart:
            timer.restart();
            break;
        case TM.Stop:
            timer.stop();
            break;
        }
    }
}
