/*
 *
 * Copyright 2013-2014 by MotionPortrait, Inc.
 *
 * All rights reserved.
 *
 */

#ifndef MPSYNTH_H_
#define MPSYNTH_H_

#include "mptypes.h"

namespace motionportrait {


class MpSynthImpl;
class faceBinMem;

/**
 * \class MpSynth
 *
 * MpSynth class synthesizes an avatar
 */
class MpSynth {

  public:

    /**
     * Initialize
     *
     * @param pathRes : path to resource directory
     * @param secure  : secure mode. default is false.
     */
    mpResult Init(const char *pathRes, bool secure = false);

    /**
     * data structure for input image
     */
    typedef struct {
        int w;                  /**< width */
        int h;                  /**< height */
        unsigned char *rgb;     /**< RGB data */
        unsigned char *alpha;   /**< alpha data */
    } Img;

    /**
     * synthesize an avatar
     *
     * @param inImg   : input image
     * @param pathOut : path to output file/directory
     */
    mpResult Synth(MpSynth::Img &inImg, const char *pathOut);

    /**
     * synthesize an avatar
     * Output format should be FORMAT_BIN
     *
     * @param inImg   : input image
     * @param pObject : [out]generated face object. This object should be removed by DestroyFaceBin() when it is no longer used.
     */
    mpResult Synth(MpSynth::Img &inImg, mpFaceObject* pObject);
    
    /**
     * converted face object
     *
     * @param pathFace  : path to face file
     * @param pObject   : [out]generated face object. This object should be removed by DestroyFaceBin() when it is no longer used.
     */
    static mpResult FaceFile2FaceBin(const char *pathFace, mpFaceObject* pObject);
    
    /**
     * Destroies the faceBinMem pointer retrieved from Synth(). faceObject will be invalid after calling this method.
     *
     * @param faceObject Object to be destroyed
     */
    static void DestroyFaceBin(mpFaceObject faceObject);

    /**
     * Load an mpMkoObject from a file.
     *
     * @param path     : path to mko file
     * @param pObject  : [out]generated mko object
     */
    static mpResult Load(const char* path, mpMkoObject* pObject);

    /**
     * Save an mpMkoObject to a file.
     *
     * @param path     : path to mko file
     * @param object   : mko object to be saved
     */
    static mpResult Save(const char* path, mpMkoObject object);

    /**
     * detection to an image
     *
     * @param inImg   : input image
     */
    mpResult Detect(MpSynth::Img &inImg);

    /**
     * makeover to an image
     *
     * @param inImg        : input image
     * @param templateFile : path to template file for MakeOver
     * @param pathOut      : path to output image file
     */
    mpResult MkOvr(MpSynth::Img &inImg,
                const char *templateFile, const char *pathOut);

    /**
     * makeover to an avatar
     *
     * @param inImg        : input image
     * @param templateFile : path to template file for MakeOver
     * @param pathOut      : path to output avatar file
     */
    mpResult MkOvrSynth(MpSynth::Img &inImg,
                const char *templateFile, const char *pathOut);

    /**
     * makeover to an avatar
     * Output format should be FORMAT_BIN
     *
     * @param inImg        : input image
     * @param templateFile : path to template file for MakeOver
     * @param pObject      : [out]generated face object. This object should be removed by DestroyFaceBin() when it is no longer used.
     */
    mpResult MkOvrSynth(MpSynth::Img &inImg,
                const char *templateFile, mpFaceObject *pObject);

    /**
     * makeover to an avatar
     * Output format should be FORMAT_BIN
     *
     * @param inImg        : input image
     * @param mko          : mko object for MakeOver
     * @param pObject      : [out]generated face object. This object should be removed by DestroyFaceBin() when it is no longer used.
     */
    mpResult MkOvrSynth(MpSynth::Img &inImg,
                mpMkoObject mko, mpFaceObject *pObject);

    /**
     * generate makeover template
     *
     * @param inImg       : input image
     * @param pathOut     : path to output template file
     */
    mpResult MkOvrTemplate(MpSynth::Img &inImg, const char *pathOut);

    /**
     * generate makeover template for 2D image
     *
     * @param inImg       : input image
     * @param pathOut     : path to output template file
     */
    mpResult MkOvrTemplate2D(MpSynth::Img &inImg, const char *pathOut);

    /**
     * generate aging mask
     *
     * @param inImg       : input image
     * @param skinDir     : path to skin directory for aging mask
     * @param pathOut     : path to output aging mask files
     */
    mpResult GenAgingMask(MpSynth::Img &inImg,
                const char *skinDir, const char *pathOut);

    /**
     * output format type
     */
    typedef enum {
        FORMAT_BIN = 4,     /**< for PC/Android/iOS */
        FORMAT_SWF = 2,     /**< for Flash */
        FORMAT_MESHED_BIN = 5,     /**< meshed bin */
    } OutFormat;

    /**
     * avatar model type
     */
    typedef enum {
        MODEL_TYPE_ANIME = 0,     /**< anime type avatar */
        MODEL_TYPE_PHOTO = 1,     /**< photo type avatar */
    } ModelType;

    /**
     * image direction of face detection
     */
    typedef enum {
        DETECT_DIR_ALL   = 0,     /**< all direction */
        DETECT_DIR_UP    = 1,     /**< normal direction */
        DETECT_DIR_DOWN  = 2,     /**< upside down */
        DETECT_DIR_LEFT  = 3,     /**< upside left */
        DETECT_DIR_RIGHT = 4,     /**< upside right */
    } DetectDirection;

    /**
     * parameter setting id
     */
    typedef enum {
        EXPAND_LEVEL = 0,       /**< blur area size of outside of avatar. */
                                /**< default is 16 */
        BLUR_LEVEL,             /**< blur level of outside area of avatar. */
                                /**< default is 64 */
        ORIGINAL_EYE,           /**< 0:use CG eye, 1:use photo's original eye */
                                /**< default is 1 */
        TEX_SIZE,               /**< texture resolution. */
                                /**< must be 4096, 2048, 1024, 512(default), or 256 */
        MODEL_SIZE,             /**< model resolution. */
                                /**< must be 4096, 2048, 1024, 512(default), or 256 */
        EYELID_EQUAL_TEX_SIZE,  /**< eyelid texture size. */
                                /**< 0:use model size, 1:use texuture size */
        LOCAL_FILTER,           /**< use smooth filter or not. */
                                /**< default is 1(=Yes) */
        FACE_SIZE,              /**< vertical face ratio in screen. */
                                /**< default 0.35 */
        FACE_POS,               /**< vertical face position in screen. */
                                /**< default 0.4. */
                                /**< if negative value is set, MpSynth */
                                /**< does not adjust face size/position */
                                /**< but use size/position of photo's face */
        BASE_EXPRESSION,        /**< path to facial expression file. */
                                /**< if not specified, default expression */
                                /**< is used */
        OUTPUT_FORMAT,          /**< BIN(default) or SWF format */
                                /**< default is BIN */
        BACKGROUND,             /**< fill background area as 1.0 for SWF */
                                /**< default is 1(=Yes) */
        MODELTYPE,              /**< avatar model type. */
                                /**< ANIME or PHOTO(default) */
        HAIRIMAGE,              /**< hair image for SWF format*/
        CROP_MARGIN,            /**< if 1 is set, crop the garbage are which */
                                /**< is caused by adjust ment with FACE_SIZE */
                                /**< and FACE_POS */
        ANCHOR_BORDER,          /**< if set to zero(default), the border of the view might move according to the amount of the animation. */
                                /**< Otherwise, it does not move */
        DETECT_DIRECTION,       /**< image direction of face detection */
                                /**< default is DETECT_DIR_ALL */
        FILL_MARGIN,            /**< specify how to fill margin of image */
                                /**< 0: use edge color(default), 1: specified color */
        MARGIN_COLOR,           /**< specify fill margin color(float rgba[4] */
        AUTO_EXPOSURE,          /**< use auto exposure */
                                /**< 0: disable auto exposure , 1: enable auto exposure(default) */
        NO_LIP,                 /**< don't generate lip mesh */
        FILL_BACK_ALPHA,        /**< fill the alpha component of background */
    } Param;

    /**
     * set synthesis parameter - integer value
     *
     * @param id  : parameter id
     * @param val : parameter value
     */
    mpResult SetParami(MpSynth::Param id, int val);

    /**
     * set synthesis parameter - float value
     *
     * @param id  : parameter id
     * @param val : parameter value
     */
    mpResult SetParamf(MpSynth::Param id, float val);

    /**
     * set synthesis parameter - string value
     *
     * @param id  : parameter id
     * @param val : parameter value
     */
    mpResult SetParams(MpSynth::Param id, const char *val);

    /**
     * set synthesis parameter - float array value
     *
     * @param id  : parameter id
     * @param val : parameter value
     */
    mpResult SetParamfv(MpSynth::Param id, const float *val);

    /**
     * MP feature point class
     */
    class Mpfp {
      public:
        Mpfp();
        ~Mpfp();
        void CopyFrom(Mpfp &mpfp);
        void *data_;
    };

    /**
     * set feature points
     *
     * @param mpfp : MP feature points
     */
    mpResult SetMpfp(Mpfp &mpfp);

    /**
     * get feature points
     *
     * @param mpfp : MP feature points
     */
    mpResult GetMpfp(Mpfp &mpfp);

    /**
     */
    MpSynth();
    virtual ~MpSynth();

    /**
     * Stores a face object retrieved from Synth() as a file.
     * Stored file can be passed the MpFace::Load() to create a 3D avatar.
     *
     * @param faceObject Face object to store
     * @param path Destination file path.
     * @return 0, if succeeded.
     */
    static int StoreFaceBin(mpFaceObject faceObject, const char* path);

    /**
     * get feature points from FORMAT_BIN file
     *
     * @param fname      : path of FORMAT_BIN file
     * @param mpfp       : MP feature points
     */
    static mpResult GetMpfp(const char *fname, MpSynth::Mpfp &mpfp);

    /**
     * get feature points from mpFaceObject
     *
     * @param faceObject : Face object
     * @param mpfp       : MP feature points
     */
    static mpResult GetMpfp(mpFaceObject faceObject, MpSynth::Mpfp &mpfp);

  private:
    MpSynthImpl *mp_;
};


} // namespace motionportrait

#endif /* MPSYNTH_H_ */
