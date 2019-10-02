from simple_array import parse_simple_array
import sys


def main():
    buf = open(sys.argv[1], "rb").read()
    arr = parse_simple_array(buf)
    print('read =>', arr)


if __name__ == "__main__":
    main()

