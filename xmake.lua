add_rules("plugin.vsxmake.autoupdate")
add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"})
add_rules("mode.debug", "mode.release")

-- project metadata
set_project("glint")
set_version("1.0.0")

set_languages("c17", "cxx17")
set_toolset("cxx", "clang++")
set_rundir("$(projectdir)")

-- global C & C++ flags
add_cxxflags("-Wall", "-Wextra")
add_cxflags("-Wno-unused-function")

if is_plat("macosx") then
    add_defines("PLATFORM_MACOS")
elseif is_plat("linux") then
    add_defines("PLATFORM_LINUX")
elseif is_plat("windows") then
    add_defines("PLATFORM_WINDOWS")
    set_runtimes(is_mode("debug") and "MDd" or "MD")
end

-- build configuration settings
if is_mode("debug") then
    set_symbols("debug")
    set_optimize("none")
    add_defines("BUILD_DEBUG")
else
    set_symbols("hidden")
    set_optimize("fastest")
end

-- dependencies
add_requires("spdlog")
add_requires("xxhash")
add_requires("zstd")

add_requires("glfw")
add_requires("vulkan-loader")
add_requires("vulkan-headers")
add_requires("shaderc")
add_requires("imgui", { configs = { glfw_vulkan = true }})

add_requires("joltphysics")

add_requires("tinyobjloader")
add_requires("stb")

-- engine target (library)
target("engine")
    set_kind("shared")
    set_basename("glint")

    -- dependencies
    add_packages("spdlog", "xxhash")
    add_packages("glfw", "joltphysics", "tinyobjloader", "imgui")
    add_packages("vulkan-loader", "vulkan-headers", "shaderc")

    -- headers & sources
    --set_pcheader("glint/engine/include/glint/pch.h")
    add_headerfiles("glint/engine/include/(**.h)")
    add_includedirs("glint/engine/include", {public = true})

    add_files("glint/engine/src/*.cpp")
    add_files("glint/engine/src/**/*.cpp")
target_end()

-- editor target (console)
target("editor")
    set_kind("binary")
    set_basename("glint")

    add_deps("engine")

    -- dependencies
    add_packages("joltphysics")
    add_packages("vulkan-headers")

    -- headers & sources
    --set_pcheader("glint/editor/include/glint/pch.h")
    add_headerfiles("glint/editor/include/(**.h)")
    add_includedirs("glint/editor/include")

    add_files("glint/editor/src/*.cpp")
    -- add_files("glint/editor/src/**/*.cpp")

    -- package resources
    after_build(function (target)
        local builddir = target:targetdir()
        local srcdir = path.join("$(projectdir)", "resources")
        local destdir = path.join(builddir, "resources")

        os.mkdir(destdir)
        os.cp(path.join(srcdir, "*"), destdir, {recursive = true})
    end)
target_end()