# Ecena
A 3D Scene Renderer currently being written in C++.

The project has two main parts:
1. __Domain-specific language__ - this is a Json-like language that you need to use to describe the scene you want Ecena to render. Note that Ecena doesn't offer a UI, so the only way to communicate with the renderer is by using the DSL.
2. __Rendering engine__ - the Ray Tracer modules responsible for the drawing of the 3D world.

For instance, to render the example scene from the _Making a Scene_ chapter of [The Ray Tracer Challenge](http://raytracerchallenge.com/) (with the addition of shadows), 
you can input the following code:

```json
{
    "camera": {
        "h_size": 700,
        "v_size": 350,
        "field_of_view": 1.0471975512,
        "transform": [[0, 1.5, -5], [0, 1, 0], [0, 1, 0]]
    },
    "world": {
        "light": { "position": [-10, 10, -10], "intensity": [1, 1, 1] },
        "objects": [
            { 
                "type": "sphere", 
                "transform": [["scale", [10, 0.01, 10]]], 
                "material": { "color": [1, 0.9, 0.9], "specular": 0 } 
            },
            { 
                "type": "sphere", 
                "transform": [
                    ["scale", [10, 0.01, 10]], 
                    ["rotate_x",  1.57079632679], 
                    ["rotate_y", -0.78539816339], 
                    ["translate", [0, 0, 5]]
                ], 
                "material": { "color": [1, 0.9, 0.9], "specular": 0 }
            },
            { 
                "type": "sphere", 
                "transform": [
                    ["scale", [10, 0.01, 10]], 
                    ["rotate_x",  1.57079632679], 
                    ["rotate_y", 0.78539816339], 
                    ["translate", [0, 0, 5]]
                ], 
                "material": { "color": [1, 0.9, 0.9], "specular": 0 }
            },
            { 
                "type": "sphere", 
                "transform": [["translate", [-0.5, 1, 0.5]]],
                "material": { "color": [0.1, 1, 0.5], "diffuse": 0.7, "specular": 0.3 }
            },
            { 
                "type": "sphere", 
                "transform": [["scale", [0.5, 0.5, 0.5]], ["translate", [1.5, 0.5, -0.5]]],
                "material": { "color": [0.5, 1, 0.1], "diffuse": 0.7, "specular": 0.3 }
            },
            { 
                "type": "sphere", 
                "transform": [["scale", [0.33, 0.33, 0.33]], ["translate", [-1.5, 0.33, -0.75]]],
                "material": { "color": [1, 0.8, 0.1], "diffuse": 0.7, "specular": 0.3 }
            }
        ]
    }
}
```

You can either enter that code directly into the stardard input, or save it to a file and feed it into the program. For instance, if you save it as `spheres.json`, you can run Ecena as follows:

```
$ ecena < spheres.json
```

You should get the following result: 

![shadowed_spheres](https://user-images.githubusercontent.com/4519785/210240177-7d0a4f94-cf7b-45dd-b1a9-f0b411f110ad.png)

---

Primary resource for the Ray Tracer: [The Ray Tracer Challenge](http://raytracerchallenge.com/).
