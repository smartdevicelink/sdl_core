/*
 * \file aoa_common.h
 * \brief Google AOA protocol header file.
 *
 * Copyright (c) 2013, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_USB_USB_CONTROL_TRANSFER_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_USB_USB_CONTROL_TRANSFER_H_

namespace transport_manager {

namespace transport_adapter {

class UsbControlTransfer;
class UsbControlInTransfer;
class UsbControlOutTransfer;

class UsbControlTransfer {
 public:
  enum TransferDirection { IN, OUT };

  enum RequestType { VENDOR };

  virtual ~UsbControlTransfer() {}
  virtual TransferDirection Direction() const = 0;
  virtual RequestType Type() const = 0;
  virtual uint8_t Request() const = 0;
  virtual uint16_t Value() const = 0;
  virtual uint16_t Index() const = 0;
  virtual uint16_t Length() const = 0;
};

class UsbControlInTransfer : public UsbControlTransfer {
 public:
  virtual ~UsbControlInTransfer() {}
  virtual TransferDirection Direction() const {
    return IN;
  }
  virtual bool OnCompleted(unsigned char* data) const = 0;
};

class UsbControlOutTransfer : public UsbControlTransfer {
 public:
  virtual ~UsbControlOutTransfer() {}
  virtual TransferDirection Direction() const {
    return OUT;
  }
  virtual const char* Data() const = 0;
};

class UsbControlTransferSequence {
 public:
  typedef std::list<UsbControlTransfer*> Transfers;
  const Transfers& transfers() const {
    return transfers_;
  }

  virtual ~UsbControlTransferSequence() {
    for (Transfers::iterator it = transfers_.begin(); it != transfers_.end();
         ++it) {
      delete *it;
    }
  }

 protected:
  void AddTransfer(UsbControlTransfer* transfer) {
    transfers_.push_back(transfer);
  }

 private:
  Transfers transfers_;
};

}  // namespace
}  // namespace

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_USB_USB_CONTROL_TRANSFER_H_
