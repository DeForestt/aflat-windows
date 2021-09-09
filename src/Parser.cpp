#include "Parser.hpp"
#include "Exceptions.hpp"


AST::Statment* parse::Parser::parseStmt(links::LinkedList<lex::Token*> &tokens){
    AST::Statment* output = new AST::Statment;
    if(dynamic_cast<lex::LObj *>(tokens.peek()) != nullptr){
        lex::LObj obj = *dynamic_cast<lex::LObj *>(tokens.peek());
        tokens.pop();
        //Declare a byte;
        if(obj.meta == "byte" | obj.meta == "int" | obj.meta == "char"){
            AST::Declare * dec = new AST::Declare();
            //ensures the the current token is an Ident
            if(dynamic_cast<lex::LObj *>(tokens.peek()) != nullptr){
                lex::LObj obj = *dynamic_cast<lex::LObj *>(tokens.peek());
                tokens.pop();
                dec->Ident = obj.meta;
                if(obj.meta == "byte") dec->type = AST::Byte; else if (obj.meta == "int") dec->type = AST::Int; else if (obj.meta == "char")dec->type = AST::Char;
                output = dec;
                if(dynamic_cast<lex::OpSym *>(tokens.peek()) != nullptr){
                    lex::OpSym sym = *dynamic_cast<lex::OpSym *>(tokens.peek());
                    if(sym.Sym == '*'){
                        tokens.pop();
                        switch(dec->type){
                            case AST::Int:
                                dec->type = AST::IntPtr;
                                break;
                            case AST::Char:
                                dec->type = AST::CharPtr;
                                break;
                            default:
                                throw err::Exception("Dont know how to creat this type of pointer " + obj.meta);
                                break;
                        }
                    }
                }
                if(dynamic_cast<lex::OpSym *>(tokens.peek()) != nullptr){
                    lex::OpSym sym = *dynamic_cast<lex::OpSym *>(tokens.peek());
                    //Checking for Perenth to see if it is a function
                    if (sym.Sym == '('){
                        tokens.pop();
                        AST::Function * func = new AST::Function();
                        func->ident.ident = dec->Ident;
                        func->type = dec->type;
                        func->args = this->parseArgs(tokens);
                        if(dynamic_cast<lex::OpSym *>(tokens.peek()) != nullptr){
                            sym = *dynamic_cast<lex::OpSym *>(tokens.peek());
                            if (sym.Sym == '{'){
                                tokens.pop();
                                func->statment = this->parseStmt(tokens);
                                output = func;
                                delete(dec);
                            }
                        }
                    }else if (sym.Sym == '=')
                    {
                        tokens.pop();
                        AST::DecAssign * assign = new AST::DecAssign;
                        assign->declare = dec;
                        assign->expr = this->parseExpr(tokens);
                        output = assign;
                    }
                    
                }
                
            }
            else{
                throw err::Exception("Unparsable token found");
            }
        }
        else if (obj.meta == "return")
        {
            AST::Return * ret = new AST::Return;
            ret->expr = this->parseExpr(tokens);
            output = ret;
        }else{
            if (dynamic_cast<lex::OpSym *>(tokens.peek()) != nullptr){
                lex::OpSym * sym = dynamic_cast<lex::OpSym *> (tokens.pop());
                if(sym->Sym == '='){
                    AST::Assign * assign = new AST::Assign();
                    assign->Ident = obj.meta;
                    assign->expr = this->parseExpr(tokens);
                    output = assign;
                }else if (sym->Sym == '(')
                {
                    AST::Call * call = new AST::Call();
                    call->ident = obj.meta;
                    bool pop = false;
                    if(dynamic_cast<lex::OpSym *>(tokens.peek()) == nullptr){
                        do{
                            if (pop) tokens.pop();
                            call->Args.push(this->parseExpr(tokens));
                            pop = true;
                        }while(dynamic_cast<lex::OpSym *>(tokens.peek()) != nullptr
                        && dynamic_cast<lex::OpSym *>(tokens.peek())->Sym == ',');

                        if(dynamic_cast<lex::OpSym *>(tokens.peek()) != nullptr){
                            lex::OpSym * symp = dynamic_cast<lex::OpSym *> (tokens.pop());
                            if (symp->Sym != ')') throw err::Exception("Expected closed perenth got " + symp->Sym);
                        }
                    }
                    output = call;
                }
                 else throw err::Exception("expected assignment oporator");
            }else throw err::Exception("expected Asignment oporator after " + obj.meta);
        }
        
    }
    
    if (tokens.head == nullptr){ 
        this->Output = *output;
        return output;
    } else if(dynamic_cast<lex::OpSym *>(tokens.peek()) != nullptr & tokens.head->next != nullptr)
    {
        lex::OpSym obj = *dynamic_cast<lex::OpSym *>(tokens.peek());
        tokens.pop();

        if(obj.Sym == ';'){
        AST::Sequence * s = new AST::Sequence; 
        s->Statment1 = output;
        s->Statment2 = this->parseStmt(tokens);
        this->Output = *s;
        return s;
        }
        else if(obj.Sym == '}'){
            this->Output = *output;
            return output;
        }
    } else{ if (dynamic_cast<lex::OpSym *>(tokens.peek()) != nullptr & tokens.head->next == nullptr){
            lex::OpSym obj = *dynamic_cast<lex::OpSym *>(tokens.peek());
            tokens.pop();
            if(obj.Sym == '}'){
                this->Output = *output;
                return nullptr;
            }else if (obj.Sym == ';')
            {
                this->Output = *output;
                return output;
            }
            
        }
    }
    return output;
}

AST::Statment* parse::Parser::parseArgs(links::LinkedList<lex::Token*> &tokens){
    AST::Statment* output = new AST::Statment();
    if(dynamic_cast<lex::LObj *>(tokens.peek()) != nullptr){
        lex::LObj obj = *dynamic_cast<lex::LObj *>(tokens.peek());
        tokens.pop();
        if(obj.meta == "int"){
            AST::Argument * declare = new AST::Argument();
            declare->type = AST::Int;
            if(dynamic_cast<lex::LObj *>(tokens.peek()) != nullptr){
                lex::LObj l = *dynamic_cast<lex::LObj *>(tokens.pop());
                declare->Ident = l.meta;
                declare->type = AST::Int;
                output = declare;
            }
        }
    }

    if (tokens.head == nullptr){
        throw err::Exception("unterminated functioncall");
    } else if(dynamic_cast<lex::OpSym *>(tokens.peek()) != nullptr & tokens.head->next != nullptr)
    {
        lex::OpSym obj = *dynamic_cast<lex::OpSym *>(tokens.peek());
        tokens.pop();

        if(obj.Sym == ','){
        AST::Sequence * s = new AST::Sequence;
        s->Statment1 = output;
        s->Statment2 = this->parseStmt(tokens);
        return s;
        }
        else if(obj.Sym == ')'){
            return output;
        }
    }
    return output;
}

AST::Expr* parse::Parser::parseExpr(links::LinkedList<lex::Token*> &tokens){
    AST::Expr * output = new AST::Expr();
    if (dynamic_cast<lex::StringObj *>(tokens.peek()) != nullptr){
        lex::StringObj stringObj = *dynamic_cast<lex::StringObj *>(tokens.peek());
        tokens.pop();
        AST::StringLiteral * slit = new AST::StringLiteral();
        slit->val = stringObj.value;
        output = slit;
    } else if(dynamic_cast<lex::INT *>(tokens.peek()) != nullptr){
        lex::INT intObj = *dynamic_cast<lex::INT *>(tokens.pop());
        AST::IntLiteral * ilit = new AST::IntLiteral();
        ilit->val = std::stoi(intObj.value);
        output = ilit;
    } else if(dynamic_cast<lex::LObj *>(tokens.peek()) != nullptr){
        lex::LObj obj = *dynamic_cast<lex::LObj *>(tokens.pop());
        AST::Var * var = new AST::Var();
        var->Ident = obj.meta;
        output = var;
    } else if (dynamic_cast<lex::CharObj *>(tokens.peek()) != nullptr){
        lex::CharObj obj = *dynamic_cast<lex::CharObj *>(tokens.pop());
        AST::CharLiteral * charlit = new AST::CharLiteral();
        charlit->value = obj.value;
        output = charlit;
    }else if (dynamic_cast<lex::Ref *>(tokens.peek()) != nullptr)
    {
        tokens.pop();
        AST::Refrence * ref = new AST::Refrence();
        if (dynamic_cast<lex::LObj *>(tokens.peek()) != nullptr){
            lex::LObj obj = * dynamic_cast<lex::LObj *>(tokens.pop());
            ref->Ident = obj.meta;
        }else throw err::Exception("No object given to refrece");
    }
    
    
    else throw err::Exception("Unknown Expr");

    if (dynamic_cast<lex::OpSym *>(tokens.peek()) != nullptr){

        AST::Compound  * compound = new AST::Compound();

        lex::OpSym sym = *dynamic_cast<lex::OpSym *>(tokens.peek());
        if (sym.Sym == '+')
        {
            tokens.pop();
            compound->op = AST::Plus;
            compound->expr1 = output;
            compound->expr2 = this->parseExpr(tokens);
            return compound;
        }
    }

    return output;
}