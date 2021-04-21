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

#include "config_profile/ini_file.h"
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/logger.h"

#ifndef _WIN32
#include <unistd.h>
#else
#define PATH_MAX _MAX_PATH
#endif

#ifdef __linux__
#define USE_MKSTEMP 1
#endif

#include <string>

namespace profile {

SDL_CREATE_LOG_VARIABLE("Profile")

char* ini_write_inst(const char* fname, uint8_t flag) {
  FILE* fp = 0;

  if (NULL == fname)
    return NULL;
  if ('\0' == *fname)
    return NULL;

  if ((fp = fopen(fname, "a")) == 0)
    if (flag & INI_FLAG_FILE_UP_CREA)
      if ((fp = fopen(fname, "w")) == 0)
        return NULL;
  if (0 == fp) {
    return NULL;
  }
  fprintf(fp, "\n");
  fprintf(fp, "; The INI-file consists of different chapters.\n");
  fprintf(fp, "; Each chapter begins with the line containing\n");
  fprintf(fp, "; the name in square brackets. Syntax:\n");
  fprintf(fp, "; [chapter]\n");
  fprintf(fp, "; The chapters consists of a set of items with a\n");
  fprintf(fp, "; assinged value. The syntax is:\n");
  fprintf(fp, "; item=value\n");
  fprintf(fp, "; All white spaces an second encounters of chapters\n");
  fprintf(fp, "; or items will be ignored.\n");
  fprintf(fp, "; Remarks start with semicolon or star as first character.\n");
  fprintf(fp, "; It is alowed for names of chapters and items to\n");
  fprintf(fp, "; contain semicolon and star. Possible syntax is:\n");
  fprintf(fp, "; [ chapter ]       ;Remark\n");
  fprintf(fp, ";    item = value\n");
  fprintf(fp, "\n");

  fclose(fp);
  return const_cast<char*>(fname);
}

char* ini_read_value(FILE* fp,
                     const char* chapter,
                     const char* item,
                     char* value) {
  bool chapter_found = false;
  char line[INI_LINE_LEN] = "";
  char val[INI_LINE_LEN] = "";
  char tag[INI_LINE_LEN] = "";

  Ini_search_id result;
  *line = '\0';
  *val = '\0';
  *tag = '\0';
  if ((NULL == fp) || (NULL == chapter) || (NULL == item) || (NULL == value)) {
    return NULL;
  }

  *value = '\0';
  if (('\0' == *chapter) || ('\0' == *item)) {
    return NULL;
  }

  snprintf(tag, INI_LINE_LEN, "%s", chapter);
  for (uint32_t i = 0; i < strlen(tag); ++i) {
    tag[i] = toupper(tag[i]);
  }

  fseek(fp, 0, SEEK_SET);
  while (NULL != fgets(line, INI_LINE_LEN, fp)) {
    // Now start the line parsing
    result = ini_parse_line(line, tag, val);
    if (!chapter_found) {
      if (INI_RIGHT_CHAPTER == result) {
        chapter_found = true;

        snprintf(tag, INI_LINE_LEN, "%s", item);
        for (uint32_t i = 0; i < strlen(tag); ++i)
          tag[i] = toupper(tag[i]);
      }
    } else {
      if (INI_WRONG_CHAPTER == result) {
        return NULL;
      }
      if (INI_RIGHT_ITEM == result) {
        snprintf(value, INI_LINE_LEN, "%s", val);
        return value;
      }
    }
  }

  return NULL;
}

#ifdef BUILD_TESTS
// cppcheck-suppress unusedFunction //Used in unit tests
char ini_write_value(const char* fname,
                     const char* chapter,
                     const char* item,
                     const char* value,
                     uint8_t flag) {
  FILE *rd_fp, *wr_fp = 0;
  uint16_t cr_count;
  int32_t wr_result;
  bool chapter_found = false;
  bool value_written = false;
  char line[INI_LINE_LEN] = "";
  char val[INI_LINE_LEN] = "";
  char tag[INI_LINE_LEN] = "";
  char temp_fname[PATH_MAX] = "";
  Ini_search_id result;
  *line = '\0';
  *val = '\0';
  *tag = '\0';
  *temp_fname = '\0';

  if ((NULL == fname) || (NULL == chapter) || (NULL == item) || (NULL == value))
    return FALSE;
  if (('\0' == *fname) || ('\0' == *chapter) || ('\0' == *item))
    return FALSE;

  if (0 == (rd_fp = fopen(fname, "r"))) {
    ini_write_inst(fname, flag);
    if (0 == (rd_fp = fopen(fname, "r")))
      return FALSE;
  }

#if USE_MKSTEMP
  {
    const char* temp_str = "./";
    if (temp_str) {
      snprintf(temp_fname, PATH_MAX, "%s/ini.XXXXXX", temp_str);
      int32_t fd = mkstemp(temp_fname);
      if (-1 == fd) {
        fclose(rd_fp);
        return FALSE;
      }
      wr_fp = fdopen(fd, "w");
      if (NULL == wr_fp) {
        unlink(temp_fname);
        close(fd);
        fclose(rd_fp);
        return FALSE;
      }
    } else {
      fclose(rd_fp);
      return FALSE;
    }
  }
#else   // #if USE_MKSTEMP
  tmpnam(temp_fname);
  if (0 == (wr_fp = fopen(temp_fname, "w"))) {
    fclose(rd_fp);
    return FALSE;
  }
#endif  // #else #if USE_MKSTEMP

  snprintf(tag, INI_LINE_LEN, "%s", chapter);
  for (uint32_t i = 0; i < strlen(tag); ++i)
    tag[i] = toupper(tag[i]);

  wr_result = 1;
  cr_count = 0;
  while ((NULL != fgets(line, INI_LINE_LEN, rd_fp)) && (0 < wr_result)) {
    // Now start the line parsing
    result = ini_parse_line(line, tag, val);

    if (!value_written) {
      if (!chapter_found) {
        if (INI_RIGHT_CHAPTER == result) {
          chapter_found = true;
          // coding style
          snprintf(tag, INI_LINE_LEN, "%s", item);
          for (uint32_t i = 0; i < strlen(tag); ++i)
            tag[i] = toupper(tag[i]);
        }
      } else {
        if ((INI_RIGHT_CHAPTER == result) || (INI_WRONG_CHAPTER == result)) {
          /* item not found but new capture */
          if (flag & INI_FLAG_ITEM_UP_CREA)
            fprintf(wr_fp, "%s=%s\n", item, value);
          /* The item must not update in an other chapter
             even it has the right name because only the
             first chapter is significant */
          value_written = true;
        } else if (result == INI_RIGHT_ITEM) {
          for (uint16_t i = 0; i < cr_count; ++i)
            fprintf(wr_fp, "\n");
          cr_count = 0;
          wr_result = fprintf(wr_fp, "%s=%s\n", item, value);
          value_written = true;
          continue;
        }
      }
    } /* if (!value_written) */

    if (0 == strcmp(val, "\n")) {
      ++cr_count;
    } else {
      for (uint32_t i = 0; i < cr_count; ++i)
        fprintf(wr_fp, "\n");
      cr_count = 0;
      wr_result = fprintf(wr_fp, "%s", line);
    }
  }
  if (0 < (wr_result) && (!value_written)) {
    if (flag & INI_FLAG_ITEM_UP_CREA) {
      if (!chapter_found)
        fprintf(wr_fp, "\n[%s]\n", chapter);
      fprintf(wr_fp, "%s=%s\n", item, value);
      value_written = true;
    }
  }
  fprintf(wr_fp, "\n");

  fclose(wr_fp);
  fclose(rd_fp);

  if (0 != rename(temp_fname, fname)) {
    if (0 != remove(temp_fname)) {
      SDL_LOG_WARN_WITH_ERRNO(
          "Unable to remove temp file: " << std::string(temp_fname));
    }
    return FALSE;
  }

  return (value_written);
}
#endif  // BUILD_TESTS

inline bool is_whitespace(char ch) {
  return ' ' == ch || '\t' == ch || '\n' == ch || '\r' == ch;
}

Ini_search_id ini_parse_line(const char* line, const char* tag, char* value) {
  const char* line_ptr;
  char temp_str[INI_LINE_LEN] = "\0";
  snprintf(value, INI_LINE_LEN, "%s", line);

  /* cut leading whitespace */
  line_ptr = line;
  while (is_whitespace(*line_ptr)) {
    ++line_ptr;
  }

  if ('\0' == *line_ptr) {
    snprintf(value, INI_LINE_LEN, "\n");
    return INI_NOTHING;
  }

  if (';' == *line_ptr || '*' == *line_ptr) {
    return INI_REMARK;
  }

  if ('[' == *line_ptr && strrchr(line_ptr, ']') != NULL) {
    /* cut leading [ and whitespace */
    do {
      ++line_ptr;
    } while (is_whitespace(*line_ptr));

    if (']' == *line_ptr) {
      return INI_NOTHING;
    }

    snprintf(temp_str, INI_LINE_LEN, "%s", line_ptr);
    char* temp_ptr = strrchr(temp_str, ']');

    /* cut trailing ] and whitespace */
    do {
      *temp_ptr = '\0';
      --temp_ptr;
    } while (is_whitespace(*temp_ptr));

    snprintf(value, INI_LINE_LEN, "%s", temp_str);
    size_t str_len = temp_ptr - temp_str + 1;
    for (size_t i = 0; i < str_len; i++) {
      temp_str[i] = toupper(temp_str[i]);
    }

    return strcmp(temp_str, tag) == 0 ? INI_RIGHT_CHAPTER : INI_WRONG_CHAPTER;
  }

  const char* equals_ptr = strchr(line_ptr, '=');
  if (NULL == equals_ptr) {
    return INI_NOTHING;
  }

  strncpy(temp_str, line_ptr, equals_ptr - line_ptr);

  /* ensure temp_str ends in \0 and remove whitespace */
  char* temp_back = temp_str + (equals_ptr - line_ptr);
  do {
    *temp_back = '\0';
    --temp_back;
  } while (is_whitespace(*temp_back));

  snprintf(value, INI_LINE_LEN, "%s", temp_str);

  size_t str_len = temp_back - temp_str + 1;
  for (size_t i = 0; i < str_len; ++i) {
    temp_str[i] = toupper(temp_str[i]);
  }

  if (strcmp(temp_str, tag) != 0) {
    return INI_WRONG_ITEM;
  }

  /* skip leading whitespace on value */
  line_ptr = equals_ptr + 1;
  while (is_whitespace(*line_ptr)) {
    ++line_ptr;
  }

  snprintf(value, INI_LINE_LEN, "%s", line_ptr);

  if (value[0] != '\0') {
    /* trim trailing whitespace on value */
    char* value_end = value + strlen(value) - 1;
    while (is_whitespace(*value_end)) {
      *value_end = '\0';
      --value_end;
    }
  }

  return INI_RIGHT_ITEM;
}

}  // namespace profile
