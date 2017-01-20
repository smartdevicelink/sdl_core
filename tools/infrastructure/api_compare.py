#!/usr/bin/env python

import json
import sys
import errno
import argparse
import xml.etree.ElementTree as ElementTree
from collections import namedtuple

reload(sys)
# Enable Utf-8 support. Needed if script will be run under Windows
sys.setdefaultencoding('utf-8')


class colors:
    """Class defining colorful output.
    Works under UNIX ONLY as Windows does not support ANSI escape sequences
    """
    HEADER = '\033[95m'
    BLUE = '\033[94m'
    GREEN = '\033[92m'
    WARN = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'


def simple_values_comparison(mobile_attribute_value, hmi_attribute_value):
    """Simple attribute value comparison"""
    return mobile_attribute_value == hmi_attribute_value


def parse_command_line_args():
    """Function adds and parses command line arguments"""
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--mobile_api_file",
        type=str, default="MOBILE_API.xml",
        help='Enter mobile api file name! ("MOBILE_API.xml" by default)'
    )
    parser.add_argument(
        "--hmi_api_file",
        type=str, default="HMI_API.xml",
        help='Enter hmi api file name! ("HMI_API.xml" by default)'
    )
    parser.add_argument(
        "-o", "--output",
        type=str, default="console",
        choices=['xml', 'json', 'console'],
        help='Choose report type! ("console" by default)'
    )
    parser.add_argument(
        "--full_info",
        action="store_true",
        help='Enter this parameter to get full detailed info!'
    )
    args = parser.parse_args()
    return args


def json_to_xml(json_obj, line_padding=" "):
    """Function which converts json to xml format"""
    result_list = list()
    json_obj_type = type(json_obj)
    if json_obj_type is list:
        for sub_elem in json_obj:
            result_list.append(json_to_xml(sub_elem, line_padding))
        return "\n".join(result_list)
    if json_obj_type is dict:
        for tag_name in json_obj:
            sub_obj = json_obj[tag_name]
            result_list.append("%s<%s>" % (line_padding, tag_name))
            result_list.append(json_to_xml(sub_obj, "\t" + line_padding))
            result_list.append("%s</%s>" % (line_padding, tag_name))
        return "\n".join(result_list)
    return "%s%s" % (line_padding, json_obj)


def dict_to_json(summary_result):
    """Function converts python dictionary to json format"""
    return json.dumps(
        summary_result, sort_keys=True, indent=4, separators=(',', ': '))


def set_build_status(comparison_result):
    """Function sets up build status depend on comparison result"""
    if len(comparison_result) > 0:
        sys.exit(errno.EPERM)
    else:
        return


def get_rpc_params(function):
    """Function which gets all params from specified RPC"""
    params = {}
    for function_tag in function:  # loop over function tags
        if function_tag.tag == 'param':
            name = function_tag.attrib['name']
            params[name] = function_tag.attrib
    return params


Rpc = namedtuple("rpc", ["rpc_name", "rpc_type"])


def extract_functions(interface, functions):
    """Function extracting rpcs from passed interface"""
    for child in interface:
        if child.tag == 'function':
            rpc_name = child.attrib['name']
            rpc_type = child.attrib['messagetype']
            key = Rpc(rpc_name=rpc_name, rpc_type=rpc_type)
            params = get_rpc_params(child)
            if key in functions:
                functions[key].update(params)
            else:
                functions[key] = params


def filter_params(rpc, params_to_filter, filter):
    "Function filters rpc parameters according passed filter"
    filtered = []
    for param in params_to_filter:
        # Filter only keys which present in filter
        if param not in filter:
            filtered.append(param)
        else:
            filter_func = filter[param]
            if filter_func(rpc):
                filtered.append(param)
    return filtered


def create_result_section(param, result):
    """Function creating appropriate section in result dictionary"""
    if param not in result:
        result[param] = {}
    if "HMI_API" not in result[param]:
        result[param]["HMI_API"] = {}
    if "Mobile_API" not in result[param]:
        result[param]["Mobile_API"] = {}
    mob_api_result_section = result[param]["Mobile_API"]
    hmi_api_result_section = result[param]["HMI_API"]
    return mob_api_result_section, hmi_api_result_section


def compare_params(param, mob_param_attrs, hmi_param_attrs):
    """Function comparing mobile and hmi parameter attributes"""
    result = {}
    mob_param_attrs = dict(mob_param_attrs)
    hmi_param_attrs = dict(hmi_param_attrs)
    for compare_rule in global_compare_rules:
        condition = compare_rule[0]
        rule = compare_rule[1]
        if (condition(mob_param_attrs, hmi_param_attrs)):
            mobile_res, hmi_res = rule(
                mob_param_attrs, hmi_param_attrs)
            if len(mobile_res) > 0 or len(hmi_res) > 0:
                m_res, h_res = create_result_section(param, result)
                m_res.update(mobile_res)
                h_res.update(hmi_res)
    return result


def fill_summary_result(rpc_name, area, value, summary_result):
    """Function filling summary comparison result"""
    if rpc_name not in summary_result:
        summary_result[rpc_name] = {}
    rpc = summary_result[rpc_name]
    if type(value) is not dict:
        rpc[area] = value
    elif type(value) is dict:
        if area not in rpc:
            rpc[area] = {}
        rpc[area].update(value)


def get_common_params(rpc_name, mob_api_functions, hmi_api_functions):
    """Function calculates common parameters from mobile and hmi apis
    for specified rpc
    """
    hmi_params = hmi_api_functions[rpc_name]
    mob_params = mob_api_functions[rpc_name]
    mob_keys = mob_params.keys()
    hmi_keys = hmi_params.keys()
    return list(set(mob_keys).intersection(set(hmi_keys)))


def get_diff(common_params, rpc_name, hmi_params, mob_params, summary_result):
    """Function which gets difference between parameters
    and writes to summary result
    """
    for param in common_params:
        mob_items = sorted(mob_params[param].items())
        hmi_items = sorted(hmi_params[param].items())
        diff_res = compare_params(param, mob_items, hmi_items)
        area = "Attributes difference:"
        if len(diff_res) > 0:
            fill_summary_result(rpc_name, area,
                                diff_res, summary_result
                                )


def print_parameters(common_params, hmi_params, mob_params):
    """Function which prints parameters in mobile, hmi api
    and common parameters
    """
    print "Parameters to check: ", common_params, "\n"
    for param in common_params:
        mob_items = sorted(mob_params[param].items())
        hmi_items = sorted(hmi_params[param].items())
        print colors.UNDERLINE + colors.BOLD + param + colors.ENDC, ":"
        print "In Mobile API :\t", dict(mob_items)
        print "In HMI API :\t", dict(hmi_items)


def print_full_info(hmi_absent_params, hmi_params,
                    mob_absent_params, mob_params, rpc_name):
    """Function prints full detailed info about every rpc"""
    print "\n" + "---" * 60 + "\n"
    rpc_color = colors.BOLD + colors.HEADER
    print rpc_color + rpc_name + colors.ENDC
    print colors.BOLD + "\nMobile API: " + colors.ENDC
    print "Parameters quantity: ", len(mob_params)
    print "Parameters list: ", sorted(mob_params.keys())
    print colors.BOLD + "HMI API: " + colors.ENDC
    print "Parameters quantity: ", len(hmi_params)
    print "Parameters list: ", sorted(hmi_params.keys())
    print "\n"
    print("{}Parameters absent in Mobile APIs: {}{}".
          format(colors.WARN, mob_absent_params, colors.ENDC))
    print("{}Parameters absent in HMI APIs: {}{}".
          format(colors.WARN, hmi_absent_params, colors.ENDC))
    print "\n"


def console_print(summary_result):
    """Function which prints summary result to console"""
    for rpc_name in sorted(summary_result.keys()):
        print "\n" + "---" * 60
        print colors.HEADER + rpc_name + colors.ENDC
        for problematic_item in summary_result[rpc_name]:
            item = summary_result[rpc_name][problematic_item]
            if len(item) > 0:
                print colors.UNDERLINE + problematic_item + colors.ENDC
            if type(item) is not dict:
                print("{}{}{}".format(colors.WARN, item, colors.ENDC))
            elif type(item) is dict:
                for param in item.keys():
                    item_print = colors.UNDERLINE + param + colors.ENDC
                    print "{} {}".format("Parameter name: ", item_print)
                    res_val = item[param]
                    for key in res_val:
                        print key, ":", colors.FAIL, res_val[key], colors.ENDC


def print_summary_info(summary_result, args):
    """Function which prints summary comparison info(if any).
    Output type depends on command line args
    """
    summary_color = colors.UNDERLINE + colors.BOLD + colors.BLUE
    print "\n"
    print summary_color, "SUMMARY COMPARISON RESULT:\n", colors.ENDC
    if len(summary_result) == 0:
        print colors.BOLD + " === NO PROBLEMS FOUND ===" + colors.ENDC
        return
    if args.output == "console":
        console_print(summary_result)
    if args.output == "json":
        json_summary_result = dict_to_json(summary_result)
        print json_summary_result
    if args.output == "xml":
        json_summary_result = dict_to_json(summary_result)
        temp = json.loads(json_summary_result)
        xml_summary_result = json_to_xml(temp)
        print xml_summary_result


def handle_absent_params(area, absent_params, rpc_name, summary_result):
    """Function which writes absent parameters(if any)
    to summary result
    """
    if len(absent_params) > 0:
        fill_summary_result(
            rpc_name, area,
            absent_params,
            summary_result
        )


def get_absent_hmi_params(hmi_params, mob_params, rpc):
    """Function caculates absent parameters in hmi api"""
    hmi_absent_params = set(mob_params.keys()) - set(hmi_params.keys())
    hmi_absent_params = filter_params(
        rpc, hmi_absent_params, exclude_redundant_from_hmi_rpc_filter)
    return hmi_absent_params


def get_absent_mob_params(hmi_params, mob_params, rpc):
    """Function caculates absent parameters in mobile api"""
    mob_absent_params = set(hmi_params.keys()) - set(mob_params.keys())
    mob_absent_params = filter_params(
        rpc, mob_absent_params, exclude_params_from_mob_rpc)
    return mob_absent_params


def get_rpc_for_handle(rpc_name, hmi_params, mob_params):
    """Function creates rpc with all parameters to check"""
    rpc = type('', (), {})()  # empty object
    rpc.mobile_params = mob_params
    rpc.hmi_params = hmi_params
    rpc.rpc_name = rpc_name.rpc_name
    rpc.rpc_type = rpc_name.rpc_type
    return rpc


def check_array_mandatory_param(attributes):
    """Function checks presence of "array" mandatory parameters"""
    result = {}
    minsize_exists = "minsize" in attributes
    if not minsize_exists:
        attributes["minsize"] = None
        result["array"] = attributes["array"]
        result["minsize"] = attributes["minsize"]
    maxsize_exists = "maxsize" in attributes
    if not maxsize_exists:
        attributes["maxsize"] = None
        result["array"] = attributes["array"]
        result["maxsize"] = attributes["maxsize"]
    return result


def array_compare_rule(mob_param_attributes, hmi_param_attributes):
    """Function checks presence of necessary parameters
    if "array"="true" field presents
    """
    attr = "array"
    mobile_result = {}
    if attr in mob_param_attributes and mob_param_attributes[attr] == "true":
        mobile_result = check_array_mandatory_param(mob_param_attributes)
    hmi_result = {}
    if attr in hmi_param_attributes and hmi_param_attributes[attr] == "true":
        hmi_result = check_array_mandatory_param(hmi_param_attributes)
    return mobile_result, hmi_result


def string_compare_rule(mob_param_attributes, hmi_param_attributes):
    """Function checks presence of "minlength"="1" in HMI_API if
    "minlength" is omitted in MobileAPI.
    Should be checked only for "type"="String"
    """
    attr = "minlength"
    if attr not in mob_param_attributes:
        if attr not in hmi_param_attributes:
            return {attr: None}, {attr: None}
        elif hmi_param_attributes[attr] != "1":
            return {attr: None}, {attr: hmi_param_attributes[attr]}
        else:
            mob_param_attributes[attr] = "1"
    return {}, {}


def all_compare_rule(mob_param_attributes, hmi_param_attributes):
    """Function used for all common arrtibutes comparison"""
    mobile_result = {}
    hmi_result = {}
    attr_names = mob_param_attributes.keys() + hmi_param_attributes.keys()
    attr_names = set(attr_names)
    for attr_name in attr_names:
        mobile_attribute_value = None
        hmi_attribute_value = None
        compare_func = simple_values_comparison
        if attr_name in attributes_comparison_rules:
            # Get appropriate comparison function from attributes comparison
            # rules
            compare_func = attributes_comparison_rules[attr_name]
        if attr_name in mob_param_attributes:
            mobile_attribute_value = mob_param_attributes[attr_name]
        if attr_name in hmi_param_attributes:
            hmi_attribute_value = hmi_param_attributes[attr_name]
        values_equal = compare_func(
            mobile_attribute_value, hmi_attribute_value)
        if not values_equal:
            hmi_result[attr_name] = hmi_attribute_value
            mobile_result[attr_name] = mobile_attribute_value
    return mobile_result, hmi_result


global_compare_rules = [
    # Comparison rule when attribute "type" = "String"
    (lambda mob_param_attributes, hmi_param_attributes:
     mob_param_attributes["type"] == "String", string_compare_rule),
    # Comparison rule when attribute "array" = "true"
    (lambda mob_param_attributes, hmi_param_attributes:
     'array' in mob_param_attributes.keys() +
     hmi_param_attributes.keys(), array_compare_rule),
    # Common comparison function for all attributes
    (lambda mob_param_attributes, hmi_param_attributes:
     True, all_compare_rule)
]

attributes_comparison_rules = {
    # If "mandatory" field is omitted i MobileApi
    # it must be present in HMI_API as "mandatory"="true" by default
    'mandatory': lambda mobile_attribute_value, hmi_attribute_value:
    True if mobile_attribute_value is None and hmi_attribute_value == "true"
    else mobile_attribute_value == hmi_attribute_value,
    # type <Common.<Struct/Enum> at HMI_API (MOBILE_APi has no <Common.>)
    'type': lambda mobile_attribute_value, hmi_attribute_value:
    True if ("Common." + mobile_attribute_value) == hmi_attribute_value
    else mobile_attribute_value == hmi_attribute_value
}

# Parameters in HMI_API which should be excluded from comparison
exclude_redundant_from_hmi_rpc_filter = {
    'resultCode': lambda rpc: rpc.rpc_type != "response",
    'info': lambda rpc: rpc.rpc_type != "response",
    'success': lambda rpc: rpc.rpc_type != "response"
}

# Parameters in MOBILE_API which should be excluded from comparison
exclude_params_from_mob_rpc = {
    'type': (lambda rpc: rpc.rpc_name not in ["AddCommand", "DeleteCommand"]),
    'appID': (lambda rpc: False),
    'grammarID': (lambda rpc: False)}


def main():
    args = parse_command_line_args()
    mobile_api_tree = ElementTree.parse(args.mobile_api_file)
    mob_api_functions = {}
    extract_functions(mobile_api_tree.getroot(), mob_api_functions)

    hmi_api_tree = ElementTree.parse(args.hmi_api_file)
    hmi_api_functions = {}
    for interface in hmi_api_tree.getroot():
        extract_functions(interface, hmi_api_functions)
    summary_result = {}
    for rpc_name in sorted(mob_api_functions.keys()):
        if rpc_name in hmi_api_functions:
            hmi_params = hmi_api_functions[rpc_name]
            mob_params = mob_api_functions[rpc_name]
            rpc = get_rpc_for_handle(rpc_name, hmi_params, mob_params)
            mob_absent_params = get_absent_mob_params(
                hmi_params, mob_params, rpc)
            hmi_absent_params = get_absent_hmi_params(
                hmi_params, mob_params, rpc)
            common_params = get_common_params(
                rpc_name, mob_api_functions, hmi_api_functions)

            rpc_name = str(rpc_name.rpc_name + " " + rpc_name.rpc_type)
            if args.full_info:
                print_full_info(hmi_absent_params, hmi_params,
                                mob_absent_params, mob_params, rpc_name)
                print_parameters(common_params, hmi_params, mob_params)
            handle_absent_params("Missed in Mobile APIs:", mob_absent_params,
                                 rpc_name, summary_result)
            handle_absent_params("Missed in HMI APIs:", hmi_absent_params,
                                 rpc_name, summary_result)
            get_diff(common_params, rpc_name, hmi_params,
                     mob_params, summary_result)
    print_summary_info(summary_result, args)
    set_build_status(summary_result)


if __name__ == '__main__':
    main()
