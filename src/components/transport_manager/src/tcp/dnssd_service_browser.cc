/*
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

#include <algorithm>
#include <map>
#include "utils/logger.h"

#include "transport_manager/transport_adapter/transport_adapter_impl.h"
#include "transport_manager/tcp/tcp_device.h"
#include "transport_manager/tcp/dnssd_service_browser.h"


namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")


bool operator==(const DnssdServiceRecord& a, const DnssdServiceRecord& b) {
  return a.name == b.name && a.type == b.type && a.interface == b.interface
      && a.protocol == b.protocol && a.domain_name == b.domain_name;
}

void DnssdServiceBrowser::Terminate() {
  LOG4CXX_AUTO_TRACE(logger_);
  if (0 != avahi_threaded_poll_) {
    avahi_threaded_poll_stop(avahi_threaded_poll_);
  }
  if (0 != avahi_service_browser_) {
    avahi_service_browser_free(avahi_service_browser_);
    avahi_service_browser_ = NULL;
  }
  if (0 != avahi_client_) {
    avahi_client_free(avahi_client_);
    avahi_client_ = NULL;
  }
  if (0 != avahi_threaded_poll_) {
    avahi_threaded_poll_free(avahi_threaded_poll_);
    avahi_threaded_poll_ = NULL;
  }
}

bool DnssdServiceBrowser::IsInitialised() const {
  return initialised_;
}

DnssdServiceBrowser::DnssdServiceBrowser(TransportAdapterController* controller)
    : controller_(controller),
      avahi_service_browser_(0),
      avahi_threaded_poll_(0),
      avahi_client_(0),
      service_records_(),
      mutex_(),
      initialised_(false) {
}

DnssdServiceBrowser::~DnssdServiceBrowser() {
}

void DnssdServiceBrowser::OnClientConnected() {
  initialised_ = true;
  LOG4CXX_INFO(logger_, "AvahiClient ready");
}

void DnssdServiceBrowser::OnClientFailure() {
  LOG4CXX_AUTO_TRACE(logger_);
  const int avahi_errno = avahi_client_errno(avahi_client_);
  if (avahi_errno == AVAHI_ERR_DISCONNECTED) {
    LOG4CXX_DEBUG(logger_, "AvahiClient disconnected");
    CreateAvahiClientAndBrowser();
  } else {
    LOG4CXX_ERROR(logger_,
                  "AvahiClient failure: " << avahi_strerror(avahi_errno));
  }
}

void AvahiClientCallback(AvahiClient* avahi_client,
                         AvahiClientState avahi_client_state, void* data) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(
      logger_,
      "avahi_client " << avahi_client << ", avahi_client_state " << avahi_client_state << ", data " << data);
  DnssdServiceBrowser* dnssd_service_browser =
      static_cast<DnssdServiceBrowser*>(data);

  switch (avahi_client_state) {
    case AVAHI_CLIENT_S_RUNNING:
      dnssd_service_browser->OnClientConnected();
      LOG4CXX_DEBUG(logger_, "avahi_client_state: AVAHI_CLIENT_S_RUNNING");
      break;
    case AVAHI_CLIENT_FAILURE:
      dnssd_service_browser->OnClientFailure();
      LOG4CXX_DEBUG(logger_, "avahi_client_state: AVAHI_CLIENT_FAILURE");
      break;
    default: {
      LOG4CXX_ERROR(logger_,
                    "Unknown avahi_client_state: " << avahi_client_state);
    }
  }
}

void AvahiServiceBrowserCallback(AvahiServiceBrowser* avahi_service_browser,
                                 AvahiIfIndex interface, AvahiProtocol protocol,
                                 AvahiBrowserEvent event, const char* name,
                                 const char* type, const char* domain,
                                 AvahiLookupResultFlags flags, void* data) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(
      logger_,
      "avahi_service_browser " << avahi_service_browser << " interface " << interface <<
      " protocol " << protocol << " event " << event << " name " << name <<
      " type " << type << " domain " << domain << " flags " << flags << " data " << data);
  DnssdServiceBrowser* dnssd_service_browser =
      static_cast<DnssdServiceBrowser*>(data);

  switch (event) {
    case AVAHI_BROWSER_FAILURE:
      LOG4CXX_ERROR(
          logger_,
          "AvahiServiceBrowser failure: " << avahi_strerror(avahi_client_errno(
              avahi_service_browser_get_client(avahi_service_browser))));
      break;

    case AVAHI_BROWSER_NEW:
      dnssd_service_browser->AddService(interface, protocol, name, type,
                                        domain);
      LOG4CXX_DEBUG(logger_, "event: AVAHI_BROWSER_NEW");
      break;

    case AVAHI_BROWSER_REMOVE:
      dnssd_service_browser->RemoveService(interface, protocol, name, type,
                                           domain);
      LOG4CXX_DEBUG(logger_, "event: AVAHI_BROWSER_REMOVE");
      break;

    case AVAHI_BROWSER_ALL_FOR_NOW:
      LOG4CXX_DEBUG(logger_, "event: AVAHI_BROWSER_ALL_FOR_NOW");
      break;
    case AVAHI_BROWSER_CACHE_EXHAUSTED:
      LOG4CXX_DEBUG(logger_, "event: AVAHI_BROWSER_CACHE_EXHAUSTED");
      break;
  }
  LOG4CXX_TRACE(logger_, "exit");
}

void DnssdServiceBrowser::ServiceResolved(
    const DnssdServiceRecord& service_record) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock locker(mutex_);
  ServiceRecords::iterator service_record_it = std::find(
      service_records_.begin(), service_records_.end(), service_record);
  if (service_record_it != service_records_.end()) {
    *service_record_it = service_record;
  }
  DeviceVector device_vector = PrepareDeviceVector();
  controller_->SearchDeviceDone(device_vector);
}

void DnssdServiceBrowser::ServiceResolveFailed(
    const DnssdServiceRecord& service_record) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_,
                "AvahiServiceResolver failure for: " << service_record.name);
  sync_primitives::AutoLock locker(mutex_);
  ServiceRecords::iterator service_record_it = std::find(
      service_records_.begin(), service_records_.end(), service_record);
  if (service_record_it != service_records_.end()) {
    service_records_.erase(service_record_it);
  }
}

void AvahiServiceResolverCallback(AvahiServiceResolver* avahi_service_resolver,
                                  AvahiIfIndex interface,
                                  AvahiProtocol protocol,
                                  AvahiResolverEvent event, const char* name,
                                  const char* type, const char* domain,
                                  const char* host_name,
                                  const AvahiAddress* avahi_address,
                                  uint16_t port, AvahiStringList* txt,
                                  AvahiLookupResultFlags flags, void* data) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(
      logger_,
      "avahi_service_resolver " << avahi_service_resolver << " interface " << interface <<
      " protocol " << protocol << " event " << event << " name " << name <<
      " type " << type << " domain " << domain << " host_name " << host_name <<
      " avahi_address " << avahi_address << " port " << port <<
      " txt " << txt << " flags " << flags << " data " << data);
  DnssdServiceBrowser* dnssd_service_browser =
      static_cast<DnssdServiceBrowser*>(data);

  DnssdServiceRecord service_record;
  service_record.interface = interface;
  service_record.protocol = protocol;
  service_record.domain_name = domain;
  service_record.host_name = host_name;
  service_record.name = name;
  service_record.type = type;
  switch (event) {
    case AVAHI_RESOLVER_FOUND:
      service_record.addr = avahi_address->data.ipv4.address;
      service_record.port = port;
      dnssd_service_browser->ServiceResolved(service_record);
      LOG4CXX_DEBUG(logger_, "event: AVAHI_RESOLVER_FOUND");
      break;
    case AVAHI_RESOLVER_FAILURE:
      dnssd_service_browser->ServiceResolveFailed(service_record);
      LOG4CXX_DEBUG(logger_, "event: AVAHI_RESOLVER_FAILURE");
      break;
  }

  avahi_service_resolver_free(avahi_service_resolver);
}

TransportAdapter::Error DnssdServiceBrowser::CreateAvahiClientAndBrowser() {
  LOG4CXX_AUTO_TRACE(logger_);
  if (0 != avahi_service_browser_) {
    avahi_service_browser_free(avahi_service_browser_);
    avahi_service_browser_ = NULL;
  }
  if (0 != avahi_client_) {
    avahi_client_free(avahi_client_);
    avahi_client_ = NULL;
  }

  int avahi_error;
  avahi_client_ = avahi_client_new(
      avahi_threaded_poll_get(avahi_threaded_poll_), AVAHI_CLIENT_NO_FAIL,
      AvahiClientCallback, this, &avahi_error);
  if (0 == avahi_client_) {
    LOG4CXX_ERROR(
        logger_,
        "Failed to create AvahiClient: " << avahi_strerror(avahi_error));
    return TransportAdapter::FAIL;
  }

  mutex_.Acquire();
  service_records_.clear();
  mutex_.Release();

  avahi_service_browser_ = avahi_service_browser_new(
      avahi_client_, AVAHI_IF_UNSPEC, /* TODO use only required iface */
      AVAHI_PROTO_INET, DNSSD_DEFAULT_SERVICE_TYPE, NULL, /* use default domain */
      static_cast<AvahiLookupFlags>(0), AvahiServiceBrowserCallback, this);
  return TransportAdapter::OK;
}

TransportAdapter::Error DnssdServiceBrowser::Init() {
  LOG4CXX_AUTO_TRACE(logger_);
  avahi_threaded_poll_ = avahi_threaded_poll_new();
  if (0 == avahi_threaded_poll_) {
    LOG4CXX_ERROR(logger_, "Failed to create AvahiThreadedPoll");
    return TransportAdapter::FAIL;
  }

  const TransportAdapter::Error err = CreateAvahiClientAndBrowser();
  if (err != TransportAdapter::OK) {
    LOG4CXX_ERROR(logger_, "Error " << err);
    return err;
  }

  const int poll_start_status = avahi_threaded_poll_start(avahi_threaded_poll_);
  if (0 != poll_start_status) {
    LOG4CXX_ERROR(logger_, "Failed to start AvahiThreadedPoll");
    return TransportAdapter::FAIL;
  }
  return TransportAdapter::OK;
}

TransportAdapter::Error DnssdServiceBrowser::Scan() {
  return TransportAdapter::NOT_SUPPORTED;
}

void DnssdServiceBrowser::AddService(AvahiIfIndex interface,
                                     AvahiProtocol protocol, const char* name,
                                     const char* type, const char* domain) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(
      logger_,
      "interface " << interface << " protocol " << protocol << " name " << name << " type " << type << " domain " << domain);
  DnssdServiceRecord record;
  record.interface = interface;
  record.protocol = protocol;
  record.domain_name = domain;
  record.name = name;
  record.type = type;

  sync_primitives::AutoLock locker(mutex_);
  if (service_records_.end()
      == std::find(service_records_.begin(), service_records_.end(), record)) {
    service_records_.push_back(record);
    avahi_service_resolver_new(avahi_client_, interface, protocol, name, type,
                               domain, AVAHI_PROTO_INET,
                               static_cast<AvahiLookupFlags>(0),
                               AvahiServiceResolverCallback, this);
  }
}

void DnssdServiceBrowser::RemoveService(AvahiIfIndex interface,
                                        AvahiProtocol protocol,
                                        const char* name, const char* type,
                                        const char* domain) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(
      logger_,
      "interface " << interface << " protocol " << protocol << " name " << name << " type " << type << " domain " << domain);
  DnssdServiceRecord record;
  record.interface = interface;
  record.protocol = protocol;
  record.name = name;
  record.type = type;
  record.domain_name = domain;

  sync_primitives::AutoLock locker(mutex_);
  service_records_.erase(
      std::remove(service_records_.begin(), service_records_.end(), record),
      service_records_.end());
}

DeviceVector DnssdServiceBrowser::PrepareDeviceVector() const {
  LOG4CXX_AUTO_TRACE(logger_);
  std::map<uint32_t, TcpDevice*> devices;
  for (ServiceRecords::const_iterator it = service_records_.begin();
      it != service_records_.end(); ++it) {
    const DnssdServiceRecord& service_record = *it;
    if (service_record.host_name.empty()) {
      continue;
    }
    if (devices[service_record.addr] == 0) {
      devices[service_record.addr] = new TcpDevice(service_record.addr,
                                                   service_record.host_name);
    }
    if (devices[service_record.addr] != 0) {
      devices[service_record.addr]->AddDiscoveredApplication(
          service_record.port);
    }
  }
  DeviceVector device_vector;
  device_vector.reserve(devices.size());
  for (std::map<uint32_t, TcpDevice*>::const_iterator it = devices.begin();
      it != devices.end(); ++it) {
    device_vector.push_back(DeviceSptr(it->second));
  }
  return device_vector;
}

}  // namespace transport_adapter
}  // namespace transport_manager
