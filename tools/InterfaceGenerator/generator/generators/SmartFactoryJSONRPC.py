"""SmartFactory code generator for JSONRPC format.

Defines JSONRPC format specific code generation rules.

"""
from generator.generators import SmartFactoryBase


class CodeGenerator(SmartFactoryBase.CodeGenerator):

    """JSONRPC SmartFactory generator.

    Defines special cases that affects base code generation to make JSONRPC
    format-friendly code.

    """

    def __init__(self):
        """Construct new object."""

        SmartFactoryBase.CodeGenerator.__init__(self)

    def _gen_schema_params_fill(self, message_type_name):
        """Generate schema params fill code.

        Provides constant set of params for the function in accordance to the
        JSONRPC format.

        Keyword arguments:
        message_type_name -- Name of the messageType enum element.

        Returns:
        String with function schema params fill code.

        """

        return "".join(
            [self._base_params,
             self._correlation_id_param
                if message_type_name is not "notification" else "",
             self._additional_response_params
                if message_type_name is "response" else ""])

    _base_params = (
        u'''params_members[NsSmartDeviceLink::NsJSONHandler::'''
        u'''strings::S_FUNCTION_ID] = CObjectSchemaItem::'''
        u'''SMember(TEnumSchemaItem<FunctionID::eType>::'''
        u'''create(function_id_items), true);\n'''
        u'''params_members[NsSmartDeviceLink::NsJSONHandler::'''
        u'''strings::S_MESSAGE_TYPE] = CObjectSchemaItem::'''
        u'''SMember(TEnumSchemaItem<messageType::eType>::'''
        u'''create(message_type_items), true);\n'''
        u'''params_members[NsSmartDeviceLink::NsJSONHandler::'''
        u'''strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::'''
        u'''SMember(TNumberSchemaItem<int>::create(), true);\n'''
        u'''params_members[NsSmartDeviceLink::NsJSONHandler::'''
        u'''strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::'''
        u'''SMember(TNumberSchemaItem<int>::create(), true);\n'''
    )

    _correlation_id_param = (
        u'''params_members[NsSmartDeviceLink::NsJSONHandler::'''
        u'''strings::S_CORRELATION_ID] = CObjectSchemaItem::'''
        u'''SMember(TNumberSchemaItem<int>::create(), true);\n'''
    )

    _additional_response_params = (
        u'''params_members[NsSmartDeviceLink::NsJSONHandler::'''
        u'''strings::kCode] = CObjectSchemaItem::'''
        u'''SMember(TNumberSchemaItem<int>::create(), true);\n'''
        u'''params_members[NsSmartDeviceLink::NsJSONHandler::'''
        u'''strings::kMethod] = CObjectSchemaItem::'''
        u'''SMember(TEnumSchemaItem<FunctionID::eType>::'''
        u'''create(function_id_items), true);\n'''
    )
