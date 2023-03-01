import json
import math
import urllib.request

try:
    with urllib.request.urlopen('https://groups.csail.mit.edu/graphics/classes/6.837/F03/models/teddy.obj') as f:
        source = f.read().decode('utf-8')
except urllib.error.URLError as e:
    print(e.reason)


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
                'type': 'obj',
                'source': source,
                'threshold': 2,
                'transform': [['scale', [0.6, 0.6, 0.6]], ['rotate_y', math.pi], ['translate', [0, 0, 100]]],
                'materials': [['default', {'color': [160.0 / 255.0, 82.0 / 255.0, 45.0 / 255.0]}]]
            }
        ]
    }
}

print(json.dumps(data, indent=2))
