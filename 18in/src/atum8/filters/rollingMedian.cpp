#include "rollingMedian.hpp"

namespace atum8
{
    RollingMedian::RollingMedian(int iSize) : size{iSize},
                                              lower{int(floor(size / 2.0))},
                                              upper{int(ceil(size / 2.0))}
    {
        for (int i{0}; i < size; i++)
            values.push_back(0.0);
    }

    void RollingMedian::update(double value)
    {
        values.insert(values.begin(), value);
        if (values.size() > size)
            values.pop_back();
        std::vector<double> sortedValues = values;
        std::sort(sortedValues.begin(), sortedValues.end());
        filteredVal = (sortedValues[lower] + sortedValues[upper]) / 2;
    }
}