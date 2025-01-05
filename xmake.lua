if is_plat("macosx", "ios", "tvos", "visionos") then
    set_config("cxx", "clang++")
    set_languages("c++17")
end

includes("extlibs/harfbuzz.lua")
includes("extlibs/freetype.lua")
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

target("test")
set_kind("binary")
libmmd_source()
