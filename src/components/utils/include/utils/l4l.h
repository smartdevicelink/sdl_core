#ifndef L4L_HEADER_FILE_
#define L4L_HEADER_FILE_

#include <map>
#include <ostream>
#include <fstream>

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

namespace l4l {
enum OutputType {
  NONE = -1,
  STANDART,
  FILE,
  TCP
};

class OutputInfo {
  public:
    virtual bool Init(const std::string& name,
                      std::ostream* const stream) ;

    virtual std::ostream& output_stream() const ;

    OutputInfo();

    ~OutputInfo();
  private:
    std::string name_;
    /** @brief
    *  Can be std::cout; ofstream; ostringstream.
    **/
    std::ostream* output_stream_;
    DISALLOW_COPY_AND_ASSIGN(OutputInfo);
};

class OutputToFileInfo : public OutputInfo {
  public:
    OutputToFileInfo(const std::string& file_name);
    ~OutputToFileInfo();
  private:
    std::string file_name_;
    DISALLOW_COPY_AND_ASSIGN(OutputToFileInfo);
};

class OutputToTCP : public OutputInfo {
};

class Severity {
  public:
    static const char kInfoType[];
    static const char kWarningType[];
    static const char kErrorType[];
    static const char kFatalType[];
    static const char kTraceType[];

    static uint16_t severity_id(const std::string& type);
  private:
    static const uint16_t kInfo_id;
    static const uint16_t kWarning_id;
    static const uint16_t kError_id;
    static const uint16_t kFatal_id;
    static const uint16_t kTrace_id;
};

class NamedLogger {
  public:
    virtual ~NamedLogger() {}

  protected:
    NamedLogger(const std::string& name,
                uint16_t id)  // id == 1 is reserved
      : name_(name),
        id_(id) {}

    const std::string& name() const;
    uint16_t id() const;

  private:
    friend class Logger;
    std::string name_;
    uint16_t id_;
};


//namespace {
class InternalLogger {
  public:
    ~InternalLogger() {}
    InternalLogger& instance();
    static uint32_t KeyFromTypes(uint16_t namedlogger_id,
                                 uint16_t output_type) {
      uint32_t result = namedlogger_id | (output_type << 16);
      return result;
    }
    static void Configure(const std::string& file_name);
    static std::map<uint32_t, OutputInfo*> output_config_;
  private:
    InternalLogger();

};
}

#endif  //  L4L_HEADER_FILE_