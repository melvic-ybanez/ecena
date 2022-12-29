# Ecena
A 3D Scene Renderer currently being written in C++.

The project has two main parts:
1. __Domain-specific language__ - this is what the users will use to describe the scene they want Ecena to render. It is a subset of the Json language.
2. __Rendering engine__ - the Ray Tracer modules responsible for the drawing of the 3D world.

Here's a simple example:

```json
{
    "shapes": [
        {
            "type": "sphere",
            "material": {
                "color": [0.5, 0, 0.5]
            }
        }
    ],
    "ray_origin": [0, 0, -5],
    "wall": {
        "location": [0, 0, 10],
        "size": [7, 7]
    },
    "canvas": [300, 300],
    "point_light": {
        "position": [-10, 10, -10],
        "intensity": [1, 1, 1]
    }
}
```

You can either enter that code directly into the stardard input, or save it to a file and feed it into the program. For instance, if you save it as `purple_ball.json`, you can run Ecena as follows:

```
$ ecena < purple_ball.json
```

You should get the following result: 

![purple_sphere](https://user-images.githubusercontent.com/4519785/209990555-50589f76-d643-435b-ac76-4c55da5366cc.png)

---

Primary resource for the Ray Tracer: [The Ray Tracer Challenge](http://raytracerchallenge.com/).
