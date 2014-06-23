/**
 * \file websocket_handler.hpp
 * \brief WebSocket Handler.
 * Supports RFC6455 standard of protocol
 * Protocol specification:
 * http://tools.ietf.org/html/rfc6455
 * \author AKara
 */

#ifndef WEBSOCKET_HANDLER_H
#define WEBSOCKET_HANDLER_H

#include <cstring>
#include <iostream>
#include <vector>

#define MAX_WEBSOCKET_04_KEY_LEN 128                              /**< Max WS key length */

#define   K(t)   _K[(t) / 20]                                     /**< K(t) */
#define   F0(b, c, d)   (((b) & (c)) | ((~(b)) & (d)))            /**< F0(b, c, d) */
#define   F1(b, c, d)   (((b) ^ (c)) ^ (d))                       /**< F1(b, c, d) */
#define   F2(b, c, d)   (((b) & (c)) | ((b) & (d)) | ((c) & (d))) /**< F2(b, c, d) */
#define   F3(b, c, d)   (((b) ^ (c)) ^ (d))                       /**< F3(b, c, d) */

#define   S(n, x)      (((x) << (n)) | ((x) >> (32 - n)))         /**< S(n, x) */
#define   H(n)   (ctxt->h.b32[(n)])                               /**< H(n) */
#define   COUNT   (ctxt->count)                                   /**< COUNT */
#define   W(n)   (ctxt->m.b32[(n)])                               /**< W(n) */

#define   PUTPAD(x)   { \
   ctxt->m.b8[(COUNT % 64)] = (x);      \
   COUNT++;            \
   COUNT %= 64;            \
   if (COUNT % 64 == 0)         \
      sha1_step(ctxt);      \
     }                                                            /**< PUTPAD(x) */

/**
 * \namespace NsMessageBroker
 * \brief MessageBroker related functions.
 */ 
namespace NsMessageBroker
{
   /**
    * \brief sha1_ctxt txt structure.
    */ 
   struct sha1_ctxt
   {
   union {
      unsigned char   b8[20];
      unsigned int   b32[5];
   } h;                       /**< h */
   union {
      unsigned char   b8[8];
      unsigned long long   b64[1];
   } c;                       /**< c */
   union {
      unsigned char   b8[64];
      unsigned int   b32[16];
   } m;                       /**< m */
   unsigned char   count;          /**< count */
   };

   
   /**
    * \brief Constant table.
    */ 
   const unsigned int _K[] = { 0x5a827999, 0x6ed9eba1, 0x8f1bbcdc, 0xca62c1d6 };

   /**
    * \brief Constant table.
    */ 
   const char encode[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
              "abcdefghijklmnopqrstuvwxyz0123456789+/";

   /**
    * \brief Constant table.
    */ 
   const char decode[] = "|$$$}rstuvwxyz{$$$$$$$>?@ABCDEFGHIJKLMNOPQRSTUVW"
              "$$$$$$XYZ[\\]^_`abcdefghijklmnopq";
   
   /**
    * \brief Type to present raw bytes.
    */
   typedef std::vector<char> rawBytes;
   

   /**
   * \class CWebSocketHandler
   * \brief WebSocket messages handler.
   */
   class CWebSocketHandler
   {
   public:
      /**
      * \brief Constructor.
      */
      CWebSocketHandler()
      {
      }

      /**
      * \brief Destructor.
      */
      ~CWebSocketHandler()
      {
      }

      /**
      * \brief Generates Sec-WebSocket-Accept key
      * \param key Sec-WebSocket-Key from client
      */
      void handshake_0405(std::string &key);
      
      /**
      * \brief Generates accept key for older (hybi-00) WebSocket protocol version.
      * \param key1 The first key of the client's handshake (a string)
      * \param key2 The second key of the client's handshake (a string)
      * \param key3 The third key of the client's handshake (raw bytes), must be 8 bytes long
      * \return Raw bytes of the generated accept key, or no bytes if error
      */
      rawBytes handshake_hybi00(const std::string &key1, const std::string &key2, const rawBytes &key3);

      /**
      * \brief Parses WebSocket data header to retrieve packet size
      * \param Buffer input buffer
      * \param b_size buffer size
      * \return Packet size
      */
      unsigned int parseWebSocketDataLength(const char* Buffer, unsigned int& b_size);

      /**
      * \brief Parses WebSocket data
      * \param Buffer input buffer
      * \param b_size buffer size
      * \return -1 in case of issues, data length in case of success
      */
      int parseWebSocketData(char* Buffer, unsigned int& b_size);

      /**
      * \brief Prepares WebSocket data
      * \param Buffer input buffer
      * \param b_size buffer size
      * \return -1 in case of issues, data length in case of success
      */
      int prepareWebSocketDataHeader(unsigned char* Buffer, unsigned long long b_size);
   private:
      /**
      * \brief SHA1 hash calculator.
      * \param d input buffer
      * \param n size of buffer
      * \param md mask buffer
      * \return pointer to result
      */
      unsigned char * SHA1(const unsigned char *d, size_t n, unsigned char *md);

      /**
      * \brief base 64 encode string.
      * \param in input buffer
      * \param in_len size of buffer
      * \param out output buffer
      * \param out_size size of buffer
      * \return -1 in case of errors
      */
      int lws_b64_encode_string(const char *in, int in_len, char *out, int out_size);

      /**
      * \brief base 64 decode string.
      * \param in input buffer
      * \param out output buffer
      * \param out_size size of buffer
      * \return -1 in case of errors
      */
      int lws_b64_decode_string(const char *in, char *out, int out_size);

      /**
      * \brief one step of SHA1.
      * \param ctxt input txt structure
      */
      void sha1_step(struct sha1_ctxt *ctxt);

      /**
      * \brief init step of SHA1.
      * \param ctxt input txt structure
      */
      void sha1_init(struct sha1_ctxt *ctxt);

      /**
      * \brief pad step of SHA1.
      * \param ctxt input txt structure
      */
      void sha1_pad(struct sha1_ctxt *ctxt);

      /**
      * \brief loop step of SHA1.
      * \param ctxt input txt structure
      * \param input input buffer buffer
      * \param len length of input buffer
      */
      void sha1_loop(struct sha1_ctxt *ctxt, const unsigned char *input, size_t len);

      /**
      * \brief result step of SHA1.
      * \param ctxt input txt structure
      * \param digest0 buffer
      */
      void sha1_result(struct sha1_ctxt *ctxt, unsigned char* digest0);

      /**
      * \brief base 64 test.
      * \return -1 in case of errors
      */
      int lws_b64_selftest(void);

      /**
      * \brief Extracts and processes the number from key,
      * according to the WebSocket hybi-00 specification.
      * \param key Key to extract the number from
      * \return The number, or 0 if error
      */
      unsigned long extractNumber(const std::string &key) const;
   };

} /* namespace NsMessageBroker */

#endif /* WEBSOCKET_HANDLER_H */
