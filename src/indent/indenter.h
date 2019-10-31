#pragma once


namespace Qutepart {

class IndentAlg {
};

class Indenter {
public:
    Indenter();
    virtual ~Indenter();


private:
    IndentAlg* alg;
};
};
