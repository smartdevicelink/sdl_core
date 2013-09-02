#include "transport_manager/transport_adapter/transport_adapter_impl.h"
#include "transport_manager/usb/usb_device_scanner.h"
#include "transport_manager/usb/usb_device.h"
#include "transport_manager/usb/libusb_handler.h"

#include <cstring>
#include <cstdlib>

namespace transport_manager {
namespace transport_adapter {

class UsbControlTransferSequence {
 public:
  void Start();
  bool Finished() const;
  libusb_device_handle* device_handle() const;

 protected:
  class UsbControlTransfer;
  class UsbControlInTransfer;
  class UsbControlOutTransfer;

  UsbControlTransferSequence(libusb_device_handle* device_handle);
  ~UsbControlTransferSequence();
  void AddTransfer(UsbControlTransfer* transfer);

 private:
  friend void UsbTransferSequenceCallback(libusb_transfer *transfer);

  void Finish();
  void SubmitTransfer();
  void Callback(libusb_transfer* transfer);

  libusb_device_handle* device_handle_;
  typedef std::list<UsbControlTransfer*> Transfers;
  Transfers transfers_;
  Transfers::iterator current_transfer_;
  bool finished_;
};
class UsbControlTransferSequence::UsbControlTransfer {
 public:
  virtual ~UsbControlTransfer() {
  }
  virtual libusb_endpoint_direction Direction() const = 0;
  virtual libusb_request_type RequestType() const = 0;
  virtual uint8_t Request() const = 0;
  virtual uint16_t Value() const = 0;
  virtual uint16_t Index() const = 0;
  virtual uint16_t Length() const = 0;
};

class UsbControlTransferSequence::UsbControlInTransfer :
    public UsbControlTransfer {
 public:
  virtual ~UsbControlInTransfer() {
  }
  virtual libusb_endpoint_direction Direction() const {
    return LIBUSB_ENDPOINT_IN;
  }
  virtual bool OnCompleted(unsigned char* data) const = 0;
};

class UsbControlTransferSequence::UsbControlOutTransfer :
    public UsbControlTransfer {
 public:
  virtual ~UsbControlOutTransfer() {
  }
  virtual libusb_endpoint_direction Direction() const {
    return LIBUSB_ENDPOINT_OUT;
  }
  virtual const char* Data() const = 0;
};

class AoaInitSequence : public UsbControlTransferSequence {
 public:
  AoaInitSequence(libusb_device_handle* device_handle);
 private:
  class AoaGetProtocolRequest;
  class AoaSendIdString;
  class AoaTurnIntoAccessoryMode;
};

bool IsGoogleAccessory(const libusb_device_descriptor& descriptor) {
  static const uint16_t AOA_VID = 0x18d1;
  static const uint16_t AOA_PID1 = 0x2d00;
  static const uint16_t AOA_PID2 = 0x2d01;
  return descriptor.idVendor == AOA_VID
      && (descriptor.idProduct == AOA_PID1 || descriptor.idProduct == AOA_PID2);
}

int ArrivedCallback(libusb_context* context, libusb_device* device,
                    libusb_hotplug_event event, void* data) {
  UsbDeviceScanner* usb_device_scanner = static_cast<UsbDeviceScanner*>(data);
  usb_device_scanner->DeviceArrived(device);
  return 0;
}

int LeftCallback(libusb_context* context, libusb_device* device,
                 libusb_hotplug_event event, void* data) {
  UsbDeviceScanner* usb_device_scanner = static_cast<UsbDeviceScanner*>(data);
  usb_device_scanner->DeviceLeft(device);
  return 0;
}

void UsbDeviceScanner::DeviceArrived(libusb_device* device) {
  LOG4CXX_INFO(
      logger_,
      "libusb device arrived (bus number " << static_cast<int>(libusb_get_bus_number(device)) << ", address " << static_cast<int>(libusb_get_device_address(device)) << ")");

  libusb_device_descriptor descriptor;
  int libusb_ret = libusb_get_device_descriptor(device, &descriptor);
  if (LIBUSB_SUCCESS != libusb_ret) {
    LOG4CXX_ERROR(logger_,
                  "libusb_get_device_descriptor failed: " << libusb_ret);
    return;
  }

  libusb_device_handle* device_handle;
  libusb_ret = libusb_open(device, &device_handle);
  if (libusb_ret != LIBUSB_SUCCESS) {
    LOG4CXX_ERROR(logger_,
                  "libusb_open failed: " << libusb_error_name(libusb_ret));
    return;
  }

  if (IsGoogleAccessory(descriptor)) {
    GoogleAccessoryFound(device, descriptor, device_handle);
  } else {
    TurnIntoAccessoryMode(descriptor, device_handle);
  }
}

void UsbDeviceScanner::DeviceLeft(libusb_device* device) {
  LOG4CXX_INFO(
      logger_,
      "libusb device left (bus number " << static_cast<int>(libusb_get_bus_number(device)) << ", address " << static_cast<int>(libusb_get_device_address(device)) << ")");

  pthread_mutex_lock(&device_descriptions_mutex_);
  for (DeviceDescriptions::iterator it = device_descriptions_.begin();
      it != device_descriptions_.end();) {
    const UsbDeviceDescription& description = *it;
    if (libusb_get_bus_number(device) == description.bus_number
        && libusb_get_device_address(device) == description.address) {
      it = device_descriptions_.erase(it);
    } else {
      ++it;
    }
  }
  pthread_mutex_unlock(&device_descriptions_mutex_);
}

UsbDeviceScanner::UsbDeviceScanner(class DeviceAdapterController* controller)
    : controller_(controller),
      arrived_callback_handle_(0),
      left_callback_handle_(0) {
  pthread_mutex_init(&device_descriptions_mutex_, 0);
}

UsbDeviceScanner::~UsbDeviceScanner() {
  pthread_mutex_destroy(&device_descriptions_mutex_);
}

void UsbTransferSequenceCallback(libusb_transfer* transfer) {
  static_cast<UsbControlTransferSequence*>(transfer->user_data)->Callback(
      transfer);
}

class AoaInitSequence::AoaGetProtocolRequest : public UsbControlInTransfer {
  virtual ~AoaGetProtocolRequest() {
  }
  virtual libusb_request_type RequestType() const {
    return LIBUSB_REQUEST_TYPE_VENDOR;
  }
  virtual uint8_t Request() const {
    return 51;
  }
  virtual uint16_t Value() const {
    return 0;
  }
  virtual uint16_t Index() const {
    return 0;
  }
  virtual uint16_t Length() const {
    return 2;
  }
  virtual bool OnCompleted(unsigned char* data) const {
    const int protocol_version = data[1] << 8 | data[0];
    LOG4CXX_INFO(logger_, "AOA protocol version " << protocol_version);
    if (protocol_version == 0) {
      // AOA protocol not supported
      return false;
    }
    return true;
  }
};

class AoaInitSequence::AoaSendIdString : public UsbControlOutTransfer {
 public:
  AoaSendIdString(uint16_t index, const char* string, uint16_t length)
      : index_(index),
        string_(string),
        length_(length) {
  }
 private:
  virtual ~AoaSendIdString() {
  }
  virtual libusb_request_type RequestType() const {
    return LIBUSB_REQUEST_TYPE_VENDOR;
  }
  virtual uint8_t Request() const {
    return 52;
  }
  virtual uint16_t Value() const {
    return 0;
  }
  virtual uint16_t Index() const {
    return index_;
  }
  virtual uint16_t Length() const {
    return length_;
  }
  virtual const char* Data() const {
    return string_;
  }
  uint16_t index_;
  const char* string_;
  uint16_t length_;
};

class AoaInitSequence::AoaTurnIntoAccessoryMode : public UsbControlOutTransfer {
  virtual ~AoaTurnIntoAccessoryMode() {
  }
  virtual libusb_request_type RequestType() const {
    return LIBUSB_REQUEST_TYPE_VENDOR;
  }
  virtual uint8_t Request() const {
    return 53;
  }
  virtual uint16_t Value() const {
    return 0;
  }
  virtual uint16_t Index() const {
    return 0;
  }
  virtual uint16_t Length() const {
    return 0;
  }
  virtual const char* Data() const {
    return 0;
  }
};

static char manufacturer[] = "Ford";
static char model_name[] = "HMI";
static char description[] = "Human machine interface";
static char version[] = "1.0";
static char uri[] = "http://www.ford.com";
static char serial_num[] = "N000000";

AoaInitSequence::AoaInitSequence(libusb_device_handle* device_handle)
    : UsbControlTransferSequence(device_handle) {
  AddTransfer(new AoaGetProtocolRequest);
  AddTransfer(new AoaSendIdString(0, manufacturer, sizeof(manufacturer)));
  AddTransfer(new AoaSendIdString(1, model_name, sizeof(model_name)));
  AddTransfer(new AoaSendIdString(2, description, sizeof(description)));
  AddTransfer(new AoaSendIdString(3, version, sizeof(version)));
  AddTransfer(new AoaSendIdString(4, uri, sizeof(uri)));
  AddTransfer(new AoaSendIdString(5, serial_num, sizeof(serial_num)));
  AddTransfer(new AoaTurnIntoAccessoryMode);
}

void UsbDeviceScanner::TurnIntoAccessoryMode(
    const libusb_device_descriptor& descriptor,
    libusb_device_handle* device_handle) {
  LOG4CXX_INFO(
      logger_,
      "USB device descriptor VID:" << descriptor.idVendor << " PID:" << descriptor.idProduct << " turning into accessory mode");

  int libusb_ret = 0;

  libusb_ret = libusb_claim_interface(device_handle, 0);
  if (LIBUSB_SUCCESS != libusb_ret) {
    LOG4CXX_INFO(
        logger_,
        "libusb_claim_interface failed: " << libusb_error_name(libusb_ret));
    return;
  }

  AoaInitSequence* aoa_init_sequence = new AoaInitSequence(device_handle);
  aoa_init_sequences_.push_back(aoa_init_sequence);
  aoa_init_sequence->Start();
}

bool FillUsbDeviceDescription(libusb_device* device,
                              const libusb_device_descriptor& descriptor,
                              libusb_device_handle* device_handle,
                              UsbDeviceDescription* description) {
  int libusb_ret = 0;
  unsigned char buf[128];
  libusb_ret = libusb_get_string_descriptor_ascii(device_handle,
                                                  descriptor.iManufacturer, buf,
                                                  sizeof(buf));
  if (libusb_ret < 0) {
    LOG4CXX_INFO(
        logger_,
        "Failed to get USB device manufacturer: " << libusb_error_name(libusb_ret));
    return false;
  }
  description->manufacturer = std::string(reinterpret_cast<char*>(buf));

  libusb_ret = libusb_get_string_descriptor_ascii(device_handle,
                                                  descriptor.iProduct, buf,
                                                  sizeof(buf));
  if (libusb_ret < 0) {
    LOG4CXX_INFO(
        logger_,
        "Failed to get USB device product name: " << libusb_error_name(libusb_ret));
    return false;
  }
  description->product = std::string(reinterpret_cast<char*>(buf));

  libusb_ret = libusb_get_string_descriptor_ascii(device_handle,
                                                  descriptor.iSerialNumber, buf,
                                                  sizeof(buf));
  if (libusb_ret < 0) {
    LOG4CXX_INFO(
        logger_,
        "Failed to get USB device serial number: " << libusb_error_name(libusb_ret));
    return false;
  }

  description->usb_device = device;
  description->serial_number = std::string(reinterpret_cast<char*>(buf));
  description->vid = descriptor.idVendor;
  description->pid = descriptor.idProduct;
  description->bus_number = libusb_get_bus_number(device);
  description->address = libusb_get_device_address(device);

  return true;
}

void UsbDeviceScanner::GoogleAccessoryFound(
    libusb_device* device, const libusb_device_descriptor& descriptor,
    libusb_device_handle* device_handle) {
  LOG4CXX_INFO(logger_, "Google accessory found");

  UsbDeviceDescription usb_device_description;
  if (FillUsbDeviceDescription(device, descriptor, device_handle,
                               &usb_device_description)) {
    pthread_mutex_lock(&device_descriptions_mutex_);
    device_descriptions_.push_back(usb_device_description);
    pthread_mutex_unlock(&device_descriptions_mutex_);
    LOG4CXX_INFO(
        logger_,
        "Google accessory (bus number " << static_cast<int>(usb_device_description.bus_number) << ", address " << static_cast<int>(usb_device_description.address) << ") identified as: " << usb_device_description.manufacturer << ", " << usb_device_description.product);
  }

  libusb_close(device_handle);
}

void UsbDeviceScanner::OnLibusbHandlerThread() {
  for (AoaInitSequences::iterator it = aoa_init_sequences_.begin();
      it != aoa_init_sequences_.end();) {
    AoaInitSequence* aoa_init_sequence = *it;
    if (aoa_init_sequence->Finished()) {
      libusb_release_interface(aoa_init_sequence->device_handle(), 0);
      libusb_close(aoa_init_sequence->device_handle());
      delete aoa_init_sequence;
      it = aoa_init_sequences_.erase(it);
    } else {
      ++it;
    }
  }
}

TransportAdapter::Error UsbDeviceScanner::Init() {
  int libusb_ret;

  libusb_context* libusb_context = GetLibusbHandler()->GetLibusbContext();
  if (libusb_context == 0) {
    LOG4CXX_ERROR(logger_, "Cannot get libusb_context");
    return TransportAdapter::FAIL;
  }

  libusb_ret = libusb_hotplug_register_callback(
      libusb_context, LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED,
      LIBUSB_HOTPLUG_ENUMERATE, LIBUSB_HOTPLUG_MATCH_ANY,
      LIBUSB_HOTPLUG_MATCH_ANY, LIBUSB_HOTPLUG_MATCH_ANY, ArrivedCallback, this,
      &arrived_callback_handle_);

  if (LIBUSB_SUCCESS != libusb_ret) {
    LOG4CXX_ERROR(logger_,
                  "libusb_hotplug_register_callback failed: " << libusb_ret);
    return TransportAdapter::FAIL;
  }

  libusb_ret = libusb_hotplug_register_callback(
      libusb_context, LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT,
      static_cast<libusb_hotplug_flag>(0), LIBUSB_HOTPLUG_MATCH_ANY,
      LIBUSB_HOTPLUG_MATCH_ANY, LIBUSB_HOTPLUG_MATCH_ANY, LeftCallback, this,
      &left_callback_handle_);

  if (LIBUSB_SUCCESS != libusb_ret) {
    LOG4CXX_ERROR(logger_,
                  "libusb_hotplug_register_callback failed: " << libusb_ret);
    return TransportAdapter::FAIL;
  }

  return TransportAdapter::OK;
}

TransportAdapter::Error UsbDeviceScanner::Scan() {
  DeviceVector device_vector;
  pthread_mutex_lock(&device_descriptions_mutex_);
  for (DeviceDescriptions::const_iterator it = device_descriptions_.begin();
      it != device_descriptions_.end(); ++it) {
    const std::string device_name = it->manufacturer + it->product;
    std::ostringstream oss;
    oss << it->manufacturer << ":" << it->product << ":" << it->serial_number
        << ":" << static_cast<int>(it->bus_number) << ":"
        << static_cast<int>(it->address);
    const DeviceUID device_uid = oss.str();
    DeviceSptr device(new UsbDevice(it->usb_device, device_name, device_uid));
    device_vector.push_back(device);
  }
  pthread_mutex_unlock(&device_descriptions_mutex_);

  controller_->searchDeviceDone(device_vector);
  return TransportAdapter::OK;
}

void UsbDeviceScanner::Terminate() {
  libusb_context* libusb_context = GetLibusbHandler()->GetLibusbContext();
  if (libusb_context != 0) {
    libusb_hotplug_deregister_callback(libusb_context,
                                       arrived_callback_handle_);
    libusb_hotplug_deregister_callback(libusb_context, left_callback_handle_);
  }
}

bool UsbDeviceScanner::IsInitialised() const {
  return true;
}

void UsbControlTransferSequence::Start() {
  current_transfer_ = transfers_.begin();
  SubmitTransfer();
}

bool UsbControlTransferSequence::Finished() const {
  return finished_;
}

UsbControlTransferSequence::UsbControlTransferSequence(
    libusb_device_handle* device_handle)
    : device_handle_(device_handle),
      finished_(false) {
}

UsbControlTransferSequence::~UsbControlTransferSequence() {
  for (Transfers::iterator it = transfers_.begin(); it != transfers_.end();
      ++it) {
    delete *it;
  }
}

void UsbControlTransferSequence::AddTransfer(UsbControlTransfer* transfer) {
  transfers_.push_back(transfer);
}

void UsbControlTransferSequence::SubmitTransfer() {
  UsbControlTransfer* transfer = *current_transfer_;
  libusb_transfer* libusb_transfer = libusb_alloc_transfer(0);
  if (0 == libusb_transfer) {
    LOG4CXX_ERROR(logger_, "libusb_alloc_transfer failed");
    Finish();
  }

  const libusb_request_type request_type = transfer->RequestType();
  const libusb_endpoint_direction endpoint_direction = transfer->Direction();
  const uint8_t request = transfer->Request();
  const uint16_t value = transfer->Value();
  const uint16_t index = transfer->Index();
  const uint16_t length = transfer->Length();

  unsigned char* buffer = static_cast<unsigned char*>(malloc(
      length + LIBUSB_CONTROL_SETUP_SIZE));
  if (NULL == buffer) {
    LOG4CXX_ERROR(logger_, "buffer allocation failed");
    libusb_free_transfer(libusb_transfer);
    Finish();
  }

  libusb_fill_control_setup(buffer, request_type | endpoint_direction, request,
                            value, index, length);

  if (0 != length && endpoint_direction == LIBUSB_ENDPOINT_OUT) {
    const char* data = static_cast<UsbControlOutTransfer*>(transfer)->Data();
    memcpy(buffer + LIBUSB_CONTROL_SETUP_SIZE, data, length);
  }
  libusb_fill_control_transfer(libusb_transfer, device_handle_, buffer,
                               UsbTransferSequenceCallback, this, 0);
  libusb_transfer->flags = LIBUSB_TRANSFER_FREE_BUFFER;

  const int libusb_ret = libusb_submit_transfer(libusb_transfer);
  if (LIBUSB_SUCCESS != libusb_ret) {
    LOG4CXX_ERROR(
        logger_,
        "libusb_submit_transfer failed: " << libusb_error_name(libusb_ret));
    libusb_free_transfer(libusb_transfer);
    Finish();
  }
}

void UsbControlTransferSequence::Finish() {
  finished_ = true;
}

libusb_device_handle* UsbControlTransferSequence::device_handle() const {
  return device_handle_;
}

void UsbControlTransferSequence::Callback(libusb_transfer* transfer) {
  if (transfer->status == LIBUSB_TRANSFER_COMPLETED) {
    LOG4CXX_INFO(logger_, "USB control transfer completed");
    UsbControlTransfer* current_transfer = *current_transfer_;
    bool submit_next = true;
    if (current_transfer->Direction() == LIBUSB_ENDPOINT_IN) {
      submit_next = static_cast<UsbControlInTransfer*>(current_transfer)
          ->OnCompleted(libusb_control_transfer_get_data(transfer));
    }

    if (submit_next && (++current_transfer_ != transfers_.end())) {
      SubmitTransfer();
    } else {
      Finish();
    }
  } else {
    LOG4CXX_ERROR(logger_, "USB control transfer failed: " << transfer->status);
    Finish();
  }
  libusb_free_transfer(transfer);
}

}  // namespace
}  // namespace
