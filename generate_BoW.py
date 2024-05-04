# This file contains the code to generate the bag of words from the tokens

import os
import torch
from collections import defaultdict

def vectorize(tokens, vocabulary):
    
    vec = torch.zeros(len(vocabulary))

    for word in tokens:
        if word in vocabulary:
            vec[vocabulary[word]] += 1

def create_BoW(directory_path):

    vocabulary = defaultdict(lambda: len(vocabulary))

    for filename in os.listdir(directory_path):
        if filename.endswith('_data_tokenised.txt'):
            with open(filename, 'r', encoding='utf-8') as txtfile:
                rawtext = txtfile.read()

            splittokens = rawtext.split('\n\n')

            for token in splittokens:
                words = token.split()
                _ = [vocabulary[word] for word in words]

            inv_vocabulary = {v: k for k, v in vocabulary.items()}

            for tokens in splittokens:
                words = tokens.split()
                bows = vectorize(words, vocabulary)
                
                if bows:
                    cache_bow(bows, filename.replace('_tokenised.txt', '_BoW.txt'))

            print(f"Finished {filename}")

def cache_bow(bows, bow_file):
    with open(bow_file, 'a') as out_dest:
        out_dest.write(str(bows) + str("\n\n"))


if __name__ == '__main__':
    directory_path = "./"
    create_BoW(directory_path)