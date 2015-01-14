// import QtQuick 2.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../WidgetLayouts"

FWidget {
    property alias focusNext: _focusNext
    property alias focusPrevious: _focusPrevious
    property alias selectElement: _selectElement

    property alias list: l.list
    property alias list_itemWidget: l.list_itemWidget
    property alias list_data: l.list_data
    property alias listWrap: l.listWrap

    FEvent { id: _focusNext; }
    FEvent { id: _focusPrevious; }
    FEvent { id: _selectElement; }

    layout: FocusListViewLayout {
        id: l
        anchors.fill: parent
    }

    triggers: [
        FTrgEvent { event: _focusPrevious
            FActScriptCall { onScript: {
                    l.list.decrementCurrentIndex();
                }
            }
        },
        FTrgEvent { event: _focusNext
            FActScriptCall { onScript: {
                    l.list.incrementCurrentIndex();
                }
            }
        },
        FTrgEvent { event: _selectElement
            FActScriptCall { onScript: {
                    list_data.selectElement(l.list.currentIndex);
                }
            }
        }
    ]
}
