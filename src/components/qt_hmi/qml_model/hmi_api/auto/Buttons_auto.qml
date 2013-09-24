import QtQuick 2.0
import ".."
Item {
  Buttons {
    id: sdlButtons
  }
  function getCapabilities(params) {
     return sdlButtons.getCapabilities()
  }

  signal onButtonEvent(int name, int mode, var customButtonID)
  signal onButtonPress(int name, int mode, var customButtonID)
}
