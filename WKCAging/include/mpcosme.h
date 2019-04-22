/*
 *
 * Copyright 2013-2014 by MotionPortrait, Inc.
 *
 * All rights reserved.
 *
 */

#ifndef MPCOSME_H_
#define MPCOSME_H_

#include "mptypes.h"

#ifdef WEBGL
#include <string>
#endif

namespace motionportrait {


class MpCosmeImpl;
class MpFace;

/**
 * \class MpCosme
 *
 * MpCosme class controls load/unload/set/unset cosme item
 */
class MpCosme {

  public:

#ifdef WEBGL
    typedef intptr_t CosmeId;
#else
    typedef void* CosmeId;
#endif

    /**
     * create cosme.
     *
     * @param pathCosme : path to cosme file
     * @return cosme id
     */
    CosmeId Create(const char *pathCosme);

    /**
     * destroy cosme.
     *
     * @param id : cosme id to be destroyed
     */
    mpResult Destroy(CosmeId id);

    /**
     * skin color type
     */
    typedef enum {
        SKIN_COLOR_TYPE_DEFAULT = 0,
        SKIN_COLOR_TYPE_FOREHEAD,
    } SkinColorType;

    /**
     * bind item to face
     * must be called from GL thread
     *
     * @param face  : face instance which cosme is bound
     * @param id    : cosme id to be bound
     * @param color : cosme color
     * @param skinColorType : skin color type
     */
#ifdef WEBGL
    mpResult SetCosme(MpFace &face, CosmeId id, mpColor &color);
#else
    mpResult SetCosme(MpFace &face, CosmeId id, mpColor *color = NULL, SkinColorType skinColorType = SKIN_COLOR_TYPE_DEFAULT);
#endif

    /**
     * unbind all items from face
     * must be called from GL thread
     *
     * @param face : face instance which cosme is bound
     */
    mpResult UnsetCosme(MpFace &face);


#ifdef WEBGL
    CosmeId Create(std::string pathCosme) {
        return this->Create(pathCosme.c_str());
    }
#endif

    /**
     */
    MpCosme();
    virtual ~MpCosme();

    MpCosmeImpl *mp_;

};


} // namespace motionportrait

#endif /* MPCOSME_H_ */
