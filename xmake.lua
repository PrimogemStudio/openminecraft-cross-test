add_requires("bullet3")
add_rules("mode.debug")
add_rules("mode.release")
add_rules("mode.minsizerel")

target("test")
set_kind("shared")
add_packages("bullet3")
add_files("test.c")
