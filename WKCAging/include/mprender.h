/*
 *
 * Copyright 2013-2014 by MotionPortrait, Inc.
 *
 * All rights reserved.
 *
 */

#ifndef MPRENDER_H_
#define MPRENDER_H_


#include "mptypes.h"

#ifdef WEBGL
#include <string>
#endif

namespace motionportrait {


class MpRenderImpl;
class MpFace;

/**
 * \class MpRender
 *
 * MpRender draws an avatar onto OpenGL screen.
 */
class MpRender {

  public:

    /**
     * get version number
     *
     * @return : version number
     */
    static const char* GetVersion();

    /**
     * @name Context
     * rendering context
     * @{
     */
    typedef struct {
        int faceMeshDiv;        /**< number of division of face mesh */
        int controlFlag;        /**< bit flag by FLAG_XXX */
    } Context;

    /** flag for motion blur of blinking */
    static const int FLAG_BLUR_EYELID = (1 << 0);
    /** set 1.0 to alpha buffer */
    static const int FLAG_ALPHA_FILL  = (1 << 1);
    /** set 1.0 to stencil buffer */
    static const int FLAG_STENCIL_FILL  = (1 << 2);
    /** @} */

    /**
     * Initialize
     *
     * @param ctxt           : rendering settings.
     *                         if NULL is set, default value is used
     */
#ifdef WEBGL
    mpResult Init();
    mpResult InitWithParam(int meshDiv, int controlFlag);
#else
    mpResult Init(MpRender::Context *ctxt = NULL);
#endif

    /**
     * set commonparts
     *
     * @param commonPartsDir : directory path including common parts.
     *                          if NULL, use default comparts
     *                          if "", use empty comparts
     */
    mpResult SetResourcePath(const char *commonPartsDir);

    /**
     * set face
     *
     * @param face : MpFace instance to be rendered
     */
    mpResult SetFace(MpFace *face);

    /**
     * set viewport
     *
     * @param viewport : viewport information
     */
    mpResult SetViewport(mpRect &viewport);

    /**
     * set Z rotation
     *
     * @param rotateZ : Z rotation
     */
    mpResult SetRotateZ(float rotateZ);

    /**
     * switch background draw
     *
     * @param enable : if ture is set, the background of the face is drawn
     */
    mpResult EnableDrawBackground(bool enable);

    /**
     * draw avatar.
     * this function must be called from GL thread
     *
     */
    mpResult Draw();

    /**
     * @name Ortho
     * API to set custom ortho view matrix
     * @{
     */
    int EnableCustomOrtho(bool enable);
    int SetCustomOrtho(float left, float right, float bottom, float top, float znear, float zfar);
    /**
     * Set Depth Test Mode
     *
     * @param enable         : depth test mode
     *                         true: enable, false: disable
     */
    mpResult SetDepthTestMode(bool enable);

    /** @} */

#ifdef WEBGL
    mpResult SetResourcePath(std::string commonPartsDir) {
        return this->SetResourcePath(commonPartsDir.c_str());
    }
#endif

    /**
     */
    MpRender();
    virtual ~MpRender();

    /** */
    MpRenderImpl *mp_;
};


} // namespace motionportrait

#endif /* MPRENDER_H_ */
