idf_component_register(
    SRCS "src/main.cpp"
    INCLUDE_DIRS "src"
    REQUIRES uart_access ushell_core ushell_core_config ushell_core_utils ushell_user_root ushell_settings
)
