#ifndef QQ_CP_CONNECTION_MAP_H_
#define QQ_CP_CONNECTION_MAP_H_

#include <memory>        /* for std::shared_ptr */
#include <unordered_map> /* for std::unordered_map */
#include <vector>        /* for std::vector */

#include "connection_desc.h"

namespace quokkaquery {
namespace cp {
class Connection;

class ConnectionMap {
 public:
  using ValueType = std::vector<std::shared_ptr<Connection>>;
  using TableType = std::unordered_map<ConnectionDesc, ValueType>;

  explicit ConnectionMap(std::size_t pool_size);
  ~ConnectionMap() = default;

  const bool Empty() const;
  const std::size_t Size() const;
  const std::size_t Capability() const;

  void Resize(std::size_t new_size);

  const std::size_t Insert(const ConnectionDesc&, std::shared_ptr<Connection>);
  void Delete(const ConnectionDesc&, std::size_t index);

  std::shared_ptr<Connection> Get(const ConnectionDesc&, std::size_t index);
  std::shared_ptr<Connection> Reuse(const ConnectionDesc&);

 private:
  TableType conn_table_;
  std::size_t size_;
  std::size_t capability_;
};
}  // namespace cp
}  // namespace quokkaquery

#endif /* QQ_CP_CONNECTION_MAP_H_ */