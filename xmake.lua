set_languages("c++17")

includes("extlibs/freetype.lua")
includes("extlibs/xxhash.lua")
includes("extlibs/openal.lua")
includes("extlibs/meshoptimizer.lua")
includes("extlibs/yoga.lua")
includes("extlibs/shaderc.lua")
if not is_plat("iphoneos", "harmony") then
    if not is_plat("linux") or is_arch("x86_64", "x64") then 
        includes("extlibs/glfw.lua")
    end 
end

add_requires("bullet3", "glm", { system = false })
add_rules("mode.debug")
add_rules("mode.release")

function libmmd_source()
    add_packages("bullet3", "glm")
    add_files("src/libmmd/**.cpp")
    add_includedirs("include")
end

target("mmd")
set_kind("shared")
libmmd_source()

target("mmdtest")
set_kind("binary")
libmmd_source()
