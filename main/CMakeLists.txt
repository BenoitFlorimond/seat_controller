idf_component_register( SRCS            "main.c" 
                                        "../fwk/utils/src/os_utils.c"
                                        "../fwk/drivers/src/buttons_driver.c"
                                        "../fwk/drivers/src/leds_driver.c"
                        INCLUDE_DIRS    "inc"
                                        "../fwk/utils/inc"
                                        "../fwk/drivers/inc")