SOLUTION_NAME = "Chip8"

MAIN_PROJECT_NAME = "EmulatorChip8"

BIN_DIR = "Generated/bin/%{prj.name}/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
OBJ_DIR = "Generated/obj/%{prj.name}/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

workspace (SOLUTION_NAME)
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }


project (MAIN_PROJECT_NAME)
    location (MAIN_PROJECT_NAME)
    kind "WindowedApp"
    language "C++"

    targetdir (BIN_DIR)
    objdir (OBJ_DIR)

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.tpp",
        "%{prj.name}/src/**.cpp"
    }

    includedirs 
    {
        "%{prj.name}/src"
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"
        defines "PLATFORM_WINDOWS"

    filter "configurations:Debug"
        symbols "on"
        defines "DEBUG"
    
    filter "configurations:Release"
        optimize "on"
        defines "RELEASE"