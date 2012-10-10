/**
 * \file CBTAdapter.cpp
 * \brief Bluetooth adapter for AppLink.
 * \author AKara
 */

#include "CBTAdapter.hpp"

#include "btinclude.h"

#include <alsa/asoundlib.h>

namespace NsTransportLayer
{

   CBTAdapter::CBTAdapter()
   {
   }

   CBTAdapter::~CBTAdapter()
   {
   }

   int CBTAdapter::scanDevices(std::vector<CBTDevice>& devicesFound)
   {
      printf("%s:%d CBTAdapter::scanDevices()\n", __FILE__, __LINE__);
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
         perror("opening socket");
         return -1;
      }

      len  = 8;
      max_rsp = 255;
      flags = IREQ_CACHE_FLUSH;
      ii = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info));
    
      num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii, flags);
      if( num_rsp < 0 )
      {
         perror("hci_inquiry");
      }

      for (i = 0; i < num_rsp; i++)
      {
         ba2str(&(ii+i)->bdaddr, addr);
         memset(name, 0, sizeof(name));
         if (hci_read_remote_name(sock, &(ii+i)->bdaddr, sizeof(name), name, 0) < 0)
         {
            strcpy(name, "[unknown]");
         }
         printf("%s  %s\n", addr, name);
         CBTDevice device = CBTDevice(name, addr);
         devicesFound.push_back(device);
      }

    free( ii );
    close( sock );
   }

   int CBTAdapter::startSDPDiscoveryOnDevice(const char* targetDevice)
   {
      printf("%s:%d CBTAdapter::startSDPDiscoveryOnDevice()\n", __FILE__, __LINE__);
      uint8_t svc_uuid_int[] = { 0x93, 0x6D, 0xA0, 0x1F, 0x9A, 0xBD, 0x4D, 0x9D,
                                 0x80, 0xC7, 0x02, 0xAF, 0x85, 0xC8, 0x22, 0xA8 };
      uuid_t svc_uuid;
      int err;
      bdaddr_t target;
      sdp_list_t *response_list = NULL, *search_list, *attrid_list;
      sdp_session_t *session = 0;

      str2ba( targetDevice, &target );

      // connect to the SDP server running on the remote machine
      const bdaddr_t src = (bdaddr_t) {{0, 0, 0, 0, 0, 0}};
      session = sdp_connect( &src, &target, SDP_RETRY_IF_BUSY );

      // specify the UUID of the application we're searching for
      sdp_uuid128_create( &svc_uuid, &svc_uuid_int );
      search_list = sdp_list_append( NULL, &svc_uuid );

      // specify that we want a list of all the matching applications' attributes
      uint32_t range = 0x0000ffff;
      attrid_list = sdp_list_append( NULL, &range );

      // get a list of service records that have UUID 0xabcd
      err = sdp_service_search_attr_req( session, search_list,
                                         SDP_ATTR_REQ_RANGE, attrid_list, &response_list);
      sdp_list_t *r = response_list;

      // go through each of the service records
      for (; r; r = r->next )
      {
         sdp_record_t *rec = (sdp_record_t*) r->data;
         sdp_list_t *proto_list;
        
         // get a list of the protocol sequences
         if( sdp_get_access_protos( rec, &proto_list ) == 0 )
         {
            sdp_list_t *p = proto_list;

            // go through each protocol sequence
            for( ; p ; p = p->next )
            {
               sdp_list_t *pds = (sdp_list_t*)p->data;

               // go through each protocol list of the protocol sequence
               for( ; pds ; pds = pds->next )
               {
                  // check the protocol attributes
                  sdp_data_t *d = (sdp_data_t*)pds->data;
                  int proto = 0;
                  for( ; d; d = d->next )
                  {
                     switch( d->dtd )
                     {
                        case SDP_UUID16:
                        case SDP_UUID32:
                        case SDP_UUID128:
                           proto = sdp_uuid_to_proto( &d->val.uuid );
                           break;
                        case SDP_UINT8:
                           if( proto == RFCOMM_UUID )
                           {
                              printf("rfcomm channel: %d\n",d->val.int8);
                           }
                           break;
                     }
                  }
               }
               sdp_list_free( (sdp_list_t*)p->data, 0 );
            }
            sdp_list_free( proto_list, 0 );

         }

         printf("found service record 0x%x\n", rec->handle);
         sdp_record_free( rec );
      }
      sdp_close(session);
   }

   int CBTAdapter::processRFCOMM(int sockid)
   {
      printf("%s:%d CBTAdapter::processRFCOMM()\n", __FILE__, __LINE__);
      if (sockid < 0)
      {
         printf("%s:%d CBTAdapter::processRFCOMM() Bad socket id!\n", __FILE__, __LINE__);
         return -1;
      }

      char rfcommbuffer[255];
      int len;

      while(true)
      {
         // read from the RFCOMM socket
         // this socket has blocking turned off so it will never block,
         // even if no data is available
         len = recv(sockid, rfcommbuffer, 8, 0);
         if ((len >0) && (len == 8))
         {
            printf("%s:%d CBTAdapter::processRFCOMM() buf[0] = 0x%2X\n", __FILE__, __LINE__, rfcommbuffer[0]);
            printf("%s:%d CBTAdapter::processRFCOMM() buf[1] = 0x%2X\n", __FILE__, __LINE__, rfcommbuffer[1]);
            printf("%s:%d CBTAdapter::processRFCOMM() buf[2] = 0x%2X\n", __FILE__, __LINE__, rfcommbuffer[2]);
            printf("%s:%d CBTAdapter::processRFCOMM() buf[3] = 0x%2X\n", __FILE__, __LINE__, rfcommbuffer[3]);
            printf("%s:%d CBTAdapter::processRFCOMM() buf[4] = 0x%2X\n", __FILE__, __LINE__, rfcommbuffer[4]);
            printf("%s:%d CBTAdapter::processRFCOMM() buf[5] = 0x%2X\n", __FILE__, __LINE__, rfcommbuffer[5]);
            printf("%s:%d CBTAdapter::processRFCOMM() buf[6] = 0x%2X\n", __FILE__, __LINE__, rfcommbuffer[6]);
            printf("%s:%d CBTAdapter::processRFCOMM() buf[7] = 0x%2X\n", __FILE__, __LINE__, rfcommbuffer[7]);
         }

      }
   }

   int CBTAdapter::setClass(unsigned int cls, int timeout)
   {
      printf("%s:%d CBTAdapter::setClass()\n", __FILE__, __LINE__);
      int id;
      int fh;
      bdaddr_t btaddr;
      char pszaddr[18];

      // get the device ID
      // passing in NULL instead of a bdaddr_t will
      // give the ID of the first bluetooth device
      if ((id = hci_get_route(NULL)) < 0)
         return -1;

      // convert the device ID into a 6 byte bluetooth address
      if (hci_devba(id, &btaddr) < 0)
         return -1;

      // convert the address into a zero terminated string
      if (ba2str(&btaddr, pszaddr) < 0)
         return -1;

      // open a file handle to the HCI
      if ((fh = hci_open_dev(id)) < 0)
         return -1;

      // set the class
      if (hci_write_class_of_dev(fh, cls, timeout) != 0)
      {
         perror("hci_write_class ");
         return -1;
      }

      // close the file handle
      hci_close_dev(fh);

      printf("set device %s to class: 0x%06x\n", pszaddr, cls);

      return 0;
   }

   int CBTAdapter::registerSDP(uint8_t channel)
   {
      printf("%s:%d CBTAdapter::registerSDP()\n", __FILE__, __LINE__);
      const char *service_name = "HSP service";
      const char *service_dsc = "HSP";
      const char *service_prov = "nebland software, LLC";

      uuid_t hs_uuid, ga_uuid;

      sdp_profile_desc_t desc;

      uuid_t root_uuid, l2cap_uuid, rfcomm_uuid;
      sdp_list_t *l2cap_list = 0,
               *rfcomm_list = 0,
               *root_list = 0,
               *proto_list = 0,
               *access_proto_list = 0;

      sdp_data_t *channel_d = 0;

      int err = 0;
      sdp_session_t *session = 0;

      sdp_record_t *record = sdp_record_alloc();

      // set the name, provider, and description
      sdp_set_info_attr(record, service_name, service_prov, service_dsc);

      // service class ID (HEADSET)
      sdp_uuid16_create(&hs_uuid, HEADSET_SVCLASS_ID);

      if (!(root_list = sdp_list_append(0, &hs_uuid)))
         return -1;

      sdp_uuid16_create(&ga_uuid, GENERIC_AUDIO_SVCLASS_ID);

      if (!(root_list = sdp_list_append(root_list, &ga_uuid)))
         return -1;

      if (sdp_set_service_classes(record, root_list) < 0)
         return -1;

      sdp_list_free(root_list, 0);
      root_list = 0;

      // make the service record publicly browsable
      sdp_uuid16_create(&root_uuid, PUBLIC_BROWSE_GROUP);

      root_list = sdp_list_append(0, &root_uuid);
      sdp_set_browse_groups( record, root_list );

      // set l2cap information
      sdp_uuid16_create(&l2cap_uuid, L2CAP_UUID);
      l2cap_list = sdp_list_append( 0, &l2cap_uuid );
      proto_list = sdp_list_append( 0, l2cap_list );

      // set rfcomm information
      sdp_uuid16_create(&rfcomm_uuid, RFCOMM_UUID);
      channel_d = sdp_data_alloc(SDP_UINT8, &channel);
      rfcomm_list = sdp_list_append( 0, &rfcomm_uuid );

      sdp_list_append( rfcomm_list, channel_d );
      sdp_list_append( proto_list, rfcomm_list );

      // attach protocol information to service record
      access_proto_list = sdp_list_append( 0, proto_list );
      sdp_set_access_protos( record, access_proto_list );

      sdp_uuid16_create(&desc.uuid, HEADSET_PROFILE_ID);

      // set the version to 1.0
      desc.version = 0x0100;

      if (!(root_list = sdp_list_append(NULL, &desc)))
         return -1;

      if (sdp_set_profile_descs(record, root_list) < 0)
         return -1;

      // connect to the local SDP server and register the service record
      const bdaddr_t src = (bdaddr_t) {{0, 0, 0, 0, 0, 0}};
      const bdaddr_t dst = (bdaddr_t) {{0, 0, 0, 0xff, 0xff, 0xff}};
      session = sdp_connect( &src, &dst, SDP_RETRY_IF_BUSY );
      err = sdp_record_register(session, record, 0);

      // cleanup
      sdp_data_free( channel_d );
      sdp_list_free( l2cap_list, 0 );
      sdp_list_free( rfcomm_list, 0 );
      sdp_list_free( root_list, 0 );
      sdp_list_free( access_proto_list, 0 );

       return err;
   }

   int CBTAdapter::rfcommListen(uint8_t channel, char *targetDevID)
   {
      printf("%s:%d CBTAdapter::rfcommListen()\n", __FILE__, __LINE__);
      int sock;                  // socket descriptor for local listener
      int client;                // socket descriptor for remote client
      unsigned int len = sizeof(struct sockaddr_rc);

      struct sockaddr_rc remote;    // local rfcomm socket address
      struct sockaddr_rc local;     // remote rfcomm socket address
      char pszremote[18];

      // initialize a bluetooth socket
      sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

      local.rc_family = AF_BLUETOOTH;

      // TODO: change this to a local address if you know what
      // address to use
      const bdaddr_t src = (bdaddr_t) {{0, 0, 0, 0, 0, 0}};
      local.rc_bdaddr = src;
      local.rc_channel = channel;

      // bind the socket to a bluetooth device
      if (bind(sock, (struct sockaddr *)&local, sizeof(struct sockaddr_rc)) < 0)
         return -1;

      // set the listening queue length
      if (listen(sock, 1) < 0)
         return -1;

      printf("accepting connections on channel: %d\n", channel);

      // accept incoming connections; this is a blocking call
      client = accept(sock, (struct sockaddr *)&remote, &len);

      ba2str(&remote.rc_bdaddr, pszremote);

      printf("received connection from: %s\n", pszremote);
      //strcpy(targetDevID, pszremote);

      // turn off blocking
      if (fcntl(client, F_SETFL, O_NONBLOCK) < 0)
         return -1;

      // return the client socket descriptor
      return client;
   }

   int CBTAdapter::scoListen()
   {
      printf("%s:%d CBTAdapter::scoListen()\n", __FILE__, __LINE__);
      int sock;
      int client;
      unsigned int len = sizeof(struct sockaddr_sco);
      char pszremote[18];

      struct sockaddr_sco local;
      struct sockaddr_sco remote;

      sock = socket(PF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_SCO);

      local.sco_family = AF_BLUETOOTH;

      const bdaddr_t src = (bdaddr_t) {{0, 0, 0, 0, 0, 0}};
      local.sco_bdaddr = src;

      if (bind(sock, (struct sockaddr*)&local, sizeof(struct sockaddr_sco)) < 0)
         return -1;

      if (listen(sock, 1) < 0)
         return -1;

      client = accept(sock, (struct sockaddr*)&remote, &len);

      ba2str(&remote.sco_bdaddr, pszremote);

      printf("sco received connection from: %s\n", pszremote);

      // close the listener
      close(sock);

      return client;
   }

   int CBTAdapter::handleConnection(int rfcommsock, int scosock)
   {
      printf("%s:%d CBTAdapter::handleConnection()\n", __FILE__, __LINE__);
      char scobuffer[255];
      char rfcommbuffer[255];
      int len;

      snd_pcm_t *sndhandle;

      // open the default sound device
      if (snd_pcm_open((snd_pcm_t **)&sndhandle, "default", SND_PCM_STREAM_PLAYBACK, 0) < 0)
      {
         perror("snd_pcm_open ");
         return -1;
      }

      // initialize the device to handle an 8khz, single channel,
      // little endian audio data stream
      if (snd_pcm_set_params((snd_pcm_t *)sndhandle, SND_PCM_FORMAT_S16_LE, SND_PCM_ACCESS_RW_INTERLEAVED, 1,
            8000, 0, 0) < 0)
      {
         perror("set_params ");
         return -1;
      }

      while (1)
      {
         // read from the SCO socket
         // it should constantly stream data
         if ((len = recv(scosock, scobuffer, 255, 0)) <= 0)
            break;

         // send the sound data to the sound device
         // the sound device expects "frames".  since we have 16 bit, single
         // channel data, each 2 bytes is one "frame"
         snd_pcm_writei((snd_pcm_t *)sndhandle, scobuffer, len / 2);

         // read from the RFCOMM socket
         // this socket has blocking turned off so it will never block,
         // even if no data is available
         len = recv(rfcommsock, rfcommbuffer, 255, 0);

         // EWOULDBLOCK indicates the socket would block if we had a
         // blocking socket.  we'll safely continue if we receive that
         // error.  treat all other errors as fatal
         if (len < 0 && errno != EWOULDBLOCK)
         {
            perror("rfcomm recv ");
            break;
         }
         else if (len > 0)
         {
            // received a message; print it to the screen and
            // return ATOK to the remote device
            rfcommbuffer[len] = '\0';

            printf("rfcomm received: %s\n", rfcommbuffer);
            send(rfcommsock, "ATOK\r\n", 6, 0);
         }

         // printf("loop\n");
      }

      // close the sound device
      snd_pcm_close((snd_pcm_t *)sndhandle);

      close(scosock);
      close(rfcommsock);

      printf("client disconnected\n");

      return 0;
   }
} /* namespace NsTransportLayer */
