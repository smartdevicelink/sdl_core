import QtQuick 2.0

Button {
  width: 60

  onPress: keyboard.letterClicked(text)
}
