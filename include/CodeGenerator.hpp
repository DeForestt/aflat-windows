#ifndef GEN
#define GEN

#include "AST.hpp"
#include "LinkedList.hpp"
#include "ASM.hpp"


namespace gen{
    class Symbol{
        public:
        std::string symbol;
        int byteMod;
    };
    class CodeGenerator{
        private:
        links::LinkedList<Symbol> SymbolTable;
        std::string GenExpr(AST::Expr);
        
        public:
        ASMC::File GenSTMT(AST::Statment * stmt);
    };
}

#endif