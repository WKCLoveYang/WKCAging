/**
 * Copyright 2013-2014 by MotionPortrait, Inc.
 * All rights reserved.
 *
 * mp_types.h
 * MP types definition
 */

#ifndef MPTYPES_HEADER
#define MPTYPES_HEADER

/**
 * \defgroup mp_types
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

namespace motionportrait {


//-----------------------------------------------------------------
// NULL pointer
//-----------------------------------------------------------------
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void*)0)
#endif /* __cplusplus */
#endif /* NULL */


//-----------------------------------------------------------------
// Function return status
//-----------------------------------------------------------------
/**
 * Function return status
 * \ref \see ErrorCode
 */
typedef int  mpResult;
typedef int  mpErrorCode;  // used by mpGetError()

/**
 * @name ErrorCode
 * @{
 */
#define MP_SUCCESS   0
#define MP_NO_ERROR  0
#define MP_ERROR_OTHERS            (-1)
#define MP_ERROR_INVALID_PARAM     (-2)
#define MP_ERROR_INVALID_MODEL     (-3)
#define MP_ERROR_INVALID_STATE     (-4)
#define MP_ERROR_OUT_OF_MEMORY     (-5)
#define MP_ERROR_IO                (-6)
#define MP_ERROR_BAD_ACCESS        (-7)
#define MP_ERROR_TIMEOUT           (-8)
#define MP_ERROR_AUTH_FAILED       (-9)
#define MP_ERROR_FACE_NOT_FOUND    (-10)
#define MP_ERROR_LICENSE           (-1001)
/** @} */


//-----------------------------------------------------------------
// MP specific types
// vector, rect, translation, rotation, color, scale
//-----------------------------------------------------------------
typedef struct {
    float x;
    float y;
} mpVector2;

typedef struct {
    float x;
    float y;
    float z;
} mpVector3;

typedef struct {
    int x;
    int y;
    int width;
    int height;
} mpRect;

typedef struct {
    float x;
    float y;
    float z;
} mpRotation;

typedef struct {
    float x;
    float y;
    float z;
} mpTranslation;

typedef struct {
    float x;
    float y;
    float z;
} mpScaling;

typedef struct {
    float r;
    float g;
    float b;
    float a;
} mpColor;

typedef void* mpFaceObject;
typedef void* mpMkoObject;

} // namespace motionportrait

#ifdef __cplusplus
}
#endif

#endif /* MPTYPES_HEADER */
/** @} */
