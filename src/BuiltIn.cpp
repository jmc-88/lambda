#include <PCH.h>
#include <BuiltIn.h>
#include <Values.h>
#include <AST.h>


static Closure const *const g_pNil = new Closure({ "x", "y" }, new VariableNode("y"), BaseEnvironment());
static Closure const *const g_pTrue = new Closure({ "x", "y" }, new VariableNode("x"), BaseEnvironment());
static Closure const *const g_pFalse = new Closure({ "x", "y" }, new VariableNode("y"), BaseEnvironment());


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


struct InputNode :
	public NativeNode
{
	virtual ~InputNode() {}

	virtual InputNode *Clone() const {
		return new InputNode();
	}

	virtual AbstractValue const *Evaluate(AbstractEnvironment const &rEnvironment) const {
		string strLine;
		::getline(cin, strLine);
		return new StringValue(strLine);
	}
};


map<string const, AbstractValue const*> BuiltInEnvironment::BuildTerms() {
	map<string const, AbstractValue const*> Terms;

	Terms["nil"] = g_pNil;
	Terms["true"] = g_pTrue;
	Terms["false"] = g_pFalse;

	Terms["Z"] = new Closure({ "f" }, new ApplicationNode({
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

	Terms["list"] = new Closure({ "element", "next", "f" }, new ApplicationNode({
		new VariableNode("f"),
		new VariableNode("element"),
		new FunctionNode({ "f", "g" }, new ApplicationNode({
			new VariableNode("f"),
			new VariableNode("next")
		}))
	}), BaseEnvironment());
	Terms["lend"] = new Closure({ "element", "f" }, new ApplicationNode({
		new VariableNode("f"),
		new VariableNode("element"),
		new FunctionNode({ "f", "g" }, new ApplicationNode({
			new VariableNode("g"),
			new FunctionNode({ "x", "y" }, new VariableNode("y"))
		}))
	}), BaseEnvironment());
	Terms["head"] = new Closure({ "list" }, new ApplicationNode({
		new VariableNode("list"),
		new FunctionNode({ "element", "next" }, new VariableNode("element"))
	}), BaseEnvironment());
	// TODO tail

	Terms["print"] = new Closure({ "value" }, new PrintNode(), BaseEnvironment());
	Terms["println"] = new Closure({ "value" }, new PrintLnNode(), BaseEnvironment());
	Terms["input"] = new Closure({ "x" }, new InputNode(), BaseEnvironment());

	return Terms;
}


BuiltInEnvironment::BuiltInEnvironment()
	:
BaseEnvironment(BuildTerms()) {}
