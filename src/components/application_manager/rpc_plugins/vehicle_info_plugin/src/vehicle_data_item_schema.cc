#include "vehicle_info_plugin/vehicle_data_item_schema.h"
#include <limits>

namespace vehicle_info_plugin {

#ifdef ENABLE_LOG
CREATE_LOGGERPTR_LOCAL(vehicle_data_logger, "VehicleDataItemSchema");
#endif  // ENABLE_LOG

VehicleDataItemSchema::VehicleDataItemSchema(PolicyDataItem& policy_item,
                                             SchemaType schema_type) {
  policy_table::VehicleDataItem policy_data_item;
  policy_item.getValue(policy_data_item);

  auto get_object_schema = [&policy_data_item,
                            &schema_type]() -> decltype(so_schema_item_) {
    smart_objects::Members members;
    auto object_schema = smart_objects::CObjectSchemaItem::create(members);

    for (auto& e : *policy_data_item.params) {
      auto schema_item = PolicyDataItem(e);
      smart_objects::SMember member(create(schema_item, schema_type),
                                    bool(e.mandatory),
                                    std::string(*e.since),
                                    std::string(*e.until),
                                    bool(*e.deprecated),
                                    bool(*e.removed));
      std::string name;
      switch (schema_type) {
        case VehicleDataItemSchema::SchemaType::HMI:
          name = e.key;
          break;
        case VehicleDataItemSchema::SchemaType::MOBILE:
        default:
          name = e.name;
          break;
      }
      object_schema->AddMemberSchemaItem(name, member);
    }

    return object_schema;
  };

  auto get_schema = [&policy_data_item,
                     &get_object_schema,
                     &schema_type,
                     this]() -> decltype(so_schema_item_) {
    // Check if policy item has POD type
    if (isPODType(policy_data_item.type)) {
      return GetPODTypeSchema(policy_data_item, schema_type);
    }

    // Check if policy item has Struct type
    if (policy_data_item.type == policy_table::VehicleDataItem::kStruct) {
      return get_object_schema();
    }

    // Check if policy item has Enum type
    auto enum_schema = getEnumSchema(policy_data_item.type);
    if (enum_schema != nullptr) {
      return enum_schema;
    }

    // If an unknown type is present in the policy table, it is assumed that it
    // is a future enum type. Since normal validation cannot be performed on
    // this value, it is treated as a raw string instead
    policy_data_item.type = "String";

    return GetPODTypeSchema(policy_data_item, schema_type);
  };

  if (*policy_data_item.array) {
    so_schema_item_ = smart_objects::CArraySchemaItem::create(
        get_schema(),
        policy_data_item.minsize.is_initialized()
            ? smart_objects::TSchemaItemParameter<size_t>(
                  *policy_data_item.minsize)
            : smart_objects::TSchemaItemParameter<size_t>(),
        policy_data_item.maxsize.is_initialized()
            ? smart_objects::TSchemaItemParameter<size_t>(
                  *policy_data_item.maxsize)
            : smart_objects::TSchemaItemParameter<size_t>());
  } else {
    so_schema_item_ = get_schema();
  }
}

VehicleDataItemSchemaPtr VehicleDataItemSchema::create(
    PolicyDataItem& policy_item, SchemaType schema_type) {
  return VehicleDataItemSchemaPtr(
      new VehicleDataItemSchema(policy_item, schema_type));
}

smart_objects::errors::eType
vehicle_info_plugin::VehicleDataItemSchema::validate(
    const smart_objects::SmartObject& Object,
    rpc::ValidationReport* report__,
    const utils::SemanticVersion& MessageVersion,
    const bool allow_unknown_enums) {
  if (!so_schema_item_) {
    std::string validation_info =
        "Invalid type: " +
        smart_objects::SmartObject::typeToString(Object.getType());
    report__->set_validation_info(validation_info);
    return smart_objects::errors::eType::ERROR;
  }

  return so_schema_item_->validate(
      Object, report__, MessageVersion, allow_unknown_enums);
}

smart_objects::ISchemaItemPtr VehicleDataItemSchema::GetPODTypeSchema(
    const policy_table::VehicleDataItem& policy_item, SchemaType schema_type) {
  using IntSchemaItem = smart_objects::TNumberSchemaItem<int64_t>;
  using IntItemParam = smart_objects::TSchemaItemParameter<int64_t>;
  using FloatSchemaItem = smart_objects::TNumberSchemaItem<double>;
  using FloatItemParam = smart_objects::TSchemaItemParameter<double>;
  using StringSchemaItem = smart_objects::CStringSchemaItem;
  using StringItemParam = smart_objects::TSchemaItemParameter<size_t>;
  using BoolSchemaItem = smart_objects::CBoolSchemaItem;
  using BoolItemParam = smart_objects::TSchemaItemParameter<bool>;

  if (policy_item.type == policy_table::VehicleDataItem::kInteger) {
    return IntSchemaItem::create(policy_item.minvalue.is_initialized()
                                     ? IntItemParam(*policy_item.minvalue)
                                     : IntItemParam(),
                                 policy_item.maxvalue.is_initialized()
                                     ? IntItemParam(*policy_item.maxvalue)
                                     : IntItemParam());
  }
  if (policy_item.type == policy_table::VehicleDataItem::kFloat ||
      policy_item.type == policy_table::VehicleDataItem::kDouble) {
    return FloatSchemaItem::create(
        policy_item.minvalue.is_initialized()
            ? FloatItemParam(double(*policy_item.minvalue))
            : FloatItemParam(),
        policy_item.maxvalue.is_initialized()
            ? FloatItemParam(double(*policy_item.maxvalue))
            : FloatItemParam());
  }
  if (policy_item.type == policy_table::VehicleDataItem::kString) {
    return StringSchemaItem::create(
        StringItemParam(policy_item.minlength.is_initialized()
                            ? *policy_item.minlength
                            : 0),
        policy_item.maxlength.is_initialized()
            ? StringItemParam(*policy_item.maxlength)
            : StringItemParam());
  }
  if (policy_item.type == policy_table::VehicleDataItem::kBoolean) {
    return BoolSchemaItem::create(BoolItemParam(true));
  }

  std::string error_msg = std::string("Invalid POD type provided: ") +
                          std::string(policy_item.type);
  LOG4CXX_ERROR(vehicle_data_logger, error_msg.c_str());
  return nullptr;
}

const bool VehicleDataItemSchema::isPODType(
    const std::string& type_name) const {
  return std::find(policy_table::VehicleDataItem::kPODTypes.begin(),
                   policy_table::VehicleDataItem::kPODTypes.end(),
                   type_name) != policy_table::VehicleDataItem::kPODTypes.end();
}

smart_objects::ISchemaItemPtr VehicleDataItemSchema::getEnumSchema(
    const std::string& type_name) const {
  auto enum_schema = policy_table::EnumSchemaItemFactory::Get(type_name);
  if (!enum_schema) {
    LOG4CXX_ERROR(vehicle_data_logger, "NULL pointer: " << type_name);
  }
  return enum_schema;
}

void VehicleDataItemSchema::applySchema(
    smart_objects::SmartObject& Object,
    const bool remove_unknown_parameters,
    const utils::SemanticVersion& MessageVersion) {
  so_schema_item_->applySchema(
      Object, remove_unknown_parameters, MessageVersion);
}

void VehicleDataItemSchema::unapplySchema(
    smart_objects::SmartObject& Object, const bool remove_unknown_parameters) {
  so_schema_item_->unapplySchema(Object, remove_unknown_parameters);
}
}  // namespace vehicle_info_plugin
