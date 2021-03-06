/* Copyright (c) 2018 by Arm Limited. All rights reserved. */

#ifndef INCLUDE_LINUX_PERF_PERF_EVENT_GROUP_IDENTIFIER_H
#define INCLUDE_LINUX_PERF_PERF_EVENT_GROUP_IDENTIFIER_H

#include <cstring>
#include <set>
#include <string>

#include "ClassBoilerPlate.h"

class GatorCpu;
class UncorePmu;

class PerfEventGroupIdentifier
{
public:

    enum class Type
    {
        PER_CLUSTER_CPU,
        UNCORE_PMU,
        SPECIFIC_CPU,
        GLOBAL
    };

    /** Constructor, for global events on all CPUs */
    PerfEventGroupIdentifier();

    /** Constructor, for each CPU PMU in a specific cluster */
    PerfEventGroupIdentifier(const GatorCpu & cluster);

    /** Constructor, for a given UncorePmu */
    PerfEventGroupIdentifier(const UncorePmu & pmu);

    /** Constructor, for global events associated with a specific core */
    PerfEventGroupIdentifier(int cpuNumber);

    CLASS_DEFAULT_COPY_MOVE(PerfEventGroupIdentifier);

    /** Equality operator, are they the same group? */
    inline bool operator == (const PerfEventGroupIdentifier & that) const
    {
        return (cluster == that.cluster) && (pmu == that.pmu) && (cpuNumber == that.cpuNumber);
    }

    /** Inequality operator, are they not the same group? */
    inline bool operator != (const PerfEventGroupIdentifier & that) const
    {
        return !(*this == that);
    }

    /** Less operator, to allow use in std::map as key */
    bool operator < (const PerfEventGroupIdentifier & that) const;

    /** Convert to string for logging purposes */
    operator std::string () const;

    /* Accessors */

    inline const GatorCpu * getCluster() const
    {
        return cluster;
    }

    inline const UncorePmu * getUncorePmu() const
    {
        return pmu;
    }

    inline int getCpuNumber() const
    {
        return cpuNumber;
    }

    inline Type getType() const
    {
        if (cluster != nullptr) {
            return Type::PER_CLUSTER_CPU;
        }
        else if (pmu != nullptr) {
            return Type::UNCORE_PMU;
        }
        else if (cpuNumber >= 0) {
            return Type::SPECIFIC_CPU;
        }
        else {
            return Type::GLOBAL;
        }
    }

private:

    const GatorCpu * const cluster;
    const UncorePmu * const pmu;
    const int cpuNumber;
};

#endif /* INCLUDE_LINUX_PERF_PERF_EVENT_GROUP_IDENTIFIER_H */
