#include "SmartObjects/CSmartSchema.hpp"
#include "SmartObjects/validation/CAlwaysTrueValidator.hpp"

NsAppLink::NsSmartObjects::CSmartSchema::CSmartSchema()
: mValidator(NsAppLink::NsSmartObjects::Validation::CAlwaysTrueValidator::create())
{
}

NsAppLink::NsSmartObjects::CSmartSchema::CSmartSchema(NsAppLink::NsSmartObjects::TSharedPtr<NsAppLink::NsSmartObjects::Validation::IValidator> validator)
: mValidator(validator)
{

}

bool NsAppLink::NsSmartObjects::CSmartSchema::validate(const NsAppLink::NsSmartObjects::CSmartObject& object)
{
    return mValidator->validate(object);
}

void NsAppLink::NsSmartObjects::CSmartSchema::setValidator(NsAppLink::NsSmartObjects::TSharedPtr<NsAppLink::NsSmartObjects::Validation::IValidator> validator)
{
    mValidator = validator;
}