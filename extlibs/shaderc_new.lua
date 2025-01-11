--package("shaderc-local")
--    add_deps("cmake", "python")
--    set_sourcedir(path.join(os.scriptdir(), "shaderc"))
--    on_install(function (package)
--        local configs = { "-DBUILD_SHARED_LIBS=OFF", "-DSHADERC_SKIP_EXAMPLES=ON", "-DSHADERC_SKIP_TESTS=ON", "-DSHADERC_ENABLE_COPYRIGHT_CHECK=OFF"}
--        os.exec("python ./utils/git-sync-deps")
--        import("package.tools.cmake").install(package, configs)
--    end)
package_end()

add_requires("shaderc", { system = false, configs = { shared = true } })

target("shaderc-wrap")
set_kind("binary")
add_packages("shaderc")
add_files("../src/external/shaderc/helper.cpp")
--add_links("shaderc_shared")
