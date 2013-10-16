#include <iostream>
#include "utils/l4l.h"

namespace l4l {

bool OutputInfo::Init(const std::string& name,
                      std::ostream* const stream) {
  output_stream_ = stream;
  name_ = name;
  return true;
}

std::ostream& OutputInfo::output_stream() const {
  return *output_stream_;
}

OutputInfo::OutputInfo()
  : output_stream_(NULL) {}

OutputInfo::~OutputInfo() {}

OutputToFileInfo::OutputToFileInfo(const std::string& file_name)
  : file_name_(file_name) {}
OutputToFileInfo::~OutputToFileInfo() {}

const char Severity::kInfoType[] = "INFO";
const char Severity::kWarningType[] = "WARNING";
const char Severity::kErrorType[] = "ERROR";
const char Severity::kFatalType[] = "FATAL";
const char Severity::kTraceType[] = "TRACE";

const uint16_t Severity::kInfo_id = 1;
const uint16_t Severity::kWarning_id = 2;
const uint16_t Severity::kError_id = 3;
const uint16_t Severity::kFatal_id = 4;
const uint16_t Severity::kTrace_id = 5;

uint16_t Severity::severity_id(const std::string& type) {
  if (type.compare(kInfoType) == 0) {
    return kInfo_id;
  }
  if (type.compare(kWarningType) == 0) {
    return kWarning_id;
  }
  if (type.compare(kErrorType) == 0) {
    return kError_id;
  }
  if (type.compare(kFatalType) == 0) {
    return kFatal_id;
  }
  if (type.compare(kTraceType) == 0) {
    return kTrace_id;
  }
  return 0;
}

std::map<uint32_t, OutputInfo*> InternalLogger::output_config_;

namespace {
enum Type {UNDEFINED = -1, SEVERITY, LOGGER, OUTPUT};

const char* SEVERITY_STRING = "severity.";
const char* LOGGER_STRING = "logger.";
const char* OUTPUT_STRING = "output.";

Type ParseType(const std::string& text) {
  if (text.compare(SEVERITY_STRING) == 0) {
    return SEVERITY;
  }
  if (text.compare(LOGGER_STRING) == 0) {
    return LOGGER;
  }
  if (text.compare(OUTPUT_STRING) == 0) {
    return OUTPUT;
  }
  return UNDEFINED;
}

bool AddOutput(const std::string& output_name,
               std::map<std::string, l4l::OutputInfo*>* outputs,
               const std::string& type,
               const std::string& name = "") {
  if (outputs->find(output_name) != outputs->end()) {
    return true;
  }
  l4l::OutputInfo* info = NULL;
  if (type.compare("Console") == 0) {
    info = new l4l::OutputInfo;
    std::ostream* stream = new std::ostream(std::cout.rdbuf());
    outputs->insert(
      std::pair<std::string, l4l::OutputInfo*>(output_name, info));
    return info->Init(output_name, stream);
  }
  if (type.compare("File") == 0) {
    info = new l4l::OutputToFileInfo(name);
    std::ostream* stream = new std::ofstream(name, std::ofstream::out);
    outputs->insert(
      std::pair<std::string, l4l::OutputInfo*>(output_name, info));
    return info->Init(output_name, stream);
  }
  return false;
}

void SplitInTwo(const std::string& input,
                char splitter,
                std::string* first,
                std::string* second) {
  size_t position = input.find(splitter);
  *second = input.substr(position + 1);
  *first = input.substr(0, position + 1);
}
}  //  namespace

void InternalLogger::Configure(const std::string& file_name) {
  std::ifstream config(file_name, std::ifstream::in);
  std::map<std::string, l4l::OutputInfo*> outputs;
  // TODO: implement
  while (config.good()) {
    std::string left;
    config >> left;

    std::string name, type;
    SplitInTwo(left, '.', &type, &name);
    switch (ParseType(type)) {
      case SEVERITY: {
        std::string output;
        config >> output; // =
        config >> output; // output.Default
        std::string temp;
        std::string output_name;
        SplitInTwo(output, '.', &temp, &output_name);
        std::map<std::string, l4l::OutputInfo*>::iterator it =
          outputs.find(output_name);
        if (outputs.end() == it) {
          std::cout << "Output type " << output_name << " is undefined." << std::endl;
          return;
        }
        uint16_t output_id = l4l::Severity::severity_id(name);
        l4l::InternalLogger::output_config_.insert(
          std::pair<uint32_t, l4l::OutputInfo*>(
            l4l::InternalLogger::KeyFromTypes(1, output_id), it->second));
        break;
      }
      case LOGGER: {
        std::string output;
        config >> output;
        if (name.find(":") != std::string::npos) {
          config >> output;
        }
        config >> output;
        break;
      }
      case OUTPUT: {
        std::string output_type;
        config >> output_type;
        config >> output_type;
        config >> output_type;
        std::string file_name;
        config >> file_name;
        if (file_name.compare("name") == 0) {
          config >> file_name;
          config >> file_name;
        } else {
          file_name = "";
        }
        AddOutput(name, &outputs, output_type, file_name);
        break;
      }
      default:
        break;
    }
  }
  config.close();
}



}
