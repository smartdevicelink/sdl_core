"""SmartFactory code generator for JSONRPC format.

Defines JSONRPC format specific code generation rules.

"""
import string

from model.enum_element import EnumElement
from generators.SmartFactoryBase import SmartFactoryBase


class CodeGenerator(SmartFactoryBase):
    """JSONRPC SmartFactory generator.

    Defines special cases that affects base code generation to make JSONRPC
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

        JSON RPC generator generates code that adds specific schema for the
        error_response and adds this schema for every available response.

        Keyword arguments:
        functions -- list of functions to generate code for.

        Returns:
        Source code with error_response schema initialization and adding to the
        base SmartFactory..

        """

        code = ''
        for func in functions:
            if func.message_type.primary_name == 'response':
                code = ''.join(
                    [code, self._error_response_insert_template.substitute(
                        function_id=func.function_id.primary_name)])

        if code:
            return self._indent_code(
                ''.join([self._error_response_schema_template, code]), 1)

        return ''

    def _preprocess_message_type(self, message_type):
        """Preprocess message_type enum.

        JSON RPC generator needs to add new message_type 'error_response' in
        case if at least one response available.

        Keyword arguments:
        message_type -- message_type enum to preprocess.

        Returns:
        Preprocessed message_type enum.

        """

        if 'response' in message_type.elements:
            message_type.elements['error_response'] = EnumElement(
                name='error_response')

        return message_type

    def _gen_schema_params_fill(self, message_type_name):
        """Generate schema params fill code.

        Provides constant set of params for the func in accordance to the
        JSONRPC format.

        Keyword arguments:
        message_type_name -- Name of the messageType enum element.

        Returns:
        String with func schema params fill code.

        """

        return ''.join(
            [self._base_params,
             self._correlation_id_param
             if message_type_name != 'notification' else "",
             self._additional_response_params
             if message_type_name == 'response' else ""])

    _error_response_insert_template = string.Template(
        '''functions_schemes_.insert(std::make_pair('''
        '''ns_smart_device_link::ns_json_handler::'''
        '''SmartSchemaKey<FunctionID::eType, messageType::eType>('''
        '''FunctionID::${function_id}, messageType::error_response), '''
        '''error_response_schema));\n''')

    _error_response_schema_template = (
        '''Members '''
        '''params_members;\n'''
        '''params_members[ns_smart_device_link::ns_json_handler::'''
        '''strings::S_FUNCTION_ID] = SMember('''
        '''TEnumSchemaItem<FunctionID::eType>::create('''
        '''function_id_items), true);\n'''
        '''params_members[ns_smart_device_link::ns_json_handler::'''
        '''strings::S_MESSAGE_TYPE] = SMember('''
        '''TEnumSchemaItem<messageType::eType>::create('''
        '''message_type_items), true);\n'''
        '''params_members[ns_smart_device_link::ns_json_handler::'''
        '''strings::S_PROTOCOL_VERSION] = SMember('''
        '''TNumberSchemaItem<int>::create(), true);\n'''
        '''params_members[ns_smart_device_link::ns_json_handler::'''
        '''strings::S_PROTOCOL_TYPE] = SMember('''
        '''TNumberSchemaItem<int>::create(), true);\n'''
        '''params_members[ns_smart_device_link::ns_json_handler::'''
        '''strings::S_CORRELATION_ID] = SMember('''
        '''TNumberSchemaItem<int>::create(), true);\n'''
        '''params_members[ns_smart_device_link::ns_json_handler::'''
        '''strings::kCode] = SMember('''
        '''TNumberSchemaItem<int>::create(), true);\n'''
        '''params_members[ns_smart_device_link::ns_json_handler::'''
        '''strings::kMessage] = SMember('''
        '''CStringSchemaItem::create(), true);\n'''
        '''\n'''
        '''Members root_members_map;\n'''
        '''root_members_map[ns_smart_device_link::ns_json_handler::'''
        '''strings::S_PARAMS] = SMember('''
        '''CObjectSchemaItem::create(params_members), true);\n'''
        '''\n'''
        '''CSmartSchema error_response_schema('''
        '''CObjectSchemaItem::create(root_members_map));\n'''
        '''\n''')

    _base_params = (
        '''params_members[ns_smart_device_link::ns_json_handler::'''
        '''strings::S_FUNCTION_ID] = SMember(TEnumSchemaItem<FunctionID::eType>::'''
        '''create(function_id_items), true);\n'''
        '''params_members[ns_smart_device_link::ns_json_handler::'''
        '''strings::S_MESSAGE_TYPE] = SMember(TEnumSchemaItem<messageType::eType>::'''
        '''create(message_type_items), true);\n'''
        '''params_members[ns_smart_device_link::ns_json_handler::'''
        '''strings::S_PROTOCOL_VERSION] = SMember(TNumberSchemaItem<int>::create(), true);\n'''
        '''params_members[ns_smart_device_link::ns_json_handler::'''
        '''strings::S_PROTOCOL_TYPE] = SMember(TNumberSchemaItem<int>::create(), true);\n'''
    )

    _correlation_id_param = (
        '''params_members[ns_smart_device_link::ns_json_handler::'''
        '''strings::S_CORRELATION_ID] = SMember(TNumberSchemaItem<int>::create(), true);\n'''
    )

    _additional_response_params = (
        '''params_members[ns_smart_device_link::ns_json_handler::'''
        '''strings::kCode] = SMember(TNumberSchemaItem<int>::create(), true);\n'''
    )
