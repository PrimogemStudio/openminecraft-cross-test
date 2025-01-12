package("spirv-tools-local")
    add_deps("cmake")
    set_sourcedir(path.join(os.scriptdir(), "spirv-tools"))
    on_install(function (package)
        local configs = { 
            "-DSPIRV-Headers_SOURCE_DIR=" .. path.join(os.scriptdir(), "spirv-headers"), 
            "-DSPIRV_SKIP_TESTS=ON"
        }
        import("package.tools.cmake").install(package, configs)
    end)
package_end()

package("glslang-local")
    add_deps("cmake")
    set_sourcedir(path.join(os.scriptdir(), "glslang"))
    add_deps("spirv-tools-local")
    on_install(function (package)
        local configs = { "-DENABLE_OPT=1", "-DALLOW_EXTERNAL_SPIRV_TOOLS=ON" }
        local pth = path.join(os.scriptdir(), "glslang/glslang/CMakeLists.txt").replace("\\", "/")
        io.replace(pth, "message(\"unknown platform\")", "add_subdirectory(OSDependent/Unix)", { plain = true })
        import("package.tools.cmake").install(package, configs)
    end)
package_end()

add_requires("glslang-local", "spirv-tools-local")

target("shaderc")
set_kind("shared")
add_packages("glslang-local", "spirv-tools-local")
add_files("../src/external/placeholder.cpp")
add_includedirs("spirv-headers/include")
