#ifndef APENGINE_PARSER_H
#define APENGINE_PARSER_H

#include <string>
#include "document.h"
#include "filewritestream.h"
#include <writer.h>
#include <sstream>
#include "lights/AmbientLight.h"
#include "lights/DirectionalLight.h"
#include "lights/PointLight.h"
#include "lights/SpotLight.h"
#include "Objects/Object3D.h"
#include "Objects/MovingObject.h"
#include "Objects/Level.h"
#include "Camera.h"

class Parser {
private:
    std::string savePath;
public:
    Parser(std::string savePath);

    Parser();

    void SaveJSON(rapidjson::Document d, std::string fileName);

    DirectionalLight CreateFromJSONDir(std::string fileName);

    PointLight CreateFromJSONPoint(std::string fileName);

    SpotLight CreateFromJSONSpot(std::string fileName);

    Object3D CreateFromJSONObj3D(std::string fileName);

    MovingObject CreateFromJSONMovingObject(std::string fileName);

    Level CreateFromJSONWalls(std::string fileName);

    Camera CreateFromJSONCam(std::string fileName);

    rapidjson::Document openJSON (std::string fileName);

    enum ParamType
    {
        Param_Sampler,
        Param_Surface
    };

    enum ShadeType
    {
        Shade_Invalid,
        Shade_Constant,
        Shade_Lambert,
        Shade_Phong,
        Shade_Blinn
    };

    struct EffectParam
    {
        ParamType mType;
        std::string mReference; // to which other thing the param is referring to.
    };

    struct Sampler
    {
        Sampler()
                :	mWrapU		(true)
                ,	mWrapV		(true)
                ,	mMirrorU	()
                ,	mMirrorV	()
                ,	mOp			(aiTextureOp_Multiply)
                ,	mUVId		(UINT_MAX)
                ,	mWeighting  (1.f)
                ,	mMixWithPrevious (1.f)
        {}

        /** Name of image reference
         */
        std::string mName;

        /** Wrap U?
         */
        bool mWrapU;

        /** Wrap V?
         */
        bool mWrapV;

        /** Mirror U?
         */
        bool mMirrorU;

        /** Mirror V?
         */
        bool mMirrorV;

        /** Blend mode
         */
        aiTextureOp mOp;

        /** UV transformation
         */
        aiUVTransform mTransform;

        /** Name of source UV channel
         */
        std::string mUVChannel;

        /** Resolved UV channel index or UINT_MAX if not known
         */
        unsigned int mUVId;

        // OKINO/MAX3D extensions from here
        // -------------------------------------------------------

        /** Weighting factor
         */
        float mWeighting;

        /** Mixing factor from OKINO
         */
        float mMixWithPrevious;
    };

    struct Effect
    {
        // Shading mode
        ShadeType mShadeType;

        // Colors
        aiColor4D mEmissive, mAmbient, mDiffuse, mSpecular,
                mTransparent, mReflective;

        // Textures
        Sampler mTexEmissive, mTexAmbient, mTexDiffuse, mTexSpecular,
                mTexTransparent, mTexBump, mTexReflective;

        // Scalar factory
        float mShininess, mRefractIndex, mReflectivity;
        float mTransparency;

        // local params referring to each other by their SID
        typedef std::map<std::string, EffectParam> ParamLibrary;
        ParamLibrary mParams;

        // MAX3D extensions
        // ---------------------------------------------------------
        // Double-sided?
        bool mDoubleSided, mWireframe, mFaceted;

        Effect()
                : mShadeType    (Shade_Phong)
                , mEmissive		( 0, 0, 0, 1)
                , mAmbient		( 0.1f, 0.1f, 0.1f, 1)
                , mDiffuse		( 0.6f, 0.6f, 0.6f, 1)
                , mSpecular		( 0.4f, 0.4f, 0.4f, 1)
                , mTransparent	( 0, 0, 0, 1)
                , mShininess    (10.0f)
                , mRefractIndex (1.f)
                , mReflectivity (1.f)
                , mTransparency (0.f)
                , mDoubleSided	(false)
                , mWireframe    (false)
                , mFaceted      (false)
        {
        }
    };

    void ReadEffectColor(aiColor4D& pColor, Sampler& psampler);

    Effect pEffect;

};

#endif //APENGINE_PARSER_H
