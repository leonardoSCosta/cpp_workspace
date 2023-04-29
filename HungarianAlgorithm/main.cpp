#include <QCoreApplication>
#include "hungarianalgorithm.h"

int main(int argc, char *argv[])
{
//     quint8 nRows = 4;
//     double testValues[] = {82, 83, 69, 92,
//                            77, 37, 49, 92,
//                            11, 69,  5, 86,
//                            8,  9, 98, 23};

//     quint8 nRows = 4;
//     double testValues[] = { 4, 2,  5,  7,
//                             8, 3, 10,  8,
//                            12, 5,  4,  5,
//                             6, 3,  7, 14};

//     quint8 nRows = 4;
//     double testValues[] = {16,  2,  3,  7,
//                             5, 13,  7,  5,
//                             8,  6,  5,  9,
//                             3,  4,  5, 11};

//     quint8 nRows = 2;
//     double testValues[] = {2000,  2200,
//                             0.1, 201};

//     quint8 nRows = 4;
//     double testValues[] = {7100.48068795346,7188.25889906589,7336.42324297065,7866.89169875879,
//                            3577.67522282278,4135.51689635044,4722.60944817587,5765.57031003872,
//                            3058.95406961268,3671.19871431662,4300.697617829,5370.64698150977,
//                            2903.30845760488,3542.55557472286,4191.41980717752,5268.60977867976};

    quint8 nRows = 5;
    double testValues[] = {5923.7,5923.7,3939.72,751.629,0,
                           1179.8,1179.8,8554.71,4381.96,0,
                           1689.11,1689.11,9144.72,5374.27,0,
                           3289.66,3289.66,8068.07,5019.26,0,
                           2834.14,2834.14,8339.84,5057.21,0};

//     quint8 nRows = 5;
//     double testValues[] = { 1687.9, 1460.8, 1460.8, 1460.8, 1460.8,
//                            915.977,953.814,953.814,953.814,953.814,
//                            996.121,1359.18,1359.18,1359.18,1359.18,
//                            2223.04,1296.86,1296.86,1296.86,1296.86,
//                            844.808,2577.72,2577.72,2577.72,2577.72};

//     quint8 nRows = 5;
//    double testValues[] = {18, 11, 16, 20, 0,
//                           14, 19, 26, 18, 0,
//                           21, 23, 35, 29, 0,
//                           32, 27, 21, 17, 0,
//                           16, 15, 28, 25, 0};

    HungarianAlgorithm solver(nRows);

    quint8 index = 0;

    for(quint8 row = 0; row < nRows; ++row)
    {
        for(quint8 column = 0; column < nRows; ++column)
        {
            solver.vSetMatrixValue(row, column, testValues[index]);
            index++;
        }
    }
    QVector<Assignment> sol = solver.runAlgorithm();

    for(Assignment s : sol)
    {
        qDebug() << "Robot:" << s.robot << " -> Task:" << s.task << "Used?" << s.used;
    }


    return 0;
}
