#ifndef PSCAMERA_DEFINE_H
#define PSCAMERA_DEFINE_H
#include <stdint.h>
#ifdef PS_EXPORT_ON
#ifdef _WIN32
#define PSCAMERA_API_EXPORT __declspec(dllexport)
#else
#define PSCAMERA_API_EXPORT __attribute__((visibility("default")))
#endif
#else
#ifdef _WIN32
#define PSCAMERA_API_EXPORT __declspec(dllimport)
#else
#define PSCAMERA_API_EXPORT __attribute__((visibility("default")))
#endif
#endif

#ifdef __cplusplus
#define PSCAMERA_C_API_EXPORT extern "C" PSCAMERA_API_EXPORT
#else
#define PSCAMERA_C_API_EXPORT PSCAMERA_API_EXPORT
#define bool uint8_t
#endif

#endif /* PSCAMERA_DEFINE_H */
