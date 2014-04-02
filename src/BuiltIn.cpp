#include <PCH.h>
#include <BuiltIn.h>
#include <Values.h>
#include <AST.h>
#include <Error.h>


static Closure const *const g_pZCombinator = new Closure({ "f" }, new ApplicationNode({
	new FunctionNode({ "x" }, new ApplicationNode({
		new VariableNode("f"),
		new FunctionNode({ "v" }, new ApplicationNode({
			new VariableNode("x"),
			new VariableNode("x"),
			new VariableNode("v")
		}))
	})), new FunctionNode({ "x" }, new ApplicationNode({
		new VariableNode("f"),
		new FunctionNode({ "v" }, new ApplicationNode({
			new VariableNode("x"),
			new VariableNode("x"),
			new VariableNode("v")
		}))
	}))
}), BaseEnvironment());

static Closure const *const g_pTrue = new Closure({ "x", "y" }, new VariableNode("x"), BaseEnvironment());
static Closure const *const g_pFalse = new Closure({ "x", "y" }, new VariableNode("y"), BaseEnvironment());


struct ThrowNode :
	public NativeNode
{
	virtual ~ThrowNode() {}

	virtual ThrowNode *Clone() const {
		return new ThrowNode();
	}

	virtual AbstractValue const *Evaluate(AbstractEnvironment const &rEnvironment) const {
		assert(rEnvironment.Has("value"));
		throw UserException(rEnvironment["value"]);
	}
};


struct ExitNode :
	public NativeNode
{
	virtual ~ExitNode() {}

	virtual ExitNode *Clone() const {
		return new ExitNode();
	}

	virtual AbstractValue const *Evaluate(AbstractEnvironment const &rEnvironment) const {
		assert(rEnvironment.Has("code"));
		AbstractValue const *const pCode = rEnvironment["code"];
		if (pCode->m_Type != AbstractValue::TYPE_INTEGER) {
			throw RuntimeError();
		} else {
			throw ExitException((IntegerValue const*)pCode);
		}
	}
};


struct InputNode :
	public NativeNode
{
	virtual ~InputNode() {}

	virtual InputNode *Clone() const {
		return new InputNode();
	}

	virtual AbstractValue const *Evaluate(AbstractEnvironment const &rEnvironment) const {
		string strLine;
		getline(cin, strLine);
		return new StringValue(strLine);
	}
};


struct PrintNode :
	public NativeNode
{
	virtual ~PrintNode() {}

	virtual PrintNode *Clone() const {
		return new PrintNode();
	}

	virtual AbstractValue const *Evaluate(AbstractEnvironment const &rEnvironment) const {
		assert(rEnvironment.Has("value"));
		cout << *(rEnvironment["value"]) << flush;
		return g_pTrue;
	}
};


struct PrintLnNode :
	public NativeNode
{
	virtual ~PrintLnNode() {}

	virtual PrintLnNode *Clone() const {
		return new PrintLnNode();
	}

	virtual AbstractValue const *Evaluate(AbstractEnvironment const &rEnvironment) const {
		assert(rEnvironment.Has("value"));
		cout << *(rEnvironment["value"]) << endl;
		return g_pTrue;
	}
};


struct ErrorNode :
	public NativeNode
{
	virtual ~ErrorNode() {}

	virtual ErrorNode *Clone() const {
		return new ErrorNode();
	}

	virtual AbstractValue const *Evaluate(AbstractEnvironment const &rEnvironment) const {
		assert(rEnvironment.Has("value"));
		cerr << *(rEnvironment["value"]) << flush;
		return g_pTrue;
	}
};


struct ErrorLnNode :
	public NativeNode
{
	virtual ~ErrorLnNode() {}

	virtual ErrorLnNode *Clone() const {
		return new ErrorLnNode();
	}

	virtual AbstractValue const *Evaluate(AbstractEnvironment const &rEnvironment) const {
		assert(rEnvironment.Has("value"));
		cerr << *(rEnvironment["value"]) << endl;
		return g_pTrue;
	}
};


map<string const, AbstractValue const*> BuiltInEnvironment::BuildTerms() {
	map<string const, AbstractValue const*> Terms;

	Terms["true"] = g_pTrue;
	Terms["false"] = g_pFalse;

	Terms["Z"] = g_pZCombinator;

	Terms["pair"] = new Closure({ "x", "y", "z" }, new ApplicationNode({
		new VariableNode("z"),
		new VariableNode("x"),
		new VariableNode("y")
	}), BaseEnvironment());
	Terms["first"] = new Closure({ "pair" }, new ApplicationNode({
		new VariableNode("pair"),
		new FunctionNode({ "x", "y" }, new VariableNode("x"))
	}), BaseEnvironment());
	Terms["second"] = new Closure({ "pair" }, new ApplicationNode({
		new VariableNode("pair"),
		new FunctionNode({ "x", "y" }, new VariableNode("y"))
	}), BaseEnvironment());

	Terms["list"] = new Closure({ "element", "next", "f", "g" }, new ApplicationNode({
		new VariableNode("f"),
		new VariableNode("element"),
		new VariableNode("next")
	}), BaseEnvironment());
	Terms["nil"] = new Closure({ "f", "g" }, new ApplicationNode({
		new VariableNode("g"),
		new FunctionNode({ "x", "y" }, new VariableNode("y"))
	}), BaseEnvironment());

	Terms["exit"] = new Closure({ "code" }, new ExitNode(), BaseEnvironment());
	Terms["throw"] = new Closure({ "value" }, new ThrowNode(), BaseEnvironment());
	Terms["input"] = new Closure({ "x" }, new InputNode(), BaseEnvironment());
	Terms["print"] = new Closure({ "value" }, new PrintNode(), BaseEnvironment());
	Terms["println"] = new Closure({ "value" }, new PrintLnNode(), BaseEnvironment());
	Terms["error"] = new Closure({ "value" }, new ErrorNode(), BaseEnvironment());
	Terms["errorln"] = new Closure({ "value" }, new ErrorLnNode(), BaseEnvironment());

	return Terms;
}


BuiltInEnvironment::BuiltInEnvironment()
	:
BaseEnvironment(BuildTerms()) {}
