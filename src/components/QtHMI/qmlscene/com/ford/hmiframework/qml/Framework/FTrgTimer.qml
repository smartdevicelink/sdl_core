// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import com.ford.hmiframework 1.0

FTrigger {
    property QtObject timer

    onInit: {
        timer.triggered.connect(timerTrigger);
    }

    onCleanup: {
        timer.triggered.disconnect(timerTrigger);
    }

    function timerTrigger() {
        executeActions();
    }
}
