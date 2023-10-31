import matplotlib.pyplot as plt
import numpy as np
from pprint import pprint

def complex_matrix(xmin, xmax, ymin, ymax, pixel_density):
    re = [xmin + i * (xmax - xmin) / (int((xmax - xmin) * pixel_density) - 1) for i in range(int((xmax - xmin) * pixel_density))]
    im = [ymin + j * (ymax - ymin) / (int((ymax - ymin) * pixel_density) - 1) for j in range(int((ymax - ymin) * pixel_density))]
    
    complex_matrix = []
    for r in re:
        row = []
        for i in im:
            row.append(r + i * 1j)
        complex_matrix.append(row)
    
    return complex_matrix

def is_stable(test: list[list[complex]]) ->list[list[complex]]:

    for rowI in range(len(test)):
        for colI in range(len(test[rowI])):
            try:
                test[rowI][colI] = test[rowI][colI]**2 + test[rowI][colI]
            except OverflowError:
                test[rowI][colI] = test[rowI][colI]
    return test


def bool2D(test):
    for rowI in range(len(test)):
        for colI in range(len(test[rowI])):
            try:
                if abs(test[rowI][colI]) <= 2:
                    test[rowI][colI] = 1
                else:
                    test[rowI][colI] = 0
            except OverflowError:
                test[rowI][colI] = 0
    return test
            
if __name__ == "__main__":
    c = complex_matrix(-2,0.5,-1.5,1.5, pixel_density=2048)
    
    for i in range(20):
        c = is_stable(c)
    bool2D(c)
    plt.imshow(c, cmap="binary")
    plt.gca().set_aspect("equal")
    plt.axis("off")
    plt.tight_layout()
    plt.savefig('test_fractal.png')
    #plt.show()
    plt.close()