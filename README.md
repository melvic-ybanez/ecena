# Ecena

![amalgamation_with_colored_texture](https://user-images.githubusercontent.com/4519785/219709904-41c24b24-d54d-49ef-af1e-bb807bd0376f.png)

Ecena is a 3D Scene rendering program currently being written in C++. It takes a JSON-like object as the description of a scene and spits out the corresponding 3D world. 

The project has two main parts:
1. [Domain-specific language](https://github.com/melvic-ybanez/ecena/wiki/DSL). This is a Json-like language that you need to use to describe the scene you want Ecena to render. Note that Ecena doesn't offer a UI, so the only way to directly communicate with the renderer is by using the DSL (though nothing's stopping you from building your own UI).
To learn more about the DSL, head to the [DSL wiki page](https://github.com/melvic-ybanez/ecena/wiki/DSL) 
2. __Rendering engine__. This is the one responsible for the drawing of the 3D world.

## Examples

While you can certainly write simple scenes directly in JSON, for more complicated scenes, a more expressive language that can generate JSON data is recommended. This project ships with [examples](https://github.com/melvic-ybanez/ecena/tree/main/examples), written in Python, that you can test and examine. 

The Python script for the image above can be found [here](https://github.com/melvic-ybanez/ecena/blob/main/examples/amalgamation.py). To pass its output to Ecena, you just use the pipe operator:

```
$ python3 ./examples/amalgamation.py | <path-to-ecena>/ecena
```

Of course, you can also just print the output directly to see the generated JSON:

```
$ python3 ./examples/amalgamation.py
```

Here's another example, called [Esena](https://github.com/melvic-ybanez/ecena/blob/main/examples/amalgamation.py) (named after another Ray Tracer project of mine, which outputs only a single image, a variant of the one below), with the corresponding script [here](https://github.com/melvic-ybanez/ecena/blob/main/examples/esena.py):

![esena_until_triangles](https://user-images.githubusercontent.com/4519785/216436883-2efa74e3-290d-41c5-966d-e7d54c516000.png)

---

Primary resource for the Ray Tracer: [The Ray Tracer Challenge](http://raytracerchallenge.com/). This book
also serves as a source for test data and scenarios.
