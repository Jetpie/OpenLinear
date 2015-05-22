// Logistic Regression
//
// @author: Bingqing Qu
//
// Copyright (C) 2014-2015  Bingqing Qu <sylar.qu@gmail.com>
//
// @license: See LICENSE at root directory
#include "logistic.hpp"
#include "solver.hpp"

/**
 * Train the coefficients using depends on the parameters.
 *
 * @param dataset training dataset
 * @param param parameters
 *
 */
void
LogisticRegression::train(const DatasetPtr dataset, const ParamPtr param)
{
    // input sanity check
    if(!param)
        cerr << "LogisticRegression::train : Error input, param NULL or not valid"
             << __FILE__ << "," << __LINE__ << endl;
    if(!dataset)
        cerr << "LogisticRegression::train : Error input, dataset NULL or not valid"
             << __FILE__ << "," << __LINE__ << endl;

    size_t n_samples = dataset->n_samples;
    size_t dimension = dataset->dimension;
    size_t n_classes = dataset->n_classes;

    // initialize model
    ModelPtr model = make_shared<Model>();
    // sanity check
    assert(model);

    model->bias = param->bias;
    model->dimension = dimension;
    model->n_classes = n_classes;

    vector<size_t> count;
    vector<size_t> start_idx;
    vector<size_t> perm_idx;
    count.reserve(n_classes);
    start_idx.reserve(n_classes);
    perm_idx.reserve(n_samples);

    // adjust the label sequence to if n_classes = 2 to keep +1 label
    // in advance
    if(n_classes==2 && dataset->labels[0] == -1 && dataset->labels[1] == 1)
        std::swap(dataset->labels[0],dataset->labels[1]);

    model->labels = dataset->labels;

    // preprocess the training dataset
    preprocess_data(dataset, count, start_idx, perm_idx);

    // permute the instances
    // -construct the Eigen permutation matrix
    PermutationMatrix<Dynamic,Dynamic> perm_matrix(n_samples);
    for(size_t i = 0; i < n_samples ;++i)
        perm_matrix.indices()[i] = perm_idx[i];
    // -permutation columns
    *(dataset->X) = (*(dataset->X) * perm_matrix).eval();

    // make copy of dataset for training
    // dataset->X is read-only so no need for deep copy

    size_t k;
    // handle two class classification problem
    if(n_classes == 2)
    {
        // initialize weights to -0.5 ~ 0.5
        srand((unsigned int) time(0));
        ColVector w = ColVector::Random(dimension,1) / 2;

        // rearrange labels
        size_t pos_end = start_idx[0] + count[0];
        for(k=0;k<pos_end;++k)
            dataset->y[k] = dataset->labels[0];
        for(;k<n_samples;++k)
            dataset->y[k] = dataset->labels[1];

        // make problem
        shared_ptr<L2R_LR_Problem> problem = make_shared<L2R_LR_Problem>(dataset);
        GradientDescent gd;
        gd.solve(problem, param, w);

        double* W_ = new double[w.rows()*w.cols()];
        for(int i = 0; i < w.rows();++i)
        {
            for(int j = 0; j < w.cols(); ++j)
            {
                W_[i * w.cols() + j] = w(i,j);
            }
        }

        // load parameter pointer into model, this is good practice if
        // this is production environment, in which read and write
        // manipulations are quite sensitive.
        model->W_ = W_;
    }
    else
    {
        cout << ">2classes" << endl;
    }

    // TODO: add training function calls


    this->trained_ = 1;
    // Finally, pass model variable to member model
    this->model_ = model;


    /*
    A benchmark test for different permuation strategies
    // start = clock();
    // SpColMatrix perm_done(dimension,n_samples);
    // for(size_t i = 0; i < n_samples ;++i)
    //     perm_done.col(i) = dataset->X->col(perm_idx[i]);
    // cout << "time perm_done:" << float(clock() -start)/CLOCKS_PER_SEC << endl;
    */
    // for(size_t i=0;i<n_samples;++i)
    // {
    //     cout << perm_idx[i] << ",";
    // }
    // cout << endl;
    // for(size_t i=0;i<n_classes;++i)
    // {
    //     cout << start_idx[i] << ",";
    // }
    // cout << endl;
    // for(size_t i=0;i<n_classes;++i)
    // {
    //     cout << count[i] << ",";
    // }
    // cout << endl;
}
