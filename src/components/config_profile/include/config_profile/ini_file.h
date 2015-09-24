/*
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

#ifndef SRC_COMPONENTS_CONFIG_PROFILE_INCLUDE_CONFIG_PROFILE_INI_FILE_H_
#define SRC_COMPONENTS_CONFIG_PROFILE_INCLUDE_CONFIG_PROFILE_INI_FILE_H_

#include <stdint.h>

namespace profile {

#define INI_FILE_VER  1000

#if !defined TRUE
#define TRUE 1
#endif
#if !defined True
#define True 1
#endif

#if !defined FALSE
#define FALSE 0
#endif
#if !defined False
#define False 0
#endif

/*
 * @brief Global defines
 */
#define INI_LINE_LEN                512
#define INI_FILE_TEMP_NAME          "ini.tmp"

#define INI_FLAG_UPDATE             0x00
#define INI_FLAG_ITEM_UP_CREA       0x01
#define INI_FLAG_FILE_UP_CREA       0x10

/*
 * @brief Global typedefs
 */
typedef enum Ini_search_id_e {
  INI_NOTHING,
  INI_RIGHT_CHAPTER,
  INI_WRONG_CHAPTER,
  INI_RIGHT_ITEM,
  INI_WRONG_ITEM,
  INI_REMARK,

  INI_SEARCH_MAX
} Ini_search_id;

/*
 * @brief Prototypes of functions
 */
#ifdef __cplusplus
extern "C" {
#endif

/*
 * @brief Write usage instructions to the end of the file
 * @param
 *
 * @return NULL if file or desired entry not found, otherwise pointer to fname
 */
extern char* ini_write_inst(const char *fname,
                            uint8_t flag);

/*
 * @brief Read a certain item of the specified chapter of a ini-file
 *
 * @return NULL if file or desired entry not found, otherwise pointer to value
 */
extern char* ini_read_value(const char *fname,
                            const char *chapter,
                            const char *item,
                            char *value);

/*
 * @brief Write a certain item of the specified chapter of a ini-file
 *
 * @return NULL if file not found, otherwise pointer to value
 */
extern char ini_write_value(const char *fname,
                            const char *chapter,
                            const char *item,
                            const char *value,
                            uint8_t flag);

/*
 * @brief Parse the given line for the item and returns the value if
 *        there is one otherwise NULL
 *
 * @return NULL if desired entry not found, otherwise pointer to value
 */
extern Ini_search_id ini_parse_line(const char *line,
                                    const char *tag,
                                    char *value);

#ifdef __cplusplus
}
#endif

}  // namespace profile

#endif  // SRC_COMPONENTS_CONFIG_PROFILE_INCLUDE_CONFIG_PROFILE_INI_FILE_H_
