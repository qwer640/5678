﻿
#pragma once

#include <cstdint>
#include <string>

#include "geometry/named_quantities.hpp"
#include "google/protobuf/descriptor.h"
#include "serialization/geometry.pb.h"

namespace principia {
namespace geometry {
namespace internal_frame {

using base::not_null;

template<typename FrameTag, FrameTag frame_tag, bool frame_is_inertial>
class Frame {
 public:
  using Tag = FrameTag;
  static Position<Frame> const origin;
  static Tag const tag = frame_tag;
  static bool const is_inertial = frame_is_inertial;

  Frame() = delete;

  static void WriteToMessage(not_null<serialization::Frame*> const message);

  // Checks that the |message| matches the current type.
  static void ReadFromMessage(serialization::Frame const& message);
};

// Extracts enough information from the |message| to contruct a |Frame| type.
void ReadFrameFromMessage(
    serialization::Frame const& message,
    google::protobuf::EnumValueDescriptor const*& enum_value_descriptor,
    bool& is_inertial);

}  // namespace internal_frame

using internal_frame::Frame;
using internal_frame::ReadFrameFromMessage;

}  // namespace geometry
}  // namespace principia

#include "geometry/frame_body.hpp"
