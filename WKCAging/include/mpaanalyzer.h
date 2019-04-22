/*
 * Copyright 2014 by MotionPortrait, Inc.
 *
 * All rights reserved.
 *
 */

#ifndef MPAANALYZER_HEADER
#define MPAANALYZER_HEADER

#include "mptypes.h"

namespace motionportrait {

class MpaAnalyzerImpl;
class MpaAnalysisResult;
class MpaRecogResult;
class MpaImage;

/**
 * \class MpaAnalyzer
 */
class MpaAnalyzer {

  public:

    MpaAnalyzer();
    virtual ~MpaAnalyzer();

    /**
     * Initialize
     *
     * @param pathRes : path to resource data
     */
    int Init(const char *pathRes);

    /**
     * Sets input image to analyze,
     *
     * Copy of the image data will be created and you can safely delete \a img after calling setImage().
     *
     * @param img    : input image data
     */
    int SetImage(const MpaImage &img);

    /**
     * Recognize - same function as MpaRecognizer::Recognize
     *
     * @param result : output recognition result
     */
    int Recognize(MpaRecogResult &result);

    /**
     * Analyze
     *
     * @param result : output analysis result
     */
    int Analyze(MpaAnalysisResult &result);

    /** parameter for EnableDetection() */
    struct MpaAnalysisSwitch {
        bool enableBasicParts;
        bool enableBasicFace;
        bool enableEye;
        bool enableIris;
        bool enableEyebrow;
        bool enableNose;
        bool enableMouth;
        bool enableContour;

        // all flags are on at first
        MpaAnalysisSwitch() :
            enableBasicParts(true),
            enableBasicFace(true),
            enableEye(true),
            enableIris(true),
            enableEyebrow(true),
            enableNose(true),
            enableMouth(true),
            enableContour(true)
        {}
    };

    /**
     * Enable/Disable detection
     *
     * @param on : enable/disable switch for each category
     */
    int EnableDetection(const MpaAnalysisSwitch &on);

protected:
    MpaAnalyzerImpl* impl_;
};


enum MPA_IMAGE_FORMAT {
    MPA_IMAGE_GRAYSCALE = 1,
    MPA_IMAGE_RGBA = 4,
    MPA_IMAGE_RGB = 3,
    MPA_IMAGE_INVALID_FORMAT = -1
};

/**
 * \class MpaImage
 */
class MpaImage {
  public:
    MpaImage()
    : width(0), height(0), format(MPA_IMAGE_INVALID_FORMAT), bytesPerComponent(0), bytesPerRow(0), data(NULL) {}
    MpaImage(int w, int h, MPA_IMAGE_FORMAT f, int bpc, int bpr, void* p)
    : width(w), height(h), format(f), bytesPerComponent(bpc), bytesPerRow(bpr), data(p) {}
    int width;
    int height;
    MPA_IMAGE_FORMAT format;
    int bytesPerComponent;
    int bytesPerRow;
    void* data; //Pixels are bottom left first
    //! Copies attributes except for MpaImage::data
    void SafeCopy(const MpaImage& src) {
        width = src.width;
        height = src.height;
        format = src.format;
        bytesPerComponent = src.bytesPerComponent;
        bytesPerRow = src.bytesPerRow;
    }
private:
    MpaImage(const MpaImage& src) {
      (void)src;
        // DO NOT USE COPY CONSTRUCTOR
    }
    MpaImage& operator=(const MpaImage& src) {
      (void)src;
        // DO NOT USE COPY OPERATOR
        return *this;
    }
};

/**
 * \class MpaRecogResult
 */
class MpaRecogResult {

  public:

    enum eFeaturePoint {
        FP_BASICPARTS_EYE_LEFT = 0,
        FP_BASICPARTS_EYE_RIGHT = 1,
        FP_BASICPARTS_EYEBROW_LEFT = 2,
        FP_BASICPARTS_EYEBROW_RIGHT = 3,
        FP_BASICPARTS_NOSE_BOTTOM = 4,
        FP_BASICPARTS_MOUTH = 5,
        FP_BASICFACE_TOP = 6,
        FP_BASICFACE_BOTTOM = 7,
        FP_BASICFACE_LEFT = 8,
        FP_BASICFACE_RIGHT = 9,
        FP_EYE_LEFT_TOP = 10,
        FP_EYE_LEFT_BOTTOM = 11,
        FP_EYE_LEFT_INSIDE = 12,
        FP_EYE_LEFT_OUTSIDE = 13,
        FP_EYE_RIGHT_TOP = 14,
        FP_EYE_RIGHT_BOTTOM = 15,
        FP_EYE_RIGHT_INSIDE = 16,
        FP_EYE_RIGHT_OUTSIDE = 17,
        FP_IRIS_LEFT_INSIDE = 18,
        FP_IRIS_LEFT_OUTSIDE = 19,
        FP_IRIS_RIGHT_INSIDE = 20,
        FP_IRIS_RIGHT_OUTSIDE = 21,
        FP_EYEBROW_LEFT_UP_INSIDE = 22,
        FP_EYEBROW_LEFT_UP_CENTER = 23,
        FP_EYEBROW_LEFT_UP_OUTSIDE = 24,
        FP_EYEBROW_LEFT_LOW_INSIDE = 25,
        FP_EYEBROW_LEFT_LOW_CENTER = 26,
        FP_EYEBROW_LEFT_LOW_OUTSIDE = 27,
        FP_EYEBROW_RIGHT_UP_INSIDE = 28,
        FP_EYEBROW_RIGHT_UP_CENTER = 29,
        FP_EYEBROW_RIGHT_UP_OUTSIDE = 30,
        FP_EYEBROW_RIGHT_LOW_INSIDE = 31,
        FP_EYEBROW_RIGHT_LOW_CENTER = 32,
        FP_EYEBROW_RIGHT_LOW_OUTSIDE = 33,
        FP_NOSE_LEFT = 34,
        FP_NOSE_RIGHT = 35,
        FP_NOSE_NOSTRIL_LEFT = 36,
        FP_NOSE_NOSTRIL_RIGHT = 37,
        FP_NOSE_CAVE_LEFT = 38,
        FP_NOSE_CAVE_RIGHT = 39,
        FP_MOUTH_LEFT = 40,
        FP_MOUTH_RIGHT = 41,
        FP_MOUTH_UPPER_LIP_INSIDE = 42,
        FP_MOUTH_UPPER_LIP_OUTSIDE = 43,
        FP_MOUTH_LOWER_LIP_INSIDE = 44,
        FP_MOUTH_LOWER_LIP_OUTSIDE = 45,
        FP_CONTOUR_EYE_LEFT = 46,
        FP_CONTOUR_EYE_RIGHT = 47,
        FP_CONTOUR_CHEEK_LEFT = 48,
        FP_CONTOUR_CHEEK_RIGHT = 49,
        FP_CONTOUR_MOUTH_LEFT = 50,
        FP_CONTOUR_MOUTH_RIGHT = 51,
        FP_CONTOUR_UPPER_CHIN_LEFT = 52,
        FP_CONTOUR_UPPER_CHIN_RIGHT = 53,
        FP_CONTOUR_LOWER_CHIN_LEFT = 54,
        FP_CONTOUR_LOWER_CHIN_RIGHT = 55,
        FP_NUM = 56,
    };

    struct tFeaturePoint {
        mpVector2 pnt;
        bool available;
    };

    tFeaturePoint fp[FP_NUM];

    MpaRecogResult();
    ~MpaRecogResult();
};


/**
 * \class MpAnalysisResult
 */
class MpaAnalysisResult {

  public:
    typedef enum {
        SCORE_ATTR_FEMALE,
        SCORE_ATTR_GLASSES,
        SCORE_ATTR_AGE,
        SCORE_ATTR_BEARD,
        SCORE_ATTR_MUSTACHE,
        SCORE_ATTR_HAIR_LONG,
        SCORE_ATTR_HAIR_FOREHEAD,
        SCORE_ATTR_SMILE,
        SCORE_ATTR_HAPPY,
        SCORE_ATTR_SAD,
        SCORE_ATTR_SURPRISED,
        SCORE_ATTR_ANGRY,
        SCORE_ATTR_FRIGHTENED,
        SCORE_ATTR_DISGUSTED,
        SCORE_ATTR_NEUTRAL,
        SCORE_ATTR_NUM,
    } eScoreAttributeId;

    struct tScore {
        tScore() {}
        tScore(bool avail, float s) : score(s), available(avail) {}
        float score;
        bool  available;
    };

    tScore scoreAttribute[SCORE_ATTR_NUM];

    MpaAnalysisResult();
    ~MpaAnalysisResult();
};

} // namespace motionportrait

#endif /* MPAANALYZER_HEADER */
