const std = @import("std");
const rl = @import("raylib");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const raylib_dep = b.dependency("raylib", .{
        .target = target,
        .optimize = optimize,
        .raudio = false,
        .rmodels = false,
    });
    const raylib_artifact = raylib_dep.artifact("raylib");

    const raygui_dep = b.dependency("raygui", .{
        .target = target,
        .optimize = optimize,
    });

    rl.addRaygui(b, raylib_artifact, raygui_dep);

    raylib_artifact.addIncludePath(raylib_dep.path("src"));
    raylib_artifact.addIncludePath(raygui_dep.path("src"));

    var scrabble = b.addStaticLibrary(.{
        .name = "scrabble",
        .target = target,
        .optimize = optimize,
        .link_libc = true,
    });

    scrabble.addCSourceFiles(.{
        .files = &.{
            "src/arbol_diccionario.c",
            "src/scrabble.c",
        },
    });

    b.installArtifact(scrabble);

    var graphics_mod = b.createModule(.{
        .target = target,
        .optimize = optimize,
        .link_libc = true,
    });

    graphics_mod.linkLibrary(scrabble);
    graphics_mod.linkLibrary(raylib_artifact);

    graphics_mod.addCSourceFile(.{
        .file = b.path("src/graphic.c"),
    });

    graphics_mod.addCMacro("ASSETS_PATH", "\"./assets/\"");

    const graphics = b.addLibrary(.{
        .name = "graphics",
        .root_module = graphics_mod,
        .linkage = .dynamic,
    });

    b.installArtifact(graphics);

    var exe = b.addExecutable(.{
        .name = "game",
        .target = target,
        .optimize = optimize,
        .link_libc = true,
    });

    exe.linkLibrary(graphics);

    exe.addCSourceFile(.{ .file = b.path("src/main.c") });

    b.installArtifact(exe);

    b.installDirectory(.{
        .source_dir = b.path("assets/"),
        .install_dir = .bin,
        .install_subdir = "./assets/",
    });
    b.installBinFile("palabras.txt", "palabras.txt");
}
