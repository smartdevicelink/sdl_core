// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0

GridView {
    id: theGrid

    property alias data : theGrid.model
    property alias itemWidget : theGrid.delegate

    /*
        Setting rows/columns fundamentally changes how children fit into the GridView.
        -Normal behaviour:  Children have a fixed size (cellWidth/cellHeight) and the NUMBER of them
        that will fit into the GridView varies with the GridView's size.
        -Modified behaviour(setting rows and/or columns triggers this mode): A fixed number of children
         will fit into the GridView (determined by number of rows/columns) and their SIZE
        (cellWidth/cellHeight) will vary with the GridView's size
        -rows/columns determines the size of VISIBLE children and simply sets cellWidth/cellHeight at runtime.
        If the number of children is different than rows * columns, it will either scroll or not
        fill the GridView completely, same as normal.
        -the item_Widget component must be designed for fluid layout using anchors, not fixed sizes,
        for this to have any effect besides spacing
    */
    property int rows
    property int columns

    Component.onCompleted: {
        if (rows) {
            theGrid.cellHeight = theGrid.height/rows
        }
        if (columns) {
            theGrid.cellWidth = theGrid.width/columns
        }
    }

}
