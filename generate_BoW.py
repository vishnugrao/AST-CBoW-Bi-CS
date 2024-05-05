# This file contains all the code to convert the ASTs into BoWs and Word2Vec embeddings

import os
import torch
import numpy as np
from collections import defaultdict
from gensim.models import Word2Vec

def vectorize(tokens, vocabulary):
    vec = torch.zeros(len(vocabulary))
    for word in tokens:
        if word in vocabulary:
            vec[vocabulary[word]] += 1
    return vec

def create_BoW_and_Word2Vec(directory_path):
    for filename in os.listdir(directory_path):
        if filename.endswith('_data_tokenised.txt'):
            with open(filename, 'r', encoding='utf-8') as txtfile:
                rawtext = txtfile.read()

            splittokens = rawtext.split('\n\n')
            vocabulary = defaultdict(lambda: len(vocabulary))
            for token in splittokens:
                words = token.split()
                _ = [vocabulary[word] for word in words]

            model = Word2Vec(sentences=[token.split() for token in splittokens], vector_size=100, window=5, min_count=1, workers=4)

            for idx, tokens in enumerate(splittokens):
                words = tokens.split()
                bows = vectorize(words, vocabulary).numpy()
                embeddings = np.array([model.wv[word] for word in words if word in model.wv])
                if embeddings.size > 0:
                    mean_embedding = torch.tensor(embeddings.mean(axis=0))
                    torch.save(bows, f"./BoWs/{filename.replace('_data_tokenised.txt', '')}/{filename.replace('_tokenised.txt', '')}_BoW_{idx}.pt")
                    torch.save(mean_embedding, f"./W2Vs/{filename.replace('_data_tokenised.txt', '')}/{filename.replace('_tokenised.txt', '')}_W2V_{idx}.pt")

            print(f"Finished processing {filename}")

if __name__ == '__main__':
    directory_path = "./Tokens/"
    create_BoW_and_Word2Vec(directory_path)
