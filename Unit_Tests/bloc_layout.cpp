#include "../src/Bloc_Layout.h"

#include <cassert>
#include <vector>
#include <iostream>

using namespace std;

int main() {
    /* -------------------------------------- TEST0 ------------------------------------------- */
    vector<vector<unsigned short>> adjacency_matrix0(4, std::vector<unsigned short>(4));

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            adjacency_matrix0[i][j] = 4*i + j;
        }

    }
    vector<vector<unsigned short>> bloc_layout0 = graphToBlocLayout(adjacency_matrix0, 2, true);

    assert(bloc_layout0[0][0] == 0);
    assert(bloc_layout0[0][1] == 1);
    assert(bloc_layout0[0][2] == 4);
    assert(bloc_layout0[0][3] == 5);
    assert(bloc_layout0[1][0] == 2);
    assert(bloc_layout0[1][1] == 3);
    assert(bloc_layout0[1][2] == 6);
    assert(bloc_layout0[1][3] == 7);
    assert(bloc_layout0[2][0] == 8);
    assert(bloc_layout0[2][1] == 9);
    assert(bloc_layout0[2][2] == 12);
    assert(bloc_layout0[2][3] == 13);
    assert(bloc_layout0[3][0] == 10);
    assert(bloc_layout0[3][1] == 11);
    assert(bloc_layout0[3][2] == 14);
    assert(bloc_layout0[3][3] == 15);

    /* -------------------------------------- TEST1 ------------------------------------------- */
    vector<vector<unsigned short>> adjacency_matrix1(6, std::vector<unsigned short>(6));
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            adjacency_matrix1[i][j] = 6*i + j;
        }
    }
    vector<vector<unsigned short>> bloc_layout1 = graphToBlocLayout(adjacency_matrix1, 2, true);

    assert(bloc_layout1[0][0] == 0);
    assert(bloc_layout1[0][1] == 1);
    assert(bloc_layout1[0][2] == 6);
    assert(bloc_layout1[0][3] == 7);
    assert(bloc_layout1[1][0] == 2);
    assert(bloc_layout1[1][1] == 3);
    assert(bloc_layout1[1][2] == 8);
    assert(bloc_layout1[1][3] == 9);
    assert(bloc_layout1[2][0] == 4);
    assert(bloc_layout1[2][1] == 5);
    assert(bloc_layout1[2][2] == 10);
    assert(bloc_layout1[2][3] == 11);
    assert(bloc_layout1[3][0] == 12);
    assert(bloc_layout1[3][1] == 13);
    assert(bloc_layout1[3][2] == 18);
    assert(bloc_layout1[3][3] == 19);
    assert(bloc_layout1[4][0] == 14);
    assert(bloc_layout1[4][1] == 15);
    assert(bloc_layout1[4][2] == 20);
    assert(bloc_layout1[4][3] == 21);
    assert(bloc_layout1[5][0] == 16);
    assert(bloc_layout1[5][1] == 17);
    assert(bloc_layout1[5][2] == 22);
    assert(bloc_layout1[5][3] == 23);
    assert(bloc_layout1[6][0] == 24);
    assert(bloc_layout1[6][1] == 25);
    assert(bloc_layout1[6][2] == 30);
    assert(bloc_layout1[6][3] == 31);
    assert(bloc_layout1[7][0] == 26);
    assert(bloc_layout1[7][1] == 27);
    assert(bloc_layout1[7][2] == 32);
    assert(bloc_layout1[7][3] == 33);
    assert(bloc_layout1[8][0] == 28);
    assert(bloc_layout1[8][1] == 29);
    assert(bloc_layout1[8][2] == 34);
    assert(bloc_layout1[8][3] == 35);

    /* -------------------------------------- TEST2 ------------------------------------------- */
    vector<vector<unsigned short>> bloc_layout2 = graphToBlocLayout(adjacency_matrix1, 3, true);
    assert(bloc_layout2[0][0] == 0);
    assert(bloc_layout2[0][1] == 1);
    assert(bloc_layout2[0][2] == 2);
    assert(bloc_layout2[0][3] == 6);
    assert(bloc_layout2[0][4] == 7);
    assert(bloc_layout2[0][5] == 8);
    assert(bloc_layout2[0][6] == 12);
    assert(bloc_layout2[0][7] == 13);
    assert(bloc_layout2[0][8] == 14);
    assert(bloc_layout2[1][0] == 3);
    assert(bloc_layout2[1][1] == 4);
    assert(bloc_layout2[1][2] == 5);
    assert(bloc_layout2[1][3] == 9);
    assert(bloc_layout2[1][4] == 10);
    assert(bloc_layout2[1][5] == 11);
    assert(bloc_layout2[1][6] == 15);
    assert(bloc_layout2[1][7] == 16);
    assert(bloc_layout2[1][8] == 17);
    assert(bloc_layout2[2][0] == 18);
    assert(bloc_layout2[2][1] == 19);
    assert(bloc_layout2[2][2] == 20);
    assert(bloc_layout2[2][3] == 24);
    assert(bloc_layout2[2][4] == 25);
    assert(bloc_layout2[2][5] == 26);
    assert(bloc_layout2[2][6] == 30);
    assert(bloc_layout2[2][7] == 31);
    assert(bloc_layout2[2][8] == 32);
    assert(bloc_layout2[3][0] == 21);
    assert(bloc_layout2[3][1] == 22);
    assert(bloc_layout2[3][2] == 23);
    assert(bloc_layout2[3][3] == 27);
    assert(bloc_layout2[3][4] == 28);
    assert(bloc_layout2[3][5] == 29);
    assert(bloc_layout2[3][6] == 33);
    assert(bloc_layout2[3][7] == 34);
    assert(bloc_layout2[3][8] == 35);

    /* ------------------------------------ TEST3 --------------------------------------------- */
    vector<vector<unsigned short>> bloc_layout3 = graphToBlocLayout(adjacency_matrix1, 6, true);
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            assert(bloc_layout3[0][6*i+j] == adjacency_matrix1[i][j]);
        }
    }

    /* ------------------------------------ TEST4 --------------------------------------------- */
    {
        int i = 4;
        int j = 2;
        int blocsPerWidth = 2;
        int blocSize1D = 3;
        std::pair<int,int> blocPos = posToBlocPos(i, j, blocsPerWidth, blocSize1D, true);
        assert(blocPos.first == 2);
        assert(blocPos.second == 5);
        std::pair<int,int> pos = blocPosToPos(blocPos.first, blocPos.second, blocsPerWidth, blocSize1D, true);
        assert(pos.first == i);
        assert(pos.second == j);
    }

    /* -------------------------------Test bijectivity function -------------------------------- */
    {
        for (int bPW = 1; bPW < 10; bPW++) {
            for (int bS1D = 1; bS1D < 100; bS1D++) {
                for (int i = 0; i < bPW*bS1D; i++) {
                    for (int j = 0; j < bPW*bS1D; j++) {
                        std::pair<int,int> blocPos = posToBlocPos(i, j, bPW, bS1D, true);
                        std::pair<int,int> pos = blocPosToPos(blocPos.first, blocPos.second, bPW, bS1D, true);
                        assert(pos.first == i);
                        assert(pos.second == j);
                    }
                }
            }
        }
    }


}
