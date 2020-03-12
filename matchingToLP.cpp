#include "matching.hpp"

static const float min_r = .5;
static const float max_r = 50;

static float unmatchedPenalty = 50;

void setPenalty(float value) {
    unmatchedPenalty = value;
}

static float distance(const Point &a, const Point &b) {
    return cv::norm(b-a);
}

static std::vector<Edge> bipartiteToLP(glp_prob *lp, const std::vector<Point> &inNodes, const std::vector<Point> &outNodes) {
    /*
     * Populates the linear problem lp with an equivalent bipartite
     * matching problem given by two sets of points in space.
     *
     * Returns a vector of index-pairs representing *all* edges that were
     * considered for matching.
     */
    const int u = inNodes.size(), v = outNodes.size();

    // minimize
    glp_set_obj_dir(lp, GLP_MIN);

    // each row represents one constraint corresponding to one node
    glp_add_rows(lp, u+v);

    // allow each vertex in U and V to be incident eactly one edge
    for (int i = 1; i <= u+v; ++i)
        glp_set_row_bnds(lp, i, GLP_FX, 1, 1);

    int col; float dist;

    // first element is dummy because glpk begins counting at 1
    const std::vector<double> valArray{0.0, 1.0, 1.0};
    std::vector<int> indArray;

    std::vector<Edge> edges;
    for (int i = 0; i < u; ++i) {
        for (int j = 0; j < v; ++j) {
            dist = distance(inNodes.at(i), outNodes.at(j));

            // only use edge when its short enough
            if ((min_r < dist) && (dist < max_r)) {
                edges.push_back({i, j});

                col = glp_add_cols(lp, 1);

                // set coefficients of incident nodes to 1.0
                indArray = {0, i+1, u+j+1};
                glp_set_mat_col(lp, col, 2, &indArray[0], &valArray[0]);

                // set objective cost to the distance
                glp_set_obj_coef(lp, col, dist);

                // constrain edge like a binary value
                glp_set_col_bnds(lp, col, GLP_DB, 0, 1);
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
        glp_set_col_bnds(lp, col, GLP_DB, 0, 1);
        ++indArray.at(1);
        ++col;
    }

    return edges;
}

std::vector<Edge> solveBipartite(const std::vector<Point> &inNodes, const std::vector<Point> &outNodes) {
    /* Initializes LP problem and applies the previous function.
     *
     * This returns a vector of all actual edges of the resulting matching.
     */
    glp_prob *lp = glp_create_prob();
    const auto edges = bipartiteToLP(lp, inNodes, outNodes);

    glp_smcp params;
    glp_init_smcp(&params);
    params.msg_lev = GLP_MSG_ERR; // be quiet unless you have to say something

    std::cout << "solving ...";
    auto r = glp_simplex(lp, &params);
    if (r != 0)
        throw std::runtime_error("simplex failed");


    std::vector<Edge> matchedEdges;
    for (int i = 0; i < edges.size(); ++i)
        // logical threshold at which an edge is considered matched
        if (glp_get_col_prim(lp, i+1) > 0.5)
            matchedEdges.push_back(std::move(edges.at(i)));

    glp_delete_prob(lp);

    std::cout << " done. matched " << matchedEdges.size() << " edges" <<std::endl;
    return matchedEdges;
}
