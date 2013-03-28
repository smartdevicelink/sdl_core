/**
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

/* C++ code produced by gperf version 3.0.3 */
/* Command-line: gperf --output-file=./src/components/JSONHandler/src/SDLRPCObjectsImpl/V1/InteractionModeMarshaller.inc ./src/components/JSONHandler/src/SDLRPCObjectsImpl/V1/InteractionModeMarshaller.gp  */
/* Computed positions: -k'' */


#define TOTAL_KEYWORDS 3
#define MIN_WORD_LENGTH 4
#define MAX_WORD_LENGTH 11
#define MIN_HASH_VALUE 4
#define MAX_HASH_VALUE 11
/* maximum key range = 8, duplicates = 0 */

class InteractionMode_intHash
{
private:
  static inline unsigned int hash (const char *str, unsigned int len);
public:
  static const struct NsSmartDeviceLinkRPC::PerfectHashTable *getPointer (const char *str, unsigned int len);
};

inline /*ARGSUSED*/
unsigned int
InteractionMode_intHash::hash (register const char *str, register unsigned int len)
{
  return len;
}

const struct NsSmartDeviceLinkRPC::PerfectHashTable *
InteractionMode_intHash::getPointer (register const char *str, register unsigned int len)
{
  static const unsigned char lengthtable[] =
    {
       0,  0,  0,  0,  4,  0,  0,  7,  0,  0,  0, 11
    };
  static const struct NsSmartDeviceLinkRPC::PerfectHashTable wordlist[] =
    {
      {""}, {""}, {""}, {""},
#line 19 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V1/InteractionModeMarshaller.gp"
      {"BOTH",2},
      {""}, {""},
#line 18 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V1/InteractionModeMarshaller.gp"
      {"VR_ONLY",1},
      {""}, {""}, {""},
#line 17 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V1/InteractionModeMarshaller.gp"
      {"MANUAL_ONLY",0}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register int key = hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= 0)
        if (len == lengthtable[key])
          {
            register const char *s = wordlist[key].name;

            if (*str == *s && !memcmp (str + 1, s + 1, len - 1))
              return &wordlist[key];
          }
    }
  return 0;
}
