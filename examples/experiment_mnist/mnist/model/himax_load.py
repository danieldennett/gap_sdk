"""Loading Himax images.
"""
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import numpy as np

def load_data(path):
    f = np.load(path)
    x_train, y_train = f['x_train'], f['y_train']
    x_test, y_test = f['x_test'], f['y_test']
    f.close()
    return (x_train, y_train), (x_test, y_test)