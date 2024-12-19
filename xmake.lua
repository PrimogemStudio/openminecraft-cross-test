add_requires("bullet3")
add_requires("glm")
add_rules("mode.debug")
add_rules("mode.release")

function files_conf()
    add_packages("bullet3")
    add_packages("glm")
    -- add_files("src/**.c")
    -- add_files("src/**.cc")
    add_files("src/**.cpp")
    add_includedirs("include")
end

target("mmd")
set_kind("shared")
files_conf()

target("test")
set_kind("binary")
files_conf()
