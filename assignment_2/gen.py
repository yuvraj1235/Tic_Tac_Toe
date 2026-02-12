import random

def generate_cramers_input(n=10, filename="input_small.txt"):
    with open(filename, "w") as f:
        # number of variables
        f.write(str(n) + "\n")

        # coefficient matrix A (n x n)
        for i in range(n):
            for j in range(n):
                f.write(str(random.randint(1, 9)) + " ")
            f.write("\n")

        # constant matrix B (n x 1)
        for i in range(n):
            f.write(str(random.randint(1, 9)) + "\n")

    print("File generated:", filename)


# 🔴 THIS WAS MISSING 🔴
if __name__ == "__main__":
    generate_cramers_input(10)
