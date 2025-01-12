package("glslang-local")
    add_deps("cmake")
    set_sourcedir(path.join(os.scriptdir(), "glslang"))
    on_install(function (package)
        local configs = { "-DENABLE_OPT=0" }
        local pth = path.join(os.scriptdir(), "glslang/glslang/CMakeLists.txt")
        io.replace(pth, "message(\"unknown platform\")", "add_subdirectory(OSDependent/Unix)", { plain = true })
        import("package.tools.cmake").install(package, configs)
    end)
package_end()

add_requires("glslang-local")

target("shaderc")
set_kind("shared")
add_packages("glslang-local")
add_files("../src/external/placeholder.cpp")