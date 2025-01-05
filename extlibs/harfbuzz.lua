function libharfbuzz_source()
    src = {
        "OT/Var/VARC/VARC.cc", 
        "hb-aat-layout.cc", 
        "hb-aat-map.cc", 
        "hb-blob.cc", 
        "hb-buffer-serialize.cc", 
        "hb-buffer-verify.cc", 
        "hb-buffer.cc", 
        "hb-common.cc", 
        "hb-coretext-font.cc", 
        "hb-coretext-shape.cc", 
        "hb-directwrite.cc", 
        "hb-draw.cc", 
        "hb-face-builder.cc", 
        "hb-face.cc", 
        "hb-fallback-shape.cc", 
        "hb-font.cc", 
        "hb-ft.cc", 
        "hb-gdi.cc", 
        "hb-glib.cc", 
        "hb-graphite2.cc", 
        "hb-map.cc", 
        "hb-number.cc", 
        "hb-ot-cff1-table.cc", 
        "hb-ot-cff2-table.cc", 
        "hb-ot-color.cc", 
        "hb-ot-face.cc", 
        "hb-ot-font.cc", 
        "hb-ot-layout.cc", 
        "hb-ot-map.cc", 
        "hb-ot-math.cc", 
        "hb-ot-meta.cc", 
        "hb-ot-metrics.cc", 
        "hb-ot-name.cc", 
        "hb-ot-shape-fallback.cc", 
        "hb-ot-shape-normalize.cc", 
        "hb-ot-shape.cc", 
        "hb-ot-shaper-arabic.cc", 
        "hb-ot-shaper-default.cc", 
        "hb-ot-shaper-hangul.cc", 
        "hb-ot-shaper-hebrew.cc", 
        "hb-ot-shaper-indic-table.cc", 
        "hb-ot-shaper-indic.cc", 
        "hb-ot-shaper-khmer.cc", 
        "hb-ot-shaper-myanmar.cc", 
        "hb-ot-shaper-syllabic.cc", 
        "hb-ot-shaper-thai.cc", 
        "hb-ot-shaper-use.cc", 
        "hb-ot-shaper-vowel-constraints.cc", 
        "hb-ot-tag.cc", 
        "hb-ot-var.cc", 
        "hb-outline.cc", 
        "hb-paint-extents.cc", 
        "hb-paint.cc", 
        "hb-set.cc", 
        "hb-shape-plan.cc", 
        "hb-shape.cc", 
        "hb-shaper.cc", 
        "hb-static.cc", 
        "hb-style.cc", 
        "hb-ucd.cc", 
        "hb-unicode.cc", 
        "hb-uniscribe.cc", 
        "hb-wasm-api.cc", 
        "hb-wasm-shape.cc", 
        "graph/gsubgpos-context.cc", 
        "hb-subset-cff-common.cc", 
        "hb-subset-cff1.cc", 
        "hb-subset-cff2.cc", 
        "hb-subset-input.cc", 
        "hb-subset-instancer-iup.cc", 
        "hb-subset-instancer-solver.cc", 
        "hb-subset-plan.cc", 
        "hb-subset-repacker.cc", 
        "hb-subset.cc"
    }

    for _, file_name in pairs(src) do
        add_files("harfbuzz/src/" .. file_name)
    end
    
    add_includedirs("harfbuzz/src")
end

target("harfbuzz")
set_kind("shared")
libharfbuzz_source()