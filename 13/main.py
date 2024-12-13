from sympy import Matrix
import re


def main():
    file = open("input.txt", 'r')

    vals = map(int, re.findall(r'\d+', file.read()))

    mats = []
    members = []
    for val in vals:
        members.append(val)

        if len(members) == 6:
            mats.append(Matrix([                    # add in part2
                [members[0], members[2], members[4] + 10000000000000],
                [members[1], members[3], members[5] + 10000000000000],
            ]))
            members = []

    sum = 0
    for mat in mats:
        r = mat.rref()
        plays = r[0].col(2)

        if float(plays[0]).is_integer() and float(plays[1]).is_integer():
            sum += 3 * plays[0] + plays[1]

    print(sum)


if __name__ == "__main__":
    main()
