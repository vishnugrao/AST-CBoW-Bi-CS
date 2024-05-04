# This file contains the code that takes the serialised ASTs in the .txt files then converts them into tokenised sequences.

import os
import nltk
from nltk.tokenize import word_tokenize
from gensim.models import Word2Vec


# NLTK data download for stopwords

# nltk.download('punkt')

def tokenize_txt(directory_path):

    for filename in os.listdir(directory_path):
        if filename.endswith('_data_output.txt'):
            with open(filename, 'r', encoding='utf-8') as txtfile:
                rawtext = txtfile.read()

            splittext = rawtext.split('\n\n')

            for cachetext in splittext:
                tokens = word_tokenize(cachetext)

                if tokens:
                    register_tokenisation(tokens, filename.replace('_output.txt', '_tokenised.txt'))

            print(f"Finished {filename}")

def register_tokenisation(tokens, token_file):
    with open(token_file, 'a') as out_dest:
        out_dest.write(str(tokens) + str("\n\n"))

if __name__ == '__main__':
    # filepath = './LibTIFF_data_output.txt'
    directory_path = './'
    tokenize_txt(directory_path)
