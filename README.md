# Ecena

![amalgamation](https://user-images.githubusercontent.com/4519785/213571211-e6ebb123-5b34-4b79-94f9-b1df9369bf5a.png)


Ecena is a 3D Scene rendering program currently being written in C++. It takes a JSON-like object as the description of a scene and spits out the corresponding 3D world. 

The project has two main parts:
1. __Domain-specific language__. This is a Json-like language that you need to use to describe the scene you want Ecena to render. Note that Ecena doesn't offer a UI, so the only way to directly communicate with the renderer is by using the DSL (though nothing's stopping you from building your own UI).
2. __Rendering engine__. This is the one responsible for the drawing of the 3D world.

## Examples

While you can certainly write simple scenes directly in JSON, for more complicated scenes, a more expressive language that can generate JSON data is recommended. This project ships with [examples](https://github.com/melvic-ybanez/ecena/tree/main/examples), written in Python, that you can test and examine. 

The Python script for the image above, [Amalgamation](https://github.com/melvic-ybanez/ecena/blob/main/examples/amalgamation.py), can be found [here](https://github.com/melvic-ybanez/ecena/blob/main/examples/amalgamation.py). To pass its output to Ecena, you just use the pipe operator:

```
$ python3 ./examples/amalgamation.py | <path-to-ecena>/ecena
```

Of course you can also just print the output directly to see the generated JSON:

```
$ python3 ./examples/amalgamation.py
```

Here's another example, called [Esena](https://github.com/melvic-ybanez/ecena/blob/main/examples/amalgamation.py), with the corresponding script [here](https://github.com/melvic-ybanez/ecena/blob/main/examples/esena.py):

![esena_until_cones](https://user-images.githubusercontent.com/4519785/212568934-3b4ece06-41d9-4f03-b7c7-81686c4e0c65.png)

---

Primary resource for the Ray Tracer: [The Ray Tracer Challenge](http://raytracerchallenge.com/). This book
also serves as a source for test data.
