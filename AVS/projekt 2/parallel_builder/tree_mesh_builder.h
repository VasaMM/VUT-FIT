/**
 * @file tree_mesh_builder.h
 * @author Václav Martinka <xmarti76@stud.fit.vutbr.cz>
 * @brief Parallel Marching Cubes implementation using OpenMP tasks
 * @date 14. 12. 2019 (17:29)
 **/


#ifndef TREE_MESH_BUILDER_H
#define TREE_MESH_BUILDER_H

#include <vector>
#include "base_mesh_builder.h"

/**
 * @brief The RefMeshBuilder class
 */
class TreeMeshBuilder : public BaseMeshBuilder
{
public:
    TreeMeshBuilder(unsigned gridEdgeSize);

protected:
    unsigned marchCubes(const ParametricScalarField &field);
    void marchSubcubes(const ParametricScalarField &field, unsigned* totalTriangles, const Vec3_t<unsigned> &position, unsigned depth);
    float evaluateFieldAt(const Vec3_t<float> &pos, const ParametricScalarField &field);
    void emitTriangle(const Triangle_t &triangle);
    const Triangle_t *getTrianglesArray() const { return mTriangles.data(); }

    std::vector<Triangle_t> mTriangles; ///< Temporary array of triangles

private:
	std::vector<float> emptyConditionValues;
	void prepareEmptyConditionValues();
	bool isEmpty(const float value, const unsigned depth) {return value > emptyConditionValues.at(depth);}
};

#endif // TREE_MESH_BUILDER_H
