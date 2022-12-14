/**
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
#pragma once

#include <optional>
#include <memory>

#include "SessionHandlingServer.h"

namespace org::apache::nifi::minifi::utils::net {

class TcpSession : public std::enable_shared_from_this<TcpSession> {
 public:
  TcpSession(asio::io_context& io_context, utils::ConcurrentQueue<Message>& concurrent_queue, std::optional<size_t> max_queue_size, std::shared_ptr<core::logging::Logger> logger);
  asio::ip::tcp::socket& getSocket();
  void start();
  void handleReadUntilNewLine(std::error_code error_code);

 private:
  utils::ConcurrentQueue<Message>& concurrent_queue_;
  std::optional<size_t> max_queue_size_;
  asio::basic_streambuf<std::allocator<char>> buffer_;
  asio::ip::tcp::socket socket_;
  std::shared_ptr<core::logging::Logger> logger_;
};

class TcpServer : public SessionHandlingServer<TcpSession> {
 public:
  TcpServer(std::optional<size_t> max_queue_size,
            uint16_t port,
            std::shared_ptr<core::logging::Logger> logger);

 protected:
  std::shared_ptr<TcpSession> createSession() override;
};

}  // namespace org::apache::nifi::minifi::utils::net
