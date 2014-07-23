#include <iostream>
#include "policy_table/table_struct/types.h"
#include "json/reader.h"
#include "utils/file_system.h"

namespace policy_table = rpc::policy_table_interface_base;

enum ResultCode {
  SUCCES = 0,
  MISSED_FILE_NAME,
  READ_ERROR,
  PARSE_ERROR
};

void help() {
  std::cout << "Usage:" << std::endl <<
               "./policy_validator {file_name}" << std::endl;
}

int main(int argc, char** argv) {
  if (argc == 1) {
    // TODO(AKutsan): No filename
    std::cout << "file name was missed"<<std::endl;
    help();
    exit(MISSED_FILE_NAME);
  }

  std::string file_name = argv[1];
  std::string json_string;
  bool read_result = file_system::ReadFile(file_name, json_string);
  if (false == read_result) {
    std::cout << "Read file error "<<std::endl;
    exit(READ_ERROR);
  }

  Json::Reader reader;
  Json::Value value;
  bool parce_result = reader.parse(json_string, value);
  if (false == parce_result) {
    std::cout << "Json parce fails" << std::endl;
    exit(PARSE_ERROR);
  }

  policy_table::Table table(&value);
  bool is_valid = table.is_valid();
  if (true == is_valid ) {
    std::cout << "Table is valid" << std::endl;
    exit(SUCCES);
  }
  std::cout << "Table is not valid" << std::endl;
  rpc::ValidationReport report("policy_table");
  table.ReportErrors(&report);
  std::cout << "Errors: " << std::endl << rpc::PrettyFormat(report) << std::endl;

  return SUCCES;
}
