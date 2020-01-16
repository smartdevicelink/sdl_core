"""SmartFactory code generator for SDLRPC format.

Defines SDLRPC format specific code generation rules.

"""
from generators.SmartFactoryBase import SmartFactoryBase


class CodeGenerator(SmartFactoryBase):

    """SDLRPC SmartFactory generator.

    Defines special cases that affects base code generation to make SDLRPC
    format-friendly code.

    """

    def __init__(self):
        """Construct new object."""
        super(CodeGenerator, self).__init__()

    @property
    def get_version(self):
        return '1.0.0'

    def _gen_pre_function_schemas(self, functions):
        """Generate specific code that goes before schema initialization.

        In SDL RPC generator there is no need to generate any code before
        schemas initialization.

        Keyword arguments:
        functions -- list of functions to generate code for.

        Returns:
        Empty string.

        """

        return ""

    def _preprocess_message_type(self, message_type):
        """Preprocess message_type enum.

        In SDL RPC generator there is no need to preprocess message_type
        enum values.

        Keyword arguments:
        message_type -- message_type enum to preprocess.

        Returns:
        Initial message_type enum without any modifications.

        """

        return message_type

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
            '''params_members[ns_smart_device_link::ns_json_handler::''' \
            '''strings::S_FUNCTION_ID] = SMember(TEnumSchemaItem<FunctionID::eType>::''' \
            '''create(function_id_items), true);\n''' \
            '''params_members[ns_smart_device_link::ns_json_handler::''' \
            '''strings::S_MESSAGE_TYPE] = SMember(TEnumSchemaItem<messageType::eType>::''' \
            '''create(message_type_items), true);\n''' \
            '''params_members[ns_smart_device_link::ns_json_handler::''' \
            '''strings::S_PROTOCOL_VERSION] = SMember(TNumberSchemaItem<int>::create(), true);\n''' \
            '''params_members[ns_smart_device_link::ns_json_handler::''' \
            '''strings::S_PROTOCOL_TYPE] = SMember(TNumberSchemaItem<int>::create(), true);\n'''

        correlation_id_param = \
            '''params_members[ns_smart_device_link::ns_json_handler::''' \
            '''strings::S_CORRELATION_ID] = SMember(TNumberSchemaItem<int>::create(), true);\n'''

        return "".join([base_params, correlation_id_param
                        if message_type_name !=
                        "notification" else ""])
