import copy
import json
from spheres import data

objects = data['world']['objects']

left_sphere = {}
for obj in objects:
    if obj['name'] == 'left_sphere':
        left_sphere = obj

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

print(json.dumps(data))
