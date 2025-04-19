import numpy as np

def read_matrix_from_txt(filename):
    with open(filename, 'r') as f:
        lines = f.readlines()
    
    rows, cols = map(int, lines[0].split())
    
    data = [list(map(int, line.split())) for line in lines[1:]]
    matrix = np.array(data)
    
    return matrix
if __name__ == "__main__":

    sizes = [100, 200, 300, 400, 500, 600, 700, 800, 900, 1000]
    for size in sizes:
        a = f"C:/Users/ROfl/Desktop/parallelprogramming/lab1/tests/a{size}.txt"
        b = f"C:/Users/ROfl/Desktop/parallelprogramming/lab1/tests/b{size}.txt"
        c = f"C:/Users/ROfl/Desktop/parallelprogramming/lab1/tests/c{size}.txt"

        a_matrix = read_matrix_from_txt(a)
        b_matrix = read_matrix_from_txt(b)
        c_matrix = read_matrix_from_txt(c)

        y = np.dot(a_matrix, b_matrix)
        if np.allclose(y, c_matrix, atol=1e-9):
            print(f"Console :::: Confirmed for size {size} ✅")
        else:
            print(f"Console :::: Failed for size {size} ❌")