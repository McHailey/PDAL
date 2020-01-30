/******************************************************************************
* Copyright (c) 2014, Connor Manning, connor@hobu.co
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
*     * Neither the name of Hobu, Inc. or Flaxen Geo Consulting nor the
*       names of its contributors may be used to endorse or promote
*       products derived from this software without specific prior
*       written permission.
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

#pragma once

#include <pdal/pdal_export.hpp>  // Suppresses windows 4251 messages
#include <pdal/Dimension.hpp>
#include "H5Cpp.h"

#include <memory>
#include <vector>
#include <string>
#include <map>

namespace pdal
{

namespace hdf5
{
    struct Hdf5ColumnData
    {
        Hdf5ColumnData(const std::string& name, const H5::PredType predType)
            : name(name)
            , predType(predType)
        { }

        const std::string name;
        const H5::PredType predType;
    };

    struct DimInfo
    {
        DimInfo(
            const std::string& name,
            const H5T_class_t hdf_type,
            const H5T_order_t endianness,
            const H5T_sign_t sign,
            const size_t size,
            const int offset,
            Dimension::Type pdal_type)
            : name(name)
            , hdf_type(hdf_type)
            , endianness(endianness)
            , sign(sign)
            , size(size)
            , offset(offset)
            , pdal_type(pdal_type)
        { }

        std::string name;
        H5T_class_t hdf_type;
        H5T_order_t endianness;
        H5T_sign_t sign;
        size_t size;
        int offset;
        Dimension::Type pdal_type;
        Dimension::Id id;
    };
}

class Hdf5Handler
{
public:
    struct error : public std::runtime_error
    {
        error(const std::string& err) : std::runtime_error(err)
        {}
    };

    Hdf5Handler();

    void initialize(
            const std::string& filename);
            // const std::vector<hdf5::Hdf5ColumnData>& columns);
    void close();

    void *getBuffer();

    uint64_t getNumPoints() const;
    std::vector<pdal::hdf5::DimInfo> getDimensionInfos();

    void getColumnEntries(
            void* data,
            const std::string& dataSetName,
            const hsize_t numEntries,
            const hsize_t offset) const;

private:
    struct ColumnData
    {
        ColumnData(
                H5::PredType predType,
                H5::DataSet dataSet,
                H5::DataSpace dataSpace)
            : predType(predType)
            , dataSet(dataSet)
            , dataSpace(dataSpace)
        { }

        ColumnData(H5::PredType predType)
            : predType(predType)
            , dataSet()
            , dataSpace()
        { }

        H5::PredType predType;
        H5::DataSet dataSet;
        H5::DataSpace dataSpace;
    };

    // std::vector<std::string> m_dimNames;
    std::vector<pdal::hdf5::DimInfo> m_dimInfos;
    hsize_t getColumnNumEntries(const std::string& dataSetName) const;
    const ColumnData& getColumnData(const std::string& dataSetName) const;
    void *m_buf;

    std::unique_ptr<H5::H5File> m_h5File;
    uint64_t m_numPoints;

    std::map<std::string, ColumnData> m_columnDataMap;
};

} // namespace pdal
