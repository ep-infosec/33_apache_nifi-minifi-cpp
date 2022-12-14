/**
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef LIBMINIFI_INCLUDE_IO_SERVERSOCKET_H_
#define LIBMINIFI_INCLUDE_IO_SERVERSOCKET_H_

#include <memory>
#include <string>

#include "io/ClientSocket.h"

namespace org {
namespace apache {
namespace nifi {
namespace minifi {
namespace io {


class BaseServerSocket  {
 public:
  virtual ~BaseServerSocket()= default;

  virtual int16_t initialize(bool loopbackOnly) = 0;

  virtual void registerCallback(std::function<bool()> accept_function, std::function<void(io::BaseStream *)> handler) = 0;
};
/**
 * Purpose: Server socket abstraction that makes focusing the accept/block paradigm
 * simpler.
 */
class ServerSocket : public BaseServerSocket, public Socket {
 public:
  explicit ServerSocket(const std::shared_ptr<SocketContext> &context, const std::string &hostname, uint16_t port, uint16_t listeners);

  virtual ~ServerSocket();

  int16_t initialize(bool loopbackOnly) override {
    is_loopback_only_ = loopbackOnly;
    return Socket::initialize();
  }

  int initialize() override {
    return Socket::initialize();
  }

  /**
   * Registers a call back and starts the read for the server socket.
   */
  void registerCallback(std::function<bool()> accept_function, std::function<void(io::BaseStream *)> handler) override;

 private:
  void close_fd(int fd);

  std::atomic<bool> running_;

  std::thread server_read_thread_;

  std::shared_ptr<core::logging::Logger> logger_;
};

}  // namespace io
}  // namespace minifi
}  // namespace nifi
}  // namespace apache
}  // namespace org
#endif  // LIBMINIFI_INCLUDE_IO_SERVERSOCKET_H_
