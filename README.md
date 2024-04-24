# AST-CBoW-Bi-CS

## Overview

Software Vulnerability Detection baseline on a Dataset Consisting of 6 Open Source Projects. Base architecture will include AST representation of code encoded by continuous bag of words (word2vec) into an embedding. Then the representations are input into a Bi-LSTM network to obtain vulnerability representation. Then, the learning is transferred.

## Projects included in the dataset:

The owners manually labeled 457 vulnerable functions and collected 32 531 nonvulnerable functions from six open-source projects across 1000+ popular releases.The owners obtained the vulnerability data from the National Vulnerability Database (NVD) and the common vulnerability and exposures (CVE).

    - LibTIFF
    - LibPNG
    - FFmpeg
    - Pidgin
    - VLC Media Player
    - Asterisk

Link: https://github.com/DanielLin1986/TransferRepresentationLearning

## Paper consulted

Cross-Project Transfer Representation Learning for Vulnerable Function Discovery

Guanjun Lin , Jun Zhang , Member, IEEE, Wei Luo, Lei Pan , Member, IEEE,
Yang Xiang , Senior Member, IEEE, Olivier De Vel, and Paul Montague

## Setup

Initially we choose the 1st five projects as the training and Asterisk as the test.
