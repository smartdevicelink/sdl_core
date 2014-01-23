/*
* Typesafe printf-like wrapper around std streams
* by Igor Kozyrenko, 2013
*/

#include "utils/safeformat.h"

#include <sstream>
#include <algorithm>
#include <iterator>

namespace typesafe_format {
namespace impl {

const char kPlaceholderPrefix = '{';
const char kPlaceholderSuffix = '}';

struct Placeholder {
  bool   escaped;
  size_t escaped_prefixes;
  size_t number;
  size_t length;
};

void missing_parameter(std::ostream& os, size_t param_number) {
  os<<"{Parameter "<<param_number<<" is missing}";
}

void missing_parameter(std::wostream& os, size_t param_number) {
  os<<L"{Parameter "<<param_number<<L" is missing}";
}

template <typename CT>
const CT* find_placeholder_tmpl(const CT* text, const CT* end,
                                Placeholder& ph) {
  const CT* prefix_begin = text;
  bool done = false;
  do {
    prefix_begin = std::find(prefix_begin, end, kPlaceholderPrefix);
    // there must be at least two symbols after prefix
    if (std::distance(prefix_begin, end) > 2) {
      const CT* prefix_end = prefix_begin + 1;
      // count prefix escapes if any
      while(*prefix_end == kPlaceholderPrefix &&
            std::distance(prefix_end, end) > 2)
        ++prefix_end;
      size_t prefixes = std::distance(prefix_begin, prefix_end);
      size_t prefix_number = *prefix_end - '0';
      const CT* suffix = prefix_end + 1;
      if (prefix_number < 10 &&  // it is in range of valid placeholders
          *suffix == kPlaceholderSuffix) {
        ph.escaped =          prefixes % 2 == 0;
        ph.escaped_prefixes = prefixes / 2;
        ph.number = prefix_number;
        ph.length = (suffix - prefix_begin) + 1;
        done = true;
      } else {
        prefix_begin = suffix; // continue from the symbol after number
      }
    } else { // prefix_begin is not at least last but two
      prefix_begin = end;
      done = true;
    }
  } while (!done);
  return prefix_begin;
}

template <typename CT>
void format_params_tmpl(const CT* fmt, size_t fmt_length,
                        std::basic_ostream<CT>& os,
                        const Streamable<CT>* params[], size_t params_count) {
  const CT* text = fmt;
  const CT* end = fmt + fmt_length;
  Placeholder placeholder;
  for (const CT* ph_pos = find_placeholder_tmpl(text, end, placeholder);
       ph_pos != end;
       ph_pos = find_placeholder_tmpl(text, end, placeholder)) {
    os.write(text, ph_pos - text);
    for (size_t i = 0; i != placeholder.escaped_prefixes; ++i)
      os.put(kPlaceholderPrefix);
    if (placeholder.escaped) {
      os<<placeholder.number<<kPlaceholderSuffix;
    } else {
      if (placeholder.number < params_count)
        params[placeholder.number]->OutputToStream(os);
      else {
        missing_parameter(os, placeholder.number);
      }
    }
    text = ph_pos + placeholder.length;
  }
  os.write(text, end - text);
}

void format_params(const char* fmt, size_t fmt_length, std::ostream& os,
                   const Streamable<char>* params[], size_t params_count) {
  format_params_tmpl(fmt, fmt_length, os, params, params_count);
}
void format_params(const wchar_t* fmt, size_t fmt_length, std::wostream& os,
                  const Streamable<wchar_t>* params[], size_t params_count) {
  format_params_tmpl(fmt, fmt_length, os, params, params_count);
}



} // namespace impl
} // namespace typesafe_format
