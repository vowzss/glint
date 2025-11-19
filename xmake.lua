
-- project metadata
set_project("glint")
set_version("1.0.0")
set_languages("c17", "cxx17")
add_cxflags("-I$(builddir)")

-- build modes
add_rules("mode.debug", "mode.release")

-- build settings
if is_mode("debug") then
    set_symbols("debug")
    set_optimize("none")
else
    set_symbols("hidden")
    set_optimize("fastest")
end

-- dependencies
add_requires("conan::spdlog/1.16.0", {alias = "spdlog" })
add_requires("conan::xxhash/0.8.3", {alias = "hash"})

add_requires("conan::imgui/1.92.4", {alias = "imgui" })

add_requires("conan::glfw/3.4", {alias = "glfw" })
add_requires("conan::glm/1.0.1", {alias = "glm" })
--add_requires("tinyobjloader", {alias = "tinyobjloader"})

add_requires(is_plat("macosx") and "conan::moltenvk/1.3.0" or "conan::vulkan-loader/1.4.313.0", {alias = "vulkan-loader"})
add_requires("conan::vulkan-headers/1.4.313.0", {alias = "vulkan-headers"})
add_requires("vulkan-hpp", {alias = "vulkan-hpp"})
add_requires("conan::shaderc/2025.3", {alias = "shaderc"})

-- engine target (library)
target("glint_engine")
    set_kind("shared")
    set_basename("glint_engine")

    -- dependencies
    add_packages("spdlog", "hash")
    add_packages("glm")
    add_packages("vulkan-headers", "vulkan-loader", "vulkan-hpp", "shaderc")

    -- headers
    set_pcheader("glint/engine/include/glint/pch.h")
    add_headerfiles("glint/engine/include/(**.h)")
    add_includedirs("glint/engine/include", {public = true})

    -- sources
    add_files("glint/engine/src/*.cpp")
    add_files("glint/engine/src/**/*.cpp")
target_end()

-- editor target (console)
target("glint_editor")
    set_kind("binary")
    set_basename("glint_editor")

    -- link
    add_deps("glint_engine")

    -- dependencies
    add_packages("imgui")
    add_packages("glfw")
    add_packages("vulkan-loader", "vulkan-headers", "vulkan-hpp")

    -- headers
    set_pcheader("glint/editor/include/glint/pch.h")
    add_headerfiles("glint/editor/include/(**.h)")
    add_headerfiles("glint/editor/third_party/(**.h)")
    add_includedirs("glint/editor/include")
    add_includedirs("glint/editor/third_party/imgui/include")

    -- sources
    add_files("glint/editor/src/*.cpp")
    add_files("glint/editor/third_party/imgui/src/*.cpp")

    -- package
    after_build(function (target)
        local builddir = target:targetdir()
        local srcdir = path.join("$(projectdir)", "resources")
        local destdir = path.join(builddir, "resources")

        os.mkdir(destdir)
        os.cp(path.join(srcdir, "*"), destdir, {recursive = true})
    end)
target_end()