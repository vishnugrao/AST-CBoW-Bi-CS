import os
import torch
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from openTSNE import TSNE

def load_embeddings(directory_path):
    project_embeddings = {}
    for project in os.listdir(directory_path):
        project_path = os.path.join(directory_path, project)
        embeddings = []
        if os.path.isdir(project_path):
            for file in os.listdir(project_path):
                if file.endswith('.pt'):
                    embedding_path = os.path.join(project_path, file)
                    embedding = torch.load(embedding_path)
                    embeddings.append(embedding.numpy())  # Convert to NumPy for openTSNE compatibility
            if embeddings:
                project_embeddings[project] = np.stack(embeddings)
    return project_embeddings

def apply_tsne(embeddings):
    all_embeddings = np.concatenate(list(embeddings.values()), axis=0)
    tsne = TSNE(n_components=2, perplexity=30, metric='euclidean', verbose=True)
    tsne_results = tsne.fit(all_embeddings)
    return tsne_results

def plot_embeddings(tsne_results, project_embeddings):
    colors = plt.colormaps["tab20"]
    start_idx = 0
    plt.figure(figsize=(10, 8))
    for idx, (project, embeds) in enumerate(project_embeddings.items()):
        end_idx = start_idx + len(embeds)
        plt.scatter(tsne_results[start_idx:end_idx, 0], tsne_results[start_idx:end_idx, 1], 
                    color=colors(idx), label=project)
        start_idx = end_idx
    plt.legend()
    plt.title('t-SNE Visualization of Project Embeddings')
    plt.xlabel('t-SNE Component 1')
    plt.ylabel('t-SNE Component 2')
    plt.savefig('project_embeddings.png')
    plt.savefig('project_embeddings.pdf')
    plt.show()

if __name__ == '__main__':
    directory_path = "./W2Vs"
    embeddings = load_embeddings(directory_path)
    tsne_results = apply_tsne(embeddings)
    plot_embeddings(tsne_results, embeddings)
