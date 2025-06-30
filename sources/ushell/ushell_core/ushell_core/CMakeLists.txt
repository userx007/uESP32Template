idf_component_register(
    SRCS "src/ushell_core.cpp"
    INCLUDE_DIRS "inc"
    REQUIRES ushell_core_config ushell_core_utils ushell_settings uart_access
)
