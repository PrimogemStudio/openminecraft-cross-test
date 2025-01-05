function libfreetype_source()
    src = {
        "autofit/autofit.c", 
        "base/ftbase.c"
    }

    for _, file_name in pairs(src) do
        add_files("freetype/src/" .. file_name)
    end

    add_defines("FT2_BUILD_LIBRARY=")
    add_includedirs("freetype/include")
    add_includedirs("freetype/include/freetype/config")

    if is_plat("macosx", "ios", "tvos", "visionos") then
        set_toolset("cc", "clang++")
    end
end

target("freetype")
set_kind("static")
libfreetype_source()