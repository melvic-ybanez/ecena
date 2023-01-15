import copy
import json
import math

left_sphere = {
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

objects = [
    {
        "name": "floor",
        "description": "All the other objects will lie on top of this one",
        "type": "plane",
        "material": {
            "pattern": {"type": "checkers", "components": ['white', [0.5, 0.5, 0.5]]},
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
                "components": [[21.0 / 255, 184.0 / 255, 0], [0.1, 1, 0.5]],
                "transform": [["scale", [0.25, 0.25, 0.25]], ["rotate_y", -math.pi / 4]]
            },
            "reflectivity": 0.5
        }
    },
    left_sphere,
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

camera = {
    "h_size": 1000,
    "v_size": 600,
    "field_of_view": math.pi / 3,
    "transform": [[0, 1.5, -5], [0, 1, 0], [0, 1, 0]],
    "anti-aliasing": True
}

world = {
    "light": {"position": [-10, 12, -10], "intensity": "white"},
    "objects": objects
}

data = {
    "camera": camera,
    "world": world
}

for i in range(0, 5):
    component_scale = 0.5 + 0.1 * i
    pattern = {"type": "gradient", "components": [[1, 0.8, 0.1], [220.0 / 255, 20.0 / 255, 60.0 / 255]]}

    small_sphere = copy.deepcopy(left_sphere)
    del small_sphere['name']

    small_sphere['transform'] += [["scale", [component_scale, component_scale, component_scale]],
                                  ["translate", [i, 0, 0]]]
    small_sphere['material']['color'] = [0.5, 0.6, 1]
    small_sphere['material']['pattern'] = pattern if i % 2 == 0 else None

    objects.append(small_sphere)


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

    return acc


def glasses():
    upper_base = {
        "type": "cylinder",
        "closed": True,
        "minimum": -0.025,
        "maximum": 0.025,
        "transform": [['scale', [0.3, 1.0, 0.3]], ['translate', [0.7, 0.575, -1.5]]],
        'material': 'glass'
    }
    body = {
        'type': 'cylinder',
        'closed': True,
        'minimum': -0.275,
        'maximum': 0.275,
        'transform': [['scale', [0.05, 1.0, 0.05]], ['translate', [0.7, 0.275, -1.5]]],
        'material': 'glass'
    }
    sphere = {
        'type': 'sphere',
        'transform': [['scale', [0.25, 0.25, 0.25]], ['translate', [0.7, 0.85, -1.5]]],
        'material': 'glass'
    }
    small_sphere = {
        'type': 'sphere',
        'transform': [['scale', [0.15, 0.15, 0.15]], ['translate', [0.7, 1.25, -1.5]]],
        'material': 'glass'
    }

    return [upper_base, body, sphere, small_sphere]


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
    return [base, cone]


objects += cylinders() + glasses() + cones()

print(json.dumps(data, indent=2))
