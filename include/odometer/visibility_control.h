#ifndef ODOMETER__VISIBILITY_CONTROL_H_
#define ODOMETER__VISIBILITY_CONTROL_H_
#if defined _WIN32 || defined __CYGWIN__
  #ifdef __GNUC__
    #define ODOMETER_EXPORT __attribute__ ((dllexport))
    #define ODOMETER_IMPORT __attribute__ ((dllimport))
  #else
    #define ODOMETER_EXPORT __declspec(dllexport)
    #define ODOMETER_IMPORT __declspec(dllimport)
  #endif
  #ifdef ODOMETER_BUILDING_LIBRARY
    #define ODOMETER_PUBLIC ODOMETER_EXPORT
  #else
    #define ODOMETER_PUBLIC ODOMETER_IMPORT
  #endif
  #define ODOMETER_PUBLIC_TYPE ODOMETER_PUBLIC
  #define ODOMETER_LOCAL
#else
  #define ODOMETER_EXPORT __attribute__ ((visibility("default")))
  #define ODOMETER_IMPORT
  #if __GNUC__ >= 4
    #define ODOMETER_PUBLIC __attribute__ ((visibility("default")))
    #define ODOMETER_LOCAL  __attribute__ ((visibility("hidden")))
  #else
    #define ODOMETER_PUBLIC
    #define ODOMETER_LOCAL
  #endif
  #define ODOMETER_PUBLIC_TYPE
#endif
#endif  // ODOMETER__VISIBILITY_CONTROL_H_
// Generated 06-Nov-2023 21:54:39
// Copyright 2019-2020 The MathWorks, Inc.
