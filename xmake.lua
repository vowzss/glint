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
add_requires("spdlog")
add_requires("imgui", {configs = {glfw_vulkan=true}})

add_requires("xxhash")
add_requires("zstd")

add_requires("fastgltf")
add_requires("stb")

add_requires("joltphysics")

add_requires("glfw")
add_requires("vulkan-loader")
add_requires("vulkan-headers")
add_requires("shaderc")

-- engine target (library)
target("glint_engine")
    set_kind("static")
    set_basename("glint_engine")

    -- dependencies
    add_packages("spdlog", "xxhash")
    add_packages("glfw", "joltphysics")
    add_packages("vulkan-loader", "vulkan-headers", "shaderc")

    -- headers
    --set_pcheader("glint/engine/include/glint/pch.h")
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
    add_packages("joltphysics")
    add_packages("vulkan-headers")

    -- headers
    --set_pcheader("glint/editor/include/glint/pch.h")
    add_headerfiles("glint/editor/include/(**.h)")
    add_includedirs("glint/editor/include")

    -- sources
    add_files("glint/editor/src/*.cpp")
    add_files("glint/editor/src/**/*.cpp")

    -- package
    after_build(function (target)
        local builddir = target:targetdir()
        local srcdir = path.join("$(projectdir)", "resources")
        local destdir = path.join(builddir, "resources")

        os.mkdir(destdir)
        os.cp(path.join(srcdir, "*"), destdir, {recursive = true})
    end)
target_end()