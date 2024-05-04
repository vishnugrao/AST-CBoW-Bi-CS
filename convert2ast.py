# This file contains the code that pipes each labelled source code in each [project_name].json file into the CodeSensor.jar file.

import os
import subprocess
import json

def get_AST(temp_cfilepath):
    # Pipe the source code to CodeSensor.jar then return the serialised AST
    try:
        # Create a subprocess to run the java -jar CodeSensor.java [filename] command
        serialised_AST = subprocess.run(['java', '-jar', './codesensor/CodeSensor.jar', temp_cfilepath], text=True, capture_output=True, check=True)
        # The std out from the command is the serialised AST
        return serialised_AST.stdout 
    except subprocess.CalledProcessError as e:
        print(f"An error occurred while using CodeSensor.jar: {e}")
        return None
    

def pipe_jsons(directory_path, temp_cfilepath):
    for filename in os.listdir(directory_path):
        if filename.endswith('_data.json'):
            file_path = os.path.join(json_directory, filename)
            with open(file_path, 'r', encoding='utf-8') as file:
                data = json.load(file)
                for labelled_pair in data:
                    # print(labelled_pair['source code'])
                    with open(temp_cfilepath, 'w', encoding='utf-8') as temp_cfile:
                        temp_cfile.write(labelled_pair['source code'])

                    output = get_AST(temp_cfilepath)
                    # print(output)
                    if output:
                        serialise_AST(output, filename.replace('.json', '_output.txt'))
        
        print(f"Finished {filename}")

def serialise_AST(output, output_file):
    # Write to file in append mode line by line
    with open(output_file, 'a') as out_dest:
        out_dest.write(output + str("\n"))
    
    return 

if __name__ == '__main__':
    json_directory = './'
    temp_cfilepath = './placeholder.c'
    pipe_jsons(json_directory, temp_cfilepath)

