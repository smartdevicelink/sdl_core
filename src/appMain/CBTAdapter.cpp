/**
 * \file CBTAdapter.cpp
 * \brief Bluetooth adapter for AppLink.
 * \author AKara
 */

#include "CBTAdapter.hpp"
#include "btinclude.h"

namespace NsTransportLayer
{
   log4cplus::Logger CBTAdapter::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("CBTAdapter"));

   CBTAdapter::CBTAdapter():
   mpProtocolHandler(NULL),
   sockID(0)
   {
   }

   CBTAdapter::~CBTAdapter()
   {
   }

   int CBTAdapter::scanDevices(std::vector<CBTDevice>& devicesFound)
   {
      LOG4CPLUS_INFO(mLogger, "Scanning devices.");
      inquiry_info *ii = NULL;
      int max_rsp, num_rsp;
      int dev_id, sock, len, flags;
      int i;
      char addr[19] = { 0 };
      char name[248] = { 0 };

      dev_id = hci_get_route(NULL);
      sock = hci_open_dev( dev_id );
      if (dev_id < 0 || sock < 0)
      {
         LOG4CPLUS_ERROR(mLogger, "Not possible to open socket!");
         return -1;
      }

      len  = 8;
      max_rsp = 255;
      flags = IREQ_CACHE_FLUSH;
      ii = (inquiry_info*)malloc(max_rsp *sizeof(inquiry_info));
    
      num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii, flags);
      if(num_rsp < 0)
      {
         LOG4CPLUS_WARN(mLogger, "hci_inquiry failed!");
      }

      for (i = 0; i < num_rsp; i++)
      {
         ba2str(&(ii+i)->bdaddr, addr);
         memset(name, 0, sizeof(name));
         if (hci_read_remote_name(sock, &(ii+i)->bdaddr, sizeof(name), name, 0) < 0)
         {
            strcpy(name, "[unknown]");
         }

         CBTDevice device = CBTDevice(name, addr);
         devicesFound.push_back(device);
      }

    free( ii );
    close( sock );
   }

   int CBTAdapter::startSDPDiscoveryOnDevice(const char* targetDevice, std::vector<int>& portsRFCOMMFound)
   {
      LOG4CPLUS_INFO_EXT(mLogger, "Start SDP Discovery.");
      uint8_t svc_uuid_int[] = { 0x93, 0x6D, 0xA0, 0x1F, 0x9A, 0xBD, 0x4D, 0x9D,
                                 0x80, 0xC7, 0x02, 0xAF, 0x85, 0xC8, 0x22, 0xA8 };
      uuid_t svc_uuid;
      int err;
      bdaddr_t target;
      sdp_list_t *response_list = NULL, *search_list, *attrid_list;
      sdp_session_t *session = 0;

      str2ba( targetDevice, &target );

      // connect to the SDP server running on the remote machine
      LOG4CPLUS_INFO_EXT(mLogger, "connect to the SDP server running on the remote machine");
      const bdaddr_t src = (bdaddr_t) {{0, 0, 0, 0, 0, 0}};
      session = sdp_connect( &src, &target, SDP_RETRY_IF_BUSY );
      if ( !session )
      {
         LOG4CPLUS_ERROR(mLogger, "Cannot connect to SDP server on remote machine.");
         return -1;
      }

      // specify the UUID of the application we're searching for
      LOG4CPLUS_INFO_EXT(mLogger, "specify the UUID of the application we're searching for");
      sdp_uuid128_create( &svc_uuid, &svc_uuid_int );
      search_list = sdp_list_append( NULL, &svc_uuid );

      // specify that we want a list of all the matching applications' attributes
      LOG4CPLUS_INFO_EXT(mLogger, "specify that we want a list of all the matching applications' attributes");
      uint32_t range = 0x0000ffff;
      attrid_list = sdp_list_append( NULL, &range );
      // get a list of service records that have UUID 0xabcd
      LOG4CPLUS_INFO_EXT(mLogger, "get a list of service records that have UUID 0xabcd");
      err = sdp_service_search_attr_req( session, search_list,
                                         SDP_ATTR_REQ_RANGE, attrid_list, &response_list);
      //check error
      if (0 > err)
      {
         LOG4CPLUS_ERROR(mLogger, "startSDPDiscoveryOnDevice: Error: " << err << ".");
         return -1;
      }

      sdp_list_t *r = response_list;

      // go through each of the service records
      for (; r; r = r->next)
      {
         sdp_record_t *rec = (sdp_record_t*) r->data;
         sdp_list_t *proto_list;
        
         // get a list of the protocol sequences
         if(sdp_get_access_protos( rec, &proto_list ) == 0)
         {
            sdp_list_t *p = proto_list;

            // go through each protocol sequence
            for(; p ; p = p->next)
            {
               sdp_list_t *pds = (sdp_list_t*)p->data;

               // go through each protocol list of the protocol sequence
               for(; pds ; pds = pds->next)
               {
                  // check the protocol attributes
                  sdp_data_t *d = (sdp_data_t*)pds->data;
                  int proto = 0;
                  for(; d; d = d->next)
                  {
                     switch(d->dtd)
                     {
                        case SDP_UUID16:
                        case SDP_UUID32:
                        case SDP_UUID128:
                           proto = sdp_uuid_to_proto(&d->val.uuid);
                           break;
                        case SDP_UINT8:
                           if(proto == RFCOMM_UUID)
                           {
                              portsRFCOMMFound.push_back(d->val.int8);
                           }
                           break;
                     }
                  }
               }
               sdp_list_free((sdp_list_t*)p->data, 0);
            }
            sdp_list_free(proto_list, 0);

         }

         sdp_record_free( rec );
      }
      sdp_close(session);
   }

   int CBTAdapter::startRFCOMMConnection(const char* targetDevice, int portRFCOMM)
   {
      LOG4CPLUS_INFO(mLogger, "StartRFCOMMConnection()device " << targetDevice << ", port " << portRFCOMM);
      struct sockaddr_rc addr = { 0 };
      int status;

      // allocate a socket
      sockID = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

      // set the connection parameters (who to connect to)
      addr.rc_family = AF_BLUETOOTH;
      addr.rc_channel = (uint8_t) portRFCOMM;
      str2ba(targetDevice, &addr.rc_bdaddr);

      // connect to server
      status = connect(sockID, (struct sockaddr *)&addr, sizeof(addr));

      if(0 < status)
      {
         LOG4CPLUS_ERROR(mLogger, "Failed to open port!");
         return -1;
      }
      LOG4CPLUS_INFO(mLogger, "Connection successful. sockID= " << sockID);
      return sockID;
   }

   int CBTAdapter::processRFCOMM(int sockid)
   {
      LOG4CPLUS_INFO(mLogger, "ProcessRFCOMM");
      if (sockid < 0)
      {
         LOG4CPLUS_ERROR(mLogger, "PprocessRFCOMM: Bad socket id");
         return -1;
      }

      unsigned char rfcommbuffer[8];
      int len;

      while(true)
      {
         // read from the RFCOMM socket
         // this socket has blocking turned off so it will never block,
         // even if no data is available
         len = recv(sockid, rfcommbuffer, 8, 0);
         if ((len > 0) && (len == 8))
         {
            if ((rfcommbuffer[0] == 16) && (rfcommbuffer[1] == 7) && (rfcommbuffer[2] == 1))
            {
               LOG4CPLUS_INFO(mLogger, "Start session (RPC Service)");

               void* sPacketData = malloc(8);
               memset(sPacketData, 0, 8);
               memcpy(sPacketData, rfcommbuffer, 8);
               blobQueue.push(Blob((UInt8*)sPacketData, 8, blobQueue.size()));
               if (NULL != mpProtocolHandler)
               {
                  mpProtocolHandler->dataReceived();
               }
            } else if ((rfcommbuffer[0] == 16) && (rfcommbuffer[1] == 15) && (rfcommbuffer[2] == 1))
            {
               LOG4CPLUS_INFO(mLogger,"Start session (Bulk Service)");
               void* sPacketData = malloc(8);
               memset(sPacketData, 0, 8);
               memcpy(sPacketData, rfcommbuffer, 8);
               blobQueue.push(Blob((UInt8*)sPacketData, 8, blobQueue.size()));
               if (NULL != mpProtocolHandler)
               {
                  mpProtocolHandler->dataReceived();
               }
            }else if ((rfcommbuffer[0] == 17) && (rfcommbuffer[1] == 7))
            {
               LOG4CPLUS_INFO(mLogger,"Single frame RPC message!");
               unsigned int frameSize = (rfcommbuffer[4]<<24);
               frameSize |= (rfcommbuffer[5]<<16);
               frameSize |= (rfcommbuffer[6]<<8);
               frameSize |= rfcommbuffer[7];
               LOG4CPLUS_INFO(mLogger,"frameSize = 0x" << frameSize);
               unsigned char rfcommpayloadbuffer[1024];
               if (672 > frameSize)
               {// one L2CAP packet
                  len = recv(sockid, rfcommpayloadbuffer, frameSize, 0);
               } else
               {
                  //TODO: We need to read all L2CAP packets to get whole frame
               }
               if (len == frameSize)
               {
                  void* sPacketData = malloc(8 + frameSize+1);
                  memset(sPacketData, 0, 8 + frameSize+1);
                  memcpy(sPacketData, rfcommbuffer, 8);
                  memcpy((UInt8*)sPacketData + 8, rfcommpayloadbuffer, frameSize);
                  blobQueue.push(Blob((UInt8*)sPacketData, frameSize+8, blobQueue.size()));

                  if (NULL != mpProtocolHandler)
                  {
                     mpProtocolHandler->dataReceived();
                  }
               } else
               {
                  LOG4CPLUS_WARN(mLogger, "Not All frame received frameSize = 0x" << frameSize);
               }

            } else if ((rfcommbuffer[0] == 18) && (rfcommbuffer[1] == 7))
            {
               LOG4CPLUS_INFO(mLogger, "MultiPacket RPC frame");
            } else
            {
               LOG4CPLUS_INFO(mLogger, "Unknown packet type!");
               return -1;
            }
         } else
         {
            LOG4CPLUS_INFO(mLogger, "len = " << len);
         }
      }
   }

  void CBTAdapter::initBluetooth(Bluetooth::IBluetoothHandler * pHandler)
  {
      LOG4CPLUS_INFO(mLogger, "InitBluetooth.");
      if (NULL != pHandler)
      {
         mpProtocolHandler = pHandler;
      }
  }

  void CBTAdapter::deinitBluetooth()
  {
      LOG4CPLUS_INFO(mLogger, "DeinitBluetooth");
  }

  const Blob CBTAdapter::getBuffer()
  {
      LOG4CPLUS_INFO(mLogger, "GetBuffer");
      return blobQueue.front();
  }

  void CBTAdapter::releaseBuffer(const Blob& blob)
  {
      LOG4CPLUS_INFO(mLogger, "ReleaseBuffer");
      free(blob.buffer());
      blobQueue.pop();
  }

  void CBTAdapter::sendBuffer(UInt8 * pBuffer, size_t size)
  {
      LOG4CPLUS_INFO(mLogger, "SendBuffer");

      if (0!= sockID)
      {
         int status = write(sockID, pBuffer, size);
         if (0 > status)
         {
            LOG4CPLUS_ERROR(mLogger, "Socket write error!");
         }
      } else
      {
         LOG4CPLUS_ERROR(mLogger, "No socket opened!");
      }
  }
} /* namespace NsTransportLayer */
