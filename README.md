# Ecena
A 3D Scene Renderer currently being written in C++.

The project has two main parts:
1. __Domain-specific language__. This is a Json-like language that you need to use to describe the scene you want Ecena to render. Note that Ecena doesn't offer a UI, so the only way to communicate with the renderer is by using the DSL.
2. __Rendering engine__. This is the one responsible for the drawing of the 3D world.

Here's a basic example of a rendered scene:

![esenca_basic_with_antialias](https://user-images.githubusercontent.com/4519785/211681017-17db5e46-8f90-461e-b78b-5012f3c48fa3.png)

To generate that scene, you need to input the following description using the DSL:

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
        "light": { "position": [-10, 12, -10], "intensity": [1, 1, 1] },
        "objects": [
            { 
                "name": "floor",
                "description": "All the other objects will lie on top of this one",
                "type": "plane", 
                "material": {
                    "pattern": { "type": "checkers", "components": [[1, 1, 1], [0.5, 0.5, 0.5]]},
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
                "material": { "color": [1, 0.5, 0.5], "diffuse": 0.7, "specular": 0.3, "reflectivity": 0.5}
            }
        ]
    }
}
```

You can either enter that code directly into the stardard input, or save it to a file and feed it into the program. For instance, if you save it as `spheres.json`, and you have
access to the Ecena execuatble, you can run Ecena as follows:

```
$ ecena < spheres.json
```

Depending on how complicated the scene you want to render is, you might find it more convenient to use a more expressive language, like Python. Using Python will allow you to assign
names to your numerical values, remove duplication via abstractions and various language constructs, and use more readable form of expressions. However, you still need to convert the Python object into Json (e.g. by printing it to the standard output) before passing it to Ecena.

The Python equivalent of the Json code above can be found at _examples/esena/spheres.py_. 

An easy way to input a Python script is to use the pipe operator:

```
$ python3 ./examples/esena/spheres.py | ecena
```

---

Primary resource for the Ray Tracer: [The Ray Tracer Challenge](http://raytracerchallenge.com/). This book
also serves as a source for test data.
