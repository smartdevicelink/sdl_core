/**
 * \file dnssd_service_browser.h
 * \brief DnssdServiceBrowser class header file.
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TCP_DNSSD_SERVICE_BROWSER_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TCP_DNSSD_SERVICE_BROWSER_H_

#include <string>
#include <vector>
#include <pthread.h>
#include <avahi-client/client.h>
#include <avahi-client/lookup.h>
#include <avahi-common/error.h>
#include <avahi-common/thread-watch.h>

#include "transport_manager/transport_adapter/device_scanner.h"
#include "transport_manager/transport_adapter/transport_adapter.h"

namespace transport_manager {

namespace transport_adapter {

struct DnssdServiceRecord {
  AvahiIfIndex interface;
  AvahiProtocol protocol;
  std::string domain_name;
  std::string host_name;
  std::string name;
  std::string type;
  uint16_t port;
  uint32_t addr;
};

#define DNSSD_DEFAULT_SERVICE_TYPE "_ford-sdlapp._tcp"

/**
 * @brief DNS service discovery class.
 */
class DnssdServiceBrowser : public DeviceScanner {
 public:
  /**
   * @brief Constructor.
   *
   * @param controller Pointer to the device adapter controller.
   */
  DnssdServiceBrowser(class TransportAdapterController* controller);
  virtual ~DnssdServiceBrowser();
 protected:
  virtual TransportAdapter::Error Init();
  virtual TransportAdapter::Error Scan();
  virtual void Terminate();
  virtual bool IsInitialised() const;
 private:
  TransportAdapter::Error CreateAvahiClientAndBrowser();
  void AddService(AvahiIfIndex interface, AvahiProtocol protocol,
                  const char *name, const char *type, const char *domain);
  void RemoveService(AvahiIfIndex interface, AvahiProtocol protocol,
                     const char *name, const char *type, const char *domain);

  void OnClientConnected();
  void OnClientFailure();

  DeviceVector PrepareDeviceVector() const;

  void ServiceResolved(const DnssdServiceRecord& service_record);
  void ServiceResolveFailed(const DnssdServiceRecord& service_record);

  friend void AvahiClientCallback(AvahiClient *avahi_client,
                                  AvahiClientState avahi_client_state,
                                  void* data);
  friend void AvahiServiceBrowserCallback(
      AvahiServiceBrowser *avahi_service_browser, AvahiIfIndex interface,
      AvahiProtocol protocol, AvahiBrowserEvent event, const char *name,
      const char *type, const char *domain, AvahiLookupResultFlags flags,
      void* data);

  friend void AvahiServiceResolverCallback(
      AvahiServiceResolver* avahi_service_resolver, AvahiIfIndex interface,
      AvahiProtocol protocol, AvahiResolverEvent event, const char* name,
      const char* type, const char* domain, const char* host_name,
      const AvahiAddress* avahi_address, uint16_t port, AvahiStringList* txt,
      AvahiLookupResultFlags flags, void *data);

  TransportAdapterController* controller_;

  AvahiServiceBrowser* avahi_service_browser_;
  AvahiThreadedPoll* avahi_threaded_poll_;
  AvahiClient* avahi_client_;

  typedef std::vector<DnssdServiceRecord> ServiceRecords;
  ServiceRecords service_records_;

  pthread_mutex_t mutex_;

  bool initialised_;
}
;

}  // namespace
}  // namespace

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_DNSSD_SERVICE_BROWSER
