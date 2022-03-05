set(srcs
    "cmd_nvs.c"
    "cmd_system.c"
    "cmd_wifi.c"
    "custom_console.c")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS .
                    REQUIRES console nvs_flash spi_flash)
