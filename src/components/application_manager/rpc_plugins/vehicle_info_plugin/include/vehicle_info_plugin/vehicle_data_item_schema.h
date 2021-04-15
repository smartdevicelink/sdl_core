#ifndef VEHICLE_DATA_ITEM_SCHEMA_H
#define VEHICLE_DATA_ITEM_SCHEMA_H
#include "policy/policy_table/types.h"
#include "smart_objects/always_false_schema_item.h"
#include "smart_objects/always_true_schema_item.h"
#include "smart_objects/array_schema_item.h"
#include "smart_objects/bool_schema_item.h"
#include "smart_objects/enum_schema_item.h"
#include "smart_objects/number_schema_item.h"
#include "smart_objects/object_schema_item.h"
#include "smart_objects/schema_item.h"
#include "smart_objects/schema_item_parameter.h"
#include "smart_objects/smart_object.h"
#include "smart_objects/string_schema_item.h"
#include "utils/logger.h"

namespace vehicle_info_plugin {
namespace smart_objects = ns_smart_device_link::ns_smart_objects;
namespace policy_table = rpc::policy_table_interface_base;

class VehicleDataItemSchema;
typedef std::shared_ptr<VehicleDataItemSchema> VehicleDataItemSchemaPtr;
typedef smart_objects::TSchemaItemParameter<policy_table::VehicleDataItem>
    PolicyDataItem;

/**
 * @brief VehicleData schema item.
 **/
class VehicleDataItemSchema : public smart_objects::ISchemaItem {
 public:
  enum SchemaType { MOBILE = 0, HMI };

  /**
   * @brief Create a new schema item.
   * @param PolicyDataItem& SchemaItem for VehicleDataItem elements.
   * @return Shared pointer to a new schema item.
   **/
  static VehicleDataItemSchemaPtr create(PolicyDataItem& policy_item,
                                         SchemaType schema_type);

  /**
   * @brief Validate smart object.
   * @param Object Object to validate.
   * @param report__ object for reporting errors during validation
   * @param MessageVersion to check mobile RPC version against RPC Spec History
   * @param allow_unknown_enums
   *   false - unknown enum values (left as string values after applySchema)
   *   will be considered invalid.
   *   true - such values will be considered valid.
   * @return ns_smart_objects::errors::eType
   **/
  smart_objects::errors::eType validate(
      const smart_objects::SmartObject& Object,
      rpc::ValidationReport* report__,
      const utils::SemanticVersion& MessageVersion = utils::SemanticVersion(),
      const bool allow_unknown_enums = false) OVERRIDE;

 private:
  /**
   * @brief Constructor.
   * @param PolicyDataItem& SchemaItem for VehicleDataItem elements.
   * @param SchemaType defines api type for schema to be generated.
   **/
  VehicleDataItemSchema(PolicyDataItem& policy_item, SchemaType schema_type);

  /*
   * @brief Check whether provided type is POD type
   * @param const std::string& type_name - name of type to check
   * @return true - if type is POD type and false - if not
   */
  const bool isPODType(const std::string& type_name) const;

  /*
   * @brief Get Enum schema for provided type
   * @param const std::string& type_name - name of enum type to provide
   * @return Shared pointer to a new schema item. If no schema found - nullptr
   * is returned.
   */
  smart_objects::ISchemaItemPtr getEnumSchema(
      const std::string& type_name) const;

  /**
   * @brief Apply schema.
   * @param Object Object to apply schema.
   * @param remove_unknown_parameters contains true if need to remove unknown
   * parameters from smart object otherwise contains false.
   **/
  void applySchema(smart_objects::SmartObject& Object,
                   const bool remove_unknown_parameters,
                   const utils::SemanticVersion& MessageVersion) OVERRIDE;

  /**
   * @brief Unapply schema.
   * @param Object Object to unapply schema.
   * @param remove_unknown_parameters contains true if need to remove unknown
   * parameters
   **/
  void unapplySchema(smart_objects::SmartObject& Object,
                     const bool remove_unknown_parameters) OVERRIDE;

  /*
   * @brief Get schema for a POD type
   * @param const VehicleDataItem& policy_item - item, that contains info about
   * type
   * @return Shared pointer to a new schema item. If no schema found - nullptr
   * is returned.
   */
  smart_objects::ISchemaItemPtr GetPODTypeSchema(
      const policy_table::VehicleDataItem& policy_item, SchemaType schema_type);

  smart_objects::ISchemaItemPtr so_schema_item_;
};

}  // namespace vehicle_info_plugin
#endif  // VEHICLE_DATA_ITEM_SCHEMA_H
