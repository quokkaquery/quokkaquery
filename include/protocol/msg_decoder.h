#ifndef QQ_PROTOCOL_MSG_DECODER_H_
#define QQ_PROTOCOL_MSG_DECODER_H_

#include <vector> /* for std::vector */

#include "msg_summary.h"

namespace quokkaquery {
namespace protocol {
namespace femsg {
class Decoder {
 public:
  using BufferType = std::vector<char>;

  Decoder();
  ~Decoder() = default;

  const Summary operator()(const BufferType&);

 private:
  enum class Progress;

  const Summary HandleStartupMessage(const BufferType&);
  const Summary HandleAuthMessage(const BufferType&);
  const Summary HandleOperationMessage(const BufferType&);

  Progress progress_;
};

enum class Decoder::Progress {
  STARTUP_REQUIRED,
  STARTUP_COMMENCING,
  STARTUP_COMPLETE
};
}  // namespace femsg

namespace bemsg {
class Decoder {
 public:
  using BufferType = std::vector<char>;

  Decoder() = default;
  ~Decoder() = default;

  const Summary operator()(const BufferType&) const;
};
}  // namespace bemsg
}  // namespace protocol
}  // namespace quokkaquery

#endif /* QQ_PROTOCOL_MSG_DECODER_H_ */