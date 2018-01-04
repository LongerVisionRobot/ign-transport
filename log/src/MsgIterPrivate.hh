/*
 * Copyright (C) 2017 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#ifndef IGNITION_TRANSPORT_LOG_MSGITERPRIVATE_HH_
#define IGNITION_TRANSPORT_LOG_MSGITERPRIVATE_HH_

#include "ignition/transport/log/Message.hh"

#include "src/raii-sqlite3.hh"


using namespace ignition::transport;
using namespace ignition::transport::log;

class ignition::transport::log::MsgIterPrivate
{
  /// \brief a statement that is being stepped
  public: std::unique_ptr<raii_sqlite3::Statement> statement;

  public: std::unique_ptr<Message> message;

  /// \brief Executes the statement once
  public: void StepStatement();
};

#endif