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

#pragma once

#include <memory>
#include <utility>
#include "LogBuffer.h"
#include "LogCompressor.h"
#include "core/logging/Logger.h"

namespace org {
namespace apache {
namespace nifi {
namespace minifi {
namespace core {
namespace logging {
namespace internal {

class ActiveCompressor {
 public:
  class Allocator {
   public:
    explicit Allocator(std::shared_ptr<logging::Logger> logger) : logger_{std::move(logger)} {}

    ActiveCompressor operator()(size_t max_size) const {
      ActiveCompressor instance;
      instance.output_.reset(new io::BufferStream());
      instance.output_->extend(max_size);
      instance.compressor_.reset(new LogCompressor(gsl::make_not_null(instance.output_.get()), logger_));
      return instance;
    }

   private:
    std::shared_ptr<logging::Logger> logger_;
  };

  LogBuffer commit() {
    compressor_->close();
    return LogBuffer{std::move(output_)};
  }

  size_t size() const {
    return output_->size();
  }

  std::unique_ptr<io::BufferStream> output_;
  std::unique_ptr<LogCompressor> compressor_;
};

}  // namespace internal
}  // namespace logging
}  // namespace core
}  // namespace minifi
}  // namespace nifi
}  // namespace apache
}  // namespace org
