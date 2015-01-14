/*
* Typesafe printf-like wrapper around std streams
* by Igor Kozyrenko, 2013
*/

#include <cstring>
#include <cwchar>
#include <ostream>
#include <sstream>

namespace typesafe_format {
namespace impl {

// Pointer avoidance magic
template<class T> struct no_pointers                { enum { allowed = 1 }; };
template<>        struct no_pointers<char*>         { enum { allowed = 1 }; };
template<>        struct no_pointers<const char*>   { enum { allowed = 1 }; };
template<>        struct no_pointers<wchar_t*>      { enum { allowed = 1 }; };
template<>        struct no_pointers<const wchar_t*>{ enum { allowed = 1 }; };
template<class T> struct no_pointers<T*>            { enum { allowed = 0 }; };

// Static assert helper
template<bool b> struct my_static_assert       {};
template<>       struct my_static_assert<true> { enum { here }; };

template <typename CT>
struct Streamable {
  virtual void OutputToStream(std::basic_ostream<CT>& stream) const = 0;
  virtual ~Streamable() {}
};

template <typename CT, typename T>
struct StreamableValue: public Streamable<CT> {
  StreamableValue(const T& val): val_(val) {
    // You can wrap pointer with typesafe_format::ptr to pass this check
    (void)my_static_assert<no_pointers<T>::allowed>::here;
  }
  void OutputToStream(std::basic_ostream<CT>& stream) const {
    stream<<val_;
  }
private:
  const T& val_;
};

inline size_t len(const char* str)    { return std::strlen(str); }
inline size_t len(const wchar_t* str) { return std::wcslen(str); }

void format_params(const char* fmt, size_t fmt_length, std::ostream& os,
                   const Streamable<char>* params[], size_t params_count);
void format_params(const wchar_t* fmt, size_t fmt_length, std::wostream& os,
                   const Streamable<wchar_t>* params[], size_t params_count);
} // namespace impl

// Wrapper class to pass anti-pointer protection
struct ptr {
  explicit ptr(void* ptr): ptr_(ptr) {}
private:
  void* ptr_;
  template <typename CT>
  friend std::basic_ostream<CT>& operator<<(std::basic_ostream<CT>&,
                                            const ptr&);
};
template <typename CT>
inline std::basic_ostream<CT>& operator<<(std::basic_ostream<CT>& os,
                                          const ptr& ptr) {
  return os<<ptr.ptr_;
}

// stream format overloads
template <typename CT>
std::basic_ostream<CT>& strmfmtl(
    std::basic_ostream<CT>& os, const CT* fmt, size_t fmt_length) {
  return os.write(fmt, fmt_length);
}

template <typename CT,
          typename T0>
std::basic_ostream<CT>& strmfmtl(
    std::basic_ostream<CT>& os, const CT* fmt, size_t fmt_length,
    const T0& p0) {
  impl::StreamableValue<CT, T0> s0(p0);
  const impl::Streamable<CT>* params[] =
      {&s0};
  impl::format_params(fmt, fmt_length, os,
      params, sizeof params/sizeof params[0]);
  return os;
}

template <typename CT,
          typename T0, typename T1>
std::basic_ostream<CT>& strmfmtl(
    std::basic_ostream<CT>& os, const CT* fmt, size_t fmt_length,
    const T0& p0, const T1& p1) {
  impl::StreamableValue<CT, T0> s0(p0);
  impl::StreamableValue<CT, T1> s1(p1);
  const impl::Streamable<CT>* params[] =
      {&s0, &s1};
  impl::format_params(fmt, fmt_length, os,
      params, sizeof params/sizeof params[0]);
  return os;
}

template <typename CT,
          typename T0, typename T1, typename T2>
std::basic_ostream<CT>& strmfmtl(
    std::basic_ostream<CT>& os, const CT* fmt, size_t fmt_length,
    const T0& p0, const T1& p1, const T2& p2) {
  impl::StreamableValue<CT, T0> s0(p0);
  impl::StreamableValue<CT, T1> s1(p1);
  impl::StreamableValue<CT, T2> s2(p2);
  const impl::Streamable<CT>* params[] =
      {&s0, &s1, &s2};
  impl::format_params(fmt, fmt_length, os,
      params, sizeof params/sizeof params[0]);
  return os;
}

template <typename CT,
          typename T0, typename T1, typename T2, typename T3>
std::basic_ostream<CT>& strmfmtl(
    std::basic_ostream<CT>& os, const CT* fmt, size_t fmt_length,
    const T0& p0, const T1& p1, const T2& p2, const T3& p3) {
  impl::StreamableValue<CT, T0> s0(p0);
  impl::StreamableValue<CT, T1> s1(p1);
  impl::StreamableValue<CT, T2> s2(p2);
  impl::StreamableValue<CT, T3> s3(p3);
  const impl::Streamable<CT>* params[] =
      {&s0, &s1, &s2, &s3};
  impl::format_params(fmt, fmt_length, os,
      params, sizeof params/sizeof params[0]);
  return os;
}

template <typename CT,
          typename T0, typename T1, typename T2, typename T3, typename T4>
std::basic_ostream<CT>& strmfmtl(
    std::basic_ostream<CT>& os, const CT* fmt, size_t fmt_length,
    const T0& p0, const T1& p1, const T2& p2, const T3& p3, const T4& p4) {
  impl::StreamableValue<CT, T0> s0(p0);
  impl::StreamableValue<CT, T1> s1(p1);
  impl::StreamableValue<CT, T2> s2(p2);
  impl::StreamableValue<CT, T3> s3(p3);
  impl::StreamableValue<CT, T4> s4(p4);
  const impl::Streamable<CT>* params[] =
      {&s0, &s1, &s2, &s3, &s4};
  impl::format_params(fmt, fmt_length, os,
      params, sizeof params/sizeof params[0]);
  return os;
}

template <typename CT,
          typename T0, typename T1, typename T2, typename T3, typename T4,
          typename T5>
std::basic_ostream<CT>& strmfmtl(
    std::basic_ostream<CT>& os, const CT* fmt, size_t fmt_length,
    const T0& p0, const T1& p1, const T2& p2, const T3& p3, const T4& p4,
    const T5& p5) {
  impl::StreamableValue<CT, T0> s0(p0);
  impl::StreamableValue<CT, T1> s1(p1);
  impl::StreamableValue<CT, T2> s2(p2);
  impl::StreamableValue<CT, T3> s3(p3);
  impl::StreamableValue<CT, T4> s4(p4);
  impl::StreamableValue<CT, T5> s5(p5);
  const impl::Streamable<CT>* params[] =
      {&s0, &s1, &s2, &s3, &s4, &s5};
  impl::format_params(fmt, fmt_length, os,
      params, sizeof params/sizeof params[0]);
  return os;
}

template <typename CT,
          typename T0, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6>
std::basic_ostream<CT>& strmfmtl(
    std::basic_ostream<CT>& os, const CT* fmt, size_t fmt_length,
    const T0& p0, const T1& p1, const T2& p2, const T3& p3, const T4& p4,
    const T5& p5, const T6& p6) {
  impl::StreamableValue<CT, T0> s0(p0);
  impl::StreamableValue<CT, T1> s1(p1);
  impl::StreamableValue<CT, T2> s2(p2);
  impl::StreamableValue<CT, T3> s3(p3);
  impl::StreamableValue<CT, T4> s4(p4);
  impl::StreamableValue<CT, T5> s5(p5);
  impl::StreamableValue<CT, T6> s6(p6);
  const impl::Streamable<CT>* params[] =
      {&s0, &s1, &s2, &s3, &s4, &s5, &s6};
  impl::format_params(fmt, fmt_length, os,
      params, sizeof params/sizeof params[0]);
  return os;
}

template <typename CT,
          typename T0, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6, typename T7>
std::basic_ostream<CT>& strmfmtl(
    std::basic_ostream<CT>& os, const CT* fmt, size_t fmt_length,
    const T0& p0, const T1& p1, const T2& p2, const T3& p3, const T4& p4,
    const T5& p5, const T6& p6, const T7& p7) {
  impl::StreamableValue<CT, T0> s0(p0);
  impl::StreamableValue<CT, T1> s1(p1);
  impl::StreamableValue<CT, T2> s2(p2);
  impl::StreamableValue<CT, T3> s3(p3);
  impl::StreamableValue<CT, T4> s4(p4);
  impl::StreamableValue<CT, T5> s5(p5);
  impl::StreamableValue<CT, T6> s6(p6);
  impl::StreamableValue<CT, T7> s7(p7);
  const impl::Streamable<CT>* params[] =
      {&s0, &s1, &s2, &s3, &s4, &s5, &s6, &s7};
  impl::format_params(fmt, fmt_length, os,
      params, sizeof params/sizeof params[0]);
  return os;
}

template <typename CT,
          typename T0, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6, typename T7, typename T8>
std::basic_ostream<CT>& strmfmtl(
    std::basic_ostream<CT>& os, const CT* fmt, size_t fmt_length,
    const T0& p0, const T1& p1, const T2& p2, const T3& p3, const T4& p4,
    const T5& p5, const T6& p6, const T7& p7, const T8& p8) {
  impl::StreamableValue<CT, T0> s0(p0);
  impl::StreamableValue<CT, T1> s1(p1);
  impl::StreamableValue<CT, T2> s2(p2);
  impl::StreamableValue<CT, T3> s3(p3);
  impl::StreamableValue<CT, T4> s4(p4);
  impl::StreamableValue<CT, T5> s5(p5);
  impl::StreamableValue<CT, T6> s6(p6);
  impl::StreamableValue<CT, T7> s7(p7);
  impl::StreamableValue<CT, T8> s8(p8);
  const impl::Streamable<CT>* params[] =
      {&s0, &s1, &s2, &s3, &s4, &s5, &s6, &s7, &s8};
  impl::format_params(fmt, fmt_length, os,
      params, sizeof params/sizeof params[0]);
  return os;
}

template <typename CT,
          typename T0, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6, typename T7, typename T8, typename T9>
std::basic_ostream<CT>& strmfmtl(
    std::basic_ostream<CT>& os, const CT* fmt, size_t fmt_length,
    const T0& p0, const T1& p1, const T2& p2, const T3& p3, const T4& p4,
    const T5& p5, const T6& p6, const T7& p7, const T8& p8, const T9& p9) {
  impl::StreamableValue<CT, T0> s0(p0);
  impl::StreamableValue<CT, T1> s1(p1);
  impl::StreamableValue<CT, T2> s2(p2);
  impl::StreamableValue<CT, T3> s3(p3);
  impl::StreamableValue<CT, T4> s4(p4);
  impl::StreamableValue<CT, T5> s5(p5);
  impl::StreamableValue<CT, T6> s6(p6);
  impl::StreamableValue<CT, T7> s7(p7);
  impl::StreamableValue<CT, T8> s8(p8);
  impl::StreamableValue<CT, T9> s9(p9);
  const impl::Streamable<CT>* params[] =
      {&s0, &s1, &s2, &s3, &s4, &s5, &s6, &s7, &s8, &s9};
  impl::format_params(fmt, fmt_length, os,
      params, sizeof params/sizeof params[0]);
  return os;
}

/*
 * Stream format overloads
 * */
template <typename CT>
std::basic_ostream<CT>& strmfmt(
    std::basic_ostream<CT>& os, const CT* fmt) {
  return strmfmtl(os, fmt, impl::len(fmt));
}

template <typename CT,
          typename T0>
std::basic_ostream<CT>& strmfmt(
    std::basic_ostream<CT>& os, const CT* fmt, const T0& p0) {
  return strmfmtl(os, fmt, impl::len(fmt), p0);
}

template <typename CT,
          typename T0, typename T1>
std::basic_ostream<CT>& strmfmt(
    std::basic_ostream<CT>& os, const CT* fmt,
    const T0& p0, const T1& p1) {
  return strmfmtl(os, fmt, impl::len(fmt), p0, p1);
}

template <typename CT,
          typename T0, typename T1, typename T2>
std::basic_ostream<CT>& strmfmt(
    std::basic_ostream<CT>& os, const CT* fmt,
    const T0& p0, const T1& p1, const T2& p2) {
  return strmfmtl(os, fmt, impl::len(fmt), p0, p1, p2);
}

template <typename CT,
          typename T0, typename T1, typename T2, typename T3>
std::basic_ostream<CT>& strmfmt(
    std::basic_ostream<CT>& os, const CT* fmt,
    const T0& p0, const T1& p1, const T2& p2, const T3& p3) {
  return strmfmtl(os, fmt, impl::len(fmt), p0, p1, p2, p3);
}

template <typename CT,
          typename T0, typename T1, typename T2, typename T3, typename T4>
std::basic_ostream<CT>& strmfmt(
    std::basic_ostream<CT>& os, const CT* fmt,
    const T0& p0, const T1& p1, const T2& p2, const T3& p3, const T4& p4) {
  return strmfmtl(os, fmt, impl::len(fmt), p0, p1, p2, p3, p4);
}

template <typename CT,
          typename T0, typename T1, typename T2, typename T3, typename T4,
          typename T5>
std::basic_ostream<CT>& strmfmt(
    std::basic_ostream<CT>& os, const CT* fmt,
    const T0& p0, const T1& p1, const T2& p2, const T3& p3, const T4& p4,
    const T5& p5) {
  return strmfmtl(os, fmt, impl::len(fmt), p0, p1, p2, p3, p4, p5);
}

template <typename CT,
          typename T0, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6>
std::basic_ostream<CT>& strmfmt(
    std::basic_ostream<CT>& os, const CT* fmt,
    const T0& p0, const T1& p1, const T2& p2, const T3& p3, const T4& p4,
    const T5& p5, const T6& p6) {
  return strmfmtl(os, fmt, impl::len(fmt), p0, p1, p2, p3, p4, p5, p6);
}

template <typename CT,
          typename T0, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6, typename T7>
std::basic_ostream<CT>& strmfmt(
    std::basic_ostream<CT>& os, const CT* fmt,
    const T0& p0, const T1& p1, const T2& p2, const T3& p3, const T4& p4,
    const T5& p5, const T6& p6, const T7& p7) {
  return strmfmtl(os, fmt, impl::len(fmt), p0, p1, p2, p3, p4, p5, p6, p7);
}

template <typename CT,
          typename T0, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6, typename T7, typename T8>
std::basic_ostream<CT>& strmfmt(
    std::basic_ostream<CT>& os, const CT* fmt,
    const T0& p0, const T1& p1, const T2& p2, const T3& p3, const T4& p4,
    const T5& p5, const T6& p6, const T7& p7, const T8& p8) {
  return strmfmtl(os, fmt, impl::len(fmt), p0, p1, p2, p3, p4, p5, p6, p7, p8);
}

template <typename CT,
          typename T0, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6, typename T7, typename T8, typename T9>
std::basic_ostream<CT>& strmfmt(
    std::basic_ostream<CT>& os, const CT* fmt,
    const T0& p0, const T1& p1, const T2& p2, const T3& p3, const T4& p4,
    const T5& p5, const T6& p6, const T7& p7, const T8& p8, const T9& p9) {
  return strmfmtl(os, fmt, impl::len(fmt), p0, p1, p2, p3, p4, p5, p6, p7, p8, p9);
}

/*
*
* Char array format overloads
*
*/
template <typename CT,
          typename T0>
std::basic_string<CT> format(const CT* fmt,
    const T0& p0) {
  std::basic_stringstream<CT> stream;
  strmfmtl(stream, fmt, impl::len(fmt), p0);
  return stream.str();
}

template <typename CT,
          typename T0, typename T1>
std::basic_string<CT> format(const CT* fmt,
    const T0& p0, const T1& p1) {
  std::basic_stringstream<CT> stream;
  strmfmtl(stream, fmt, impl::len(fmt), p0, p1);
  return stream.str();
}

template <typename CT,
          typename T0, typename T1, typename T2>
std::basic_string<CT> format(const CT* fmt,
    const T0& p0, const T1& p1, const T2& p2) {
  std::basic_stringstream<CT> stream;
  strmfmtl(stream, fmt, impl::len(fmt), p0, p1, p2);
  return stream.str();
}

template <typename CT,
          typename T0, typename T1, typename T2, typename T3>
std::basic_string<CT> format(const CT* fmt,
    const T0& p0, const T1& p1, const T2& p2, const T3& p3) {
  std::basic_stringstream<CT> stream;
  strmfmtl(stream, fmt, impl::len(fmt), p0, p1, p2, p3);
  return stream.str();
}

template <typename CT,
          typename T0, typename T1, typename T2, typename T3, typename T4>
std::basic_string<CT> format(const CT* fmt,
    const T0& p0, const T1& p1, const T2& p2, const T3& p3, const T4& p4) {
  std::basic_stringstream<CT> stream;
  strmfmtl(stream, fmt, impl::len(fmt), p0, p1, p2, p3, p4);
  return stream.str();
}

template <typename CT,
          typename T0, typename T1, typename T2, typename T3, typename T4,
          typename T5>
std::basic_string<CT> format(const CT* fmt,
    const T0& p0, const T1& p1, const T2& p2, const T3& p3, const T4& p4,
    const T5& p5) {
  std::basic_stringstream<CT> stream;
  strmfmtl(stream, fmt, impl::len(fmt), p0, p1, p2, p3, p4, p5);
  return stream.str();
}

template <typename CT,
          typename T0, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6>
std::basic_string<CT> format(const CT* fmt,
    const T0& p0, const T1& p1, const T2& p2, const T3& p3, const T4& p4,
    const T5& p5, const T6& p6) {
  std::basic_stringstream<CT> stream;
  strmfmtl(stream, fmt, impl::len(fmt), p0, p1, p2, p3, p4, p5, p6);
  return stream.str();
}

template <typename CT,
          typename T0, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6, typename T7>
std::basic_string<CT> format(const CT* fmt,
    const T0& p0, const T1& p1, const T2& p2, const T3& p3, const T4& p4,
    const T5& p5, const T6& p6, const T7& p7) {
  std::basic_stringstream<CT> stream;
  strmfmtl(stream, fmt, impl::len(fmt), p0, p1, p2, p3, p4, p5, p6, p7);
  return stream.str();
}

template <typename CT,
          typename T0, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6, typename T7, typename T8>
std::basic_string<CT> format(const CT* fmt,
    const T0& p0, const T1& p1, const T2& p2, const T3& p3, const T4& p4,
    const T5& p5, const T6& p6, const T7& p7, const T8& p8) {
  std::basic_stringstream<CT> stream;
  strmfmtl(stream, fmt, impl::len(fmt), p0, p1, p2, p3, p4, p5, p6, p7, p8);
  return stream.str();
}

template <typename CT,
          typename T0, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6, typename T7, typename T8, typename T9>
std::basic_string<CT> format(const CT* fmt,
    const T0& p0, const T1& p1, const T2& p2, const T3& p3, const T4& p4,
    const T5& p5, const T6& p6, const T7& p7, const T8& p8, const T9& p9) {
  std::basic_stringstream<CT> stream;
  strmfmtl(stream, fmt, impl::len(fmt), p0, p1, p2, p3, p4, p5, p6, p7, p8, p9);
  return stream.str();
}

/*
*
* STL string overloads
*
*/
template <typename CT,
          typename T0>
std::basic_string<CT> format(const std::basic_string<CT> fmt,
    const T0& p0) {
  std::basic_stringstream<CT> stream;
  strmfmtl(stream, fmt.c_str(), fmt.size(),
      p0);
  return stream.str();
}

template <typename CT,
          typename T0, typename T1>
std::basic_string<CT> format(const std::basic_string<CT> fmt,
    const T0& p0, const T1& p1) {
  std::basic_stringstream<CT> stream;
  strmfmtl(stream, fmt.c_str(), fmt.size(),
      p0, p1);
  return stream.str();
}

template <typename CT,
          typename T0, typename T1, typename T2>
std::basic_string<CT> format(const std::basic_string<CT> fmt,
    const T0& p0, const T1& p1, const T2& p2) {
  std::basic_stringstream<CT> stream;
  strmfmtl(stream, fmt.c_str(), fmt.size(),
      p0, p1, p2);
  return stream.str();
}

template <typename CT,
          typename T0, typename T1, typename T2, typename T3>
std::basic_string<CT> format(const std::basic_string<CT> fmt,
    const T0& p0, const T1& p1, const T2& p2, const T3& p3) {
  std::basic_stringstream<CT> stream;
  strmfmtl(stream, fmt.c_str(), fmt.size(),
      p0, p1, p2, p3);
  return stream.str();
}

template <typename CT,
          typename T0, typename T1, typename T2, typename T3, typename T4>
std::basic_string<CT> format(const std::basic_string<CT> fmt,
    const T0& p0, const T1& p1, const T2& p2, const T3& p3, const T4& p4) {
  std::basic_stringstream<CT> stream;
  strmfmtl(stream, fmt.c_str(), fmt.size(),
      p0, p1, p2, p3, p4);
  return stream.str();
}

template <typename CT,
          typename T0, typename T1, typename T2, typename T3, typename T4,
          typename T5>
std::basic_string<CT> format(const std::basic_string<CT> fmt,
    const T0& p0, const T1& p1, const T2& p2, const T3& p3, const T4& p4,
    const T5& p5) {
  std::basic_stringstream<CT> stream;
  strmfmtl(stream, fmt.c_str(), fmt.size(),
      p0, p1, p2, p3, p4, p5);
  return stream.str();
}

template <typename CT,
          typename T0, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6>
std::basic_string<CT> format(const std::basic_string<CT> fmt,
    const T0& p0, const T1& p1, const T2& p2, const T3& p3, const T4& p4,
    const T5& p5, const T6& p6) {
  std::basic_stringstream<CT> stream;
  strmfmtl(stream, fmt.c_str(), fmt.size(),
      p0, p1, p2, p3, p4, p5, p6);
  return stream.str();
}

template <typename CT,
          typename T0, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6, typename T7>
std::basic_string<CT> format(const std::basic_string<CT> fmt,
    const T0& p0, const T1& p1, const T2& p2, const T3& p3, const T4& p4,
    const T5& p5, const T6& p6, const T7& p7) {
  std::basic_stringstream<CT> stream;
  strmfmtl(stream, fmt.c_str(), fmt.size(),
      p0, p1, p2, p3, p4, p5, p6, p7);
  return stream.str();
}

template <typename CT,
          typename T0, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6, typename T7, typename T8>
std::basic_string<CT> format(const std::basic_string<CT> fmt,
    const T0& p0, const T1& p1, const T2& p2, const T3& p3, const T4& p4,
    const T5& p5, const T6& p6, const T7& p7, const T8& p8) {
  std::basic_stringstream<CT> stream;
  strmfmtl(stream, fmt.c_str(), fmt.size(),
      p0, p1, p2, p3, p4, p5, p6, p7, p8);
  return stream.str();
}

template <typename CT,
          typename T0, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6, typename T7, typename T8, typename T9>
std::basic_string<CT> format(const std::basic_string<CT> fmt,
    const T0& p0, const T1& p1, const T2& p2, const T3& p3, const T4& p4,
    const T5& p5, const T6& p6, const T7& p7, const T8& p8, const T9& p9) {
  std::basic_stringstream<CT> stream;
  strmfmtl(stream, fmt.c_str(), fmt.size(),
      p0, p1, p2, p3, p4, p5, p6, p7, p8, p9);
  return stream.str();
}

} // namespace typesafe_format
