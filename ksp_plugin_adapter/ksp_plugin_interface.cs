﻿using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace principia {
namespace ksp_plugin_adapter {

public partial class PrincipiaPluginAdapter : ScenarioModule {

#if __MonoCS__
  internal const string kDllPath = "GameData/Principia/principia.so";
#else
  internal const string kDllPath = "GameData/Principia/principia.dll";
#endif


  [StructLayout(LayoutKind.Sequential)]
  internal struct XYZ {
    public double x, y, z;
    public static explicit operator XYZ(Vector3d v) {
      return new XYZ{x = v.x, y = v.y, z = v.z};
    }
    public static explicit operator Vector3d(XYZ v) {
      return new Vector3d{x = v.x, y = v.y, z = v.z};
    }
  };

  [StructLayout(LayoutKind.Sequential)]
  internal struct WXYZ {
    public double w, x, y, z;
    public static explicit operator WXYZ(UnityEngine.QuaternionD q) {
      return new WXYZ{w = q.w, x = q.x, y = q.y, z = q.z};
    }
    public static explicit operator UnityEngine.QuaternionD(WXYZ q) {
      return new UnityEngine.QuaternionD{w = q.w, x = q.x, y = q.y, z = q.z};
    }
  };

  [StructLayout(LayoutKind.Sequential)]
  internal struct LineSegment {
    public XYZ begin, end;
  };

  [StructLayout(LayoutKind.Sequential)]
  internal struct QP {
    public XYZ q, p;
  };

  [StructLayout(LayoutKind.Sequential)]
  internal struct KSPPart {
    public XYZ world_position;
    public XYZ world_velocity;
    public double mass;
    public XYZ gravitational_acceleration_to_be_applied_by_ksp;
    public uint id;
  };

  // Plugin interface.

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__SayHello",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern IntPtr SayHello();

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__NewPlugin",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern IntPtr NewPlugin(
      double initial_time,
      double planetarium_rotation_in_degrees);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__DeletePlugin",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern void DeletePlugin(ref IntPtr plugin);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__DirectlyInsertMassiveCelestial",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern void DirectlyInsertMassiveCelestial(
      IntPtr plugin,
      int celestial_index,
      ref int parent_index,
      [MarshalAs(UnmanagedType.LPStr)] String  gravitational_parameter,
      [MarshalAs(UnmanagedType.LPStr)] String  x,
      [MarshalAs(UnmanagedType.LPStr)] String  y,
      [MarshalAs(UnmanagedType.LPStr)] String  z,
      [MarshalAs(UnmanagedType.LPStr)] String  vx,
      [MarshalAs(UnmanagedType.LPStr)] String  vy,
      [MarshalAs(UnmanagedType.LPStr)] String  vz);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__DirectlyInsertOblateCelestial",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern void DirectlyInsertOblateCelestial(
      IntPtr plugin,
      int celestial_index,
      ref int parent_index,
      [MarshalAs(UnmanagedType.LPStr)] String gravitational_parameter,
      [MarshalAs(UnmanagedType.LPStr)] String axis_right_ascension,
      [MarshalAs(UnmanagedType.LPStr)] String axis_declination,
      [MarshalAs(UnmanagedType.LPStr)] String j2,
      [MarshalAs(UnmanagedType.LPStr)] String reference_radius,
      [MarshalAs(UnmanagedType.LPStr)] String x,
      [MarshalAs(UnmanagedType.LPStr)] String y,
      [MarshalAs(UnmanagedType.LPStr)] String z,
      [MarshalAs(UnmanagedType.LPStr)] String vx,
      [MarshalAs(UnmanagedType.LPStr)] String vy,
      [MarshalAs(UnmanagedType.LPStr)] String vz);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__DirectlyInsertMassiveCelestial",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern void DirectlyInsertMassiveCelestial(
      IntPtr plugin,
      int celestial_index,
      IntPtr parent_index,
      [MarshalAs(UnmanagedType.LPStr)] String  gravitational_parameter,
      [MarshalAs(UnmanagedType.LPStr)] String  x,
      [MarshalAs(UnmanagedType.LPStr)] String  y,
      [MarshalAs(UnmanagedType.LPStr)] String  z,
      [MarshalAs(UnmanagedType.LPStr)] String  vx,
      [MarshalAs(UnmanagedType.LPStr)] String  vy,
      [MarshalAs(UnmanagedType.LPStr)] String  vz);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__DirectlyInsertOblateCelestial",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern void DirectlyInsertOblateCelestial(
      IntPtr plugin,
      int celestial_index,
      IntPtr parent_index,
      [MarshalAs(UnmanagedType.LPStr)] String gravitational_parameter,
      [MarshalAs(UnmanagedType.LPStr)] String axis_right_ascension,
      [MarshalAs(UnmanagedType.LPStr)] String axis_declination,
      [MarshalAs(UnmanagedType.LPStr)] String j2,
      [MarshalAs(UnmanagedType.LPStr)] String reference_radius,
      [MarshalAs(UnmanagedType.LPStr)] String x,
      [MarshalAs(UnmanagedType.LPStr)] String y,
      [MarshalAs(UnmanagedType.LPStr)] String z,
      [MarshalAs(UnmanagedType.LPStr)] String vx,
      [MarshalAs(UnmanagedType.LPStr)] String vy,
      [MarshalAs(UnmanagedType.LPStr)] String vz);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__InsertCelestial",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern void InsertCelestial(
      IntPtr plugin,
      int celestial_index,
      double gravitational_parameter,
      int parent_index,
      QP from_parent);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__InsertSun",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern void InsertSun(
      IntPtr plugin,
      int celestial_index,
      double gravitational_parameter);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__EndInitialization",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern void EndInitialization(IntPtr plugin);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__UpdateCelestialHierarchy",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern void UpdateCelestialHierarchy(
      IntPtr plugin,
      int celestial_index,
      int parent_index);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__InsertOrKeepVessel",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern bool InsertOrKeepVessel(
      IntPtr plugin,
      [MarshalAs(UnmanagedType.LPStr)] String vessel_guid,
      int parent_index);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__SetVesselStateOffset",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern void SetVesselStateOffset(
      IntPtr plugin,
      [MarshalAs(UnmanagedType.LPStr)] String vessel_guid,
      QP from_parent);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__AdvanceTime",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern void AdvanceTime(
      IntPtr plugin, 
      double t,
      double planetarium_rotation);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__ForgetAllHistoriesBefore",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern void ForgetAllHistoriesBefore(IntPtr plugin,
                                                      double t);

  [DllImport(dllName: kDllPath,
             EntryPoint        = "principia__VesselFromParent",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern QP VesselFromParent(
      IntPtr plugin,
      [MarshalAs(UnmanagedType.LPStr)] String vessel_guid);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__CelestialFromParent",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern QP CelestialFromParent(
      IntPtr plugin,
      int celestial_index);

  [DllImport(dllName           : kDllPath,
             EntryPoint        =
                 "principia__NewBodyCentredNonRotatingTransforms",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern IntPtr NewBodyCentredNonRotatingTransforms(
      IntPtr plugin,
      int reference_body_index);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__NewBarycentricRotatingTransforms",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern IntPtr NewBarycentricRotatingTransforms(
      IntPtr plugin,
      int primary_index,
      int secondary_index);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__DeleteTransforms",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern void DeleteTransforms(ref IntPtr transforms);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__ManœuvreCount",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern int ManœuvreCount(
      IntPtr plugin,
      [MarshalAs(UnmanagedType.LPStr)] String vessel_guid);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__VesselManœuvre",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern IntPtr VesselManœuvre(
      IntPtr plugin,
      [MarshalAs(UnmanagedType.LPStr)] String vessel_guid,
      int index);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__SetVesselManœuvre",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern void SetVesselManœuvre(
      IntPtr plugin,
      [MarshalAs(UnmanagedType.LPStr)] String vessel_guid,
      int index,
      ref IntPtr manœuvre);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__InsertVesselManœuvre",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern void InsertVesselManœuvre(
      IntPtr plugin,
      [MarshalAs(UnmanagedType.LPStr)] String vessel_guid,
      int index,
      ref IntPtr manœuvre);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__ClearVesselManœuvres",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern void ClearVesselManœuvres(
      IntPtr plugin,
      [MarshalAs(UnmanagedType.LPStr)] String vessel_guid);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__UpdatePrediction",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern void UpdatePrediction(
      IntPtr plugin,
      [MarshalAs(UnmanagedType.LPStr)] String vessel_guid);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__UpdateFlightPlan",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern void UpdateFlightPlan(
      IntPtr plugin,
      [MarshalAs(UnmanagedType.LPStr)] String vessel_guid,
      double last_time);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__RenderedVesselTrajectory",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern IntPtr RenderedVesselTrajectory(
      IntPtr plugin,
      [MarshalAs(UnmanagedType.LPStr)] String vessel_guid,
      IntPtr transforms,
      XYZ sun_world_position);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__HasPrediction",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern bool HasPrediction(
      IntPtr plugin,
      [MarshalAs(UnmanagedType.LPStr)] String vessel_guid);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__FlightPlanSize",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern int FlightPlanSize(
      IntPtr plugin,
      [MarshalAs(UnmanagedType.LPStr)] String vessel_guid);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__RenderedPrediction",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern IntPtr RenderedPrediction(
      IntPtr plugin,
      [MarshalAs(UnmanagedType.LPStr)] String vessel_guid,
      IntPtr transforms,
      XYZ sun_world_position);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__RenderedFlightPlan",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern IntPtr RenderedFlightPlan(
      IntPtr plugin,
      [MarshalAs(UnmanagedType.LPStr)] String vessel_guid,
      int plan_phase,
      IntPtr transforms,
      XYZ sun_world_position);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__NumberOfSegments",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern int NumberOfSegments(IntPtr line);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__FetchAndIncrement",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern LineSegment FetchAndIncrement(IntPtr line);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__AtEnd",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern bool AtEnd(IntPtr line);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__DeleteLineAndIterator",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern void DeleteLineAndIterator(ref IntPtr line);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__set_prediction_length",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern void set_prediction_length(IntPtr plugin, double t);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__set_prediction_length_tolerance",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern void set_prediction_length_tolerance(IntPtr plugin,
                                                             double t);
  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__set_prediction_speed_tolerance",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern void set_prediction_speed_tolerance(IntPtr plugin,
                                                            double t);

  [DllImport(dllName             : kDllPath,
             EntryPoint =        "principia__has_vessel",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern bool has_vessel(
      IntPtr plugin,
      [MarshalAs(UnmanagedType.LPStr)] String vessel_guid);

  [DllImport(dllName: kDllPath,
             EntryPoint        = "principia__AddVesselToNextPhysicsBubble",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern void AddVesselToNextPhysicsBubble(
      IntPtr plugin,
      [MarshalAs(UnmanagedType.LPStr)] String vessel_guid,
      KSPPart[] parts,
      int count);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__PhysicsBubbleIsEmpty",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern bool PhysicsBubbleIsEmpty(IntPtr plugin);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__BubbleDisplacementCorrection",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern XYZ BubbleDisplacementCorrection(IntPtr plugin,
                                                          XYZ sun_position);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__BubbleVelocityCorrection",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern XYZ BubbleVelocityCorrection(IntPtr plugin,
                                                      int reference_body_index);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__NavballOrientation",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern WXYZ NavballOrientation(
      IntPtr plugin,
      IntPtr transforms,
      XYZ sun_world_position,
      XYZ ship_world_position);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__VesselTangent",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern XYZ VesselTangent(
      IntPtr plugin,
      [MarshalAs(UnmanagedType.LPStr)] String vessel_guid,
      IntPtr transforms);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__current_time",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern double current_time(IntPtr plugin);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__SerializePlugin",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern IntPtr SerializePlugin(IntPtr plugin,
                                               ref IntPtr serializer);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__DeletePluginSerialization",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern void DeletePluginSerialization(
      ref IntPtr serialization);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__DeserializePlugin",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern void DeserializePlugin(
      [MarshalAs(UnmanagedType.LPStr)] String serialization,
      int serialization_size,
      ref IntPtr deserializer,
      ref IntPtr plugin);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__NewManœuvreIspByWeight",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern IntPtr NewManœuvreIspByWeight(
      double thrust,
      double initial_mass,
      double specific_impulse_by_weight,
      double right_ascension,
      double declination);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__set_duration",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern void set_duration(IntPtr manœuvre, double duration);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__set_Δv",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern void set_Δv(IntPtr manœuvre, double Δv);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__set_initial_time",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern IntPtr set_initial_time(IntPtr manœuvre,
                                                double initial_time);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__ManœuvreΔv",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern XYZ ManœuvreΔv(IntPtr plugin, IntPtr manœuvre);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__initial_time",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern double initial_time(IntPtr manœuvre);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__time_of_half_Δv",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern double time_of_half_Δv(IntPtr manœuvre);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__final_time",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern double final_time(IntPtr manœuvre);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__final_mass",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern double final_mass(IntPtr manœuvre);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__thrust",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern double thrust(IntPtr manœuvre);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__initial_mass",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern double initial_mass(IntPtr manœuvre);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__specific_impulse_by_weight",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern double specific_impulse_by_weight(IntPtr manœuvre);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__Δv",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern double Δv(IntPtr manœuvre);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__duration",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern double duration(IntPtr manœuvre);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__right_ascension",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern double right_ascension(IntPtr manœuvre);

  [DllImport(dllName           : kDllPath,
             EntryPoint        = "principia__declination",
             CallingConvention = CallingConvention.Cdecl)]
  internal static extern double declination(IntPtr manœuvre);
}

}  // namespace ksp_plugin_adapter
}  // namespace principia
