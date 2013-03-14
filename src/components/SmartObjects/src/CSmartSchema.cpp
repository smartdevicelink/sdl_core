#include "SmartObjects/CSmartSchema.hpp"

NsAppLink::NsSmartObjects::CSmartSchema::CSmartSchema()
: mValidator()
, mValidatorAvailable(false)
{

}

NsAppLink::NsSmartObjects::CSmartSchema::CSmartSchema(NsAppLink::NsSmartObjects::Validation::IValidator validator)
: mValidator(validator)
, mValidatorAvailable(true)
{

}

bool NsAppLink::NsSmartObjects::CSmartSchema::validate(const NsAppLink::NsSmartObjects::CSmartObject& object)
{
    if(mValidatorAvailable)
    {
        return mValidator.validate(object);
    }
    else
    {
        return true;
    }

}

void NsAppLink::NsSmartObjects::CSmartSchema::setValidator(NsAppLink::NsSmartObjects::Validation::IValidator validator)
{
    mValidator = validator;
    mValidatorAvailable = true;
}