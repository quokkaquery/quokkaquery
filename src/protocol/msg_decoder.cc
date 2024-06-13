#include "msg_decoder.h"
#include <cassert>
#include <cstdint>
#include <utility>
#include <algorithm>
#include <type_traits>
#include <bit>

namespace quokkaquery {
namespace protocol {
namespace femsg {
static constexpr int32_t CANCEL_REQUEST_CODE = 80877102;
static constexpr int32_t GSSENC_REQUEST_CODE = 80877104;
static constexpr int32_t SSL_REQUEST_CODE = 80877103;
static const Summary INVALID_SUMMARY{Type::NONE, 0, ContentBase<Type::NONE>()};

using LengthType = const int32_t;
using StartupReqCode = const int32_t;
using VersionType = const int16_t;
using ProtocolVersion = const std::pair<VersionType, VersionType>;
using BufferType = Decoder::BufferType;
using BufferIter = BufferType::const_iterator;
using ParameterPair = const std::pair<const std::string, const std::string>;

template <typename T>
static inline T CastBufferTo(BufferIter begin, BufferIter end) {
  static_assert(std::is_integral_v<T>);

  BufferType buf(begin, end);
  if constexpr (std::endian::native == std::endian::little) {
    std::reverse(buf.begin(), buf.end());
  }

  return *reinterpret_cast<T*>(buf.data());
}

/* input is buffer's begin iterator */
static inline LengthType GetMessageLength(BufferIter begin) {
  return CastBufferTo<LengthType>(begin, begin + sizeof(LengthType));
}

/* input is buffer's begin iterator */
static inline StartupReqCode GetStartupRequestCode(BufferIter begin) {
  auto iter = begin + sizeof(LengthType);
  return CastBufferTo<StartupReqCode>(iter, iter + sizeof(StartupReqCode));
}

static inline ProtocolVersion GetProtocolVersion(BufferIter begin) {
  auto iter = begin + sizeof(LengthType);
  auto major_version = CastBufferTo<VersionType>(iter, iter + sizeof(VersionType));

  iter += sizeof(VersionType);
  auto minor_version = CastBufferTo<VersionType>(iter, iter + sizeof(VersionType));

  return std::make_pair(major_version, minor_version);
}

static inline const std::string Split(BufferIter& iter, const char delimeter) {
  std::string str;
  char byte = std::tolower(*iter);

  while (byte != delimeter) {
    if (!isspace(byte)) {
      str.push_back(byte);
    }
    byte = std::tolower(*(++iter));
  }

  return str;
}

static inline ParameterPair GetNextParameterPair(BufferIter& iter) {
  std::string key = Split(iter, '=');
  std::string value = Split(++iter, '\0');
  return std::make_pair(key, value);
}

static inline const ConnectionDesc GetConnectionDesc(BufferIter begin, BufferIter end) {
  std::string dbname;
  std::string username;
  std::string parameter;

  auto iter = begin + sizeof(LengthType) + sizeof(StartupReqCode);
  for (; iter != end; ++iter) {
    auto [key, value] = GetNextParameterPair(iter);

    if (key.compare("user") == 0) {
      username = value;
    } else if (key.compare("database") == 0) {
      dbname = value;
    } else {
      parameter.append(key + "=" + value + "&");
    }
  }

  return ConnectionDesc{dbname, username, parameter};
}

template <Type>
const Summary CreateSummary(const BufferType& msg) {
  assert("Cannot reach this point");
  return INVALID_SUMMARY;
}

template <>
const Summary CreateSummary<Type::STARTUP_MESSAGE>(const BufferType& msg) {
  auto protocol_version = GetProtocolVersion(msg.begin());
  ContentBase<Type::STARTUP_MESSAGE> content{
    protocol_version.first,
    protocol_version.second,
    GetConnectionDesc(msg.begin(), msg.end())
  };

  return Summary{Type::STARTUP_MESSAGE, GetMessageLength(msg.begin()), content};
}

Decoder::Decoder() : progress_(Progress::STARTUP_REQUIRED) {}

const Summary Decoder::operator()(const BufferType& msg) {
  switch (progress_) {
    case Progress::STARTUP_REQUIRED:
      return HandleStartupMessage(msg);
    case Progress::STARTUP_COMMENCING:
      return HandleAuthMessage(msg);
    case Progress::STARTUP_COMPLETE:
      return HandleOperationMessage(msg);
  }

  assert("Cannot reach this point");
  return INVALID_SUMMARY;
}

const Summary Decoder::HandleStartupMessage(const BufferType& msg) {
  assert(progress_ == Progress::STARTUP_REQUIRED);
  auto req_code = GetStartupRequestCode(msg.begin());

  if (req_code == CANCEL_REQUEST_CODE) {
    return CreateSummary<Type::CANCEL_REQUEST>(msg);
  }
  
  progress_ = Progress::STARTUP_COMMENCING;
  if (req_code == GSSENC_REQUEST_CODE) {
    return CreateSummary<Type::GSSENC_REQUEST>(msg);
  }
  
  if (req_code == SSL_REQUEST_CODE) {
    return CreateSummary<Type::SSL_REQUEST>(msg);
  }

  return CreateSummary<Type::STARTUP_MESSAGE>(msg);
}

const Summary Decoder::HandleAuthMessage(const BufferType& msg) {
  assert(progress_ == Progress::STARTUP_COMMENCING);
  return CreateSummary<Type::PASSWORD_MESSAGE>(msg);
}

const Summary Decoder::HandleOperationMessage(const BufferType& msg) {
  assert(progress_ == Progress::STARTUP_COMPLETE);

  auto first_byte = msg.front();

  switch (first_byte) {
    case 'c':
    case 'd':
    case 'f':
    case 'p':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    case 'H':
    case 'P':
    case 'Q':
    case 'S':
    case 'X':
      assert("Not implemented");
  }

  assert("Cannot reach this point");
  return INVALID_SUMMARY;
}
}  // namespace femsg

namespace bemsg {
static const Summary INVALID_SUMMARY{Type::NONE, 0, ContentBase<Type::NONE>()};

const Summary Decoder::operator()(const BufferType& msg) const {
  return INVALID_SUMMARY;
}
}  // namespace bemsg
}  // namespace protocol
}  // namespace quokkaquery