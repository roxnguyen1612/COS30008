#include "Vector3D.h"
#include <sstream>
#include <iomanip>
#include <cmath>

std::string formatComponent(float aComponent)
{
    std::stringstream lStream; 

    if (std::fabs(aComponent - std::round(aComponent)) < 0.0001)
    {
        lStream << static_cast<int>(std::round(aComponent));
    }
    else
    {
        if (std::fabs(aComponent) < 10000)
        {
            //numbers <10000, use four decimal places  
            lStream << std::fixed << std::setprecision(4) << aComponent;
        }
        else
        {
            //larger numbers, use fewer decimal places  -> the case vector c
            lStream << std::fixed << std::setprecision(1) << aComponent;
        }
    }
    return lStream.str(); 
}

std::string Vector3D::toString() const noexcept {
    std::string lFormattedX = formatComponent(this->fBaseVector.x()); 
    std::string lFormattedY = formatComponent(this->fBaseVector.y()); 
    std::string lFormattedW = formatComponent(this->fW);             

    return "[" + lFormattedX + "," + lFormattedY + "," + lFormattedW + "]";
}
