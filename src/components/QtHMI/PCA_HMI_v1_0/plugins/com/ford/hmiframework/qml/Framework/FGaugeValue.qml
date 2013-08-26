// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0

// The FGauge allows to transform an input value range into an output value rang using a smooth transition
// it can be e.g. used to transfrom the speed of a vehicle into an angle of the pointer needle
// The borders of the input and output ranges are mapped to each other and a linear transformation is executed

Item {
    id: theGauge

    // the time required to move through the full range from e.g. min to max
    property int fullSweepTime: 1000

    // minimum border, the input value can reach
    property double minInput: 0.0
    // maximum value, the input can reach
    property double maxInput: 100.0
    // the current input value. Each change will trigger an animated change of the output
    property double currentInput: 50.0

    // minimum border, the output value can reach
    property double minOutput: 0.0
    // maximum value, the output can reach
    property double maxOutput: 100.0
    //
    property double currentOutput: 50.0

    // this is the targetted output. Changing this, will trigger the animation to be executed
    property double targetOutput
    // this is the time required to move to the target value
    property int targetTime

    PropertyAnimation { id: anim; target: theGauge; properties: "currentOutput"; to: theGauge.targetOutput; duration: theGauge.targetTime }

    onCurrentInputChanged: {
        // we need to calculate the target and the required time to reach it
        var target = (currentInput - minInput) * (maxOutput - minOutput) / (maxInput - minInput) + minOutput;
        if(target < minOutput) {
            target = minOutput;
        }
        else if (target > maxOutput) {
            target = maxOutput;
        }

        targetTime = Math.abs((target - currentOutput) / (maxOutput - minOutput)) * fullSweepTime;
        if(targetTime > 0) {
            targetOutput = target;
            anim.restart();
        }
    }
}
