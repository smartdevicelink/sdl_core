// import QtQuick 2.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Widgets"
import "../WidgetLayouts"

FWidget {
    height: 50

    layout: TGListItemLayout {
        id: l
        anchors.fill: parent
        ax_bitmap: ""
        cx_text: menuItem
        activeMarker_visible: false
    }

    FViewUseCaseGroup {
        FViewUseCase {
            condition: type === "popup"
            PropertyChanges { target: l
                ax_bitmap: "../Resources/g137p.png"
            }

        }

        FViewUseCase {
            condition: type === "arrow"
            PropertyChanges { target: l
                ax_bitmap: "../Resources/g136p.png"
            }
        }

        FViewUseCase {
            condition: type === "tickboxChecked"
            PropertyChanges { target: l
                gx_bitmap: "../Resources/g134p.png"
                cx_width: ++ax_width
            }
        }

        FViewUseCase {
            condition: type === "tickboxUnchecked"
            PropertyChanges { target: l
                gx_bitmap: "../Resources/g135p.png"
                cx_width: ++ax_width
            }
        }


        FViewUseCase {
            condition: type === ""
            PropertyChanges { target: l
                visible: true
                ax_bitmap: ""
                cx_width: ++(ax_width+gx_width)
            }
        }
    }
}
