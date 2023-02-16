import json
import math

data = {
    "camera": {
        "h_size": 1000,
        "v_size": 600,
        "field_of_view": math.pi / 3,
        "transform": [[0, 1.5, -5], [0, 1, 0], [0, 1, 0]],
        "anti-aliasing": False
    },
    "world": {
        "light": {"position": [-10, 12, -10], "intensity": "white"},
        "objects": [
            {
                'type': 'sphere',
                'material': {
                    'pattern': {
                        'type': 'texture',
                        'path': '/Users/melvic/Documents/ppms/map.ppm',
                        'uv_pattern': 'image'
                    }
                }
            }
        ]
    }
}

print(json.dumps(data, indent=2))

