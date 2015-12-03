#pragma once
#ifndef idC1ED82CB_DA80_4427_AB12876C8C3D2134
#define idC1ED82CB_DA80_4427_AB12876C8C3D2134

#include <string>

namespace ImageProcessor
{
    namespace Const
    {
        extern const std::string thresh;
        extern const std::string gamma;
        extern const std::string bright;
        extern const std::string contrast;
        extern const std::string histogram;
        extern const std::string erode;
        extern const std::string alpha;
        extern const std::string beta;
        extern const std::string ksize;
        extern const std::string sigc;
        extern const std::string sigx;
        extern const std::string blur;
        extern const std::string kmask;
        extern const std::string rmin;
        extern const std::string rmax;

        extern const std::string GaussianBlur;
        extern const std::string BilateralFilter;

        extern const std::string CONFIGFILEEXT;
        extern const std::string CONFIG_ISDEFAULT;
        extern const std::string CONFIG_NAME;
        extern const std::string CONFIG_CONFIGURATIONS;
        extern const std::string CONFIG_OPERATIONS;
    }
}

#endif // header
