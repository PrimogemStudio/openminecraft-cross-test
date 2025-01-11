package("glslang-mod")
    set_homepage("https://github.com/KhronosGroup/glslang/")
    set_description("Khronos-reference front end for GLSL/ESSL, partial front end for HLSL, and a SPIR-V generator.")
    set_license("Apache-2.0")

    add_urls("https://github.com/KhronosGroup/glslang.git")
    add_versions("1.2.154+1", "bacaef3237c515e40d1a24722be48c0a0b30f75f")
    add_versions("1.2.162+0", "c594de23cdd790d64ad5f9c8b059baae0ee2941d")
    add_versions("1.2.189+1", "2fb89a0072ae7316af1c856f22663fde4928128a")
    add_versions("1.3.211+0", "9bb8cfffb0eed010e07132282c41d73064a7a609")
    add_versions("1.3.231+1", "5755de46b07e4374c05fb1081f65f7ae1f8cca81")
    add_versions("1.3.236+0", "77551c429f86c0e077f26552b7c1c0f12a9f235e")
    add_versions("1.3.239+0", "ca8d07d0bc1c6390b83915700439fa7719de6a2a")
    add_versions("1.3.246+1", "14e5a04e70057972eef8a40df422e30a3b70e4b5")
    add_versions("1.3.250+1", "d1517d64cfca91f573af1bf7341dc3a5113349c0")
    add_versions("1.3.261+1", "76b52ebf77833908dc4c0dd6c70a9c357ac720bd")
    add_versions("1.3.268+0", "36d08c0d940cf307a23928299ef52c7970d8cee6")
    add_versions("1.3.275+0", "a91631b260cba3f22858d6c6827511e636c2458a")
    add_versions("1.3.280+0", "ee2f5d09eaf8f4e8d0d598bd2172fce290d4ca60")
    add_versions("1.3.283+0", "e8dd0b6903b34f1879520b444634c75ea2deedf5")
    add_versions("1.3.290+0", "fa9c3deb49e035a8abcabe366f26aac010f6cbfb")

    add_patches("1.3.246+1", "https://github.com/KhronosGroup/glslang/commit/1e4955adbcd9b3f5eaf2129e918ca057baed6520.patch", "47893def550f1684304ef7c49da38f0a8fe35c190a3452d3bf58370b3ee7165d")

    add_configs("binaryonly", {description = "Only use binary program.", default = false, type = "boolean"})
    add_configs("exceptions", {description = "Build with exception support.", default = false, type = "boolean"})
    add_configs("rtti",       {description = "Build with RTTI support.", default = false, type = "boolean"})
    add_configs("default_resource_limits",       {description = "Build with default resource limits.", default = false, type = "boolean"})
    if is_plat("wasm") then
        add_configs("shared", {description = "Build shared library.", default = false, type = "boolean", readonly = true})
    end

    add_deps("cmake", "python 3.x", {kind = "binary"})
    add_deps("spirv-tools")
    if is_plat("linux") then
        add_syslinks("pthread")
    end

    add_defines("ENABLE_HLSL")

    on_load(function (package)
        if package:config("binaryonly") then
            package:set("kind", "binary")
        end
    end)

    on_fetch(function (package, opt)
        if opt.system and package:config("binaryonly") then
            return package:find_tool("glslangValidator")
        end
    end)

    on_install(function (package)
        package:addenv("PATH", "bin")
        io.replace("CMakeLists.txt", "ENABLE_OPT OFF", "ENABLE_OPT ON")
        io.replace("StandAlone/CMakeLists.txt", "target_link_libraries(glslangValidator ${LIBRARIES})", [[
            target_link_libraries(glslangValidator ${LIBRARIES} SPIRV-Tools-opt SPIRV-Tools-link SPIRV-Tools-reduce SPIRV-Tools)
        ]], {plain = true})
        io.replace("SPIRV/CMakeLists.txt", "target_link_libraries(SPIRV PRIVATE MachineIndependent SPIRV-Tools-opt)", [[
            target_link_libraries(SPIRV PRIVATE MachineIndependent SPIRV-Tools-opt SPIRV-Tools-link SPIRV-Tools-reduce SPIRV-Tools)
        ]], {plain = true})
        -- glslang will add a debug lib postfix for win32 platform, disable this to fix compilation issues under windows
        io.replace("CMakeLists.txt", 'set(CMAKE_DEBUG_POSTFIX "d")', [[
            message(WARNING "Disabled CMake Debug Postfix for xmake package generation")
        ]], {plain = true})
        if package:is_plat("wasm") then
            -- wasm-ld doesn't support --no-undefined
            io.replace("CMakeLists.txt", [[add_link_options("-Wl,--no-undefined")]], "", {plain = true})
        end
        local configs = {"-DENABLE_CTEST=OFF", "-DBUILD_EXTERNAL=OFF"}
        table.insert(configs, "-DCMAKE_BUILD_TYPE=" .. (package:debug() and "Debug" or "Release"))
        if package:is_plat("windows") then
            table.insert(configs, "-DBUILD_SHARED_LIBS=OFF")
            if package:debug() then
                table.insert(configs, "-DCMAKE_COMPILE_PDB_OUTPUT_DIRECTORY=''")
            end
        else
            table.insert(configs, "-DBUILD_SHARED_LIBS=OFF")
        end
        table.insert(configs, "-DENABLE_EXCEPTIONS=" .. (package:config("exceptions") and "ON" or "OFF"))
        table.insert(configs, "-DENABLE_RTTI=" .. (package:config("rtti") and "ON" or "OFF"))
        table.insert(configs, "-DALLOW_EXTERNAL_SPIRV_TOOLS=ON")
        import("package.tools.cmake").install(package, configs, {packagedeps = {"spirv-tools"}})
        if not package:config("binaryonly") then
            package:add("links", "glslang", "MachineIndependent", "GenericCodeGen", "OGLCompiler", "OSDependent", "HLSL", "SPIRV", "SPVRemapper")
        end
        if package:config("default_resource_limits") then
            package:add("links", "glslang", "glslang-default-resource-limits")
        end

        -- https://github.com/KhronosGroup/glslang/releases/tag/12.3.0
        local bindir = package:installdir("bin")
        local glslangValidator = path.join(bindir, "glslangValidator" .. (is_host("windows") and ".exe" or ""))
        if not os.isfile(glslangValidator) then
            local glslang = path.join(bindir, "glslang" .. (is_host("windows") and ".exe" or ""))
            os.trycp(glslang, glslangValidator)
        end
    end)

    on_test(function (package)
        if not package:is_cross() then
            os.vrun("glslangValidator --version")
        end

        if not package:config("binaryonly") then
            assert(package:has_cxxfuncs("ShInitialize", {configs = {languages = "c++11"}, includes = "glslang/Public/ShaderLang.h"}))
        end
    end)
package_end()

package("spirv-tools-mod")
    set_homepage("https://github.com/KhronosGroup/SPIRV-Tools/")
    set_description("SPIR-V Tools")
    set_license("Apache-2.0")

    add_urls("https://github.com/KhronosGroup/SPIRV-Tools.git")
    add_versions("1.2.154+1", "sdk-1.2.154.1")
    add_versions("1.2.162+0", "sdk-1.2.162.0")
    add_versions("1.2.189+1", "sdk-1.2.189.1")
    add_versions("1.3.211+0", "sdk-1.3.211.0")
    add_versions("1.3.231+1", "sdk-1.3.231.1")
    add_versions("1.3.236+0", "sdk-1.3.236.0")
    add_versions("1.3.239+0", "sdk-1.3.239.0")
    add_versions("1.3.246+1", "sdk-1.3.246.1")
    add_versions("1.3.250+1", "sdk-1.3.250.1")
    add_versions("1.3.261+1", "sdk-1.3.261.1")
    add_versions("1.3.268+0", "vulkan-sdk-1.3.268.0")
    add_versions("1.3.275+0", "vulkan-sdk-1.3.275.0")
    add_versions("1.3.280+0", "vulkan-sdk-1.3.280.0")
    add_versions("1.3.283+0", "vulkan-sdk-1.3.283.0")
    add_versions("1.3.290+0", "vulkan-sdk-1.3.290.0")

    add_deps("cmake >=3.17.2")
    add_deps("python 3.x", {kind = "binary"})

    on_load(function (package)
        local sdkver = package:version():split("%+")[1]
        package:add("deps", "spirv-headers " .. sdkver)
    end)

    on_fetch("macosx", function (package, opt)
        if opt.system then
            -- fix missing includedirs when the system library is found on macOS
            local result = package:find_package("spirv-tools-mod")
            if result and not result.includedirs then
                for _, linkdir in ipairs(result.linkdirs) do
                    if linkdir:startswith("/usr") then
                        local includedir = path.join(path.directory(linkdir), "include", "spirv-tools-mod")
                        if os.isdir(includedir) then
                            includedir = path.directory(includedir)
                            result.includedirs = result.includedirs or {}
                            table.insert(result.includedirs, includedir)
                        end
                    end
                end
            end
            return result
        end
    end)

    on_install(function (package)
        package:addenv("PATH", "bin")
        local configs = {"-DSPIRV_SKIP_TESTS=ON", "-DSPIRV_WERROR=OFF"}
        -- walkaround for potential conflict with parallel build & debug pdb generation
        if package:debug() then
            table.insert(configs, "-DCMAKE_COMPILE_PDB_OUTPUT_DIRECTORY=''")
        end
        table.insert(configs, "-DCMAKE_BUILD_TYPE=" .. (package:debug() and "Debug" or "Release"))
        table.insert(configs, "-DBUILD_SHARED_LIBS=OFF")
        local spirv = package:dep("spirv-headers")
        table.insert(configs, "-DSPIRV-Headers_SOURCE_DIR=" .. spirv:installdir():gsub("\\", "/"))
        if package:is_plat("windows") then
            import("package.tools.cmake").install(package, configs, {buildir = os.tmpfile() .. ".dir"})
        else
            import("package.tools.cmake").install(package, configs)
        end
        package:add("links", "SPIRV-Tools-link", "SPIRV-Tools-reduce", "SPIRV-Tools-opt")
        package:add("links", "SPIRV-Tools")
    end)

    on_test(function (package)
        if not package:is_cross() then
            os.runv("spirv-as --help")
            os.runv("spirv-opt --help")
        end
        assert(package:has_cxxfuncs("spvContextCreate", {configs = {languages = "c++17"}, includes = "spirv-tools/libspirv.hpp"}))
    end)
package_end()

package("shaderc-mod")
    set_homepage("https://github.com/google/shaderc")
    set_description("A collection of tools, libraries, and tests for Vulkan shader compilation.")
    set_license("Apache-2.0")

    add_urls("https://github.com/google/shaderc/archive/refs/tags/$(version).tar.gz",
             "https://github.com/google/shaderc.git")

    add_versions("v2024.1", "eb3b5f0c16313d34f208d90c2fa1e588a23283eed63b101edd5422be6165d528")
    add_versions("v2024.0", "c761044e4e204be8e0b9a2d7494f08671ca35b92c4c791c7049594ca7514197f")
    add_versions("v2022.2", "517d36937c406858164673db696dc1d9c7be7ef0960fbf2965bfef768f46b8c0")

    add_configs("binaryonly", {description = "Only use binary program.", default = false, type = "boolean"})
    add_configs("exceptions", {description = "Enable exception handling", default = true, type = "boolean"})
    if is_plat("windows", "wasm") then
        add_configs("shared", {description = "Build shared library.", default = false, type = "boolean", readonly = true})
    end

    add_deps("cmake")

    if is_plat("linux") then
        add_syslinks("pthread")
    end

    on_load(function (package)
        if package:config("binaryonly") then
            package:set("kind", "binary")
        end

        package:add("deps", "glslang-mod")
        package:add("deps", "spirv-tools-mod", "spirv-headers")

        package:add("links", "shaderc_shared")
    end)

    on_fetch(function (package, opt)
        if opt.system and package:is_binary() then
            return package:find_tool("glslc")
        end
    end)

    on_install(function (package)
        local opt = {}
        opt.packagedeps = {"glslang-mod", "spirv-tools-mod", "spirv-headers"}
        io.replace("CMakeLists.txt", "add_subdirectory(third_party)", "", {plain = true})
        io.replace("libshaderc_util/src/compiler.cc", "SPIRV/GlslangToSpv.h", "glslang/SPIRV/GlslangToSpv.h", {plain = true})

        if not package:has_tool("sh", "link") then
            local links = {}
            for _, dep in ipairs({"glslang-mod", "spirv-tools-mod"}) do
                local fetchinfo = package:dep(dep):fetch()
                if fetchinfo then
                    for _, link in ipairs(fetchinfo.links) do
                        table.insert(links, link)
                    end
                end
            end
            if package:version():ge("2023.8") then
                io.replace("libshaderc_util/CMakeLists.txt", "glslang SPIRV", table.concat(links, " "), {plain = true})
            else
                io.replace("glslc/CMakeLists.txt", "glslang OSDependent OGLCompiler HLSL glslang SPIRV", "", {plain = true})
                io.replace("libshaderc_util/CMakeLists.txt", "glslang OSDependent OGLCompiler HLSL glslang SPIRV", table.concat(links, " "), {plain = true})
            end
            links = table.join({"shaderc", "shaderc_util"}, links)
            io.replace("glslc/CMakeLists.txt", "shaderc_util shaderc", table.concat(links, " "), {plain = true})
        end
        -- glslc --version
        local version_str = format("shaderc %s\nspirv-tools %s\nglslang %s\0", 
            package:version(),
            package:dep("spirv-tools-mod"):version(),
            package:dep("glslang-mod"):version()
        )

        -- const char[] = {'s', 'h' ...};
        local version_c_array = "{"
        for i = 1, #version_str do
            local char = version_str:sub(i, i)
            if char == "\n" then
                char = [[\n]]
            end
            if char == "\0" then
                char = [[\0]]
            end
            version_c_array = format([[%s'%s',]], version_c_array, char)
        end
        version_c_array = version_c_array .. "}"

        -- remove python codegen
        io.writefile("glslc/src/build-version.inc", version_c_array)
        io.replace("glslc/CMakeLists.txt", "add_dependencies(glslc_exe build-version)", "", {plain = true})

        local configs = {
            "-DSHADERC_SKIP_EXAMPLES=ON",
            "-DSHADERC_SKIP_TESTS=ON",
            "-DSHADERC_ENABLE_COPYRIGHT_CHECK=OFF"
        }
        table.insert(configs, "-DCMAKE_BUILD_TYPE=" .. (package:debug() and "Debug" or "Release"))
        table.insert(configs, "-DBUILD_SHARED_LIBS=ON")
        if package:is_plat("windows") then
            io.replace("CMakeLists.txt", [[set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")]], "", {plain = true})
            io.replace("CMakeLists.txt", [[set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")]], "", {plain = true})
            table.insert(configs, "-DSHADERC_ENABLE_SHARED_CRT=" .. (package:config("vs_runtime"):startswith("MT") and "OFF" or "ON"))
        end

        if package:config("exceptions") then
            table.insert(configs, "-DDISABLE_EXCEPTIONS=OFF")
            if package:is_plat("windows") and package:has_tool("cxx", "cl", "clang_cl") then
                opt.cxflags = "/EHsc"
            end
        else
            table.insert(configs, "-DDISABLE_EXCEPTIONS=ON")
        end
        import("package.tools.cmake").install(package, configs, opt)
        package:addenv("PATH", "bin")
    end)

    on_test(function (package)
        if not package:is_cross() then
            os.vrun("glslc --version")
        end

        if not package:is_binary() then
            assert(package:has_cfuncs("shaderc_compiler_initialize", {includes = "shaderc/shaderc.h"}))
        end
    end)
package_end()

add_requires("shaderc-mod", { system = false })

target("shaderc")
set_kind("shared")
add_packages("shaderc-mod")
add_files("../src/external/shaderc/helper.cpp")
