"""SmartFactory code generator for JSONRPC format.

Defines JSONRPC format specific code generation rules.

"""
import string

from generator.generators import SmartFactoryBase
from generator import Model


class CodeGenerator(SmartFactoryBase.CodeGenerator):

    """JSONRPC SmartFactory generator.

    Defines special cases that affects base code generation to make JSONRPC
    format-friendly code.

    """

    def __init__(self):
        """Construct new object."""

        SmartFactoryBase.CodeGenerator.__init__(self)

    def _gen_pre_function_schemas(self, functions):
        """Generate specific code that goes before schema initialization.

        JSON RPC generator generates code that adds specific schema for the
        error_response and adds this schema for every available response.

        Keyword arguments:
        functions -- list of functions to generate code for.

        Returns:
        Source code with error_response schema initialization and adding to the
        base SmartFactory..

        """

        code = u""
        for function in functions:
            if unicode(function.message_type.primary_name) == u"response":
                code = u"".join(
                    [code, self._error_response_insert_template.substitute(
                        function_id=function.function_id.primary_name)])

        if code:
            return self._indent_code(
                u"".join([self._error_response_schema_template, code]), 1)

        return u""

    def _preprocess_message_type(self, message_type):
        """Preprocess message_type enum.

        JSON RPC generator needs to add new message_type "error_response" in
        case if at least one response available.

        Keyword arguments:
        message_type -- message_type enum to preprocess.

        Returns:
        Preprocessed message_type enum.

        """

        if "response" in message_type.elements:
            message_type.elements[u"error_response"] = Model.EnumElement(
                name=u"error_response")

        return message_type

    def _gen_schema_params_fill(self, message_type_name):
        """Generate schema params fill code.

        Provides constant set of params for the function in accordance to the
        JSONRPC format.

        Keyword arguments:
        message_type_name -- Name of the messageType enum element.

        Returns:
        String with function schema params fill code.

        """

        return u"".join(
            [self._base_params,
             self._correlation_id_param
                if unicode(message_type_name) != u"notification" else u"",
             self._additional_response_params
                if unicode(message_type_name) == u"response" else u""])

    _error_response_insert_template = string.Template(
        u'''functions_schemes_.insert(std::make_pair('''
        u'''ns_smart_device_link::ns_json_handler::'''
        u'''SmartSchemaKey<FunctionID::eType, messageType::eType>('''
        u'''FunctionID::${function_id}, messageType::error_response), '''
        u'''error_response_schema));\n''')

    _error_response_schema_template = (
        u'''CObjectSchemaItem::Members '''
        u'''params_members;\n'''
        u'''params_members[ns_smart_device_link::ns_json_handler::'''
        u'''strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember('''
        u'''TEnumSchemaItem<FunctionID::eType>::create('''
        u'''function_id_items), true);\n'''
        u'''params_members[ns_smart_device_link::ns_json_handler::'''
        u'''strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember('''
        u'''TEnumSchemaItem<messageType::eType>::create('''
        u'''message_type_items), true);\n'''
        u'''params_members[ns_smart_device_link::ns_json_handler::'''
        u'''strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember('''
        u'''TNumberSchemaItem<int>::create(), true);\n'''
        u'''params_members[ns_smart_device_link::ns_json_handler::'''
        u'''strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember('''
        u'''TNumberSchemaItem<int>::create(), true);\n'''
        u'''params_members[ns_smart_device_link::ns_json_handler::'''
        u'''strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember('''
        u'''TNumberSchemaItem<int>::create(), true);\n'''
        u'''params_members[ns_smart_device_link::ns_json_handler::'''
        u'''strings::kCode] = CObjectSchemaItem::SMember('''
        u'''TNumberSchemaItem<int>::create(), true);\n'''
        u'''params_members[ns_smart_device_link::ns_json_handler::'''
        u'''strings::kMessage] = CObjectSchemaItem::SMember('''
        u'''CStringSchemaItem::create(), true);\n'''
        u'''\n'''
        u'''CObjectSchemaItem::Members root_members_map;\n'''
        u'''root_members_map[ns_smart_device_link::ns_json_handler::'''
        u'''strings::S_PARAMS] = CObjectSchemaItem::SMember('''
        u'''CObjectSchemaItem::create(params_members), true);\n'''
        u'''\n'''
        u'''CSmartSchema error_response_schema('''
        u'''CObjectSchemaItem::create(root_members_map));\n'''
        u'''\n''')

    _base_params = (
        u'''params_members[ns_smart_device_link::ns_json_handler::'''
        u'''strings::S_FUNCTION_ID] = CObjectSchemaItem::'''
        u'''SMember(TEnumSchemaItem<FunctionID::eType>::'''
        u'''create(function_id_items), true);\n'''
        u'''params_members[ns_smart_device_link::ns_json_handler::'''
        u'''strings::S_MESSAGE_TYPE] = CObjectSchemaItem::'''
        u'''SMember(TEnumSchemaItem<messageType::eType>::'''
        u'''create(message_type_items), true);\n'''
        u'''params_members[ns_smart_device_link::ns_json_handler::'''
        u'''strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::'''
        u'''SMember(TNumberSchemaItem<int>::create(), true);\n'''
        u'''params_members[ns_smart_device_link::ns_json_handler::'''
        u'''strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::'''
        u'''SMember(TNumberSchemaItem<int>::create(), true);\n'''
    )

    _correlation_id_param = (
        u'''params_members[ns_smart_device_link::ns_json_handler::'''
        u'''strings::S_CORRELATION_ID] = CObjectSchemaItem::'''
        u'''SMember(TNumberSchemaItem<int>::create(), true);\n'''
    )

    _additional_response_params = (
        u'''params_members[ns_smart_device_link::ns_json_handler::'''
        u'''strings::kCode] = CObjectSchemaItem::'''
        u'''SMember(TNumberSchemaItem<int>::create(), true);\n'''
    )
