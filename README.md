# Raytracer

Attempting to build a minimal zero-dependency raytracing/raycasting engine for my Computer Graphics class.

This is an implementation of a simple raytracing algorithm which renders a scene to a bitmap file. The scene can be described as a [TOML](https://toml.io/en/) file (see the example in `scene.toml`).

It supports 4 basic objects:
    - Cube      (Type 1)
    - Plane     (Type 2)
    - Sphere    (Type 4)
    - Cylinder  (Type 8)


These objects can be defined in the toml file, with a specific type and attributes to be rendered.
