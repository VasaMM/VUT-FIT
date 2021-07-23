/**
 * @file tree_mesh_builder.cpp
 * @author Václav Martinka <xmarti76@stud.fit.vutbr.cz>
 * @brief Parallel Marching Cubes implementation using OpenMP tasks
 * @date 14. 12. 2019 (18:52)
 **/

#include <iostream>
#include <math.h>
#include <limits>
#include "omp.h"

#include "tree_mesh_builder.h"
#include "base_mesh_builder.h"


TreeMeshBuilder::TreeMeshBuilder(unsigned gridEdgeSize)
    : BaseMeshBuilder(gridEdgeSize, "Octree") {
}

void TreeMeshBuilder::prepareEmptyConditionValues() {
    for (size_t i = mGridSize; i > 1; i /= 2) {
        emptyConditionValues.push_back(mIsoLevel + (sqrt(3.0) / 2.0) * static_cast<float>(i) * static_cast<float>(mGridResolution));
    }
}


// unsigned totalTests = 0;

unsigned TreeMeshBuilder::marchCubes(const ParametricScalarField &field) {
    prepareEmptyConditionValues();


    // 1. Compute total number of cubes in the grid.
    size_t mGridSize_square = mGridSize * mGridSize;
    size_t totalCubesCount = mGridSize * mGridSize_square;

    unsigned totalTriangles = 0;

    // Vytvorim prvni krychli
    marchSubcubes(field, &totalTriangles, Vec3_t<unsigned> (0,0,0), 0);

    #pragma omp taskwait

    // printf("total: %d\n", totalTests);

    return totalTriangles;
}




void TreeMeshBuilder::marchSubcubes(const ParametricScalarField &field, unsigned* totalTriangles, const Vec3_t<unsigned> &position, unsigned depth) {
    unsigned a = mGridSize / pow(2, depth);

    // Spoctu vysledek
    if (a == 1) {
        unsigned triangles = buildCube(Vec3_t<float> (position.x, position.y, position.z), field);
        
        #pragma omp atomic update
        *totalTriangles += triangles;
        
        // #pragma omp atomic update
        // ++totalTests;
    }
    // Jinak vytvorim 8 podkrychli
    else {
        // FIXME stred
        Vec3_t<float> realPos(
            static_cast<float>(position.x + a / 2) * mGridResolution, 
            static_cast<float>(position.y + a / 2) * mGridResolution, 
            static_cast<float>(position.z + a / 2) * mGridResolution);
        float value = evaluateFieldAt(realPos, field);

        if (!isEmpty(value, depth)) {
            // Posunu se o uroven
            a /= 2;
            ++depth;

            // Vytvorim 8 novych podkrychli
            for (size_t z = 0; z < 2; ++z) {
                for (size_t y = 0; y < 2; ++y) {
                    #pragma omp simd
                    for (size_t x = 0; x < 2; ++x) {
                        #pragma omp task
                        {
                            Vec3_t<unsigned> newPosition(position.x + x * a, position.y + y * a, position.z + z * a);
                            marchSubcubes(field, totalTriangles, newPosition, depth);
                        }
                    }
                }
            }
        }
    }
}




float TreeMeshBuilder::evaluateFieldAt(const Vec3_t<float> &pos, const ParametricScalarField &field) {

    // NOTE: This method is called from "buildCube(...)"!
    float value = std::numeric_limits<float>::max();
    
    // 1. Store pointer to and number of 3D points in the field
    //    (to avoid "data()" and "size()" call in the loop).
    const Vec3_t<float> *pPoints = field.getPoints().data();
    const unsigned count = unsigned(field.getPoints().size());


    // 2. Find minimum square distance from points "pos" to any point in the
    //    field.
    // #pragma omp parallel for simd reduction(min:value)
    #pragma omp simd
    for(unsigned i = 0; i < count; ++i) {
        float distanceSquared  = (pos.x - pPoints[i].x) * (pos.x - pPoints[i].x);
        distanceSquared       += (pos.y - pPoints[i].y) * (pos.y - pPoints[i].y);
        distanceSquared       += (pos.z - pPoints[i].z) * (pos.z - pPoints[i].z);

        // Comparing squares instead of real distance to avoid unnecessary
        // "sqrt"s in the loop.
        value = std::min(value, distanceSquared);
    }

    // 3. Finally take square root of the minimal square distance to get the real distance
	return sqrt(value);
}

void TreeMeshBuilder::emitTriangle(const Triangle_t &triangle)
{
	// NOTE: This method is called from "buildCube(...)"!

	// Store generated triangle into vector (array) of generated triangles.
	// The pointer to data in this array is return by "getTrianglesArray(...)" call
	// after "marchCubes(...)" call ends.
	#pragma omp critical(emitTriangle)
	{
		mTriangles.push_back(triangle);
	}
}
