set_languages("c++17")

includes("extlibs/freetype.lua")
includes("extlibs/xxhash.lua")
includes("extlibs/openal.lua")

add_requires("bullet3", "glm")
add_rules("mode.debug")
add_rules("mode.release")

function libmmd_source()
    add_packages("bullet3", "glm")
    add_files("src/**.cpp")
    add_includedirs("include")
end

target("mmd")
set_kind("shared")
libmmd_source()

target("mmdtest")
set_kind("binary")
libmmd_source()
