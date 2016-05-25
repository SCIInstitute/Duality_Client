#ifndef IVDA_MACROS
#define IVDA_MACROS

#include <iostream>
#include <type_traits>
#include "DebugOutHandler.h"

namespace IVDA {
  
// print vectors
template <class T> std::ostream& operator<<(std::ostream& os, std::vector<T> const& v)
{
  bool bFirst = true;
  for (auto i : v) {
    if (bFirst)
      bFirst = false;
    else
      os << ", ";
    os << i;
  }
  return os;
}

// print pairs
template <class T1, class T2> std::ostream& operator<<(std::ostream& os, std::pair<T1, T2> const& p)
{
  os << "(" << p.first << "; " << p.second << ")";
  return os;
}

// print vector of pairs
template <class T1, class T2> std::ostream& operator<<(std::ostream& os, std::vector<std::pair<T1, T2>> const& vp)
{
  bool bFirst = true;
  for (auto const& i : vp) {
    if (bFirst)
      bFirst = false;
    else
      os << ", ";
    os << "(" << i.first << "; " << i.second << ")";
  }
  return os;
}

#if 1
// helper to determine whether there's a const_iterator for T.
template<typename T>
struct has_const_iterator
{
private:
  template<typename C> static char test(typename C::const_iterator*);
  template<typename C> static int  test(...);
public:
  enum { value = sizeof(test<T>(0)) == sizeof(char) };
};

// try to print all containers with const_interator defined except std::string
template <typename Container>
typename std::enable_if<has_const_iterator<Container>::value &&
                        !std::is_same<Container, std::string>::value
                        , std::ostream&>::type
operator<<(std::ostream& os, Container const& c)
{
  bool bFirst = true;
  for (auto const& i : c) {
    if (bFirst)
      bFirst = false;
    else
      os << ", ";
    os << i;
  }
  return os;
}
#else
// print vectors
template <class T> std::ostream& operator<<(std::ostream& os, std::vector<T> const& v)
{
  bool bFirst = true;
  for (auto i : v) {
    if (bFirst)
      bFirst = false;
    else
      os << ", ";
    os << i;
  }
  return os;
}

// print arrays
template <class T, size_t N> std::ostream& operator<<(std::ostream& os, std::array<T, N> const& v)
{
  bool bFirst = true;
  for (auto i : v) {
    if (bFirst)
      bFirst = false;
    else
      os << ", ";
    os << i;
  }
  return os;
}
#endif

#ifdef __OBJC__
#define OBJC_CLASS(name) @class name
#else
#define OBJC_CLASS(name) typedef struct objc_object name
#endif

#ifndef _DEBUG
#define COUT(stream) do {} while(0)
#else
#define COUT(stream) IVDA_OTHER(stream)
#endif

// output exception of type SocketException
#define COUT_EXCEPTION(e) COUT("Exception: " << e.what() << " (" << e.where() << " returning with error " << e.withErrorCode() << ")")

#define ERASE(p)	\
delete p;			\
p = NULL;

#define ERASE_ARRAY(p)	\
delete [] p;			\
p = NULL;

#define FREE(p)	\
free(p);			\
p = NULL;

bool isBigEndian();

#define IS_BIG_ENDIAN isBigEndian()
#define IS_LITTLE_ENDIAN !BIG_ENDIAN

#define REVERSE_INT16(i) ((i >> 8) | (i << 8))
#define REVERSE_INT32(i) ((i >> 24) | ((i << 8) & 0x00FF0000) | ((i >> 8) & 0x0000FF00) | (i << 24));
#define REVERSE_INT64(i) ((i >> 56) | ((i << 40) & 0x00FF000000000000) | ((i << i) & 0x0000FF0000000000) | ((i << 8) & 0x000000FF00000000) | ((i >> 8) & 0x00000000FF000000) | ((i >> i) & 0x0000000000FF0000) | ((i >> 40) & 0x000000000000FF00) | (i << 56))

  
} // namespace IVDA

#endif
