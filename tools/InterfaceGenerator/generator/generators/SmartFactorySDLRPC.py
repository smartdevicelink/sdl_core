"""SmartFactory code generator for SDLRPC format.

Defines SDLRPC format specific code generation rules.

"""
from generator.generators import SmartFactoryBase


class CodeGenerator(SmartFactoryBase.CodeGenerator):

    """SDLRPC SmartFactory generator.

    Defines special cases that affects base code generation to make SDLRPC
    format-friendly code.

    """

    def __init__(self):
        """Construct new object."""

        SmartFactoryBase.CodeGenerator.__init__(self)

    def _gen_schema_params_fill(self, message_type_name):
        """Generate schema params fill code.

        Provides constant set of params for the function in accordance to the
        SDLRPC format (both v1 and v2).

        Keyword arguments:
        message_type_name -- Name of the messageType enum element.

        Returns:
        String with function schema params fill code.

        """

        base_params = \
            u'''params_members[NsSmartDeviceLink::NsJSONHandler::''' \
            u'''strings::S_FUNCTION_ID] = CObjectSchemaItem::''' \
            u'''SMember(TEnumSchemaItem<FunctionID::eType>::''' \
            u'''create(function_id_items), true);\n''' \
            u'''params_members[NsSmartDeviceLink::NsJSONHandler::''' \
            u'''strings::S_MESSAGE_TYPE] = CObjectSchemaItem::''' \
            u'''SMember(TEnumSchemaItem<messageType::eType>::''' \
            u'''create(message_type_items), true);\n''' \
            u'''params_members[NsSmartDeviceLink::NsJSONHandler::''' \
            u'''strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::''' \
            u'''SMember(TNumberSchemaItem<int>::create(), true);\n''' \
            u'''params_members[NsSmartDeviceLink::NsJSONHandler::''' \
            u'''strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::''' \
            u'''SMember(TNumberSchemaItem<int>::create(), true);\n'''

        correlation_id_param = \
            u'''params_members[NsSmartDeviceLink::NsJSONHandler::''' \
            u'''strings::S_CORRELATION_ID] = CObjectSchemaItem::''' \
            u'''SMember(TNumberSchemaItem<int>::create(), true);\n'''

        return u"".join([base_params, correlation_id_param
                        if unicode(message_type_name) !=
                        u"notification" else u""])
