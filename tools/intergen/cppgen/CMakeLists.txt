include_directories(
  include
  ${intergen_SOURCE_DIR}/model/include
  ${intergen_SOURCE_DIR}/utils/include
)

set (SOURCES
  src/cppgen/comment.cc
  src/cppgen/cpp_api_code_generator.cc
  src/cppgen/cpp_class.cc
  src/cppgen/cpp_file.cc
  src/cppgen/cpp_function.cc
  src/cppgen/cpp_interface_code_generator.cc
  src/cppgen/declaration_generator.cc
  src/cppgen/definition_generator.cc
  src/cppgen/enum_from_json_value_function.cc
  src/cppgen/enum_to_json_value_function.cc
  src/cppgen/function_id_method.cc
  src/cppgen/generator_preferences.cc
  src/cppgen/handler_interface.cc
  src/cppgen/is_valid_enum_function.cc
  src/cppgen/literal_generator.cc
  src/cppgen/message_factory_function.cc
  src/cppgen/message_handle_with_method.cc
  src/cppgen/message_interface.cc
  src/cppgen/module_manager.cc
  src/cppgen/namespace.cc
  src/cppgen/naming_convention.cc
  src/cppgen/struct_type_constructor.cc
  src/cppgen/struct_type_from_json_method.cc
  src/cppgen/struct_type_is_initialized_method.cc
  src/cppgen/struct_type_is_valid_method.cc
  src/cppgen/struct_type_report_erros_method.cc
  src/cppgen/type_name_code_generator.cc
)

set (HEADERS
  include/cppgen/comment.h
  include/cppgen/cpp_api_code_generator.h
  include/cppgen/cpp_class.h
  include/cppgen/cpp_file.h
  include/cppgen/cpp_function.h
  include/cppgen/cpp_interface_code_generator.h
  include/cppgen/declaration_generator.h
  include/cppgen/definition_generator.h
  include/cppgen/enum_from_json_value_function.h
  include/cppgen/enum_to_json_value_function.h
  include/cppgen/function_id_method.h
  include/cppgen/generator_preferences.h
  include/cppgen/handler_interface.h
  include/cppgen/is_valid_enum_function.h
  include/cppgen/literal_generator.h
  include/cppgen/message_factory_function.h
  include/cppgen/message_handle_with_method.h
  include/cppgen/message_interface.h
  include/cppgen/module_manager.h
  include/cppgen/namespace.h
  include/cppgen/naming_convention.h
  include/cppgen/struct_type_constructor.h
  include/cppgen/struct_type_from_json_method.h
  include/cppgen/struct_type_is_initialized_method.h
  include/cppgen/struct_type_is_valid_method.h
  include/cppgen/struct_type_report_erros_method.h
  include/cppgen/type_name_code_generator.h
)

add_library(intergen_cppgen ${HEADERS} ${SOURCES})
target_link_libraries(intergen_cppgen intergen_model intergen_utils)
