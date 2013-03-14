#include "SmartObjects/validation/CAlwaysTrueValidator.hpp"

bool NsAppLink::NsSmartObjects::Validation::CAlwaysTrueValidator::validate(const NsAppLink::NsSmartObjects::CSmartObject& object)
{
    return true;
}