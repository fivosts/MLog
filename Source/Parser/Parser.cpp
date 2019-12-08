#include "Parser.h"
#include "Import.h"
#include "ModelProperties.h"
#include "LayerProperties.h"
#include <algorithm>


std::unique_ptr<Model> Parser::parseModel()
{

    /* The order of evaluation of function arguments is unspecified. */
    /* They have to be specified explicitly */
    setPtr<Import> imp;
    setPtr<Attribute> attr;
    setPtr<Layer> lay;

    parseImport(std::move(imp));
    parseAttribute(std::move(attr));
    parseLayer(std::move(lay));

    return std::make_unique<Model>(std::move(imp), std::move(attr), std::move(lay));
}

void Parser::parseImport(setPtr<Import> &&imports)
{
#ifdef PARDBG
    printf("PARSER:\t\tparseImport()\n");
#endif
    if (accept(TType::IMPORT))
    {
        expect(TType::IMPORT);
        Token importElement = expect(TType::STR_LITERAL);
        parseNewLines();
        imports.insert(std::make_unique<Import>(Import(importElement.getData())));
        parseImport(std::move(imports));   
    }
    return;
}

void Parser::parseAttribute(setPtr<Attribute> &&attributes)
{
#ifdef PARDBG
    printf("PARSER:\t\tparseAttribute()\n");
#endif

    if (accept(TType::DEFINE))
    {
        expect(TType::DEFINE);
        std::unique_ptr<Attribute> attr = parseIdentifier<Attribute>(expect(TType::IDENTIFIER));
        parseNewLines();
        attributes.insert(std::move(attr));
        parseAttribute(std::move(attributes));
    }
    return;
}

// TODO
void Parser::parseLayer(setPtr<Layer> /* &&layers */ )
{
#ifdef PARDBG
    printf("PARSER:\t\tparseLayer()\n");
#endif
    // setPtr<Layer> a;
    // return a;
    return;
}

template<typename T>
std::unique_ptr<T> Parser::parseIdentifier(Token expectedID, specializer<T>)
{
    //TODO add info to the exception
    (void) expectedID;
    throw CompExcept("Parsing Error: Identifier type not recognized");
}

std::unique_ptr<Attribute> Parser::parseIdentifier(Token expectedID, specializer<Attribute>)
{
    std::string IDName = expectedID.getData();

    if (IDName == "network_name")
    {
        return std::unique_ptr<Attribute>(new NetworkName(parseStrLiteral()));
    }
    else if (IDName == "backend")
    {
        return std::unique_ptr<Attribute>(new Backend(parseStrLiteral()));
    }
    else if (IDName == "target")
    {
        return std::unique_ptr<Attribute>(new Target(parseStrLiteral()));
    }
    else if (IDName == "cuda_available")
    {
        return std::unique_ptr<Attribute>(new CUDA(parseBoolLiteral()));
    }
    else if (IDName == "input_tensors")
    {
        return std::unique_ptr<Attribute>(new InpTensors(parseStrArrLiteral()));
    }
    else if (IDName == "output_tensors")
    {
        return std::unique_ptr<Attribute>(new OutTensors(parseStrArrLiteral()));
    }
    else
    {
        throw CompExcept("Parsing Error: Attribute not recognized");
    }
    return nullptr;
}

std::unique_ptr<LayerParams> Parser::parseIdentifier(Token expectedID, specializer<LayerParams>)
{
    std::string IDName = expectedID.getData();
    expect(TType::ASSIGN);

    if (IDName == "input")
    {
        std::unique_ptr<BinExpr> e = parseBinExpr();
        parseNewLines();
        return std::unique_ptr<LayerParams>(new Input(std::move(e)));
    }
    else if (IDName == "output")
    {
        std::string str = parseStrLiteral();
        parseNewLines();
        return std::unique_ptr<LayerParams>(new Output(str));
    }
    else if (IDName == "layer_name")
    {
        std::string str = parseStrLiteral();
        parseNewLines();
        return std::unique_ptr<LayerParams>(new LayerName(str));
    }
    else
    {
        throw CompExcept("Parsing Error: LayerParam not recognized");
    }
    return nullptr;
    
}

std::unique_ptr<LSTMParams> Parser::parseIdentifier(Token expectedID, specializer<LSTMParams>)
{
    std::string IDName = expectedID.getData();
    expect(TType::ASSIGN);
    int id = parseIntLiteral();

    if (IDName == "input_size")
    {
        parseNewLines();
        return std::unique_ptr<LSTMParams>(new InputSize(id));
    }
    else if (IDName == "output_timestep")
    {
        parseNewLines();
        return std::unique_ptr<LSTMParams>(new OutputTimestep(id));
    }
    else if (IDName == "hidden_size")
    {
        parseNewLines();
        return std::unique_ptr<LSTMParams>(new HiddenSize(id));
    }
    else if (IDName == "num_layers")
    {
        parseNewLines();
        return std::unique_ptr<LSTMParams>(new NumLayers(id));
    }
    else
    {
        throw CompExcept("Parsing Error: LSTMParam not recognized");
    }
    return nullptr;
}

std::unique_ptr<MLParams> Parser::parseIdentifier(Token expectedID, specializer<MLParams>)
{
    std::string IDName = expectedID.getData();
    expect(TType::ASSIGN);
    
    if (IDName == "in_features")
    {
        //! TODO this looks troublesome
        // return new InFeatures<T>();
    }
    else if (IDName == "out_features")
    {
        int id = parseIntLiteral();
        parseNewLines();
        return std::unique_ptr<MLParams>(new OutFeatures(id));
    }
    else
    {
        throw CompExcept("Parsing Error: MLParam not recognized");
    }
    return nullptr;
}

// It is unclear whether this specialization is actually needed.
// See, the entity that will search for a hyperparam block will be pretty sure
// that the only thing it is seeking is a hyperparam block.
// std::unique_ptr<ASTNode> Parser::parseIdentifier(Token expectedID, specializer<ASTNode>)
// {
//     std::string IDName = expectedID.getData();
    
//     if (IDName == "hyperparam_block")
//     {
//         // return parseHyperparamBlock();
//         return parseHyperparamBlock();
//     }
//     else if (IDName == "len")
//     {

//     }
//     else
//     {
//         throw CompExcept("Parsing Error: ASTNode not recognized");
//     }
//     return nullptr;
// }

// TODO
std::unique_ptr<Layer> Parser::parseIdentifier(Token expectedID, specializer<Layer>)
{
    std::string IDName = expectedID.getData();
    expect(TType::ASSIGN);
    expect(TType::LBRA);
    // parseParams here TODO

    if (IDName == "LSTM")
    {

    }
    else if (IDName == "MLP")
    {

    }
    else if (IDName == "SIGMOID")
    {

    }
    else
    {
        throw CompExcept("Parsing Error: Layer not recognized");
    }

    return nullptr;
}

template<typename T>
std::unique_ptr<HyperparamBlock> Parser::parseHyperparamBlock()
{
#ifdef PARDBG
    printf("PARSER:\t\tparseHyperparamBlock()\n");
#endif
    expect(TType::IDENTIFIER, "hyperparam_block");
    expect(TType::LBRA);
    setPtr<T> params;
    parseBlockParams<T>(params);
    expect(TType::RBRA);
    parseNewLines();
    return std::unique_ptr<HyperparamBlock>(new HyperparamBlock(params));
}

// TODO
template<typename T>
void Parser::parseBlockParams(setPtr<T> &&bl)
{
#ifdef PARDBG
    printf("PARSER:\t\tparseBlockParams()\n");
#endif
    // setPtr<T> a;
    (void)bl;
    return;
}

void Parser::parseNewLines()
{
#ifdef PARDBG
    printf("PARSER:\t\tparseNewLines()\n");
#endif
    expect(TType::NEWLINE);
    if (accept(TType::NEWLINE))
    {
        parseNewLines();
    }
    return;
}

std::string Parser::parseStrLiteral()
{
#ifdef PARDBG
    printf("PARSER:\t\tparseStrLiteral()\n");
#endif
    return expect(TType::STR_LITERAL).getData();
}

std::unordered_set<std::string> Parser::parseStrArrLiteral()
{
#ifdef PARDBG
    printf("PARSER:\t\tparseStrArrLiteral()\n");
#endif
    std::unordered_set<std::string> strArr;
    if (accept(TType::STR_LITERAL))
        return 
        {
            parseStrLiteral()
        };

    expect(TType::LSBR);
    if (!accept(TType::RSBR))
    {
        strArr.insert(parseStrLiteral());
        while(accept(TType::COMMA))
        {
            expect(TType::COMMA);
            strArr.insert(parseStrLiteral());
        }
    }
    expect(TType::RSBR);
    return strArr;
}

int Parser::parseIntLiteral()
{
#ifdef PARDBG
    printf("PARSER:\t\tparseIntLiteral()\n");
#endif
    Token intID = expect(TType::INT_LITERAL);
    try
    {
        return std::stoi(expect(TType::INT_LITERAL).getData());
    }
    catch(const std::exception& e)
    {
        throw CompExcept("Parsing Error: Invalid int literal");
    }
}

bool Parser::parseBoolLiteral()
{
#ifdef PARDBG
    printf("PARSER:\t\tparseBoolLiteral()\n");
#endif
    return expect(TType::BOOL_LITERAL).getData() == "True";
}

//! TODO Dummy!!
std::unique_ptr<BinExpr> Parser::parseBinExpr()
{
#ifdef PARDBG
    printf("PARSER:\t\tparseBinExpr()\n");
#endif
    std::string lhs = parseStrLiteral();
    if (accept(TType::PLUS))
    {
        expect(TType::PLUS);
        return std::unique_ptr<BinExpr>(new BinExpr(parseBinExpr(), lhs));
    }
    return std::unique_ptr<BinExpr>(new BinExpr(lhs));
}

