-- set minimum xmake version
set_xmakever("2.8.2")

-- includes
includes ("lib/commonlibsse-ng")

-- set project
set_project("Ammo_Patcher")
set_version("3.0.0")
set_license("Apache-2.0")

-- set defaults
set_languages("c++23")
set_warnings("allextra")

-- set policies
set_policy("package.requires_lock", true)

-- add rules
add_rules("mode.debug", "mode.releasedbg")
add_rules("plugin.vsxmake.autoupdate")

-- set default mode to releasedbg
set_defaultmode("releasedbg")

-- require packages
add_requires("rapidjson", "ctre")

-- targets
target("Ammo_Patcher")
    -- add dependencies to target
    add_deps("commonlibsse-ng")

    -- add packages to target
    add_packages("rapidjson", "ctre")
    add_defines("SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE")

    -- add commonlibsse-ng plugin
    add_rules("commonlibsse-ng.plugin", {
        name = "Ammo_Patcher",
        author = "Ersh",
        description = "A simple SKSE plugin built with CommonLibSSE-NG to patch AMMO"
    })

    -- add src files
    add_files("src/**.cpp")
    add_headerfiles("include/**.h")
    add_includedirs("include")
    set_pcxxheader("include/PCH.h")

    -- postbuild: copy .dll and .pdb files to contrib/Distribution/data/skse/plugins
    after_build(function(target)
        import("core.project.depend")
        import("core.project.task")
        
        depend.on_changed(function()
            local target_file = target:targetfile()
            local symbol_file = target:symbolfile()
            local dest_dir = "contrib/Distribution/data/skse/plugins"
            
            -- ensure destination directory exists
            os.mkdir(dest_dir)
            
            -- copy .dll file
            if os.isfile(target_file) then
                local dll_dest = path.join(dest_dir, path.filename(target_file))
                os.trycp(target_file, dll_dest)
                print("Copied %s to %s", target_file, dll_dest)
            end
            
            -- copy .pdb file if it exists
            if os.isfile(symbol_file) then
                local pdb_dest = path.join(dest_dir, path.filename(symbol_file))
                os.trycp(symbol_file, pdb_dest)
                print("Copied %s to %s", symbol_file, pdb_dest)
            end
        end, { changed = target:is_rebuilt(), files = { target:targetfile() } })
    end)