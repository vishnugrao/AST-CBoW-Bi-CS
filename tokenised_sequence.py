# This file contains the code that takes the serialised ASTs in the .txt files then converts them into tokenised sequences.

import os
import nltk
from nltk.tokenize import word_tokenize
from gensim.models import Word2Vec


# NLTK data download for stopwords

nltk.download('punkt')

def tokenize_txt(filepath):
    with open(filepath, 'r', encoding='utf-8') as txtfile:
        rawtext = txtfile.read()

    splittext = rawtext.split('\n')

    # print(splittext)

    tokens = word_tokenize(splittext[0])

    print(tokens)

    return tokens


if __name__ == '__main__':
    filepath = './LibTIFF_data_output.txt'
    tokenize_txt(filepath)
