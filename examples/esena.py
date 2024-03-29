import copy
import json
import math


def data():
    return {
        "camera": camera(),
        "world": world()
    }


def camera():
    return {
        "h_size": 1000,
        "v_size": 600,
        "field_of_view": math.pi / 3,
        "transform": [[0, 1.5, -5], [0, 1, 0], [0, 1, 0]],
        "anti-aliasing": True
    }


def world():
    return {
        "light": {"position": [-10, 12, -10], "intensity": "white"},
        "objects": objects()
    }


def left_sphere():
    return {
        "type": "sphere",
        "name": "left_sphere",
        "transform": [["scale", [0.33, 0.33, 0.33]], ["translate", [-1.5, 0.33, -0.75]]],
        "material": {
            "diffuse": 0.7,
            "specular": 0.3,
            "pattern": {
                "type": "ring",
                "components": [[1, 0.8, 0.1], 'white'],
                "transform": [["scale", [0.33, 0.33, 0.33]], ["rotate_x", -math.pi / 4]]
            },
            "reflectivity": 0.5
        }
    }


def floor():
    return {
        "name": "floor",
        "description": "All the other objects will lie on top of this one",
        "type": "plane",
        "material": {
            "pattern": {"type": "checkers", "components": ['white', [0.5, 0.5, 0.5]]},
            "reflectivity": 0.2
        }
    }


def middle_sphere():
    return {
        "type": "sphere",
        "name": "middle_sphere",
        "transform": [["translate", [-0.5, 1, 0.5]]],
        "material": {
            "color": [0.1, 1, 0.5],
            "diffuse": 0.7,
            "specular": 0.3,
            "pattern": {
                "type": "checkers",
                "components": [[21.0 / 255, 184.0 / 255, 0], [0.1, 1, 0.5]],
                "transform": [["scale", [0.25, 0.25, 0.25]], ["rotate_y", -math.pi / 4]]
            },
            "reflectivity": 0.5
        }
    }


def back():
    return {
        'type': 'group',
        'children': [
            {
                "type": "cube",
                "name": "right_cube",
                "transform": [["scale", [0.7, 0.7, 0.7]], ["rotate_y", math.pi / 4], ["translate", [1.1, 0.7, 3]]],
                "material": {
                    "diffuse": 0.7,
                    "specular": 0.3,
                    "pattern": {
                        "type": "checkers",
                        "components": [[1, 0.8, 0.1], [1, 1, 1]],
                        "transform": [["scale", [0.33, 0.33, 0.33]], ["rotate_x", -math.pi / 4]]
                    }
                }
            },
            {
                "type": "sphere",
                "name": "right_sphere",
                "transform": [["scale", [0.7, 0.7, 0.7]], ["translate", [1.1, 2.1, 3]]],
                "material": {"color": [1, 0.5, 0.5], "diffuse": 0.7, "specular": 0.3, "reflectivity": 0.5}
            }
        ]
    }


def small_spheres():
    children = []
    for i in range(0, 5):
        component_scale = 0.5 + 0.1 * i
        pattern = {"type": "gradient", "components": [[1, 0.8, 0.1], [220.0 / 255, 20.0 / 255, 60.0 / 255]]}

        small_sphere = copy.deepcopy(left_sphere())
        del small_sphere['name']

        small_sphere['transform'] += [["scale", [component_scale, component_scale, component_scale]],
                                      ["translate", [i, 0, 0]]]
        small_sphere['material']['color'] = [0.5, 0.6, 1]
        small_sphere['material']['pattern'] = pattern if i % 2 == 0 else None

        children.append(small_sphere)

    return {
        'type': 'group',
        'children': children
    }


def cylinders():
    colors = [[40, 103, 160], [72, 120, 170], [99, 141, 187], [121, 158, 196], [157, 179, 208]]
    offset_scale = 0.8
    acc = [
        {
            "type": "cylinder",
            "minimum": -0.1,
            "maximum": 0.1,
            "material": {
                "color": [7.0 / 255, 87.0 / 255, 152.0 / 255]
            },
            "transform": [["scale", [offset_scale, 1, offset_scale]], ["translate", [2, 0.1, 0.5]]]
        }
    ]
    for i in range(0, 5):
        last = acc[-1]
        scale_factor = offset_scale - ((i + 1) * 0.2)
        if scale_factor < 0.2:
            scale_factor = offset_scale / math.pow(2, i)
        r, g, b = colors[i]

        new_mat = copy.deepcopy(last['material'])
        new_mat['color'] = [r / 255.0, g / 255.0, b / 255.0]

        new_cyl = {
            'type': 'cylinder',
            'minimum': last['minimum'] - 0.1,
            'maximum': last['maximum'] + 0.1,
            'material': new_mat,
            'transform': [['scale', [scale_factor, 1, scale_factor]], ['translate', [2, last['maximum'] + 0.1, 0.5]]]
        }

        acc.append(new_cyl)

    return {
        'type': 'group',
        'children': acc
    }


def glasses():
    upper_base = {
        "type": "cylinder",
        "closed": True,
        "minimum": -0.025,
        "maximum": 0.025,
        "transform": [['scale', [0.3, 1.0, 0.3]], ['translate', [0.7, 0.575, -1.5]]],
        'material': glass_mat()
    }
    body = {
        'type': 'cylinder',
        'closed': True,
        'minimum': -0.275,
        'maximum': 0.275,
        'transform': [['scale', [0.05, 1.0, 0.05]], ['translate', [0.7, 0.275, -1.5]]],
        'material': glass_mat()
    }
    sphere = {
        'type': 'sphere',
        'transform': [['scale', [0.25, 0.25, 0.25]], ['translate', [0.7, 0.85, -1.5]]],
        'material': glass_mat()
    }
    small_sphere = {
        'type': 'sphere',
        'transform': [['scale', [0.15, 0.15, 0.15]], ['translate', [0.7, 1.25, -1.5]]],
        'material': glass_mat()
    }

    return {
        'type': 'group',
        'children': [upper_base, body, sphere, small_sphere]
    }


def glass_mat():
    return {
        'color': 'white',
        'diffuse': 0.01,
        'specular': 1,
        'shininess': 300,
        'transparency': 1,
        'reflectivity': 1,
        'refractive_index': 1.5
    }


def cones():
    base_color = [1.0, 168.0 / 255.0, 18.0 / 255.0]
    cone = {
        'type': 'cone',
        'minimum': -1.0,
        'maximum': 0,
        'closed': True,
        'material': {
            'pattern': {
                'type': 'stripe',
                'components': ['white', base_color],
                'transform': [['scale', [0.15, 0.15, 0.15]], ['rotate_z', math.pi / 2.0]]
            }
        },
        'transform': [['scale', [0.5, 1.5, 0.5]], ['translate', [-3.5, 1.6, 4.5]]]
    }
    base = {
        'type': 'cylinder',
        'closed': True,
        'minimum': -0.1,
        'maximum': 0.1,
        'material': {
            'color': base_color
        },
        'transform': [['scale', [0.6, 1.0, 0.6]], ['translate', [-3.5, 0.1, 4.5]]]
    }
    return {
        'type': 'group',
        'children': [base, cone]
    }


def triangles():
    def side():
        return {
            'type': 'triangle',
            'points': [[0, math.sqrt(2), 0], [-1, 0, 0], [1, 0, 0]],
            'material': glass_mat(),
        }

    side1 = side()
    side1['transform'] = [['rotate_x', math.pi / 4]]

    side2 = side()
    side2['transform'] = [['rotate_y', math.pi / 4 * 2], ['rotate_z', -math.pi / 4], ['translate', [-1, 0, 1]]]

    side3 = side()
    side3['transform'] = [['rotate_x', -math.pi / 4], ['translate', [0, 0, 2]]]

    side4 = side()
    side4['transform'] = [['rotate_y', -math.pi / 4 * 2], ['rotate_z', math.pi / 4], ['translate', [1, 0, 1]]]

    sphere = {
        'type': 'sphere',
        'material': {'color': [0.1, 0.1, 0.6], 'reflective': 0.5},
        'transform': [['scale', [0.2, 0.2, 0.2]], ['translate', [-0.5, 0.2, 0.5]]]
    }

    cylinder = {
        'type': 'cylinder',
        'min': -0.2,
        'max': 0.2,
        'closed': True,
        'material': {'color': [0.6, 0.1, 0.1], 'reflective': 0.5},
        'transform': [['scale', [0.2, 1.0, 0.2]], ['translate', [0, 0.2, 1]]]
    }

    cube = {
        'type': 'cube',
        'material': {'color': [0.1, 0.6, 0.1], 'reflective': 0.5},
        'transform': [['scale', [0.17, 0.17, 0.17]], ['translate', [0.3, 0.17, 0.5]]]
    }

    return {
        'type': 'group',
        'children': [side1, side2, side3, side4, sphere, cylinder, cube],
        'transform': [['translate', [4.5, 0, 4]]]
    }


def objects():
    return [
        {
            'type': 'group',
            'children': [middle_sphere(), back(), left_sphere(), small_spheres(), cylinders(), glasses(), cones(),
                         triangles()],
            'threshold': 2
        },
        floor()
    ]


print(json.dumps(data(), indent=2))
