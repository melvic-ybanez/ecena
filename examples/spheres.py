import json
import math


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
        'transform': [['scale', [0.25, 0.75, 0.25]], ['translate', [0, 1.76, 0]], ['rotate_z', -math.pi / 4],
                      ["translate", [-0.5, 1, 0.5]]]
    }
    base = {
        'type': 'cylinder',
        'closed': True,
        'minimum': -0.1,
        'maximum': 0.1,
        'material': {
            'color': base_color
        },
        'transform': [['scale', [0.3, 1.0, 0.3]], ['translate', [0, 1, 0]], ['rotate_z', -math.pi / 4],
                      ["translate", [-0.5, 1.0, 0.5]]]
    }
    return [base, cone]


spheres = [
    {
        "type": "sphere",
        "name": "middle_sphere",
        "transform": [['rotate_z', -math.pi / 4], ["translate", [-0.5, 1, 0.5]]],
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
                "transform": [["scale", [0.33, 0.33, 0.33]], ["rotate_x", -math.pi / 4]]
            },
            "reflectivity": 0.5
        }
    },
    {
        "type": "sphere",
        "name": "right_sphere",
        "transform": [["scale", [0.9, 0.9, 0.9]], ["translate", [2, 0.9, 2]]],
        "material": {"color": [1, 0.5, 0.5], "diffuse": 0.7, "specular": 0.3, "reflectivity": 0.5}
    }
]

data = json.dumps({
    "camera": {
        "h_size": 1000,
        "v_size": 600,
        "field_of_view": math.pi / 3,
        "transform": [[0, 1.5, -5], [0, 1, 0], [0, 1, 0]],
        "anti-aliasing": True,
        "background": ['white', [0.5, 0.7, 1.0]]
    },
    "world": {
        "light": {"position": [-4, 7, -10], "intensity": [1, 1, 1]},
        "objects": spheres + cones()
    }
}, indent=2)

print(data)
