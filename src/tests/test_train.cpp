#include "high_level_function.hpp"
#include "logistic.hpp"
using namespace oplin;
using namespace std;
using namespace Eigen;
int main(int argc, char** argv)
{
    std::cout.precision(10);
    string filename = "res/sample";
    string result = "res/result";
    size_t dimension = 13;

    DatasetPtr dataset = read_dataset(filename,dimension,-1,270);

    ParamPtr param = make_shared<Parameter>();
    param->solver_type = 0;
    param->problem_type = 0;
    param->bias = 0.001;
    param->rela_tol = 0.0001;
    param->abs_tol = 0.00001;
    param->max_epoch = 500;
    param->learning_rate = 0.01;
    vector<double> C(dataset->n_samples,1.);
    param->C = C;

    shared_ptr<LinearBase> lr= make_shared<LogisticRegression>();

    // try
    // {
    //     lr->train(dataset, param);
    // }
    // catch(...)
    // {
    //     cout << "bad allocation" << endl;
    // }

    lr->train(dataset, param);

    predict_all(filename,result,lr,true);

}
