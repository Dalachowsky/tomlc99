if(CONFIG_TOMLC99_LIBRARY)
  add_library(TOMLC99 INTERFACE)

  target_include_directories(TOMLC99 INTERFACE
    ${ZEPHYR_TOMLC99_MODULE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
  )
  target_link_libraries(TOMLC99 INTERFACE zephyr_interface)

  zephyr_include_directories(include)
  zephyr_library()
  zephyr_library_sources(
    toml.c
    toml_wrap.c
    toml_helpers.c
  )
  zephyr_library_link_libraries(TOMLC99)
endif()
