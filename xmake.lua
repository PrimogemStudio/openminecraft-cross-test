add_requires("bullet3")

target("test")
set_kind("shared")
add_packages("bullet3")
add_files("test.c")
