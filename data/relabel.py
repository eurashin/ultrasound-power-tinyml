import os 

# REPLACE THE NEXT TWO LINES
mapping = {'sitting': 'stationary', 'walking': 'moving', 'running':'moving', 'stairs':'moving', 'pushing': 'moving', 'walkingdisabled':'walkingdisabled', 'other':'stationary'} # Map from original label to ne
dirname = "stationary-or-moving" # name of directory to output relabeled files

import json
labels = json.load(open('original/info.labels', 'rb'))
label_files = labels['files']

new_labels = labels.copy()

from pathlib import Path
Path("{}".format(dirname)).mkdir(parents=True, exist_ok=True)

for i, file in enumerate(label_files):
    original_label = file['label']['label']
    original_filename = file['path']

    new_label = mapping[original_label]
    new_labels['files'][i]['label']['label'] = new_label

    new_filename = "{}.{}.cbor".format(new_label, i)
    new_labels['files'][i]['path']  = new_filename
    new_labels['files'][i]['name']  = "{}.{}".format(new_label, i)
    
    print(original_filename, new_filename)

    os.popen('cp original/{} {}/{}'.format(original_filename, dirname, new_filename))

json_object = json.dumps(new_labels, indent=4)
with open("{}/info.labels".format(dirname), "w") as outfile:
    outfile.write(json_object)


