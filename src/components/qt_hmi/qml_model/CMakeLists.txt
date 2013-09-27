cmake_minimum_required(VERSION 2.8.9)

set(components_dir ${CMAKE_CURRENT_SOURCE_DIR}/../..)
set(dbus_dir ${components_dir}/dbus)
set(codegen_dir ${dbus_dir}/codegen)
set(interfaces_dir ${components_dir}/interfaces)
add_custom_target(qml_auto ALL COMMAND python ${codegen_dir}/make_qml_dbus_qml.py --infile ${interfaces_dir}/HMI_API.xml --outdir ${CMAKE_CURRENT_SOURCE_DIR}/hmi_api/auto)
