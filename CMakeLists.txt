# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

cmake_minimum_required(VERSION 3.20)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(NOT DEFINED ENV{PICO_SDK_PATH})
    message(FATAL_ERROR "PICO_SDK_PATH is not set. Set it to your pico-sdk folder.")
endif()

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(doom_picocalc C CXX ASM)

pico_sdk_init()

add_executable(doom_picocalc
    doomgeneric/dummy.c
    doomgeneric/am_map.c
    doomgeneric/doomdef.c
    doomgeneric/doomstat.c
    doomgeneric/dstrings.c
    doomgeneric/d_event.c
    doomgeneric/d_items.c
    doomgeneric/d_iwad.c
    doomgeneric/d_loop.c
    doomgeneric/d_main.c
    doomgeneric/d_mode.c
    doomgeneric/d_net.c
    doomgeneric/f_finale.c
    doomgeneric/f_wipe.c
    doomgeneric/g_game.c
    doomgeneric/hu_lib.c
    doomgeneric/hu_stuff.c
    doomgeneric/info.c
    doomgeneric/i_cdmus.c
    doomgeneric/i_endoom.c
    doomgeneric/i_joystick.c
    doomgeneric/i_scale.c
    doomgeneric/i_sound.c
    doomgeneric/i_system.c
    doomgeneric/i_timer.c
    doomgeneric/memio.c
    doomgeneric/m_argv.c
    doomgeneric/m_bbox.c
    doomgeneric/m_cheat.c
    doomgeneric/m_config.c
    doomgeneric/m_controls.c
    doomgeneric/m_fixed.c
    doomgeneric/m_menu.c
    doomgeneric/m_misc.c
    doomgeneric/m_random.c
    doomgeneric/p_ceilng.c
    doomgeneric/p_doors.c
    doomgeneric/p_enemy.c
    doomgeneric/p_floor.c
    doomgeneric/p_inter.c
    doomgeneric/p_lights.c
    doomgeneric/p_map.c
    doomgeneric/p_maputl.c
    doomgeneric/p_mobj.c
    doomgeneric/p_plats.c
    doomgeneric/p_pspr.c
    doomgeneric/p_saveg.c
    doomgeneric/p_setup.c
    doomgeneric/p_sight.c
    doomgeneric/p_spec.c
    doomgeneric/p_switch.c
    doomgeneric/p_telept.c
    doomgeneric/p_tick.c
    doomgeneric/p_user.c
    doomgeneric/r_bsp.c
    doomgeneric/r_data.c
    doomgeneric/r_draw.c
    doomgeneric/r_main.c
    doomgeneric/r_plane.c
    doomgeneric/r_segs.c
    doomgeneric/r_sky.c
    doomgeneric/r_things.c
    doomgeneric/sha1.c
    doomgeneric/sounds.c
    doomgeneric/statdump.c
    doomgeneric/st_lib.c
    doomgeneric/st_stuff.c
    doomgeneric/s_sound.c
    doomgeneric/tables.c
    doomgeneric/v_video.c
    doomgeneric/wi_stuff.c
    doomgeneric/w_checksum.c
    doomgeneric/w_file.c
    doomgeneric/w_main.c
    doomgeneric/w_wad.c
    doomgeneric/z_zone.c
    doomgeneric/w_file_stdc.c
    doomgeneric/i_input.c
    doomgeneric/i_video.c
    doomgeneric/doomgeneric.c

    doomgeneric/picocalc_present.c
    doomgeneric/picocalc_sys.c
    doomgeneric/doomgeneric_picocalc.c
)

target_include_directories(doom_picocalc PRIVATE
    doomgeneric
)

target_compile_definitions(doom_picocalc PRIVATE
    DOOMGENERIC_RESX=320
    DOOMGENERIC_RESY=200
    NORMALUNIX
    _DEFAULT_SOURCE
)

target_compile_options(doom_picocalc PRIVATE
    -Wall
    -Wno-unused-function
    -Wno-unused-variable
    -Wno-maybe-uninitialized
    -Wno-format
)

target_link_libraries(doom_picocalc
    pico_stdlib
    hardware_spi
    hardware_dma
)

pico_enable_stdio_usb(doom_picocalc 1)
pico_enable_stdio_uart(doom_picocalc 0)

pico_add_extra_outputs(doom_picocalc)