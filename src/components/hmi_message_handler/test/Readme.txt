28.11.2014 Andriy Byzhynar

1) Tests for mqueue_adapter failed as mqueue transport still not used.
2) Tests are not actual at the moment until PASA moves to use mqueue transport.
3) To activate tests - just uncomment following lines in CMakeLists.txt of hmi_message_handler :
  
  - #if(BUILD_TESTS)
  - #  add_subdirectory(test)
  - #endif()
  
  They are located at the end of file.
