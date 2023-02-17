import copy
import json
import math


def middle_group():
    group = {
        'type': 'group',
        'children': [
            {
                "type": "sphere",
                "name": "middle_sphere",
                "transform": [['rotate_y', math.pi * 1.3], ['rotate_x', -math.pi / 4], ['rotate_z', -math.pi / 4],
                              ["translate", [-0.5, 1, 0.5]]],
                'material': {
                    'pattern': {
                        'type': 'texture',
                        'path': 'examples/ppms/map_green.ppm',
                        'uv_pattern': 'image',
                    },
                    'diffuse': 0.7,
                    'specular': 0.3,
                    'reflectivity': 0.5
                }
            },
            cube,
            cylinders()
        ]
    }
    group['children'] += cones()
    return group


def cones():
    base_color = [1.0, 168.0 / 255.0, 18.0 / 255.0]
    rotate_and_translate = [['rotate_z', -math.pi / 4], ["translate", [-0.5, 1, 0.5]]]
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
        'transform': [['scale', [0.5 / 3, 0.5, 0.5 / 3]], ['translate', [0, 1.7, 0]]] + rotate_and_translate
    }
    base = {
        'type': 'cylinder',
        'closed': True,
        'minimum': -0.1,
        'maximum': 0.1,
        'material': {
            'color': base_color
        },
        'transform': [['scale', [0.2, 1.0, 0.2]], ['translate', [0, 1.1, 0]]] + rotate_and_translate
    }
    return [base, cone]


def cylinders():
    colors = [[40, 103, 160], [72, 120, 170], [99, 141, 187], [121, 158, 196], [157, 179, 208]]
    offset_scale = 0.8 / 3
    acc = [
        {
            "type": "cylinder",
            "minimum": -0.1,
            "maximum": 0.1,
            "material": {
                "color": [7.0 / 255, 87.0 / 255, 152.0 / 255]
            },
            "transform": [["scale", [offset_scale, 1, offset_scale]], ["translate", [-0.5, 2.1, 0.5]]]
        }
    ]
    for i in range(0, 5):
        last = acc[-1]
        factor = 0.2 / 3
        scale_factor = offset_scale - ((i + 1) * factor)
        if scale_factor < factor:
            scale_factor = offset_scale / math.pow(2, i)
        r, g, b = colors[i]

        new_mat = copy.deepcopy(last['material'])
        new_mat['color'] = [r / 255.0, g / 255.0, b / 255.0]

        new_cyl = {
            'type': 'cylinder',
            'minimum': last['minimum'] - 0.1 / 3,
            'maximum': last['maximum'] + 0.1 / 3,
            'material': new_mat,
            'transform': [['scale', [scale_factor, 1, scale_factor]], ['translate', [-0.5, last['maximum'] + 2.1, 0.5]]]
        }

        acc.append(new_cyl)

    return {'type': 'group', 'children': acc}


def right_group():
    return {
        'type': 'group',
        'children': [
            {
                "type": "sphere",
                "name": "right_sphere",
                "transform": [['scale', [0.9, 0.9, 0.9]], ['rotate_y', math.pi / 2], ["translate", [2.1, 0.9, 2]]],
                "material": {
                    'pattern': {
                        'type': 'texture',
                        'path': 'examples/ppms/light_red_marble.ppm',
                        'uv_pattern': 'image',
                    },
                    "reflectivity": 0.5
                }
            },
            {
                'type': 'sphere',
                'name': 'right_sphere_moon',
                'transform': [["scale", [0.3, 0.3, 0.3]], ["translate", [2.1, 2.1, 2]]],
                'material': {
                    'color': [0.8, 0.8, 0.8],
                    'reflectivity': 0.5
                }
            },
            {
                'type': 'sphere',
                'name': 'glass_sphere',
                'transform': [['scale', [0.25, 0.25, 0.25]], ['translate', [0.7, 0.85, -1.5]]],
                'material': {
                    'color': [0.2, 0.2, 0.2],
                    'diffuse': 0.01,
                    'specular': 1,
                    'shininess': 300,
                    'transparency': 1,
                    'reflectivity': 1,
                    'refractive_index': 1.5
                }
            }
        ]
    }


rest_of_spheres = [
    {
        "type": "sphere",
        "name": "left_sphere",
        "transform": [["scale", [0.35, 0.35, 0.35]], ["translate", [-1.7, 0.35, -0.75]]],
        "material": {
            'pattern': {
                'type': 'texture',
                'path': 'examples/ppms/blue_marble.ppm',
                'uv_pattern': 'image',
            },
            "reflectivity": 0.5
        }
    },
    {
        'type': 'sphere',
        'name': 'colossal',
        'transform': [['scale', [4, 4, 4]], ['translate', [4, -3, -2]]],
        'material': {
            'color': [225 / 255.0, 193.0 / 255, 110.0 / 255],
            'reflectivity': 0.3
        }
    },
    {
        'type': 'sphere',
        'name': 'far_away_sphere',
        'transform': [['translate', [-15, 5, 25]]],
        'material': {
            "diffuse": 0.7,
            "specular": 0.3,
            "pattern": {
                "type": "ring",
                "components": [[1, 0.8, 0.1], 'white'],
                "transform": [["scale", [0.33, 0.33, 0.33]]]
            },
            "reflectivity": 0.5
        }
    },
]

background = {
    'type': 'plane',
    'material': {
        'pattern': {
            'type': 'gradient',
            'components': ['white', [0.5, 0.7, 1.0]],
            'transform': [['scale', [410, 1, 1]]],
        },
        'specular': 0,
        'transparency': 1,
    },
    'transform': [['rotate_x', -math.pi / 2], ['rotate_z', math.pi / 2], ['translate', [0, 115, 500]]]
}

cube = {
    "type": "cube",
    "transform": [["scale", [0.13, 0.17, 0.13]], ['translate', [0, 1.17, 0]], ['rotate_z', math.pi / 4],
                  ['rotate_y', -math.pi / 4], ['rotate_x', -math.pi / 4], ["translate", [-0.5, 1, 0.5]]],
    "material": {
        "diffuse": 0.7,
        "specular": 0.3,
        'color': [1, 1, 143.0 / 255],
        'reflectivity': 0.23
    }
}

sphere_group = {
    'type': 'group',
    'children': [middle_group(), right_group()],
    'threshold': 2,
}

data = {
    "camera": {
        "h_size": 1000,
        "v_size": 563,
        "field_of_view": math.pi / 3,
        "transform": [[0, 1.5, -5], [0, 1, 0], [0, 1, 0]],
        "anti-aliasing": True,
    },
    "world": {
        "light": {"position": [-2, 13, -7], "intensity": [1, 1, 1]},
        "objects": [background, sphere_group] + rest_of_spheres
    }
}

print(json.dumps(data, indent=2))
