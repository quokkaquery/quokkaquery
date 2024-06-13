#ifndef QQ_PROTOCOL_MSG_SUMMARY_H_
#define QQ_PROTOCOL_MSG_SUMMARY_H_

#include <variant> /* for std::variant */

#include "connection_desc.h"

namespace quokkaquery {
namespace protocol {
namespace femsg {
enum class Type {
  NONE,
  /* Messages without initial message type byte */
  STARTUP_MESSAGE,
  CANCEL_REQUEST,
  GSSENC_REQUEST,
  SSL_REQUEST,
  /* c */
  COPY_DONE,
  /* d */
  COPY_DATA,
  /* f */
  COPY_FAIL,
  /* p */
  PASSWORD_MESSAGE,
  GSS_RESPONSE,
  SAS_INITIAL_RESPONSE,
  SASL_RESPONSE,
  /* B */
  BIND,
  /* C */
  CLOSE,
  /* D */
  DESCRIBE,
  /* E */
  EXECUTE,
  /* F */
  FUNCTION_CALL,
  /* H */
  FLUSH,
  /* P */
  PARSE,
  /* Q */
  QUERY,
  /* S */
  SYNC,
  /* X */
  TERMINATE
};

template <Type>
struct ContentBase {};

template <>
struct ContentBase<Type::NONE> {};

template <>
struct ContentBase<Type::STARTUP_MESSAGE> {
  const int16_t protocol_major_version;
  const int16_t protocol_minor_version;
  const quokkaquery::protocol::ConnectionDesc connection_desc;
};

template <>
struct ContentBase<Type::CANCEL_REQUEST> {

};

template <>
struct ContentBase<Type::GSSENC_REQUEST> {

};

template <>
struct ContentBase<Type::SSL_REQUEST> {

};

template <>
struct ContentBase<Type::COPY_DONE> {

};

template <>
struct ContentBase<Type::COPY_DATA> {

};

template <>
struct ContentBase<Type::COPY_FAIL> {

};

template <>
struct ContentBase<Type::PASSWORD_MESSAGE> {

};

template <>
struct ContentBase<Type::GSS_RESPONSE> {

};

template <>
struct ContentBase<Type::SAS_INITIAL_RESPONSE> {

};

template <>
struct ContentBase<Type::SASL_RESPONSE> {

};

template <>
struct ContentBase<Type::BIND> {

};

template <>
struct ContentBase<Type::CLOSE> {

};

template <>
struct ContentBase<Type::DESCRIBE> {

};

template <>
struct ContentBase<Type::EXECUTE> {

};

template <>
struct ContentBase<Type::FUNCTION_CALL> {

};

template <>
struct ContentBase<Type::FLUSH> {

};

template <>
struct ContentBase<Type::PARSE> {

};

template <>
struct ContentBase<Type::QUERY> {

};

template <>
struct ContentBase<Type::SYNC> {};

template <>
struct ContentBase<Type::TERMINATE> {};

using Content = std::variant<
    ContentBase<Type::NONE>,
    ContentBase<Type::STARTUP_MESSAGE>,
    ContentBase<Type::CANCEL_REQUEST>,
    ContentBase<Type::GSSENC_REQUEST>,
    ContentBase<Type::SSL_REQUEST>,
    ContentBase<Type::COPY_DONE>,
    ContentBase<Type::COPY_DATA>,
    ContentBase<Type::COPY_FAIL>,
    ContentBase<Type::PASSWORD_MESSAGE>,
    ContentBase<Type::GSS_RESPONSE>,
    ContentBase<Type::SAS_INITIAL_RESPONSE>,
    ContentBase<Type::SASL_RESPONSE>,
    ContentBase<Type::BIND>,
    ContentBase<Type::CLOSE>,
    ContentBase<Type::DESCRIBE>,
    ContentBase<Type::EXECUTE>,
    ContentBase<Type::FUNCTION_CALL>,
    ContentBase<Type::FLUSH>,
    ContentBase<Type::PARSE>,
    ContentBase<Type::QUERY>,
    ContentBase<Type::SYNC>,
    ContentBase<Type::TERMINATE>
>;

struct Summary {
  const Type type;
  const int32_t length;
  const Content content;
};
}  // namespace femsg

namespace bemsg {
enum class Type {
  NONE,
  /* 1 */
  PARSE_COMPLETE,
  /* 2 */
  BIND_COMPLETE,
  /* 3 */
  CLOSE_COMPLETE,
  /* c */
  COPY_DONE,
  /* d */
  COPY_DATA,
  /* n */
  NO_DATA,
  /* s */
  PORTAL_SUSPENDED,
  /* t */
  PARAMETER_DESCRIPTION,
  /* v */
  NEGOTIATE_PROTOCOL_VERSION,
  /* A */
  NOTIFICATION_RESPONSE,
  /* C */
  COMMAND_COMPLETE,
  /* D */
  DATA_ROW,
  /* E */
  ERROR_RESPONSE,
  /* G */
  COPY_IN_RESPONSE,
  /* H */
  COPY_OUT_RESPONSE,
  /* I */
  EMPTY_QUERY_RESPONSE,
  /* K */
  BACKEND_KEY_DATA,
  /* N */
  NOTICE_RESPONSE,
  /* R */
  AUTHENTICATION_OK,
  AUTHENTICATION_KERBEROS_V5,
  AUTHENTICATION_CLEARTEXT_PASSWORD,
  AUTHENTICATION_MD5_PASSWORD,
  AUTHENTICATION_GSS,
  AUTHENTICATION_GSS_CONTINUE,
  AUTHENTICATION_SSPI,
  AUTHENTICATION_SASL,
  AUTHENTICATION_SASL_CONTINUE,
  AUTHENTICATION_SASL_FINAL,
  /* S */
  PARAMETER_STATUS,
  /* T */
  ROW_DESCRIPTION,
  /* V */
  FUNCTION_CALL_RESPONSE,
  /* W */
  COPY_BOTH_RESPONSE,
  /* Z */
  READY_FOR_QUERY
};

template <Type>
struct ContentBase {};

template <>
struct ContentBase<Type::NONE> {};

template <>
struct ContentBase<Type::PARSE_COMPLETE> {

};

template <>
struct ContentBase<Type::BIND_COMPLETE> {

};

template <>
struct ContentBase<Type::CLOSE_COMPLETE> {

};

template <>
struct ContentBase<Type::COPY_DONE> {

};

template <>
struct ContentBase<Type::COPY_DATA> {

};

template <>
struct ContentBase<Type::NO_DATA> {

};

template <>
struct ContentBase<Type::PORTAL_SUSPENDED> {

};

template <>
struct ContentBase<Type::PARAMETER_DESCRIPTION> {

};

template <>
struct ContentBase<Type::NEGOTIATE_PROTOCOL_VERSION> {

};

template <>
struct ContentBase<Type::NOTIFICATION_RESPONSE> {

};

template <>
struct ContentBase<Type::COMMAND_COMPLETE> {

};

template <>
struct ContentBase<Type::DATA_ROW> {

};

template <>
struct ContentBase<Type::ERROR_RESPONSE> {

};

template <>
struct ContentBase<Type::COPY_IN_RESPONSE> {

};

template <>
struct ContentBase<Type::COPY_OUT_RESPONSE> {

};

template <>
struct ContentBase<Type::EMPTY_QUERY_RESPONSE> {

};

template <>
struct ContentBase<Type::BACKEND_KEY_DATA> {

};

template <>
struct ContentBase<Type::NOTICE_RESPONSE> {

};

template <>
struct ContentBase<Type::AUTHENTICATION_OK> {

};

template <>
struct ContentBase<Type::AUTHENTICATION_KERBEROS_V5> {

};

template <>
struct ContentBase<Type::AUTHENTICATION_CLEARTEXT_PASSWORD> {

};

template <>
struct ContentBase<Type::AUTHENTICATION_MD5_PASSWORD> {

};

template <>
struct ContentBase<Type::AUTHENTICATION_GSS> {

};

template <>
struct ContentBase<Type::AUTHENTICATION_GSS_CONTINUE> {

};

template <>
struct ContentBase<Type::AUTHENTICATION_SSPI> {

};

template <>
struct ContentBase<Type::AUTHENTICATION_SASL> {

};

template <>
struct ContentBase<Type::AUTHENTICATION_SASL_CONTINUE> {

};

template <>
struct ContentBase<Type::AUTHENTICATION_SASL_FINAL> {

};

template <>
struct ContentBase<Type::PARAMETER_STATUS> {

};

template <>
struct ContentBase<Type::ROW_DESCRIPTION> {

};

template <>
struct ContentBase<Type::FUNCTION_CALL_RESPONSE> {

};

template <>
struct ContentBase<Type::COPY_BOTH_RESPONSE> {

};

template <>
struct ContentBase<Type::READY_FOR_QUERY> {

};

using Content = std::variant<
    ContentBase<Type::NONE>,
    ContentBase<Type::PARSE_COMPLETE>,
    ContentBase<Type::BIND_COMPLETE>,
    ContentBase<Type::CLOSE_COMPLETE>,
    ContentBase<Type::COPY_DONE>,
    ContentBase<Type::COPY_DATA>,
    ContentBase<Type::NO_DATA>,
    ContentBase<Type::PORTAL_SUSPENDED>,
    ContentBase<Type::PARAMETER_DESCRIPTION>,
    ContentBase<Type::NEGOTIATE_PROTOCOL_VERSION>,
    ContentBase<Type::NOTIFICATION_RESPONSE>,
    ContentBase<Type::COMMAND_COMPLETE>,
    ContentBase<Type::DATA_ROW>,
    ContentBase<Type::ERROR_RESPONSE>,
    ContentBase<Type::COPY_IN_RESPONSE>,
    ContentBase<Type::COPY_OUT_RESPONSE>,
    ContentBase<Type::EMPTY_QUERY_RESPONSE>,
    ContentBase<Type::BACKEND_KEY_DATA>,
    ContentBase<Type::NOTICE_RESPONSE>,
    ContentBase<Type::AUTHENTICATION_OK>,
    ContentBase<Type::AUTHENTICATION_KERBEROS_V5>,
    ContentBase<Type::AUTHENTICATION_CLEARTEXT_PASSWORD>,
    ContentBase<Type::AUTHENTICATION_MD5_PASSWORD>,
    ContentBase<Type::AUTHENTICATION_GSS>,
    ContentBase<Type::AUTHENTICATION_GSS_CONTINUE>,
    ContentBase<Type::AUTHENTICATION_SSPI>,
    ContentBase<Type::AUTHENTICATION_SASL>,
    ContentBase<Type::AUTHENTICATION_SASL_CONTINUE>,
    ContentBase<Type::AUTHENTICATION_SASL_FINAL>,
    ContentBase<Type::PARAMETER_STATUS>,
    ContentBase<Type::ROW_DESCRIPTION>,
    ContentBase<Type::FUNCTION_CALL_RESPONSE>,
    ContentBase<Type::COPY_BOTH_RESPONSE>,
    ContentBase<Type::READY_FOR_QUERY> >;

struct Summary {
  const Type type;
  const int32_t length;
  const Content content;
};
}  // namespace bemsg
}  // namespace protocol
}  // namespace quokkaquery
#endif /* QQ_PROTOCOL_MSG_SUMMARY_H_ */