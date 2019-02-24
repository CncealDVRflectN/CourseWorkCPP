#ifndef COURSEWORKCPP_RIGHTSWEEP_H
#define COURSEWORKCPP_RIGHTSWEEP_H

#include <vector>

class RightSweep
{
public:
    void calcRightSweep(std::vector<double> &lowerDiagonal,
                        std::vector<double> &mainDiagonal,
                        std::vector<double> &upperDiagonal,
                        std::vector<double> &vect,
                        std::vector<double> &solutionDest);

private:
    std::vector<double> alpha;
    std::vector<double> beta;


    void calcAlpha(std::vector<double> &lowerDiagonal,
                   std::vector<double> &mainDiagonal,
                   std::vector<double> &upperDiagonal);

    void calcBeta(std::vector<double> &lowerDiagonal,
                  std::vector<double> &mainDiagonal,
                  std::vector<double> &vect);

    void calcSolution(std::vector<double> &solutionDest);

    bool isCorrect(
            std::vector<double> &lowerDiagonal,
            std::vector<double> &mainDiagonal,
            std::vector<double> &upperDiagonal);
};


#endif //COURSEWORKCPP_RIGHTSWEEP_H
