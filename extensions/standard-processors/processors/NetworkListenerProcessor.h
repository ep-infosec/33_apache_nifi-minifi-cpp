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

#include <memory>
#include <string>
#include <utility>

#include "core/Processor.h"
#include "core/logging/Logger.h"
#include "core/ProcessContext.h"
#include "core/ProcessSession.h"
#include "core/Property.h"
#include "utils/net/Server.h"
#include "utils/net/SslServer.h"

namespace org::apache::nifi::minifi::processors {

class NetworkListenerProcessor : public core::Processor {
 public:
  NetworkListenerProcessor(std::string name, const utils::Identifier& uuid, std::shared_ptr<core::logging::Logger> logger)
    : core::Processor(std::move(name), uuid),
      logger_(std::move(logger)) {
  }
  ~NetworkListenerProcessor() override;

  void onTrigger(const std::shared_ptr<core::ProcessContext>& context, const std::shared_ptr<core::ProcessSession>& session) override;

  EXTENSIONAPI static constexpr bool SupportsDynamicProperties = false;
  EXTENSIONAPI static constexpr bool SupportsDynamicRelationships = false;
  EXTENSIONAPI static constexpr core::annotation::Input InputRequirement = core::annotation::Input::INPUT_FORBIDDEN;
  EXTENSIONAPI static constexpr bool IsSingleThreaded = false;

  ADD_COMMON_VIRTUAL_FUNCTIONS_FOR_PROCESSORS

  void notifyStop() override {
    stopServer();
  }

 protected:
  void startTcpServer(const core::ProcessContext& context, const core::Property& ssl_context_property, const core::Property& client_auth_property);
  void startUdpServer(const core::ProcessContext& context);

 private:
  struct ServerOptions {
    std::optional<uint64_t> max_queue_size;
    int port = 0;
  };

  void stopServer();
  void startServer(const ServerOptions& options, utils::net::IpProtocol protocol);
  ServerOptions readServerOptions(const core::ProcessContext& context);

  virtual void transferAsFlowFile(const utils::net::Message& message, core::ProcessSession& session) = 0;
  virtual const core::Property& getMaxBatchSizeProperty() = 0;
  virtual const core::Property& getMaxQueueSizeProperty() = 0;
  virtual const core::Property& getPortProperty() = 0;

  uint64_t max_batch_size_{500};
  std::unique_ptr<utils::net::Server> server_;
  std::thread server_thread_;
  std::shared_ptr<core::logging::Logger> logger_;
};

}  // namespace org::apache::nifi::minifi::processors
