# Ecena

![esena_until_cones](https://user-images.githubusercontent.com/4519785/212568934-3b4ece06-41d9-4f03-b7c7-81686c4e0c65.png)

Ecena is a 3D Scene rendering program currently being written in C++.

The project has two main parts:
1. __Domain-specific language__. This is a Json-like language that you need to use to describe the scene you want Ecena to render. Note that Ecena doesn't offer a UI, so the only way to directly communicate with the renderer is by using the DSL (though nothing's stopping you from building your own UI).
2. __Rendering engine__. This is the one responsible for the drawing of the 3D world.

Here's a basic example of a scene written directly in Ecena's DSL:

```json
{
    "camera": {
        "h_size": 1000,
        "v_size": 600,
        "field_of_view": 1.0471975512,
        "transform": [[0, 1.5, -5], [0, 1, 0], [0, 1, 0]],
        "anti-aliasing": true
    },
    "world": {
        "light": { "position": [-10, 12, -10], "intensity": "white" },
        "objects": [
            { 
                "name": "floor",
                "description": "All the other objects will lie on top of this one",
                "type": "plane", 
                "material": {
                    "pattern": { "type": "checkers", "components": ["white", [0.5, 0.5, 0.5]]},
                    "reflectivity": 0.2
                }
            },
            {
                "type": "sphere",
                "name": "middle_sphere",
                "transform": [["translate", [-0.5, 1, 0.5]]],
                "material": { 
                    "color": [0.1, 1, 0.5],
                    "diffuse": 0.7,
                    "specular": 0.3,
                    "pattern": {
                        "type": "checkers",
                        "components": [[0.08235294117, 0.72156862745, 0], [0.1, 1, 0.5]],
                        "transform": [["scale", [0.25, 0.25, 0.25]], ["rotate_y", -0.78539816339]]
                    },
                    "reflectivity": 0.5
                } 
            },
            {
                "type": "sphere",
                "name": "left_sphere",
                "transform": [["scale", [0.33, 0.33, 0.33]], ["translate", [-1.5, 0.33, -0.75]]],
                "material": {
                    "diffuse": 0.7,
                    "specular": 0.3,
                    "pattern": {
                        "type": "ring",
                        "components": [[1, 0.8, 0.1], [1, 1, 1]],
                        "transform": [["scale", [0.33, 0.33, 0.33]], ["rotate_x", -0.72156862745]]
                    },
                    "reflectivity": 0.5
                }
            },
            {
                "type": "sphere",
                "name": "right_sphere",
                "transform": [["scale", [0.9, 0.9, 0.9]], ["translate", [2, 0.9, 2]]],
                "material": { 
                    "color": [1, 0.5, 0.5], 
                    "diffuse": 0.7, 
                    "specular": 0.3, 
                    "reflectivity": 0.5
                }
            }
        ]
    }
}
```

You can either enter that code directly into the stardard input, or save it to a file and feed it into the program. For instance, if you save it as `spheres.json`, and you have access to the Ecena executable, you can run Ecena as follows:

```
$ ecena < spheres.json
```

This will generate a scene that is a subset of the one in the image above, containing only the three big spheres and the plane (and with the right sphere transformed differently). The file will be in PPM format, which you can open using any viewing tool that supports PPMs. 

The whole example scene above corresponds to a code much more complicated that it would be more convenient to write it in a more expressive language, like Python. In fact, that's what I did. Here's the [Python script](https://github.com/melvic-ybanez/ecena/blob/main/examples/esena.py) that does that.

You can pass it to Ecena using the pipe operator:

```
$ python3 ./examples/esena.py | ecena
```

Note that the name of the file isn't a typo. It's the same name as an old project of mine Ecena is based on.

---

Primary resource for the Ray Tracer: [The Ray Tracer Challenge](http://raytracerchallenge.com/). This book
also serves as a source for test data.
