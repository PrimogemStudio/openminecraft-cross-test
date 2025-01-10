includes("@builtin/check")

configvar_check_cfuncs("HAVE_PROC_PIDPATH", "proc_pidpath", { includes = "libproc.h" })
configvar_check_cincludes("HAVE_DLFCN_H", "dlfcn.h")
set_config("ALSOFT_EMBED_HRTF_DATA", 1)

target("openal")
set_kind("static")

set_configdir("../include/configs/openal")

add_configfiles("../include/configs/openal/config.h.in")

add_files("openal-soft/common/*.cpp")
add_includedirs("openal-soft/include")
add_includedirs("openal-soft/common")
add_includedirs("../include/configs/openal")