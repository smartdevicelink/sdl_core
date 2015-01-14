/**
 * \file websocket_handler.cpp
 * \brief WebSocket Handler.
 * \author AKara
 */

#include <cstdio>

#include <cstring>
#include <sstream>
#include <netinet/in.h>

#ifdef _WIN32
#include <winsock2.h>
#endif//_WIN32

#include "websocket_handler.hpp"

#include "libMBDebugHelper.h"
#include "md5.h"

namespace NsMessageBroker 
{

   unsigned int CWebSocketHandler::parseWebSocketDataLength(
       const char* Buffer, unsigned int& b_size) {

     unsigned char payload =
         (unsigned char)((Buffer[1] & 0x40) | (Buffer[1] & 0x20) |
         (Buffer[1] & 0x10) | (Buffer[1] & 0x08) | (Buffer[1] & 0x04) |
         (Buffer[1] & 0x02) | (Buffer[1] & 0x01));
      unsigned long length = 0;
      unsigned char position = 2; // current buffer position

      switch(payload) {
         case 126:
            {
               length = (unsigned char)Buffer[position++];
               length <<=8;
               length |= (unsigned char)Buffer[position++];
               break;
            }
         case 127:
            {
               length = (unsigned char)Buffer[position++];
               length <<=8;
               length |= (unsigned char)Buffer[position++];
               length <<=8;
               length |= (unsigned char)Buffer[position++];
               length <<=8;
               length |= (unsigned char)Buffer[position++];
               length <<=8;
               length |= (unsigned char)Buffer[position++];
               length <<=8;
               length |= (unsigned char)Buffer[position++];
               length <<=8;
               length |= (unsigned char)Buffer[position++];
               length <<=8;
               length |= (unsigned char)Buffer[position++];
               break;
            }
         default:
            {
               length = payload;
               return length;
            }
      }

      return length;
   }

   int CWebSocketHandler::parseWebSocketData(char* Buffer, unsigned int& b_size)
   {
     // Please see RFC6455 standard protocol specification:
     //http://tools.ietf.org/html/rfc6455
     // Chapter 5.2
     DBG_MSG(("CWebSocketHandler::parseWebSocketData()b_size = %d\n", b_size));
     char* recBuffer = Buffer;
     unsigned int parsedBufferPosition = 0;
     unsigned char position = 0; // current buffer position
     unsigned int size = b_size;

     while (0 < size) {

       bool fin = ((recBuffer[0] & 0x80) | (recBuffer[0] & 0x01)) == 0x81;
       bool rsv1 = (recBuffer[0] & 0x40) == 0x40;
       bool rsv2 = (recBuffer[0] & 0x20) == 0x20;
       bool rsv3 = (recBuffer[0] & 0x10) == 0x10;
       unsigned char opCode = ((recBuffer[0] & 0x08) | (recBuffer[0] & 0x04) |
           (recBuffer[0] & 0x02) | (recBuffer[0] & 0x01));

       bool mask = (recBuffer[1] & 0x80) == 0x80;

       DBG_MSG(("CWebSocketHandler::fin = %d recBuffer[0] = 0x%02X\n"
                " parsedlength = %d b_size= %d parsedBufferPosition = %d\n"
                "rsv1 = %d, rsv2 = %d, rsv3 = %d, opCode = %u\n",
           fin, recBuffer[0], parsedBufferPosition + position,
           size, parsedBufferPosition, rsv1, rsv2, rsv3, opCode));

       if ((rsv1)|(rsv2)|(rsv3)) {
         DBG_MSG(("rsv1 or rsv2 or rsv3 is 0 \n"));
         break;
       }

       switch(opCode) {
         case 0x0: break; //Continuation frame
         case 0x1: break; //Text Frame
         case 0x2: break; //Binary Frame
         case 0x8: break; //Connection close Frame
         case 0x9: break; //ping Frame
         case 0xA: break; //Pong Frame
         default: break; //Unknown frame
       }

       if (false == fin) {
          break;
       }

       unsigned char payload = (unsigned char)
        ((recBuffer[1] & 0x40) | (recBuffer[1] & 0x20) | (recBuffer[1] & 0x10) |
        (recBuffer[1] & 0x08) | (recBuffer[1] & 0x04) | (recBuffer[1] & 0x02) |
        (recBuffer[1] & 0x01));

       unsigned long length = parseWebSocketDataLength(recBuffer, size);
       position = 2;

       if (length > size) {
          DBG_MSG_ERROR(("Incomplete message"));
          return b_size;
       }

       switch(payload) {
          case 126:
             {
                position +=2;
                break;
             }
          case 127:
             {
                position +=8;
                break;
             }
          default:
             {
                break;
             }
       }

       if (mask)
       {
          unsigned char maskKeys[4];
          maskKeys[0] = recBuffer[position++];
          maskKeys[1] = recBuffer[position++];
          maskKeys[2] = recBuffer[position++];
          maskKeys[3] = recBuffer[position++];
          DBG_MSG(("CWebSocketHandler::parseWebSocketData()maskKeys[0]:0x%02X;"
                 " maskKeys[1]:0x%02X; maskKeys[2]:0x%02X; maskKeys[3]:0x%02X\n"
                 , maskKeys[0], maskKeys[1], maskKeys[2], maskKeys[3]));
          for (unsigned long i = position; i < position+length; i++)
          {
             recBuffer[i] = recBuffer[i] ^ maskKeys[(i-position)%4];
          }
       }
       DBG_MSG(("CWebSocketHandler::parseWebSocketData()length:%d; size:%d;"
                " position:%d\n", (int)length, size, position));

       for (unsigned long i = position; (i < size && i < position+length); i++)
       {
          Buffer[parsedBufferPosition++] = recBuffer[i];
       }

       recBuffer += length+position;
       size -= length+position;
     }

     b_size = parsedBufferPosition;
     return b_size;
   }

   int CWebSocketHandler::prepareWebSocketDataHeader(unsigned char* Buffer,
                                                     unsigned long long b_size)
   {
      unsigned int headerLength = 2;
      unsigned char payload;

      memset(Buffer, 0, headerLength);
      Buffer[0] = 0x81;    // 129

      if (b_size <= 125)
      {
        payload = b_size;
        Buffer[1] = b_size;      // string length
      } else if (b_size >= 126 && b_size <= 65535)
      {
        headerLength += 2;
        payload = 126;
        Buffer[1] = 0x7E;  // 126
      } else
      {
        headerLength += 8;
        payload = 127;
        Buffer[1] = 0x7F;  // 127
      }


      if (payload == 126)
      {
         Buffer[2] = (b_size>>8);
         Buffer[3] = b_size;
      } else if (payload == 127)
      {
         Buffer[9] = (b_size       & 0xFF);
         Buffer[8] = ((b_size>>8)  & 0xFF);
         Buffer[7] = ((b_size>>16) & 0xFF);
         Buffer[6] = ((b_size>>24) & 0xFF);
         Buffer[5] = ((b_size>>32) & 0xFF);
         Buffer[4] = ((b_size>>40) & 0xFF);
         Buffer[3] = ((b_size>>48) & 0xFF);
         Buffer[2] = ((b_size>>56) & 0xFF);
      }
      return headerLength;
}

   void CWebSocketHandler::handshake_0405(std::string &key)
   {
      static const char *websocket_magic_guid_04 =
                   "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
      char accept_buf[MAX_WEBSOCKET_04_KEY_LEN + 37];
      unsigned char hash[20] = {0xb3, 0x7a, 0x4f, 0x2c, 0xc0, 0x62, 0x4f, 0x16, 0x90, 0xf6, 0x46, 0x06, 0xcf, 0x38, 0x59, 0x45, 0xb2, 0xbe, 0xc4, 0xea};
      int accept_len;

      strcpy(accept_buf, key.c_str());
      strcpy(accept_buf + key.length(), websocket_magic_guid_04);

      SHA1((unsigned char *)accept_buf, key.length() + strlen(websocket_magic_guid_04), hash);

      accept_len = lws_b64_encode_string((char *)hash, 20, accept_buf, sizeof accept_buf);
      if (accept_len < 0)
      {
         fprintf(stderr, "Base64 encoded hash too long\n");
      }
      fprintf(stderr, "accept_buf: %s\n", accept_buf);
      key = accept_buf;
   }

   void CWebSocketHandler::sha1_step(struct sha1_ctxt *ctxt)
   {
      unsigned int   a, b, c, d, e;
      size_t t, s;
      unsigned int   tmp;

      struct sha1_ctxt tctxt;

      memcpy(&tctxt.m.b8[0], &ctxt->m.b8[0], 64);
      ctxt->m.b8[0] = tctxt.m.b8[3]; ctxt->m.b8[1] = tctxt.m.b8[2];
      ctxt->m.b8[2] = tctxt.m.b8[1]; ctxt->m.b8[3] = tctxt.m.b8[0];
      ctxt->m.b8[4] = tctxt.m.b8[7]; ctxt->m.b8[5] = tctxt.m.b8[6];
      ctxt->m.b8[6] = tctxt.m.b8[5]; ctxt->m.b8[7] = tctxt.m.b8[4];
      ctxt->m.b8[8] = tctxt.m.b8[11]; ctxt->m.b8[9] = tctxt.m.b8[10];
      ctxt->m.b8[10] = tctxt.m.b8[9]; ctxt->m.b8[11] = tctxt.m.b8[8];
      ctxt->m.b8[12] = tctxt.m.b8[15]; ctxt->m.b8[13] = tctxt.m.b8[14];
      ctxt->m.b8[14] = tctxt.m.b8[13]; ctxt->m.b8[15] = tctxt.m.b8[12];
      ctxt->m.b8[16] = tctxt.m.b8[19]; ctxt->m.b8[17] = tctxt.m.b8[18];
      ctxt->m.b8[18] = tctxt.m.b8[17]; ctxt->m.b8[19] = tctxt.m.b8[16];
      ctxt->m.b8[20] = tctxt.m.b8[23]; ctxt->m.b8[21] = tctxt.m.b8[22];
      ctxt->m.b8[22] = tctxt.m.b8[21]; ctxt->m.b8[23] = tctxt.m.b8[20];
      ctxt->m.b8[24] = tctxt.m.b8[27]; ctxt->m.b8[25] = tctxt.m.b8[26];
      ctxt->m.b8[26] = tctxt.m.b8[25]; ctxt->m.b8[27] = tctxt.m.b8[24];
      ctxt->m.b8[28] = tctxt.m.b8[31]; ctxt->m.b8[29] = tctxt.m.b8[30];
      ctxt->m.b8[30] = tctxt.m.b8[29]; ctxt->m.b8[31] = tctxt.m.b8[28];
      ctxt->m.b8[32] = tctxt.m.b8[35]; ctxt->m.b8[33] = tctxt.m.b8[34];
      ctxt->m.b8[34] = tctxt.m.b8[33]; ctxt->m.b8[35] = tctxt.m.b8[32];
      ctxt->m.b8[36] = tctxt.m.b8[39]; ctxt->m.b8[37] = tctxt.m.b8[38];
      ctxt->m.b8[38] = tctxt.m.b8[37]; ctxt->m.b8[39] = tctxt.m.b8[36];
      ctxt->m.b8[40] = tctxt.m.b8[43]; ctxt->m.b8[41] = tctxt.m.b8[42];
      ctxt->m.b8[42] = tctxt.m.b8[41]; ctxt->m.b8[43] = tctxt.m.b8[40];
      ctxt->m.b8[44] = tctxt.m.b8[47]; ctxt->m.b8[45] = tctxt.m.b8[46];
      ctxt->m.b8[46] = tctxt.m.b8[45]; ctxt->m.b8[47] = tctxt.m.b8[44];
      ctxt->m.b8[48] = tctxt.m.b8[51]; ctxt->m.b8[49] = tctxt.m.b8[50];
      ctxt->m.b8[50] = tctxt.m.b8[49]; ctxt->m.b8[51] = tctxt.m.b8[48];
      ctxt->m.b8[52] = tctxt.m.b8[55]; ctxt->m.b8[53] = tctxt.m.b8[54];
      ctxt->m.b8[54] = tctxt.m.b8[53]; ctxt->m.b8[55] = tctxt.m.b8[52];
      ctxt->m.b8[56] = tctxt.m.b8[59]; ctxt->m.b8[57] = tctxt.m.b8[58];
      ctxt->m.b8[58] = tctxt.m.b8[57]; ctxt->m.b8[59] = tctxt.m.b8[56];
      ctxt->m.b8[60] = tctxt.m.b8[63]; ctxt->m.b8[61] = tctxt.m.b8[62];
      ctxt->m.b8[62] = tctxt.m.b8[61]; ctxt->m.b8[63] = tctxt.m.b8[60];

      a = H(0); b = H(1); c = H(2); d = H(3); e = H(4);

      for (t = 0; t < 20; t++)
      {
         s = t & 0x0f;
         if (t >= 16)
            W(s) = S(1, W((s+13) & 0x0f) ^ W((s+8) & 0x0f) ^
                        W((s+2) & 0x0f) ^ W(s));

         tmp = S(5, a) + F0(b, c, d) + e + W(s) + K(t);
         e = d; d = c; c = S(30, b); b = a; a = tmp;
      }
      for (t = 20; t < 40; t++)
      {
         s = t & 0x0f;
         W(s) = S(1, W((s+13) & 0x0f) ^ W((s+8) & 0x0f) ^
                        W((s+2) & 0x0f) ^ W(s));
         tmp = S(5, a) + F1(b, c, d) + e + W(s) + K(t);
         e = d; d = c; c = S(30, b); b = a; a = tmp;
      }
      for (t = 40; t < 60; t++)
      {
         s = t & 0x0f;
         W(s) = S(1, W((s+13) & 0x0f) ^ W((s+8) & 0x0f) ^
                        W((s+2) & 0x0f) ^ W(s));
         tmp = S(5, a) + F2(b, c, d) + e + W(s) + K(t);
         e = d; d = c; c = S(30, b); b = a; a = tmp;
      }
      for (t = 60; t < 80; t++)
      {
         s = t & 0x0f;
         W(s) = S(1, W((s+13) & 0x0f) ^ W((s+8) & 0x0f) ^
                        W((s+2) & 0x0f) ^ W(s));
         tmp = S(5, a) + F3(b, c, d) + e + W(s) + K(t);
         e = d; d = c; c = S(30, b); b = a; a = tmp;
      }

      H(0) = H(0) + a;
      H(1) = H(1) + b;
      H(2) = H(2) + c;
      H(3) = H(3) + d;
      H(4) = H(4) + e;

      memset(&ctxt->m.b8[0], 0, 64);
   }

   void CWebSocketHandler::sha1_init(struct sha1_ctxt *ctxt)
   {
      memset(ctxt, 0, sizeof(struct sha1_ctxt));
      H(0) = 0x67452301;
      H(1) = 0xefcdab89;
      H(2) = 0x98badcfe;
      H(3) = 0x10325476;
      H(4) = 0xc3d2e1f0;
   }

   void CWebSocketHandler::sha1_pad(struct sha1_ctxt *ctxt)
   {
      size_t padlen;      /*pad length in bytes*/
      size_t padstart;

      PUTPAD(0x80);

      padstart = COUNT % 64;
      padlen = 64 - padstart;
      if (padlen < 8)
      {
         memset(&ctxt->m.b8[padstart], 0, padlen);
         COUNT += padlen;
         COUNT %= 64;
         sha1_step(ctxt);
         padstart = COUNT % 64;   /* should be 0 */
         padlen = 64 - padstart;   /* should be 64 */
      }
      memset(&ctxt->m.b8[padstart], 0, padlen - 8);
      COUNT += (padlen - 8);
      COUNT %= 64;

      PUTPAD(ctxt->c.b8[7]); PUTPAD(ctxt->c.b8[6]);
      PUTPAD(ctxt->c.b8[5]); PUTPAD(ctxt->c.b8[4]);
      PUTPAD(ctxt->c.b8[3]); PUTPAD(ctxt->c.b8[2]);
      PUTPAD(ctxt->c.b8[1]); PUTPAD(ctxt->c.b8[0]);
   }

   void CWebSocketHandler::sha1_loop(struct sha1_ctxt *ctxt, const unsigned char *input, size_t len)
   {
      size_t gaplen;
      size_t gapstart;
      size_t off;
      size_t copysiz;

      off = 0;

      while (off < len)
      {
         gapstart = COUNT % 64;
         gaplen = 64 - gapstart;

         copysiz = (gaplen < len - off) ? gaplen : len - off;
         memcpy(&ctxt->m.b8[gapstart], &input[off], copysiz);
         COUNT += copysiz;
         COUNT %= 64;
         ctxt->c.b64[0] += copysiz * 8;
         if (COUNT % 64 == 0)
            sha1_step(ctxt);
         off += copysiz;
      }
   }

   void CWebSocketHandler::sha1_result(struct sha1_ctxt *ctxt, unsigned char* digest0)
   {
      unsigned char *digest;

      digest = (unsigned char *)digest0;
      sha1_pad(ctxt);
      digest[0] = ctxt->h.b8[3]; digest[1] = ctxt->h.b8[2];
      digest[2] = ctxt->h.b8[1]; digest[3] = ctxt->h.b8[0];
      digest[4] = ctxt->h.b8[7]; digest[5] = ctxt->h.b8[6];
      digest[6] = ctxt->h.b8[5]; digest[7] = ctxt->h.b8[4];
      digest[8] = ctxt->h.b8[11]; digest[9] = ctxt->h.b8[10];
      digest[10] = ctxt->h.b8[9]; digest[11] = ctxt->h.b8[8];
      digest[12] = ctxt->h.b8[15]; digest[13] = ctxt->h.b8[14];
      digest[14] = ctxt->h.b8[13]; digest[15] = ctxt->h.b8[12];
      digest[16] = ctxt->h.b8[19]; digest[17] = ctxt->h.b8[18];
      digest[18] = ctxt->h.b8[17]; digest[19] = ctxt->h.b8[16];
   }

   /*
    * This should look and work like the libcrypto implementation
    */

   unsigned char * CWebSocketHandler::SHA1(const unsigned char *d, size_t n, unsigned char *md)
   {
      struct sha1_ctxt ctx;

      sha1_init(&ctx);
      sha1_loop(&ctx, d, n);
      sha1_result(&ctx, (unsigned char*)md);

      return md;
   }

   int CWebSocketHandler::lws_b64_encode_string(const char *in, int in_len, char *out, int out_size)
   {
      unsigned char triple[3];
      int i;
      int len;
      int line = 0;
      int done = 0;

      while (in_len)
      {
         len = 0;
         for (i = 0; i < 3; i++)
         {
            if (in_len)
            {
               triple[i] = *in++;
               len++;
               in_len--;
            } else
               triple[i] = 0;
         }
         if (len)
         {

            if (done + 4 >= out_size)
               return -1;

            *out++ = encode[triple[0] >> 2];
            *out++ = encode[((triple[0] & 0x03) << 4) |
                          ((triple[1] & 0xf0) >> 4)];
            *out++ = (len > 1 ? encode[((triple[1] & 0x0f) << 2) |
                       ((triple[2] & 0xc0) >> 6)] : '=');
            *out++ = (len > 2 ? encode[triple[2] & 0x3f] : '=');

            done += 4;
            line += 4;
         }
         if (line >= 72)
         {

            if (done + 2 >= out_size)
               return -1;

            *out++ = '\r';
            *out++ = '\n';
            done += 2;
            line = 0;
         }
      }

      if (done + 1 >= out_size)
         return -1;

      *out++ = '\0';

      return done;
   }

   /*
    * returns length of decoded string in out, or -1 if out was too small
    * according to out_size
    */

   int CWebSocketHandler::lws_b64_decode_string(const char *in, char *out, int out_size)
   {
      int len;
      int i;
      int done = 0;
      unsigned char v;
      unsigned char quad[4];

      while (*in)
      {

         len = 0;
         for (i = 0; i < 4 && *in; i++)
         {

            v = 0;
            while (*in && !v)
            {

               v = *in++;
               v = (v < 43 || v > 122) ? 0 : decode[v - 43];
               if (v)
                  v = (v == '$') ? 0 : v - 61;
               if (*in)
               {
                  len++;
                  if (v)
                     quad[i] = v - 1;
               } else
                  quad[i] = 0;
            }
         }
         if (!len)
            continue;

         if (out_size < (done + len - 1))
            /* out buffer is too small */
            return -1;

         if (len >= 2)
            *out++ = quad[0] << 2 | quad[1] >> 4;
         if (len >= 3)
            *out++ = quad[1] << 4 | quad[2] >> 2;
         if (len >= 4)
            *out++ = ((quad[2] << 6) & 0xc0) | quad[3];

         done += len - 1;
      }

      if (done + 1 >= out_size)
         return -1;

      *out++ = '\0';

      return done;
   }

   int CWebSocketHandler::lws_b64_selftest(void)
   {
      char buf[64];
      int n;
      unsigned int test;
      static const char *plaintext[] = {"sanity check base 64"};
      static const char *coded[] = {"c2FuaXR5IGNoZWNrIGJhc2UgNjQ="};

      for (test = 0; test < sizeof plaintext / sizeof(plaintext[0]); test++)
      {

         buf[sizeof(buf) - 1] = '\0';
         n = lws_b64_encode_string(plaintext[test],
                     strlen(plaintext[test]), buf, sizeof buf);
         if (n != (int)strlen(coded[test]) || strcmp(buf, coded[test]))
         {
            fprintf(stderr, "Failed lws_b64 encode selftest "
                     "%d result '%s' %d\n", test, buf, n);
            return -1;
         }

         buf[sizeof(buf) - 1] = '\0';
         n = lws_b64_decode_string(coded[test], buf, sizeof buf);
         if (n != (int)strlen(plaintext[test]) ||
                      strcmp(buf, plaintext[test]))
         {
            fprintf(stderr, "Failed lws_b64 decode selftest "
                     "%d result '%s' %d\n", test, buf, n);
            return -1;
         }
      }

      return 0;
   }
   
   rawBytes CWebSocketHandler::handshake_hybi00(const std::string &key1, const std::string &key2, const rawBytes &key3)
   {
      if (key3.size() < 8)
      {
         DBG_MSG_ERROR(("key3's size is %d, less than 8 bytes\n", key3.size()));
         return rawBytes();
      }
      
      unsigned long number1 = extractNumber(key1);
      unsigned long number2 = extractNumber(key2);
      DBG_MSG(("number1 is %ld, number2 is %ld\n", number1, number2));
      
      if ((number1 == 0) || (number2 == 0))
      {
         return rawBytes();
      }
      
      // represent the numbers in big-endian format (network-byte order)
      unsigned long bigEndianNumber1 = htonl(number1);
      unsigned long bigEndianNumber2 = htonl(number2);
      
      // the temporary key consists of bytes of the first and second numbers
      // and the key3
      rawBytes key(8);
      memcpy(&key[0], &bigEndianNumber1, 4);
      memcpy(&key[4], &bigEndianNumber2, 4);
      key.insert(key.end(), key3.begin(), key3.begin() + 8);
      
      MD5 md5(std::string(key.begin(), key.end()));
      char digest[16];
      md5.getdigest(digest);
      rawBytes resultBytes(&digest[0], &digest[16]);
      
      return resultBytes;
   }
   
   unsigned long CWebSocketHandler::extractNumber(const std::string &key) const
   {
      // leave digits only
      // and count the number of spaces in the key
      std::string keyDigits;
      int spacesCountKey = 0;
      for (unsigned int index = 0; index < key.length(); ++index)
      {
         char keyChar = key[index];
         if (keyChar == ' ')
         {
            ++spacesCountKey;
         }
         else if (isdigit(keyChar))
         {
            keyDigits += keyChar;
         }
      }
      
      unsigned long result = 0;
      
      // convert string to number
      long long numberKey;
      if (std::stringstream(keyDigits) >> numberKey)
      {
         if (spacesCountKey != 0)
         {
            if (numberKey % spacesCountKey == 0)
            {
               // divide the number by the count
               result = numberKey / spacesCountKey;
            }
            else
            {
               // key is not an integral multiple of spaces count
            }
         }
         else
         {
            // the denominator is 0
         }
      }
      else
      {
         // couldn't convert
      }
      
      return result;
   }
   
} /* namespace NsMessageBroker */

