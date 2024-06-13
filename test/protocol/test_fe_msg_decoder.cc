#include <gtest/gtest.h>
#include "msg_decoder.h"
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <type_traits>
#include <bit>

using namespace quokkaquery::protocol::femsg;
using BufferType = std::vector<char>;

template <typename T>
static inline void AppendIntegralValue(BufferType& buffer, T arg) {
  static_assert(std::is_integral_v<T>);
  auto ptr = reinterpret_cast<char*>(&arg);
  if constexpr (std::endian::native == std::endian::little) {
    for (std::size_t i = 1; i <= sizeof(T); ++i) {
      /* change to network byte order */
      buffer.push_back(ptr[sizeof(T) - i]);
    }
  } else if constexpr (std::endian::native == std::endian::big) {
    for (std::size_t i = 0; i < sizeof(T); ++i) {
      buffer.push_back(ptr[i]);
    }
  }
}

static inline void AppendStringValue(BufferType& buffer, std::string arg) {
  for (const auto& c : arg) {
    buffer.push_back(c);
  }
  buffer.push_back('\0');
}

template <typename T>
static inline void CreateMessage(BufferType& buffer, T arg) {
  if constexpr (std::is_integral_v<T>) {
    AppendIntegralValue(buffer, arg);
  } else if constexpr (std::is_same_v<std::string, std::decay_t<T>>) {
    AppendStringValue(buffer, arg);
  }
  
  /* if the first byte is '0', then the message doesn't start with type byte */
  int32_t size = buffer.front() == '0' ? buffer.size() : buffer.size() - 1;
  auto ptr = reinterpret_cast<char*>(&size);
  if constexpr (std::endian::native == std::endian::little) {
    for (std::size_t i = 1; i <= sizeof(int32_t); ++i) {
      /* change to network byte order */
      buffer[i - 1] = ptr[sizeof(T) - i];
    }
  } else if constexpr (std::endian::native == std::endian::big) {
    for (std::size_t i = 0; i < sizeof(int32_t); ++i) {
      buffer[i] = ptr[i];
    }
  }
}

template <typename T, typename... Types>
static inline void CreateMessage(BufferType& buffer, T arg, Types... args) {
  if constexpr (std::is_integral_v<T>) {
    AppendIntegralValue(buffer, arg);
  } else if constexpr (std::is_same_v<std::string, std::decay_t<T>>) {
    AppendStringValue(buffer, arg);
  }

  CreateMessage(buffer, args...);
}

template <typename T, typename... Types>
static inline BufferType CreateMessageWithoutType(T arg, Types... args) {
  BufferType buffer(4, '0');
  CreateMessage(buffer, arg, args...);
  return buffer;
}

template <typename T, typename... Types>
static inline BufferType CreateMessageWithType(char type, T arg, Types... args) {
  BufferType buffer(5, '0');
  buffer[0] = type;
  CreateMessage(buffer, arg, args...);
  return buffer;
}

static inline std::vector<std::string> CreateStartupParams() {
  std::vector<std::string> parameters;

  parameters.push_back("user = postgres");
  parameters.push_back("database = postgres");
  parameters.push_back("param1 = value1");
  parameters.push_back("param2 = value2");

  std::shuffle(parameters.begin(), parameters.end(), std::default_random_engine());
  return parameters;
}

TEST(TestFeMsgDecoder, startup) {
  int32_t protocol_version = 196608; /* 3.0 */
  auto parameters = CreateStartupParams();
  auto startup_message = CreateMessageWithoutType(protocol_version, parameters[0], parameters[1],
                                                  parameters[2], parameters[3]);

  Decoder msg_decoder;
  auto summary = msg_decoder(startup_message);

  EXPECT_EQ(summary.type, Type::STARTUP_MESSAGE);

  auto content = std::get<ContentBase<Type::STARTUP_MESSAGE>>(summary.content);
  EXPECT_EQ(content.protocol_major_version, 3);
  EXPECT_EQ(content.protocol_minor_version, 0);
  EXPECT_EQ(content.connection_desc.username.compare("postgres"), 0);
  EXPECT_EQ(content.connection_desc.dbname.compare("postgres"), 0);

  auto expected_param1 = "param1=value1&param2=value2&";
  auto expected_param2 = "param2=value2&param1=value1&";
  auto& parameter = content.connection_desc.parameter;
  EXPECT_TRUE(parameter.compare(expected_param1) == 0 || parameter.compare(expected_param2) == 0);
}