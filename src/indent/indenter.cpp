#include "indenter.h"

namespace Qutepart {

class BaseIndentAlg: public IndentAlg {
};

Indenter::Indenter():
    alg(new BaseIndentAlg())
{
}

Indenter::~Indenter() {
    delete alg;
}

};
