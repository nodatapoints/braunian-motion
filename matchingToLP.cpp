#include <iostream>
#include <vector>
#include <glpk.h>

#include <opencv2/opencv.hpp>

const float max_r = 50;
const float unmatchedPenalty = 5000;

using Point = cv::Point2f;

float distance(const Point &a, const Point &b) {
    return cv::norm(b-a);
}

struct Edge {
    int in, out;
};

std::vector<Edge> bipartiteToLP(glp_prob *lp, const std::vector<Point> &inNodes, const std::vector<Point> &outNodes) {
    /*
     * Populates the linear problem lp with an equivalent bipartite
     * matching problem given by two sets of points in space.
     *
     * Returns a vector of index-pairs representing all edges of the matching.
     */
    const int u = inNodes.size(), v = outNodes.size();

    // minimize
    glp_set_obj_dir(lp, GLP_MIN);

    // each row represents one constraint corresponding to one node
    glp_add_rows(lp, u+v);

    // allow each vertex in U and V to be incident to one edge at most
    // by constraining 0 <= x_1 ... x_(u+v) <= 1
    for (int i = 1; i <= u+v; ++i)
        glp_set_row_bnds(lp, i, GLP_FX, 0, 1);

    int val, col;
    float dist;

    // first element is dummy because glpk begins counting at 1
    const std::vector valArray{0.0, 1.0, 1.0};
    std::vector<int> indArray;

    std::vector<Edge> edges;
    for (int i = 0; i < u; ++i) {
        for (int j = 0; j < v; ++j) {
            dist = distance(inNodes.at(i), outNodes.at(j));

            // only use edge when its short enough
            if ((val = (dist < max_r) ? 1 : 0)) {
                edges.emplace_back(i, j);

                col = glp_add_cols(lp, 1);

                // set coefficients of incident nodes to 1.0
                indArray = {0, i+1, u+j+1};
                glp_set_mat_col(lp, col, 2, &indArray[0], &valArray[0]);

                // set objective cost to the distance
                glp_set_obj_coef(lp, col, dist);

                // constrain edge like a binary value
                glp_set_col_bnds(lp, col, GLP_FX, 0, 1);
            }
        }
    }

    // create one virtual edge to a nonexistent node for each node
    // column i+u+v represents the i-th virtual edge
    col = glp_add_cols(lp, u+v);
    indArray = {0, 1};
    for (int i = 1; i <= u+v; ++i) {
        // set coefficient to 1 only for the corresponing node
        glp_set_mat_col(lp, col, 1, &indArray[0], &valArray[0]);

        // weigh edge with a penalty
        glp_set_obj_coef(lp, col, unmatchedPenalty);

        // constrain edge like a binary value
        glp_set_col_bnds(lp, col, GLP_FX, 0, 1);
        ++indArray.at(1);
        ++col;
    }

    return edges;
}

std::vector<Edge> bipartiteToLP(const std::vector<Point> &inNodes, const std::vector<Point> &outNodes) {
    glp_prob *lp = glp_create_prob();
    const auto edges = bipartiteToLP(lp, inNodes, outNodes);

    auto r = glp_simplex(lp, nullptr);
    if (r != 0) {
        std::cerr << "simplex failed: " << r << std::endl;
    }

    std::vector<Edge> matchedEdges;
    for (int i = 0; i < edges.size(); ++i)
        // logical threshold
        if (glp_get_col_prim(lp, i+1) > 0.5)
            matchedEdges.push_back(std::move(edges.at(i)));

    glp_delete_prob(lp);
    return matchedEdges;
}
