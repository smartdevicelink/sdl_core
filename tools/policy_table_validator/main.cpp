#include <iostream>
#include <cstdlib>
#include "policy_table/table_struct/types.h"

#include "json/reader.h"
#include "utils/file_system.h"

namespace policy_table = rpc::policy_table_interface_base;

enum ResultCode {
  SUCCES = 0,
  MISSED_FILE_NAME,
  READ_ERROR,
  PARSE_ERROR,
  PT_TYPE_ERROR
};

rpc::policy_table_interface_base::PolicyTableType StringToPolicyTableType(const std::string& str_pt_type) {
  if (str_pt_type == "PT_PRELOADED") {
    return rpc::policy_table_interface_base::PT_PRELOADED;
  }
  if (str_pt_type == "PT_SNAPSHOT") {
    return rpc::policy_table_interface_base::PT_SNAPSHOT;
  }
  if (str_pt_type == "PT_UPDATE") {
    return rpc::policy_table_interface_base::PT_UPDATE;
  }
  return rpc::policy_table_interface_base::INVALID_PT_TYPE;
}

void help() {
  std::cout << "Usage:" << std::endl <<
               "./policy_validator {Policy table type} {file_name}" << std::endl;
  std::cout << "Policy table types:"
               "\t PT_PRELOADED , PT_UPDATE , PT_SNAPSHOT" <<std::endl;
}

int main(int argc, char** argv) {
  if (argc != 3) {
    // TODO(AKutsan): No filename
    help();
    exit(MISSED_FILE_NAME);
  }
  std::string pt_type_str = argv[1];
  std::string file_name = argv[2];
  std::string json_string;
  rpc::policy_table_interface_base::PolicyTableType pt_type;
  pt_type = StringToPolicyTableType(pt_type_str);
  if (rpc::policy_table_interface_base::PolicyTableType::INVALID_PT_TYPE
      == pt_type) {
    std::cout << "Invalid policy table type: " << pt_type_str << std::endl;
    exit(PT_TYPE_ERROR);
  }
  bool read_result = file_system::ReadFile(file_name, json_string);
  if (false == read_result) {
    std::cout << "Read file error: " << file_name << std::endl;
    exit(READ_ERROR);
  }

  Json::Reader reader;
  Json::Value value;

  bool parce_result = reader.parse(json_string, value);
  if (false == parce_result) {
    std::cout << "Json parce fails" << std::endl;
    exit(PARSE_ERROR);
  }
  std::cout << "DEFAULT_POLICY" << std::endl;
  policy_table::Table table(&value);
  table.SetPolicyTableType(pt_type);
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
