import random
import time
import laplace
from collections import defaultdict


def build_laplace_freqs(corpus, n_value, alpha):
    result = defaultdict(lambda : defaultdict(float))
    for text in corpus:
        iter_text = "$" * (n_value - 1) + text
        for i in range(len(iter_text) - n_value + 1):
            result[iter_text[i : (i + n_value - 1)]][iter_text[i + n_value - 1]] += 1
    for key in result:
        norm = sum(result[key].values()) + len(result[key]) * alpha
        for letter in result[key]:
            result[key][letter] = (result[key][letter] + alpha) / norm

    return result



def test_small():
    corpus = ['abcabc ab bc', 'abcab', 'aabb', 'русские буквы', '심지어 한국어']
    print(build_laplace_freqs(corpus, 2, 1.))
    print('\n\n')
    print(laplace.build_laplace_frequences(corpus, 2, 1.))
    print('\n\n')


def test_big():
    with open('shakespeare_input.txt', 'r') as fi:
        corpus = list(map(lambda x: x.strip(), fi.readlines()))

    python_start = time.process_time()
    build_laplace_freqs(corpus, 4, 1.)
    python_end = time.process_time()
    python_time = python_end - python_start
    print("Python calculations time: {:.1f} sec".format(python_time))

    python_start = time.process_time()
    laplace.build_laplace_frequences(corpus, 4, 1.)
    python_end = time.process_time()
    python_time = python_end - python_start
    print("Python/C++ calculations time: {:.1f} sec".format(python_time))



if __name__ == "__main__":
    print(laplace.__doc__)
    print('\n\n')
    print(laplace.build_laplace_frequences.__doc__)
    print('\n\n')
    test_small()
    test_big()
    # If you want, I can add text generation to this module, but this is optional functionaity
