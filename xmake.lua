
-- Project metadata
set_project("glint")
set_version("1.0.0")
set_languages("c17", "cxx17")

-- Build modes
add_rules("mode.debug", "mode.release")

-- Dependencies
add_requires("conan::spdlog/1.16.0", {alias = "spdlog" })
add_requires("conan::imgui/1.92.4", {alias = "imgui" })
add_requires("conan::glfw/3.4", {alias = "glfw" })
add_requires("conan::glm/1.0.1", {alias = "glm" })
--add_requires("tinyobjloader", {alias = "tinyobjloader"})

add_requires(is_plat("macosx") and "conan::moltenvk/1.3.0" or "conan::vulkan-loader/1.4.313.0", {alias = "vulkan-loader"})
add_requires("conan::vulkan-headers/1.4.313.0", {alias = "vulkan-headers"})
add_requires("vulkan-hpp", {alias = "vulkan-hpp"})

-- Target
target("glint")
    set_kind("binary")
    set_basename("glint")

    -- Link dependencies
    add_packages("spdlog")
    add_packages("imgui")
    add_packages("glfw", "glm")
    add_packages("vulkan-headers", "vulkan-loader", "vulkan-hpp")
    -- add_packages("tinyobjloader")

    -- Headers files
    add_headerfiles("include/(**.h)")
    add_headerfiles("third_party/(**.h)")

    -- Source files
    add_files("src/*.cpp")
    add_files("src/**/*.cpp")
    add_files("third_party/imgui/src/*.cpp")

    --
    add_includedirs("include", {public = true})
    add_includedirs("third_party/imgui/include", {public = true})

    -- Build modes
    if is_mode("debug") then
        set_symbols("debug")
        set_optimize("none")
    else
        set_symbols("hidden")
        set_optimize("fastest")
    end

    add_defines("PROJECT_ROOT=\"$(projectdir)\"")
target_end()