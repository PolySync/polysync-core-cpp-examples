//A* Searcher
#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <PolySyncNode.hpp>
#include <PolySyncDataModel.hpp>
#include <cmath>
#include <iostream>
#include <armadillo>

#include "GridMap.hpp"

using namespace arma;
using namespace std;
using namespace polysync::datamodel;

class Search {

public:

    Search( );

    ~Search( );

    void searchAStar( int startLoc );

    void plotOptimalPath( );

    GridMap* world;
    int curLoc;
    std::vector< std::vector<int> > path;


private:

    void initializeSearchSpace( );

    int getSearchNode( );

    void getNeighbors( int index );

    arma::Mat<int> searchMap;
    arma::Mat<float> heuristic;
    arma::Mat<float> globalScore;
    arma::Mat<float> pathScore;
    float epsilon{2};
    bool endGame{false};
    int newLoc;
    arma::uword curLocU;
    arma::uword newLocU;
    std::vector< std::vector<int> > moves;
    std::array<int, 8> tempMoves;
    std::vector<int> closedSet;
    std::vector<int> openSet;
    int exploredNodes{1};

};

#endif // SEARCH_HPP
