A few comments on test writting for QML Unit Testing

- set TestCase inside Item - this allows to send Mouse and keyboard events to controls
- content of QML is reachable directly from tests. It is necessary to add getters into 
  source QML file to handle internal items
- test execution order is alfabetical. If you need manage this order use numbers in test function names
- all functions that starts with "test_" prefix is treated as test case
- all files that starts with "tst_" prefix will be executed as test set
- for async testing use timer
- each system's entity that changes during RPC must be tested
- test incoming arguments is not necessary as it will be verified by Qt abstraction layer
- run test with following command: 
 "./qt_hmi_test -import ../git push qml_model_qt5/"

