idf_component_register(
    SRCS "src/ushell_root_interface.cpp" "src/ushell_root_usercode.cpp"
    INCLUDE_DIRS "inc"
    REQUIRES ushell_core_config ushell_core_utils
)
