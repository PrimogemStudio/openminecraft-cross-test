add_requires("bullet3", "glm", "opengl")
add_rules("mode.debug")
add_rules("mode.release")

function libmmd_source()
    add_packages("bullet3", "glm", "opengl")
    add_files("src/**.cpp")
    add_includedirs("include")
end

target("mmd")
set_kind("shared")
libmmd_source()

target("test")
set_kind("binary")
libmmd_source()
