includes("harfbuzz.lua")
function libfreetype_source()
    src = {
        "autofit/autofit.c", 
        "base/ftbase.c", 
        "base/ftbbox.c", 
        "base/ftbdf.c", 
        "base/ftbitmap.c", 
        "base/ftcid.c", 
        "base/ftfstype.c", 
        "base/ftgasp.c", 
        "base/ftglyph.c", 
        "base/ftgxval.c", 
        "base/ftinit.c", 
        "base/ftmm.c", 
        "base/ftotval.c", 
        "base/ftpatent.c", 
        "base/ftpfr.c", 
        "base/ftstroke.c", 
        "base/ftsynth.c", 
        "base/fttype1.c", 
        "base/ftwinfnt.c", 
        "bdf/bdf.c", 
        "bzip2/ftbzip2.c", 
        "cache/ftcache.c", 
        "cff/cff.c", 
        "cid/type1cid.c", 
        "gzip/ftgzip.c", 
        "lzw/ftlzw.c", 
        "pcf/pcf.c", 
        "pfr/pfr.c", 
        "psaux/psaux.c", 
        "pshinter/pshinter.c", 
        "psnames/psnames.c", 
        "raster/raster.c", 
        "sdf/sdf.c", 
        "sfnt/sfnt.c", 
        "smooth/smooth.c", 
        "svg/svg.c", 
        "truetype/truetype.c", 
        "type1/type1.c", 
        "type42/type42.c", 
        "winfonts/winfnt.c", 
        "base/ftdebug.c", 
        "base/ftsystem.c"
    }

    for _, file_name in pairs(src) do
        add_files("freetype/src/" .. file_name)
    end

    add_defines("FT2_BUILD_LIBRARY=")
    add_includedirs("freetype/include")
    add_includedirs("freetype/include/freetype/config")
end

target("freetype")
set_kind("shared")
libfreetype_source()
libharfbuzz_source()
add_defines("HAVE_FREETYPE=", "HB_HAVE_FREETYPE=")