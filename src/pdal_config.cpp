/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  LAS version related functions.
 * Author:   Mateusz Loskot, mateusz@loskot.net
 *           Frank Warmerdam, warmerdam@pobox.com
 *
 ******************************************************************************
 * Copyright (c) 2008, Mateusz Loskot
 * Copyright (c) 2010, Frank Warmerdam
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following
 * conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of the Martin Isenburg or Iowa Department
 *       of Natural Resources nor the names of its contributors may be
 *       used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 ****************************************************************************/

#include <pdal/pdal_config.hpp>

#include <sstream>
#include <pdal/pdal_defines.h>
#include <pdal/gitsha.h>

#ifdef PDAL_HAVE_LIBGEOTIFF
#include <geotiff.h>
#endif

#ifdef PDAL_HAVE_GDAL
#include <gdal.h>
#endif

#ifdef PDAL_HAVE_LASZIP
#include <laszip/laszip.hpp>
#endif

namespace pdal
{

/// Check if GDAL support has been built in to PDAL
bool IsGDALEnabled()
{
#ifdef PDAL_HAVE_GDAL
    return true;
#else
    return false;
#endif
}

/// Check if GeoTIFF support has been built in to PDAL
bool IsLibGeoTIFFEnabled()
{
#ifdef PDAL_HAVE_LIBGEOTIFF
    return true;
#else
    return false;
#endif
}

/// Check if LasZip compression support has been built in to PDAL
bool IsLasZipEnabled()
{
#ifdef PDAL_HAVE_LASZIP
    return true;
#else
    return false;
#endif
}

bool IsEmbeddedBoost()
{
#ifdef PDAL_EMBED_BOOST
    return true;
#else
    return false;
#endif
}

int GetVersionMajor()
{
    return PDAL_VERSION_MAJOR;
}

int GetVersionMinor()
{
    return PDAL_VERSION_MINOR;
}

int GetVersionPatch()
{
    return PDAL_VERSION_PATCH;
}

std::string GetVersionString()
{
    return std::string(PDAL_VERSION_STRING);
}

int GetVersionInteger()
{
    return PDAL_VERSION_INTEGER;
}


/// Tell the user a bit about PDAL's compilation
std::string GetFullVersionString()
{
    std::ostringstream os;



#ifdef PDAL_HAVE_LIBGEOTIFF
    os << " GeoTIFF "
       << (LIBGEOTIFF_VERSION / 1000) << '.'
       << (LIBGEOTIFF_VERSION / 100 % 10) << '.'
       << (LIBGEOTIFF_VERSION % 100 / 10);
#endif

#ifdef PDAL_HAVE_GDAL
    os << " GDAL " << GDALVersionInfo("RELEASE_NAME");
#endif

#ifdef PDAL_HAVE_LASZIP
    os << " LASzip "
       << LASZIP_VERSION_MAJOR << "."
       << LASZIP_VERSION_MINOR << "."
       << LASZIP_VERSION_REVISION;
#endif

    if (IsEmbeddedBoost())
        os << " Embed ";
    else
        os << " System ";

    std::string info(os.str());
    os.str("");
    os << "PDAL " << PDAL_VERSION_STRING;

    std::ostringstream revs;
    revs << g_GIT_SHA1;

    os << " (" << revs.str().substr(0, 6) <<")";

    if (!info.empty())
    {
        os << " with" << info;
    }

    return os.str();
}

} // namespace pdal
