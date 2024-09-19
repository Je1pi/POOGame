#ifndef MAP_HPP
#define MAP_HPP

#include "../ASCII_Engine/ObjetoDeJogo.hpp"
#include "Entity.hpp"
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class Map : public ObjetoDeJogo {
    private:
        vector<vector<int>> mapMatrix;
        string path;
        int rows, cols;
        int offsetL, offsetC;

    public:
        Map(const string& path, const int& posL, const int& posC, const int& offsetL = 0, const int& offsetC = 0) 
            : path(path), rows(0), cols(0), offsetL(offsetL), offsetC(offsetC), ObjetoDeJogo("Map", Sprite(path), posL, posC) {
            genMap();
        }

        ~Map() {}

        void genMap() {
            ifstream file(path);
            if (!file.is_open()) {
                throw runtime_error("Unable to open file");
            }

            string line;
            vector<vector<int>> tempMatrix;
            while (getline(file, line)) {
                vector<int> row;
                for (char ch : line) {
                    if (ch == '_' || ch == '|') {
                        row.push_back(1);
                    } else {
                        row.push_back(0);
                    }
                }
                tempMatrix.push_back(row);
            }
            file.close();

            mapMatrix = tempMatrix;
            rows = mapMatrix.size();
            cols = rows > 0 ? mapMatrix[0].size() : 0;
        }

        bool colission(Entity* entity) const {
            int entityPosL = entity->getPosL() - offsetL;
            int entityPosC = entity->getPosC() - offsetC;
            int entityHeight = entity->getSprite()->getAltura();
            int entityWidth = entity->getSprite()->getLarguraMax();

            for (int i = entityPosL; i < entityPosL + entityHeight; ++i) {
                for (int j = entityPosC; j < entityPosC + entityWidth; ++j) {
                    if (i >= 0 && i < rows && j >= 0 && j < cols) {
                        if (mapMatrix[i][j] == 1) {
                            return true;
                        }
                    }
                }
            }

            return false;
        }

        void draw(SpriteBase &screen) {
            ObjetoDeJogo::draw(screen, getPosL(), getPosC());
        }
};

#endif