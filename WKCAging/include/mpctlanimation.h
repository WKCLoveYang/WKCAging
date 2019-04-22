/*
 *
 * Copyright 2013-2014 by MotionPortrait, Inc.
 *
 * All rights reserved.
 *
 */

#ifndef MPCTLANIMATION_H_
#define MPCTLANIMATION_H_

#include "mptypes.h"

#ifdef WEBGL
#include <string>
#endif

namespace motionportrait {


class MpCtlAnimationImpl;

/**
 * \class MpCtlAnimation
 *
 * MpCtlAnimation class controls animation of an avatar
 */
class MpCtlAnimation {

  public:

    /**
     * update animation
     *
     * @param curTime : current time in milli second
     */
    mpResult Update(long curTime);

    /**
     * @name unconscious animation API
     * @{
     */

    /**
     * unconscious animation parameter types
     */
    typedef enum {
        NECK_X_ENABLE = 0,          /**< X axis neck rotation on/off */
        NECK_Y_ENABLE,              /**< Y axis neck rotation on/off */
        NECK_Z_ENABLE,              /**< Z axis neck rotation on/off */
        NECK_X_DURATION_FACTOR,     /**< X axis neck rotation duration factor. */
                                    /**< default is 1.0 */
        NECK_Y_DURATION_FACTOR,     /**< X axis neck rotation duration factor. */
                                    /**< default is 1.0 */
        NECK_Z_DURATION_FACTOR,     /**< X axis neck rotation duration factor. */
                                    /**< default is 1.0 */
        NECK_X_MAX_ROT,             /**< X axis neck rotation max angle in degree. */
                                    /**< default is 6.0 */
        NECK_Y_MAX_ROT,             /**< X axis neck rotation max angle in degree. */
                                    /**< default is 6.0 */
        NECK_Z_MAX_ROT,             /**< X axis neck rotation max angle in degree. */
                                    /**< default is 6.0 */
        BLINK_ENABLE,               /**< blink on/off */
        BLINK_DURATION_FACTOR,      /**< blink duration factor. default is 1.0 */
        BLINK_FREQS,                /**< blink frequency */
        BLINK_GAIN_FACTOR,          /**< blink gain factor. default is 1.0 */
        PUPIL_ENABLE,               /**< pupil movement on/off */
        PUPIL_DURATION_FACTOR,      /**< pupil movement duration factor */
        PUPIL_MOVE_GAIN_FACTOR,     /**< pupil movement gain factor. default is 1.0 */
        PUPIL_X_MAX,                /**< X axis maximum pupil movement */
        PUPIL_Y_MAX,                /**< Y axis maximum pupil movement */
        EXPR_ENABLE,                /**< not used */
        EXPR_DURATION_FACTOR,       /**< unconscious animation duration factor. */
                                    /**< default is 1.0 */
        EXPR_BREATH_DURATION_FACTOR,    /**< breath duration factor. */
                                    /**< default is 1.0 */
        EXPR_GAIN_FACTORS,          /**< unconscious animation gain factor. */
                                    /**< default is 1.0 */
        BREATH_ENABLE,              /**< breath animatino on/off */
    } Param;

    /**
     * set unconscious animation parameters - float
     * @param type  : parameter type.
     *                the following parameters can be set.
     *                NECK_X_DURATION_FACTOR,
     *                NECK_Y_DURATION_FACTOR,
     *                NECK_Z_DURATION_FACTOR,
     *                NECK_X_MAX_ROT,
     *                NECK_Y_MAX_ROT,
     *                NECK_Z_MAX_ROT,
     *                BLINK_DURATION_FACTOR,
     *                BLINK_GAIN_FACTOR,
     *                PUPIL_DURATION_FACTOR,
     *                PUPIL_X_MAX,
     *                PUPIL_Y_MAX,
     *                EXPR_DURATION_FACTOR,
     *                EXPR_BREATH_DURATION_FACTOR,
     * @param value : parameter value
     */
    mpResult SetParamf(MpCtlAnimation::Param type, float  value);

    /**
     * get unconscious animation parameters - float
     * @param type  : parameter type. same as SetParamf
     * @param value : parameter value
     */
    mpResult GetParamf(MpCtlAnimation::Param type, float &value);

    /**
     * set unconscious animation parameters - float vector
     * @param type  : parameter type.
     *                must be EXPR_GAIN_FACTORS.
     * @param value : parameter value
     */
    mpResult SetParamfv(MpCtlAnimation::Param type, float value[]);

    /**
     * get unconscious animation parameters - float vector
     * @param type  : parameter type. same as SetParamfv
     * @param value : parameter value
     */
    mpResult GetParamfv(MpCtlAnimation::Param type, float value[]);

    /**
     * set unconscious animation parameters - integer
     * @param type  : parameter type.
     *                the following parameters can be set.
     *                NECK_X_ENABLE,
     *                NECK_Y_ENABLE,
     *                NECK_Z_ENABLE,
     *                EXPR_ENABLE,
     *                BREATH_ENABLE,
     *                BLINK_ENABLE,
     *                PUPIL_ENABLE,
     * @param value : parameter value
     */
    mpResult SetParami(MpCtlAnimation::Param type, int  value);

    /**
     * get unconscious animation parameters - integer
     * @param type  : parameter type. same as SetParami
     * @param value : parameter value
     */
    mpResult GetParami(MpCtlAnimation::Param type, int &value);

    /**
     * set unconscious animation parameters - integer vector
     * @param type  : parameter type.
     *                must be BLINK_FREQS.
     * @param value : parameter value.
     *                for BLINK_FREQS, value is an array of 3 float.
     *                frequency of blink type is controlled as follows.
     *                single blink = value[0] / (sum of value[0,1,2]
     *                half   blink = value[1] / (sum of value[0,1,2]
     *                double blink = value[2] / (sum of value[0,1,2]
     */
    mpResult SetParamiv(MpCtlAnimation::Param type, int value[]);

    /**
     * get unconscious animation parameters - integer vector
     * @param type  : parameter type. same as SetParamiv
     * @param value : parameter value
     */
    mpResult GetParamiv(MpCtlAnimation::Param type, int value[]);

    /**
     * set unconscious gain
     *
     * @param gain : unconscious gain as [0.0, 1.0]
     */
    mpResult SetUnconsciousGain(float gain);

    /** @}  - unconscious animation API */

    /**
     * @name conscious animation API
     * @{
     */

    /**
     * shift face
     * @param msec   : duration of animation in milli second
     * @param trans  : translation vector
     * @param weight : weight of blending with unconscious animation
     */
    mpResult Shift(int msec, mpTranslation &trans, float weight);

    /**
     * lean face
     * @param msec   : duration of animation in milli second
     * @param rotate : tilt angle in degree
     * @param weight : weight of blending with unconscious animation
     */
    mpResult Lean(int msec, mpRotation &rotate, float weight);

    /**
     * turn gaze and tilt face
     * @param msec     : duration of animation in milli second
     * @param position : position to turn gaze at [0.0, 1.0]
     * @param weight   : weight of blending with unconscious animation
     */
    mpResult LookAt(int msec, mpVector2 &position, float weight);

    /**
     * close eye
     * @param msec   : duration of animation in milli second
     * @param close  : eye closing ration. 0.0=open, 1.0=close
     */
    mpResult CloseEye(int msec, float close);

    typedef enum {
        BLINK_TYPE_SINGLE = 0,
        BLINK_TYPE_HALF,
        BLINK_TYPE_DOUBLE,
    } BlinkType;
    /**
     * blink
     * note:
     *   when you use this API, unconscious blink animation should be
     *   disabled by SetParami() to avoid unexpected result.
     * @param type : blink type
     * @param gain : gain of blink. [0.0, 1.0]
     */
    mpResult Blink(MpCtlAnimation::BlinkType type, float gain);

    /**
     * set data set of facial expression
     *
     * @param pathExprData : path to facial expression data file.
     *                       this file can be generated by MPExpression tool.
     */
    mpResult SetExprData(const char *pathExprData);

    /**
     * restore data set of facial expression.
     * the previous expression data before SetExprData is restored.
     */
    mpResult RestoreExprData();

    /**
     * control facial expression
     *
     * @param msec   : duration of animation in milli second
     * @param gain   : array of facial expression gains
     * @param weight : weight of blending with unconscious animation
     */
    mpResult Express(int msec, float gain[], float weight);
    mpResult ExpressBySlot(int slot, int msec, float gain, float weight);

    /** @}  - animation data API */

    /**
     * @name animation data API
     * @{
     */
#ifdef WEBGL
    typedef intptr_t AnimDataId;
#else
    typedef void* AnimDataId;
#endif

    /**
     * create animation data
     *
     * @param pathAnimData : path to animation data file.
     *                       animation data file can be generated by
     *                       MPAnimator tool
     * @return animation data id
     */
    AnimDataId CreateAnimation(const char *pathAnimData);

    /**
     * destroy animation data
     *
     * @param id : animation data id to be destroyed
     */
    mpResult DestroyAnimation(AnimDataId id);

    /**
     * play animation data
     *
     * @param startTime : start time of animation in milli second
     * @param curTime   : current time in milli second
     * @param dataId    : animation data id to be played
     * @param blendEnable : enable unconscious animation blending
     * @return  1 = playing  0 = finished
     */
    int AnimateData(long startTime, long curTime, AnimDataId dataId, bool blendEnable = true);

    /** @}  - conscious animation API */

    /**
     * @name direct animation API
     * @{
     */

    typedef enum {
        SIDE_RIGHT = 0,
        SIDE_LEFT,
    } Side;

    void GetEyePos(Side side, mpVector2 &position);
    void GetPupilPos(Side side, mpVector2 &position);
    void SetPupilPos(Side side, mpVector2 &position);
    void GetNeckRot(mpRotation &rotation);
    void SetNeckRot(mpRotation &rotation);
    void GetNeckTransl(mpTranslation &translation);
    void SetNeckTransl(mpTranslation &translation);
    void SetEyeClose(MpCtlAnimation::Side side, float close);
    float GetEyeClose(MpCtlAnimation::Side side);

    // for WebGL
    float GetEyePosX(Side side);
    float GetEyePosY(Side side);
    float GetPupilPosX(Side side);
    float GetPupilPosY(Side side);
    float GetNeckRotX();
    float GetNeckRotY();
    float GetNeckRotZ();
    float GetNeckTranslX();
    float GetNeckTranslY();

    /** @}  - direct animation API */

#ifdef WEBGL
    float GetParamf(MpCtlAnimation::Param type) {
        float value = 0.0f;
        this->GetParamf(type, value);
        return value;
    }
    int GetParami(MpCtlAnimation::Param type) {
        int value = 0.0f;
        this->GetParami(type, value);
        return value;
    }
    mpResult SetParamiv(MpCtlAnimation::Param type, intptr_t value) {
        return this->SetParamiv(type, reinterpret_cast<int*>(value));
    }
    mpResult SetExprData(std::string pathExprData) {
        return this->SetExprData(pathExprData.c_str());
    }
    mpResult Express(int msec, intptr_t gain, float weight) {
        return this->Express(msec, reinterpret_cast<float*>(gain), weight);
    }
    AnimDataId CreateAnimation(std::string pathAnimData) {
        return this->CreateAnimation(pathAnimData.c_str());
    }
#endif


    /**
     */
    MpCtlAnimation();
    virtual ~MpCtlAnimation();

    MpCtlAnimationImpl *mp_;
};


} // namespace motionportrait

#endif /* MPCTLANIMATION_H_ */
