import sys
import numpy as np
import matplotlib.pyplot as plt


def plot_raw_image(filename):
    fd = open(filename, 'rb')
    rows = 512
    cols = 512
    f = np.fromfile(fd, dtype=np.uint16, count=rows*cols)
    print(len(f))
    print(max(f), min(f))
    im = f.reshape((rows, cols))
    fd.close()

    # plt.figure()
    # plt.imshow(im, cmap='gray')
    # plt.colorbar()
    # plt.show()

if __name__ == "__main__":
    filename = sys.argv[1]
    plot_raw_image(filename)