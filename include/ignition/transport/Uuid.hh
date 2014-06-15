/*
 * Copyright (C) 2014 Open Source Robotics Foundation
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

#ifndef __IGN_TRANSPORT_UUID_HH_INCLUDED__
#define __IGN_TRANSPORT_UUID_HH_INCLUDED__

#include <uuid/uuid.h>
#include <iostream>
#include <string>
#include "ignition/transport/Helpers.hh"

namespace ignition
{
  namespace transport
  {
    /// \class Uuid Uuid.hh
    /// \brief A portable class for representing a Universally Unique Identifier
    class IGNITION_VISIBLE Uuid
    {
      /// \brief Constructor.
      public: Uuid();

      /// \brief Destructor.
      public: virtual ~Uuid();

      /// \brief Return the string representation of the Uuid.
      /// \return the UUID in string format.
      public: std::string ToString() const;

      public: friend std::ostream &operator<<(std::ostream &_out,
                                        const ignition::transport::Uuid &_uuid)
      {
        _out << _uuid.ToString();
        return _out;
      }

      /// \brief Length of a UUID in string format.
      private: static const int UuidStrLen = (sizeof(uuid_t) * 2) + 4 + 1;

      /// \brief Internal representation.
      private: uuid_t data;
    };
  }
}
#endif