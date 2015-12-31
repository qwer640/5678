void AddVesselToNextPhysicsBubble::Fill(In const& in, not_null<Message*> const message) {
  auto* m = message->mutable_in();
  m->set_plugin(SerializePointer(in.plugin));
  m->set_vessel_guid(in.vessel_guid);
  for (KSPPart const* parts = in.parts; parts < in.parts + in.count; ++parts) {
    *m->add_parts() = SerializeKSPPart(*parts);
  }
}

void AddVesselToNextPhysicsBubble::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto plugin = DeserializePointer<Plugin*>(*pointer_map, in.plugin());
  auto vessel_guid = in.vessel_guid().c_str();
  auto parts = [](::google::protobuf::RepeatedPtrField<serialization::KSPPart> const& messages) -> std::vector<KSPPart> {
      std::vector<KSPPart> deserialized_parts;
      for (auto const& message : messages) {
        deserialized_parts.push_back(DeserializeKSPPart(message));
      }
      return deserialized_parts;
    }(in.parts());
  ksp_plugin::principia__AddVesselToNextPhysicsBubble(plugin, vessel_guid, &parts[0], parts.size());
}

void AdvanceTime::Fill(In const& in, not_null<Message*> const message) {
  auto* m = message->mutable_in();
  m->set_plugin(SerializePointer(in.plugin));
  m->set_t(in.t);
  m->set_planetarium_rotation(in.planetarium_rotation);
}

void AdvanceTime::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto plugin = DeserializePointer<Plugin*>(*pointer_map, in.plugin());
  auto t = in.t();
  auto planetarium_rotation = in.planetarium_rotation();
  ksp_plugin::principia__AdvanceTime(plugin, t, planetarium_rotation);
}

void AtEnd::Fill(In const& in, not_null<Message*> const message) {
  message->mutable_in()->set_line_and_iterator(SerializePointer(in.line_and_iterator));
}

void AtEnd::Fill(Return const& result, not_null<Message*> const message) {
  message->mutable_return_()->set_at_end(result);
}

void AtEnd::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto line_and_iterator = DeserializePointer<LineAndIterator const*>(*pointer_map, in.line_and_iterator());
  auto const result = ksp_plugin::principia__AtEnd(line_and_iterator);
  CHECK(message.return_().at_end() == result);
}

void BubbleDisplacementCorrection::Fill(In const& in, not_null<Message*> const message) {
  auto* m = message->mutable_in();
  m->set_plugin(SerializePointer(in.plugin));
  *m->mutable_sun_position() = SerializeXYZ(in.sun_position);
}

void BubbleDisplacementCorrection::Fill(Return const& result, not_null<Message*> const message) {
  *message->mutable_return_()->mutable_bubble_displacement_correction() = SerializeXYZ(result);
}

void BubbleDisplacementCorrection::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto plugin = DeserializePointer<Plugin const*>(*pointer_map, in.plugin());
  auto sun_position = DeserializeXYZ(in.sun_position());
  auto const result = ksp_plugin::principia__BubbleDisplacementCorrection(plugin, sun_position);
  CHECK(DeserializeXYZ(message.return_().bubble_displacement_correction()) == result);
}

void BubbleVelocityCorrection::Fill(In const& in, not_null<Message*> const message) {
  auto* m = message->mutable_in();
  m->set_plugin(SerializePointer(in.plugin));
  m->set_reference_body_index(in.reference_body_index);
}

void BubbleVelocityCorrection::Fill(Return const& result, not_null<Message*> const message) {
  *message->mutable_return_()->mutable_bubble_velocity_correction() = SerializeXYZ(result);
}

void BubbleVelocityCorrection::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto plugin = DeserializePointer<Plugin const*>(*pointer_map, in.plugin());
  auto reference_body_index = in.reference_body_index();
  auto const result = ksp_plugin::principia__BubbleVelocityCorrection(plugin, reference_body_index);
  CHECK(DeserializeXYZ(message.return_().bubble_velocity_correction()) == result);
}

void CelestialFromParent::Fill(In const& in, not_null<Message*> const message) {
  auto* m = message->mutable_in();
  m->set_plugin(SerializePointer(in.plugin));
  m->set_celestial_index(in.celestial_index);
}

void CelestialFromParent::Fill(Return const& result, not_null<Message*> const message) {
  *message->mutable_return_()->mutable_celestial_from_parent() = SerializeQP(result);
}

void CelestialFromParent::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto plugin = DeserializePointer<Plugin const*>(*pointer_map, in.plugin());
  auto celestial_index = in.celestial_index();
  auto const result = ksp_plugin::principia__CelestialFromParent(plugin, celestial_index);
  CHECK(DeserializeQP(message.return_().celestial_from_parent()) == result);
}

void CurrentTime::Fill(In const& in, not_null<Message*> const message) {
  message->mutable_in()->set_plugin(SerializePointer(in.plugin));
}

void CurrentTime::Fill(Return const& result, not_null<Message*> const message) {
  message->mutable_return_()->set_current_time(result);
}

void CurrentTime::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto plugin = DeserializePointer<Plugin const*>(*pointer_map, in.plugin());
  auto const result = ksp_plugin::principia__CurrentTime(plugin);
  CHECK(message.return_().current_time() == result);
}

void DeleteLineAndIterator::Fill(In const& in, not_null<Message*> const message) {
  message->mutable_in()->set_line_and_iterator(SerializePointer(*in.line_and_iterator));
}

void DeleteLineAndIterator::Fill(Out const& out, not_null<Message*> const message) {
  message->mutable_out()->set_line_and_iterator(SerializePointer(*out.line_and_iterator));
}

void DeleteLineAndIterator::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto line_and_iterator = DeserializePointer<LineAndIterator*>(*pointer_map, in.line_and_iterator());
  ksp_plugin::principia__DeleteLineAndIterator(&line_and_iterator);
  Delete(pointer_map, in.line_and_iterator());
}

void DeletePlugin::Fill(In const& in, not_null<Message*> const message) {
  message->mutable_in()->set_plugin(SerializePointer(*in.plugin));
}

void DeletePlugin::Fill(Out const& out, not_null<Message*> const message) {
  message->mutable_out()->set_plugin(SerializePointer(*out.plugin));
}

void DeletePlugin::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto plugin = DeserializePointer<Plugin const*>(*pointer_map, in.plugin());
  ksp_plugin::principia__DeletePlugin(&plugin);
  Delete(pointer_map, in.plugin());
}

void DeletePluginSerialization::Fill(In const& in, not_null<Message*> const message) {
  message->mutable_in()->set_serialization(SerializePointer(*in.serialization));
}

void DeletePluginSerialization::Fill(Out const& out, not_null<Message*> const message) {
  message->mutable_out()->set_serialization(SerializePointer(*out.serialization));
}

void DeletePluginSerialization::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto serialization = DeserializePointer<char const*>(*pointer_map, in.serialization());
  ksp_plugin::principia__DeletePluginSerialization(&serialization);
  Delete(pointer_map, in.serialization());
}

void DeserializePlugin::Fill(In const& in, not_null<Message*> const message) {
  auto* m = message->mutable_in();
  m->set_serialization(std::string(in.serialization, in.serialization_size));
  m->set_deserializer(SerializePointer(*in.deserializer));
  m->set_plugin(SerializePointer(*in.plugin));
}

void DeserializePlugin::Fill(Out const& out, not_null<Message*> const message) {
  auto* m = message->mutable_out();
  m->set_deserializer(SerializePointer(*out.deserializer));
  m->set_plugin(SerializePointer(*out.plugin));
}

void DeserializePlugin::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto serialization = &in.serialization();
  auto deserializer = DeserializePointer<PushDeserializer*>(*pointer_map, in.deserializer());
  auto plugin = DeserializePointer<Plugin const*>(*pointer_map, in.plugin());
  ksp_plugin::principia__DeserializePlugin(serialization->c_str(), serialization->size(), &deserializer, &plugin);
  if (serialization->empty()) {
    Delete(pointer_map, in.deserializer());
  }
  if (!serialization->empty()) {
    Insert(pointer_map, message.out().deserializer(), deserializer);
  }
  Insert(pointer_map, message.out().plugin(), plugin);
}

void DirectlyInsertCelestial::Fill(In const& in, not_null<Message*> const message) {
  auto* m = message->mutable_in();
  m->set_plugin(SerializePointer(in.plugin));
  m->set_celestial_index(in.celestial_index);
if (in.parent_index != nullptr) {
    m->set_parent_index(*in.parent_index);
  }
  m->set_gravitational_parameter(in.gravitational_parameter);
if (in.axis_right_ascension != nullptr) {
    m->set_axis_right_ascension(in.axis_right_ascension);
  }
if (in.axis_declination != nullptr) {
    m->set_axis_declination(in.axis_declination);
  }
if (in.j2 != nullptr) {
    m->set_j2(in.j2);
  }
if (in.reference_radius != nullptr) {
    m->set_reference_radius(in.reference_radius);
  }
  m->set_x(in.x);
  m->set_y(in.y);
  m->set_z(in.z);
  m->set_vx(in.vx);
  m->set_vy(in.vy);
  m->set_vz(in.vz);
}

void DirectlyInsertCelestial::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto plugin = DeserializePointer<Plugin*>(*pointer_map, in.plugin());
  auto celestial_index = in.celestial_index();
  auto parent_index = in.has_parent_index() ? std::make_unique<int const>(in.parent_index()) : nullptr;
  auto gravitational_parameter = in.gravitational_parameter().c_str();
  auto axis_right_ascension = in.has_axis_right_ascension() ? in.axis_right_ascension().c_str() : nullptr;
  auto axis_declination = in.has_axis_declination() ? in.axis_declination().c_str() : nullptr;
  auto j2 = in.has_j2() ? in.j2().c_str() : nullptr;
  auto reference_radius = in.has_reference_radius() ? in.reference_radius().c_str() : nullptr;
  auto x = in.x().c_str();
  auto y = in.y().c_str();
  auto z = in.z().c_str();
  auto vx = in.vx().c_str();
  auto vy = in.vy().c_str();
  auto vz = in.vz().c_str();
  ksp_plugin::principia__DirectlyInsertCelestial(plugin, celestial_index, parent_index.get(), gravitational_parameter, axis_right_ascension, axis_declination, j2, reference_radius, x, y, z, vx, vy, vz);
}

void EndInitialization::Fill(In const& in, not_null<Message*> const message) {
  message->mutable_in()->set_plugin(SerializePointer(in.plugin));
}

void EndInitialization::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto plugin = DeserializePointer<Plugin*>(*pointer_map, in.plugin());
  ksp_plugin::principia__EndInitialization(plugin);
}

void FetchAndIncrement::Fill(In const& in, not_null<Message*> const message) {
  message->mutable_in()->set_line_and_iterator(SerializePointer(in.line_and_iterator));
}

void FetchAndIncrement::Fill(Return const& result, not_null<Message*> const message) {
  *message->mutable_return_()->mutable_fetch_and_increment() = SerializeXYZSegment(result);
}

void FetchAndIncrement::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto line_and_iterator = DeserializePointer<LineAndIterator*>(*pointer_map, in.line_and_iterator());
  auto const result = ksp_plugin::principia__FetchAndIncrement(line_and_iterator);
  CHECK(DeserializeXYZSegment(message.return_().fetch_and_increment()) == result);
}

void FlightPlanSize::Fill(In const& in, not_null<Message*> const message) {
  auto* m = message->mutable_in();
  m->set_plugin(SerializePointer(in.plugin));
  m->set_vessel_guid(in.vessel_guid);
}

void FlightPlanSize::Fill(Return const& result, not_null<Message*> const message) {
  message->mutable_return_()->set_flight_plan_size(result);
}

void FlightPlanSize::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto plugin = DeserializePointer<Plugin const*>(*pointer_map, in.plugin());
  auto vessel_guid = in.vessel_guid().c_str();
  auto const result = ksp_plugin::principia__FlightPlanSize(plugin, vessel_guid);
  CHECK(message.return_().flight_plan_size() == result);
}

void ForgetAllHistoriesBefore::Fill(In const& in, not_null<Message*> const message) {
  auto* m = message->mutable_in();
  m->set_plugin(SerializePointer(in.plugin));
  m->set_t(in.t);
}

void ForgetAllHistoriesBefore::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto plugin = DeserializePointer<Plugin*>(*pointer_map, in.plugin());
  auto t = in.t();
  ksp_plugin::principia__ForgetAllHistoriesBefore(plugin, t);
}

void GetBufferDuration::Fill(Return const& result, not_null<Message*> const message) {
  message->mutable_return_()->set_get_buffer_duration(result);
}

void GetBufferDuration::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const result = ksp_plugin::principia__GetBufferDuration();
  CHECK(message.return_().get_buffer_duration() == result);
}

void GetBufferedLogging::Fill(Return const& result, not_null<Message*> const message) {
  message->mutable_return_()->set_get_buffered_logging(result);
}

void GetBufferedLogging::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const result = ksp_plugin::principia__GetBufferedLogging();
  CHECK(message.return_().get_buffered_logging() == result);
}

void GetStderrLogging::Fill(Return const& result, not_null<Message*> const message) {
  message->mutable_return_()->set_get_stderr_logging(result);
}

void GetStderrLogging::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const result = ksp_plugin::principia__GetStderrLogging();
  CHECK(message.return_().get_stderr_logging() == result);
}

void GetSuppressedLogging::Fill(Return const& result, not_null<Message*> const message) {
  message->mutable_return_()->set_get_suppressed_logging(result);
}

void GetSuppressedLogging::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const result = ksp_plugin::principia__GetSuppressedLogging();
  CHECK(message.return_().get_suppressed_logging() == result);
}

void GetVerboseLogging::Fill(Return const& result, not_null<Message*> const message) {
  message->mutable_return_()->set_get_verbose_logging(result);
}

void GetVerboseLogging::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const result = ksp_plugin::principia__GetVerboseLogging();
  CHECK(message.return_().get_verbose_logging() == result);
}

void HasPrediction::Fill(In const& in, not_null<Message*> const message) {
  auto* m = message->mutable_in();
  m->set_plugin(SerializePointer(in.plugin));
  m->set_vessel_guid(in.vessel_guid);
}

void HasPrediction::Fill(Return const& result, not_null<Message*> const message) {
  message->mutable_return_()->set_has_prediction(result);
}

void HasPrediction::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto plugin = DeserializePointer<Plugin const*>(*pointer_map, in.plugin());
  auto vessel_guid = in.vessel_guid().c_str();
  auto const result = ksp_plugin::principia__HasPrediction(plugin, vessel_guid);
  CHECK(message.return_().has_prediction() == result);
}

void HasVessel::Fill(In const& in, not_null<Message*> const message) {
  auto* m = message->mutable_in();
  m->set_plugin(SerializePointer(in.plugin));
  m->set_vessel_guid(in.vessel_guid);
}

void HasVessel::Fill(Return const& result, not_null<Message*> const message) {
  message->mutable_return_()->set_has_vessel(result);
}

void HasVessel::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto plugin = DeserializePointer<Plugin*>(*pointer_map, in.plugin());
  auto vessel_guid = in.vessel_guid().c_str();
  auto const result = ksp_plugin::principia__HasVessel(plugin, vessel_guid);
  CHECK(message.return_().has_vessel() == result);
}

void InitGoogleLogging::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  ksp_plugin::principia__InitGoogleLogging();
}

void InsertCelestial::Fill(In const& in, not_null<Message*> const message) {
  auto* m = message->mutable_in();
  m->set_plugin(SerializePointer(in.plugin));
  m->set_celestial_index(in.celestial_index);
  m->set_gravitational_parameter(in.gravitational_parameter);
  m->set_parent_index(in.parent_index);
  *m->mutable_from_parent() = SerializeQP(in.from_parent);
}

void InsertCelestial::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto plugin = DeserializePointer<Plugin*>(*pointer_map, in.plugin());
  auto celestial_index = in.celestial_index();
  auto gravitational_parameter = in.gravitational_parameter();
  auto parent_index = in.parent_index();
  auto from_parent = DeserializeQP(in.from_parent());
  ksp_plugin::principia__InsertCelestial(plugin, celestial_index, gravitational_parameter, parent_index, from_parent);
}

void InsertOrKeepVessel::Fill(In const& in, not_null<Message*> const message) {
  auto* m = message->mutable_in();
  m->set_plugin(SerializePointer(in.plugin));
  m->set_vessel_guid(in.vessel_guid);
  m->set_parent_index(in.parent_index);
}

void InsertOrKeepVessel::Fill(Return const& result, not_null<Message*> const message) {
  message->mutable_return_()->set_insert_or_keep_vessel(result);
}

void InsertOrKeepVessel::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto plugin = DeserializePointer<Plugin*>(*pointer_map, in.plugin());
  auto vessel_guid = in.vessel_guid().c_str();
  auto parent_index = in.parent_index();
  auto const result = ksp_plugin::principia__InsertOrKeepVessel(plugin, vessel_guid, parent_index);
  CHECK(message.return_().insert_or_keep_vessel() == result);
}

void InsertSun::Fill(In const& in, not_null<Message*> const message) {
  auto* m = message->mutable_in();
  m->set_plugin(SerializePointer(in.plugin));
  m->set_celestial_index(in.celestial_index);
  m->set_gravitational_parameter(in.gravitational_parameter);
}

void InsertSun::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto plugin = DeserializePointer<Plugin*>(*pointer_map, in.plugin());
  auto celestial_index = in.celestial_index();
  auto gravitational_parameter = in.gravitational_parameter();
  ksp_plugin::principia__InsertSun(plugin, celestial_index, gravitational_parameter);
}

void LogError::Fill(In const& in, not_null<Message*> const message) {
  message->mutable_in()->set_text(in.text);
}

void LogError::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto text = in.text().c_str();
  ksp_plugin::principia__LogError(text);
}

void LogFatal::Fill(In const& in, not_null<Message*> const message) {
  message->mutable_in()->set_text(in.text);
}

void LogFatal::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto text = in.text().c_str();
  ksp_plugin::principia__LogFatal(text);
}

void LogInfo::Fill(In const& in, not_null<Message*> const message) {
  message->mutable_in()->set_text(in.text);
}

void LogInfo::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto text = in.text().c_str();
  ksp_plugin::principia__LogInfo(text);
}

void LogWarning::Fill(In const& in, not_null<Message*> const message) {
  message->mutable_in()->set_text(in.text);
}

void LogWarning::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto text = in.text().c_str();
  ksp_plugin::principia__LogWarning(text);
}

void NavballOrientation::Fill(In const& in, not_null<Message*> const message) {
  auto* m = message->mutable_in();
  m->set_plugin(SerializePointer(in.plugin));
  *m->mutable_sun_world_position() = SerializeXYZ(in.sun_world_position);
  *m->mutable_ship_world_position() = SerializeXYZ(in.ship_world_position);
}

void NavballOrientation::Fill(Return const& result, not_null<Message*> const message) {
  *message->mutable_return_()->mutable_navball_orientation() = SerializeWXYZ(result);
}

void NavballOrientation::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto plugin = DeserializePointer<Plugin const*>(*pointer_map, in.plugin());
  auto sun_world_position = DeserializeXYZ(in.sun_world_position());
  auto ship_world_position = DeserializeXYZ(in.ship_world_position());
  auto const result = ksp_plugin::principia__NavballOrientation(plugin, sun_world_position, ship_world_position);
  CHECK(DeserializeWXYZ(message.return_().navball_orientation()) == result);
}

void NewBarycentricRotatingNavigationFrame::Fill(In const& in, not_null<Message*> const message) {
  auto* m = message->mutable_in();
  m->set_plugin(SerializePointer(in.plugin));
  m->set_primary_index(in.primary_index);
  m->set_secondary_index(in.secondary_index);
}

void NewBarycentricRotatingNavigationFrame::Fill(Return const& result, not_null<Message*> const message) {
  message->mutable_return_()->set_new_barycentric_rotating_navigation_frame(SerializePointer(result));
}

void NewBarycentricRotatingNavigationFrame::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto plugin = DeserializePointer<Plugin const*>(*pointer_map, in.plugin());
  auto primary_index = in.primary_index();
  auto secondary_index = in.secondary_index();
  auto const result = ksp_plugin::principia__NewBarycentricRotatingNavigationFrame(plugin, primary_index, secondary_index);
  Insert(pointer_map, message.return_().new_barycentric_rotating_navigation_frame(), result);
}

void NewBodyCentredNonRotatingNavigationFrame::Fill(In const& in, not_null<Message*> const message) {
  auto* m = message->mutable_in();
  m->set_plugin(SerializePointer(in.plugin));
  m->set_reference_body_index(in.reference_body_index);
}

void NewBodyCentredNonRotatingNavigationFrame::Fill(Return const& result, not_null<Message*> const message) {
  message->mutable_return_()->set_new_body_centred_non_rotating_navigation_frame(SerializePointer(result));
}

void NewBodyCentredNonRotatingNavigationFrame::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto plugin = DeserializePointer<Plugin const*>(*pointer_map, in.plugin());
  auto reference_body_index = in.reference_body_index();
  auto const result = ksp_plugin::principia__NewBodyCentredNonRotatingNavigationFrame(plugin, reference_body_index);
  Insert(pointer_map, message.return_().new_body_centred_non_rotating_navigation_frame(), result);
}

void NewPlugin::Fill(In const& in, not_null<Message*> const message) {
  auto* m = message->mutable_in();
  m->set_initial_time(in.initial_time);
  m->set_planetarium_rotation_in_degrees(in.planetarium_rotation_in_degrees);
}

void NewPlugin::Fill(Return const& result, not_null<Message*> const message) {
  message->mutable_return_()->set_new_plugin(SerializePointer(result));
}

void NewPlugin::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto initial_time = in.initial_time();
  auto planetarium_rotation_in_degrees = in.planetarium_rotation_in_degrees();
  auto const result = ksp_plugin::principia__NewPlugin(initial_time, planetarium_rotation_in_degrees);
  Insert(pointer_map, message.return_().new_plugin(), result);
}

void NumberOfSegments::Fill(In const& in, not_null<Message*> const message) {
  message->mutable_in()->set_line_and_iterator(SerializePointer(in.line_and_iterator));
}

void NumberOfSegments::Fill(Return const& result, not_null<Message*> const message) {
  message->mutable_return_()->set_number_of_segments(result);
}

void NumberOfSegments::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto line_and_iterator = DeserializePointer<LineAndIterator const*>(*pointer_map, in.line_and_iterator());
  auto const result = ksp_plugin::principia__NumberOfSegments(line_and_iterator);
  CHECK(message.return_().number_of_segments() == result);
}

void PhysicsBubbleIsEmpty::Fill(In const& in, not_null<Message*> const message) {
  message->mutable_in()->set_plugin(SerializePointer(in.plugin));
}

void PhysicsBubbleIsEmpty::Fill(Return const& result, not_null<Message*> const message) {
  message->mutable_return_()->set_physics_bubble_is_empty(result);
}

void PhysicsBubbleIsEmpty::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto plugin = DeserializePointer<Plugin const*>(*pointer_map, in.plugin());
  auto const result = ksp_plugin::principia__PhysicsBubbleIsEmpty(plugin);
  CHECK(message.return_().physics_bubble_is_empty() == result);
}

void RenderedFlightPlan::Fill(In const& in, not_null<Message*> const message) {
  auto* m = message->mutable_in();
  m->set_plugin(SerializePointer(in.plugin));
  m->set_vessel_guid(in.vessel_guid);
  m->set_plan_phase(in.plan_phase);
  *m->mutable_sun_world_position() = SerializeXYZ(in.sun_world_position);
}

void RenderedFlightPlan::Fill(Return const& result, not_null<Message*> const message) {
  message->mutable_return_()->set_rendered_flight_plan(SerializePointer(result));
}

void RenderedFlightPlan::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto plugin = DeserializePointer<Plugin*>(*pointer_map, in.plugin());
  auto vessel_guid = in.vessel_guid().c_str();
  auto plan_phase = in.plan_phase();
  auto sun_world_position = DeserializeXYZ(in.sun_world_position());
  auto const result = ksp_plugin::principia__RenderedFlightPlan(plugin, vessel_guid, plan_phase, sun_world_position);
  Insert(pointer_map, message.return_().rendered_flight_plan(), result);
}

void RenderedPrediction::Fill(In const& in, not_null<Message*> const message) {
  auto* m = message->mutable_in();
  m->set_plugin(SerializePointer(in.plugin));
  m->set_vessel_guid(in.vessel_guid);
  *m->mutable_sun_world_position() = SerializeXYZ(in.sun_world_position);
}

void RenderedPrediction::Fill(Return const& result, not_null<Message*> const message) {
  message->mutable_return_()->set_rendered_prediction(SerializePointer(result));
}

void RenderedPrediction::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto plugin = DeserializePointer<Plugin*>(*pointer_map, in.plugin());
  auto vessel_guid = in.vessel_guid().c_str();
  auto sun_world_position = DeserializeXYZ(in.sun_world_position());
  auto const result = ksp_plugin::principia__RenderedPrediction(plugin, vessel_guid, sun_world_position);
  Insert(pointer_map, message.return_().rendered_prediction(), result);
}

void RenderedVesselTrajectory::Fill(In const& in, not_null<Message*> const message) {
  auto* m = message->mutable_in();
  m->set_plugin(SerializePointer(in.plugin));
  m->set_vessel_guid(in.vessel_guid);
  *m->mutable_sun_world_position() = SerializeXYZ(in.sun_world_position);
}

void RenderedVesselTrajectory::Fill(Return const& result, not_null<Message*> const message) {
  message->mutable_return_()->set_rendered_vessel_trajectory(SerializePointer(result));
}

void RenderedVesselTrajectory::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto plugin = DeserializePointer<Plugin const*>(*pointer_map, in.plugin());
  auto vessel_guid = in.vessel_guid().c_str();
  auto sun_world_position = DeserializeXYZ(in.sun_world_position());
  auto const result = ksp_plugin::principia__RenderedVesselTrajectory(plugin, vessel_guid, sun_world_position);
  Insert(pointer_map, message.return_().rendered_vessel_trajectory(), result);
}

void SayHello::Fill(Return const& result, not_null<Message*> const message) {
  message->mutable_return_()->set_say_hello(result);
}

void SayHello::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const result = ksp_plugin::principia__SayHello();
  CHECK(message.return_().say_hello().c_str() == result);
}

void SerializePlugin::Fill(In const& in, not_null<Message*> const message) {
  auto* m = message->mutable_in();
  m->set_plugin(SerializePointer(in.plugin));
  m->set_serializer(SerializePointer(*in.serializer));
}

void SerializePlugin::Fill(Out const& out, not_null<Message*> const message) {
  message->mutable_out()->set_serializer(SerializePointer(*out.serializer));
}

void SerializePlugin::Fill(Return const& result, not_null<Message*> const message) {
  message->mutable_return_()->set_serialize_plugin(SerializePointer(result));
}

void SerializePlugin::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto plugin = DeserializePointer<Plugin const*>(*pointer_map, in.plugin());
  auto serializer = DeserializePointer<PullSerializer*>(*pointer_map, in.serializer());
  auto const result = ksp_plugin::principia__SerializePlugin(plugin, &serializer);
  if (result == nullptr) {
    Delete(pointer_map, in.serializer());
  }
  if (result != nullptr) {
    Insert(pointer_map, message.out().serializer(), serializer);
  }
  if (result != nullptr) {
    Insert(pointer_map, message.return_().serialize_plugin(), result);
  }
}

void SetBufferDuration::Fill(In const& in, not_null<Message*> const message) {
  message->mutable_in()->set_seconds(in.seconds);
}

void SetBufferDuration::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto seconds = in.seconds();
  ksp_plugin::principia__SetBufferDuration(seconds);
}

void SetBufferedLogging::Fill(In const& in, not_null<Message*> const message) {
  message->mutable_in()->set_max_severity(in.max_severity);
}

void SetBufferedLogging::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto max_severity = in.max_severity();
  ksp_plugin::principia__SetBufferedLogging(max_severity);
}

void SetPlottingFrame::Fill(In const& in, not_null<Message*> const message) {
  auto* m = message->mutable_in();
  m->set_plugin(SerializePointer(in.plugin));
  m->set_navigation_frame(SerializePointer(*in.navigation_frame));
}

void SetPlottingFrame::Fill(Out const& out, not_null<Message*> const message) {
  message->mutable_out()->set_navigation_frame(SerializePointer(*out.navigation_frame));
}

void SetPlottingFrame::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto plugin = DeserializePointer<Plugin*>(*pointer_map, in.plugin());
  auto navigation_frame = DeserializePointer<NavigationFrame*>(*pointer_map, in.navigation_frame());
  ksp_plugin::principia__SetPlottingFrame(plugin, &navigation_frame);
  Delete(pointer_map, in.navigation_frame());
}

void SetPredictionLength::Fill(In const& in, not_null<Message*> const message) {
  auto* m = message->mutable_in();
  m->set_plugin(SerializePointer(in.plugin));
  m->set_t(in.t);
}

void SetPredictionLength::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto plugin = DeserializePointer<Plugin*>(*pointer_map, in.plugin());
  auto t = in.t();
  ksp_plugin::principia__SetPredictionLength(plugin, t);
}

void SetPredictionLengthTolerance::Fill(In const& in, not_null<Message*> const message) {
  auto* m = message->mutable_in();
  m->set_plugin(SerializePointer(in.plugin));
  m->set_l(in.l);
}

void SetPredictionLengthTolerance::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto plugin = DeserializePointer<Plugin*>(*pointer_map, in.plugin());
  auto l = in.l();
  ksp_plugin::principia__SetPredictionLengthTolerance(plugin, l);
}

void SetPredictionSpeedTolerance::Fill(In const& in, not_null<Message*> const message) {
  auto* m = message->mutable_in();
  m->set_plugin(SerializePointer(in.plugin));
  m->set_v(in.v);
}

void SetPredictionSpeedTolerance::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto plugin = DeserializePointer<Plugin*>(*pointer_map, in.plugin());
  auto v = in.v();
  ksp_plugin::principia__SetPredictionSpeedTolerance(plugin, v);
}

void SetStderrLogging::Fill(In const& in, not_null<Message*> const message) {
  message->mutable_in()->set_min_severity(in.min_severity);
}

void SetStderrLogging::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto min_severity = in.min_severity();
  ksp_plugin::principia__SetStderrLogging(min_severity);
}

void SetSuppressedLogging::Fill(In const& in, not_null<Message*> const message) {
  message->mutable_in()->set_min_severity(in.min_severity);
}

void SetSuppressedLogging::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto min_severity = in.min_severity();
  ksp_plugin::principia__SetSuppressedLogging(min_severity);
}

void SetVerboseLogging::Fill(In const& in, not_null<Message*> const message) {
  message->mutable_in()->set_level(in.level);
}

void SetVerboseLogging::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto level = in.level();
  ksp_plugin::principia__SetVerboseLogging(level);
}

void SetVesselStateOffset::Fill(In const& in, not_null<Message*> const message) {
  auto* m = message->mutable_in();
  m->set_plugin(SerializePointer(in.plugin));
  m->set_vessel_guid(in.vessel_guid);
  *m->mutable_from_parent() = SerializeQP(in.from_parent);
}

void SetVesselStateOffset::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto plugin = DeserializePointer<Plugin*>(*pointer_map, in.plugin());
  auto vessel_guid = in.vessel_guid().c_str();
  auto from_parent = DeserializeQP(in.from_parent());
  ksp_plugin::principia__SetVesselStateOffset(plugin, vessel_guid, from_parent);
}

void UpdateCelestialHierarchy::Fill(In const& in, not_null<Message*> const message) {
  auto* m = message->mutable_in();
  m->set_plugin(SerializePointer(in.plugin));
  m->set_celestial_index(in.celestial_index);
  m->set_parent_index(in.parent_index);
}

void UpdateCelestialHierarchy::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto plugin = DeserializePointer<Plugin const*>(*pointer_map, in.plugin());
  auto celestial_index = in.celestial_index();
  auto parent_index = in.parent_index();
  ksp_plugin::principia__UpdateCelestialHierarchy(plugin, celestial_index, parent_index);
}

void UpdatePrediction::Fill(In const& in, not_null<Message*> const message) {
  auto* m = message->mutable_in();
  m->set_plugin(SerializePointer(in.plugin));
  m->set_vessel_guid(in.vessel_guid);
}

void UpdatePrediction::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto plugin = DeserializePointer<Plugin const*>(*pointer_map, in.plugin());
  auto vessel_guid = in.vessel_guid().c_str();
  ksp_plugin::principia__UpdatePrediction(plugin, vessel_guid);
}

void VesselBinormal::Fill(In const& in, not_null<Message*> const message) {
  auto* m = message->mutable_in();
  m->set_plugin(SerializePointer(in.plugin));
  m->set_vessel_guid(in.vessel_guid);
}

void VesselBinormal::Fill(Return const& result, not_null<Message*> const message) {
  *message->mutable_return_()->mutable_vessel_binormal() = SerializeXYZ(result);
}

void VesselBinormal::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto plugin = DeserializePointer<Plugin const*>(*pointer_map, in.plugin());
  auto vessel_guid = in.vessel_guid().c_str();
  auto const result = ksp_plugin::principia__VesselBinormal(plugin, vessel_guid);
  CHECK(DeserializeXYZ(message.return_().vessel_binormal()) == result);
}

void VesselFromParent::Fill(In const& in, not_null<Message*> const message) {
  auto* m = message->mutable_in();
  m->set_plugin(SerializePointer(in.plugin));
  m->set_vessel_guid(in.vessel_guid);
}

void VesselFromParent::Fill(Return const& result, not_null<Message*> const message) {
  *message->mutable_return_()->mutable_vessel_from_parent() = SerializeQP(result);
}

void VesselFromParent::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto plugin = DeserializePointer<Plugin const*>(*pointer_map, in.plugin());
  auto vessel_guid = in.vessel_guid().c_str();
  auto const result = ksp_plugin::principia__VesselFromParent(plugin, vessel_guid);
  CHECK(DeserializeQP(message.return_().vessel_from_parent()) == result);
}

void VesselNormal::Fill(In const& in, not_null<Message*> const message) {
  auto* m = message->mutable_in();
  m->set_plugin(SerializePointer(in.plugin));
  m->set_vessel_guid(in.vessel_guid);
}

void VesselNormal::Fill(Return const& result, not_null<Message*> const message) {
  *message->mutable_return_()->mutable_vessel_normal() = SerializeXYZ(result);
}

void VesselNormal::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto plugin = DeserializePointer<Plugin const*>(*pointer_map, in.plugin());
  auto vessel_guid = in.vessel_guid().c_str();
  auto const result = ksp_plugin::principia__VesselNormal(plugin, vessel_guid);
  CHECK(DeserializeXYZ(message.return_().vessel_normal()) == result);
}

void VesselTangent::Fill(In const& in, not_null<Message*> const message) {
  auto* m = message->mutable_in();
  m->set_plugin(SerializePointer(in.plugin));
  m->set_vessel_guid(in.vessel_guid);
}

void VesselTangent::Fill(Return const& result, not_null<Message*> const message) {
  *message->mutable_return_()->mutable_vessel_tangent() = SerializeXYZ(result);
}

void VesselTangent::Run(Message const& message, not_null<Player::PointerMap*> const pointer_map) {
  auto const& in = message.in();
  auto plugin = DeserializePointer<Plugin const*>(*pointer_map, in.plugin());
  auto vessel_guid = in.vessel_guid().c_str();
  auto const result = ksp_plugin::principia__VesselTangent(plugin, vessel_guid);
  CHECK(DeserializeXYZ(message.return_().vessel_tangent()) == result);
}

