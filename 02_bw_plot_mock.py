import matplotlib.pyplot as plt
import MPIpy_v3
MPI = MPIpy_v3.MPIpy()

def onehalf(xmin, xmax,pixel_density):
    re = [xmin + i * (xmax - xmin) / (int((xmax - xmin) * pixel_density) - 1) for i in range(int((xmax - xmin) * pixel_density))]
    return re
def secondhalf(half, ymin, ymax, pixel_density):
    im = [ymin + j * (ymax - ymin) / (int((ymax - ymin) * pixel_density) - 1) for j in range(int((ymax - ymin) * pixel_density))]
    complex_matrix = []
    for r in half:
        row = []
        for i in im:
            row.append(r + i * 1j)
        complex_matrix.append(row)
    
    return complex_matrix

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
    density = 3072
    if density % MPI.size != 0:
        print("improper combination of size and pixel_density")
        exit(1)
    chunk = int((0.5 - (-2)) * density) // MPI.size
    #c = complex_matrix(-2,0.5,-1.5,1.5, pixel_density=2048)
    if MPI.rank == 0:
        store = onehalf(-2,0.5, pixel_density=density)
        # print(store)
        #print("master: ",len(store))
    else:
        store = []    
    MPI.Scatter(store,0)
    displacement = len(store) * MPI.rank
    #print(MPI.rank, " : ", displacement, ", ", chunk, ", ", len(store)
    store = secondhalf(store, -1.5,1.5,density)
    forFuture = len(store[0])
    MPI.barrier()
    # print(MPI.rank, " : ", len(store), ", ", len(store[0]))
    for i in range(20):
        store = is_stable(store)
    bool2D(store)
    master = [] #trust me, its needed for now
    for i in store:
        master.extend(i)
    MPI.barrier()
    print(MPI.rank, ": ", len(master))

    MPI.gather(master, 0) # it brings it back as a 1D

    if MPI.rank == 0:
        masterF = []
        for i in range(0, len(master), forFuture):
            subTmp = master[i:i + forFuture]
            masterF.append(subTmp)
        plt.imshow(masterF, cmap="binary")
        plt.gca().set_aspect("equal")
        plt.axis("off")
        plt.tight_layout()
        plt.savefig('test_fractal.png')
        #plt.show()
        plt.close()