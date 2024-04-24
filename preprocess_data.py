# This file exclusively contains the preprocessing steps on how the .c files are converted into a dataset before embedding / learning

import os
import json 
from pathlib import Path

def preprocess_files(directory_path):
    data = []
    for root, dirs, files in os.walk(directory_path):
        for file in files:
            path = os.path.join(root, file)
            with open(path, 'r', encoding='utf-8') as file_content:
                content = file_content.read()
                label = 1 if file.startswith('cve-') or file.startswith('CVE') else 0
                data.append({'source_code': content, 'label': label})

    return data

def save_preprocessed_data(data, output_file):
    with open(output_file, 'w', encoding='utf-8') as f:
        json.dump(data, f)

if __name__ == '__main__':
    directory_path = './OffData/TransferRepresentationLearning/Data/VulnerabilityData/'
    output_json = 'processed_data.json'

    processed_data = preprocess_files(directory_path)
    save_preprocessed_data(processed_data, output_json)

