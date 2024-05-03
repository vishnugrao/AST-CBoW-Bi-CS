# This file exclusively contains the preprocessing steps on how the .c files are converted into a dataset before embedding / learning

import os
import json 
from pathlib import Path

def preprocess_files(directory_path):
    projects_data = {}

    # print(directory_path)

    for project_name in os.listdir(directory_path):
        print(project_name)
        # Append the project name to the path
        project_path = os.path.join(directory_path, project_name)
        if os.path.isdir(project_path):
            for file in os.listdir(project_path):
                if file.endswith(".c"):
                    # Append file to the path
                    file_path = os.path.join(project_path, file)
                    try: 
                        with open(file_path, 'r', encoding='utf-8') as file_content:
                            content = file_content.read()

                    except UnicodeDecodeError:
                        # Could not be read in utf-8
                        try:
                            with open(file_path, 'r', encoding='iso-8859-1') as file_content:
                                content = file_content.read()
                        except Exception as e:
                            # The content of the file does not register under iso 
                            print(f"Failed to read {file}: {e}")
                            continue
                    
                    label = 1 if 'cve-' in file.lower() or 'CVE-' in file.lower() else 0

                    if content == "":
                        continue

                    if project_name not in projects_data:
                        projects_data[project_name] = []
                    projects_data[project_name].append({'filename': file, 'source code': content, 'label': label})

    # print(projects_data)

    return projects_data

def save_preprocessed_data(projects_data):
    for project_name, data in projects_data.items():
        output_file = f"{project_name}_data.json"
        with open(output_file, 'w', encoding='utf-8') as f:
            json.dump(data, f, ensure_ascii=False, indent=4)

if __name__ == '__main__':
    directory_path = './OffData/TransferRepresentationLearning/Data/VulnerabilityData/6_projects_functions'
    output_json = 'processed_data.json'
    
    # print(parent_directories)

    processed_data = preprocess_files(directory_path)
    save_preprocessed_data(processed_data)

